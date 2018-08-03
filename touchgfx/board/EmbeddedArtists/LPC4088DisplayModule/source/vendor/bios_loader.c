/*
 *  Copyright 2014 Embedded Artists AB
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#include "board.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include "vendor/bios_loader.h"
#include "vendor/bios.h"
#include "vendor/touch.h"
#include "vendor/display.h"

#include <string.h>


/******************************************************************************
 * Defines and typedefs
 *****************************************************************************/

#define MOVE_POINTER(__x, __off) (   ( (uint32_t*)(__x) ) = (uint32_t*)( (uint32_t)(__x) + (__off) ) )

/*
 * Make sure that we reserve at least this amount of RAM for future
 * expansion of the BIOS. This prevents the user from squeezing out
 * the last drop of available RAM in his application.
 */
#define BIOS_RESERVED_CHUNK  SDRAM_SZ_BIOS2
#define BIOS_MAX_SIZE        SDRAM_SZ_BIOS1
#ifndef MAX
  #define MAX(__a, __b) (((__a)>(__b))?(__a):(__b))
#endif

/*
 * The BIOS is API compatible as long as the Major component of the
 * version is the same.
 */
#define SUPPORTED_BIOS_VER   0x000000
#define SUPPORTED_BIOS_MASK  0xff0000  // only look at the Major component
#define SUPPORTED_VERSION(__ver) (((__ver)&SUPPORTED_BIOS_MASK) == SUPPORTED_BIOS_VER)

#define BIOS_EEPROM_ADDR      (0xAC)

/* The touch IRQ pin is always p2.25 */
#define TPIRQ_GPIO_PORT    2
#define TPIRQ_GPIO_PIN    25

/* Maximum possible number of fingers supported on a multitouch display */
#define TOUCH_MAX_FINGERS_SUPPORTED  (5)

typedef struct {
  bool           initialized;
  bios_header_t  biosHeader;
  void*          biosParams;
  uint32_t       stats;
  bool           usTickerInitialized;
} vars_for_bios_t;

typedef struct {
  bool           initialized;
  bool           poweredOn;
  uint16_t       width;
  uint16_t       height;
  uint16_t       bpp;
  uint16_t       supportedRes;
  Resolution_t   activeRes;
  bool           landscape;
} vars_for_display_t;

typedef struct {
  bool           initialized;
  bool           poweredOn;
  bool           haveInfo;
  xTaskHandle    touchTask;
  bool           supportsTouch;
  bool           supportsTouchCalibration;
  uint8_t        numFingers;
  bool           fallingTPIRQ;
  touch_coordinate_t fingersIn[TOUCH_MAX_FINGERS_SUPPORTED];
  touch_coordinate_t fingersOut[TOUCH_MAX_FINGERS_SUPPORTED];
  xSemaphoreHandle   mutex;
  xSemaphoreHandle   notifier;
  touchEventFunc     listener;
} vars_for_touch_t;

/******************************************************************************
 * Local variables
 *****************************************************************************/

static vars_for_bios_t    _bios = {0};
static vars_for_display_t _display = {0};
static vars_for_touch_t   _touch = {0};


/******************************************************************************
 * Global functions
 *****************************************************************************/

void I2C0_IRQHandler(void)
{
  _bios.biosHeader.i2cIRQHandler(_bios.biosParams);
}

void GPIO_IRQHandler(void)
{
  if (_touch.initialized) {
    if (_touch.fallingTPIRQ && (LPC_GPIOINT->IO2.STATF & (1<<TPIRQ_GPIO_PIN))) {
      _bios.biosHeader.touchIrqHandler(_bios.biosParams);
    } else if (!_touch.fallingTPIRQ && (LPC_GPIOINT->IO2.STATR & (1<<TPIRQ_GPIO_PIN))) {
      _bios.biosHeader.touchIrqHandler(_bios.biosParams);
    }
  }
  LPC_GPIOINT->IO2.CLR = (1<<TPIRQ_GPIO_PIN);
}

/******************************************************************************
 * Private Functions
 *****************************************************************************/

// Timer to get basic delay functions using TIMER3
void bios_us_ticker_init(void) {
  if (!_bios.usTickerInitialized) {
    uint32_t PCLK, prescale;
    _bios.usTickerInitialized = true;

    LPC_SYSCTL->PCONP |= 1 << 23; // Clock TIMER_3

    LPC_TIMER3->CTCR = 0x0; // timer mode
    PCLK = Chip_Clock_GetPeripheralClockRate();

    LPC_TIMER3->TCR = 0x2;  // reset

    prescale = PCLK / 1000000; // default to 1MHz (1 us ticks)
    LPC_TIMER3->PR = prescale - 1;
    LPC_TIMER3->TCR = 1; // enable = 1, reset = 0
  }
}

uint32_t bios_us_ticker_read() {
  if (!_bios.usTickerInitialized) {
    bios_us_ticker_init();
  }

  return LPC_TIMER3->TC;
}


// Function called from the BIOS
static uint32_t bios_readTimeMs()
{
  return bios_us_ticker_read()/1000;
}

static void bios_wait_us(int us) {
  uint32_t start = bios_us_ticker_read();
  while ((bios_us_ticker_read() - start) < (uint32_t)us);
}

//static void bios_wait_ms(int ms) {
//  bios_wait_us(ms * 1000);
//}

static void allowExecutionOfCodeInSDRAM(void)
{
  //----- Adjust MPU to allow execution of code from SDRAM

  /* Disable MPU */
  MPU->CTRL = 0x00;

  /* Select to use the default memory map as base and only modify parts */
  MPU->CTRL = 0x04; // PRIVDEFENA=1

#define _RBAR(_ADDR, _VALID, _REGION) \
    ((_ADDR) | ((_VALID) << 4) | (_REGION))

#define _RASR(_XN, _AP, _TYPE, _SRD, _SIZE, _ENABLE) \
    (((_XN) << 28) | ((_AP) << 24) | ((_TYPE) << 16) | ((_SRD) << 8) | ((_SIZE) << 1) | (_ENABLE))

#define AP_RW         0x03 // 011 = Full Access
#define MEM_TYPE_ERAM 0x07 // Normal, Sharable, Cached, Buffered, write-back & write allocate
#define SIZE_32M      0x18 // Region size in bytes = 2^(0x18+1) = 32MB

  /* Setup MPU Region 4 for the SDRAM (0xA0000000 - 0xA1FFFFFF*/
  MPU->RBAR = _RBAR(0xA0000000, 1, 4);
  MPU->RASR = _RASR(0, AP_RW, MEM_TYPE_ERAM, 0, SIZE_32M, 1);

  /* (Re-)Enable MPU */
  MPU->CTRL |= 0x01;

  //----- End of MPU adjustments
}

static bool bios_readEEPROM(uint32_t address, uint8_t* data, uint32_t size)
{
  uint8_t buf[2];

  Board_I2C_Init(I2C0);
  Chip_I2C_Init(I2C0);
  Chip_I2C_SetClockRate(I2C0, 400000);

  // Use POLLING mode here as there is nothing else running in the system
  NVIC_DisableIRQ(I2C0_IRQn);
  Chip_I2C_SetMasterEventHandler(I2C0, Chip_I2C_EventHandlerPolling);

  buf[0] = address >> 8;
  buf[1] = address & 0xff;

  if (Chip_I2C_MasterSend(I2C0, (BIOS_EEPROM_ADDR>>1), buf, 2) == 2) {
    if (Chip_I2C_MasterRead(I2C0, (BIOS_EEPROM_ADDR>>1), data, size) == size) {
      return true; // success
    }
  }
  return false;
}

static BiosLoaderError_t bios_readBIOS(uint8_t** data, uint32_t* size)
{
  static uint8_t* conf = NULL;
  static uint32_t confSize = 0;

  BiosLoaderError_t err = BiosLoaderError_Ok;
  file_header_t fh;
  uint32_t crc;

  if (conf != NULL) {
    *data = conf;
    *size = confSize;
    return BiosLoaderError_Ok;
  }

  do {
    if (!bios_readEEPROM(0, (uint8_t*)&fh, sizeof(file_header_t))) {
      if (!bios_readEEPROM(0, (uint8_t*)&fh, sizeof(file_header_t))) {
        err = BiosLoaderError_StorageError;
        break;
      }
    }

    if (fh.magic != BIOS_MAGIC) {
      err = BiosLoaderError_BiosInvalid;
      break;
    }

    if (!SUPPORTED_VERSION(fh.version)) {
      err = BiosLoaderError_BiosVersion;
      break;
    }

    if ((fh.headerSize + fh.size) > BIOS_MAX_SIZE) {
      err = BiosLoaderError_BiosInvalid;
      break;
    }

    confSize = fh.headerSize + fh.size;
    conf = (uint8_t*)SDRAM_ADDR_BIOS1;//_bios.biosEEPROM;

    if (!bios_readEEPROM(0, conf, confSize)) {
      err = BiosLoaderError_StorageError;
      break;
    }

    crc = Chip_CRC_CRC32((uint32_t*)(&conf[fh.headerSize]), fh.size/4);
    if (crc != fh.crc) {
      err = BiosLoaderError_BiosInvalid;
      break;
    }

    // Bios header has been verified and seems ok
    *data = conf;
    *size = confSize;
    _bios.stats = fh.version;
    err = BiosLoaderError_Ok;
  } while (false);

  if (err != BiosLoaderError_Ok) {
    conf = 0;
    confSize = 0;
  }

  return err;
}

// Function called from the BIOS
static void touchIrqEnabler(uint32_t arg, bool enable, touch_irq_trigger_t trigger)
{
  NVIC_DisableIRQ(GPIO_IRQn);

  /* Configure GPIO interrupt pin as input */
  Chip_GPIO_SetPinDIRInput(LPC_GPIO, TPIRQ_GPIO_PORT, TPIRQ_GPIO_PIN);

  /* Configure the GPIO interrupt */
  switch (trigger) {
    case TOUCH_IRQ_RISING_EDGE:
      if (enable) {
        // enable Rising edge, disable Falling edge
        LPC_GPIOINT->IO2.ENR |= (1<<TPIRQ_GPIO_PIN);
        LPC_GPIOINT->IO2.ENF &= ~(1<<TPIRQ_GPIO_PIN);
      } else {
        // disable Rising edge, disable Falling edge
        LPC_GPIOINT->IO2.ENR &= ~(1<<TPIRQ_GPIO_PIN);
        LPC_GPIOINT->IO2.ENF &= ~(1<<TPIRQ_GPIO_PIN);
      }
      _touch.fallingTPIRQ = false;
      break;

    case TOUCH_IRQ_FALLING_EDGE:
      if (enable) {
        // disable Rising edge, enable Falling edge
        LPC_GPIOINT->IO2.ENR &= ~(1<<TPIRQ_GPIO_PIN);
        LPC_GPIOINT->IO2.ENF |= (1<<TPIRQ_GPIO_PIN);
      } else {
        // disable Rising edge, disable Falling edge
        LPC_GPIOINT->IO2.ENR &= ~(1<<TPIRQ_GPIO_PIN);
        LPC_GPIOINT->IO2.ENF &= ~(1<<TPIRQ_GPIO_PIN);
      }
      _touch.fallingTPIRQ = true;
      break;

    case TOUCH_IRQ_HIGH_LEVEL:
    case TOUCH_IRQ_LOW_LEVEL:
    default:
      DEBUGOUT("BIOS requests unknown trigger type %d\n", trigger);
      break;
  }

  NVIC_SetPriority(GPIO_IRQn, (1 << __NVIC_PRIO_BITS) - 1);
  NVIC_ClearPendingIRQ(GPIO_IRQn);
  NVIC_EnableIRQ(GPIO_IRQn);
}

// Function called from the BIOS
static void touchNewData(uint32_t arg, touch_coordinate_t* coords, int num)
{
  portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;

  // Save the new touch coordinates
  memcpy(_touch.fingersIn, coords, num*sizeof(touch_coordinate_t));

  // Notify the touch task
  xSemaphoreGiveFromISR( _touch.notifier, &xHigherPriorityTaskWoken );
  portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
}

/******************************************************************************
 * Threads
 *****************************************************************************/

static void handleSingleTouch(void)
{
  touchEventFunc fp;
  while(true) {

    // wait forever
    if (xSemaphoreTake( _touch.notifier, portMAX_DELAY ) != pdTRUE) {
      // timeout, should not be possible
      continue;
    }

    // copy the new coordinates
    xSemaphoreTake(_touch.mutex, portMAX_DELAY);
    memcpy(_touch.fingersOut, _touch.fingersIn, _touch.numFingers*sizeof(touch_coordinate_t));
    fp = _touch.listener;
    xSemaphoreGive(_touch.mutex);

    // notify possible listener about the new coordinates
    if (fp != NULL) {
      (fp)();
    }
  }
}

static void handleMultiTouch(void)
{
  touchEventFunc fp;
  portTickType maxDelay = portMAX_DELAY;

  while(true) {
    // Wait for new coordinates or a timeout (possibly indicating the
    // end of a gesture)
    if (xSemaphoreTake( _touch.notifier, maxDelay ) == pdTRUE) {
      // got new coordinates

      // copy the new coordinates (and the callback function)
      xSemaphoreTake(_touch.mutex, portMAX_DELAY);
      memcpy(_touch.fingersOut, _touch.fingersIn, _touch.numFingers*sizeof(touch_coordinate_t));
      fp = _touch.listener;
      xSemaphoreGive(_touch.mutex);

      // wait up to 25ms for next event before injecting a "pen up" event
      maxDelay = 25/portTICK_RATE_MS;
    } else {
      // timeout

      int numNonZero = 0;
      int i;
      for (i = 0; i < _touch.numFingers; i++) {
        if (_touch.fingersOut[i].z > 0) {
          numNonZero++;
        }
        _touch.fingersOut[i].z = 0;
      }
      maxDelay = portMAX_DELAY;
      if (numNonZero == 0) {
        // last event was a pen-up event so no need to inject one
        //DEBUGSTR("skip penup\n");
        continue;
      }
      //DEBUGSTR("inject penup\n");

      // copy the callback function
      xSemaphoreTake(_touch.mutex, portMAX_DELAY);
      fp = _touch.listener;
      xSemaphoreGive(_touch.mutex);
    }

    // notify possible listener about the new coordinates
    if (fp != NULL) {
      (fp)();
    }
  }
}

/* Touch thread */
static void vTouchTask(void *pvParameters)
{
  if (_touch.numFingers > 1) {
    // multitouch - inject end event if missing
    handleMultiTouch();
  } else {
    // normal singe-touch
    handleSingleTouch();
  }
}

/******************************************************************************
 * Public Functions
 *****************************************************************************/

BiosLoaderError_t bios_init(void)
{
  BiosLoaderError_t err = BiosLoaderError_Ok;
  file_header_t* file_header;
  uint32_t offset;
  uint32_t* functions;
  uint32_t i;

  if (!_bios.initialized) {
    // Make sure that code can be relocated to external SDRAM and still
    // be executable
    allowExecutionOfCodeInSDRAM();

    // Make sure that the SystemCoreClock variable is updated
    SystemCoreClockUpdate();

    do {
      // Read the BIOS from the EEPROM and verify it
      uint8_t* p = NULL;
      uint32_t size = 0;
      err = (BiosLoaderError_t)bios_readBIOS(&p, &size);
      if (err != BiosLoaderError_Ok) {
        break;
      }

      // Extract the function pointers so that they can be modified to match the
      // actual location of the code
      file_header = (file_header_t*)p;
      memcpy(&_bios.biosHeader, &file_header->header, sizeof(bios_header_t));

      // Allocate memory for the BIOS instance data
      _bios.biosParams = (uint8_t*)SDRAM_ADDR_BIOS2;//&(_bios.biosInstance[0]);
      if (BIOS_RESERVED_CHUNK < file_header->paramSize) {
        err = BiosLoaderError_Memory;
        break;
      }

      // All offsets must be moved by two factors:
      // 1) The position of the code in RAM (location of "p")
      // 2) The header size (the code/data comes after it)
      offset = ((uint32_t)p) + file_header->headerSize;
      functions = (uint32_t*)&_bios.biosHeader;
      for (i = 0; i < (sizeof(bios_header_t)/sizeof(uint32_t)); i++) {
        functions[i] += offset;
      }

#if defined(DM_BOARD_BIOS_DEVELOPMENT)
      // This requires that the project contains the source code for the BIOS
      {
        const char* msg;
        uint32_t tmp = 0;
        bios_debug_aid(&_bios.biosHeader, &msg, &tmp);
        if (tmp > BIOS_RESERVED_CHUNK) {
          err = BiosLoaderError_Memory;
          break;
        }
        DEBUGOUT("BIOS info: %s\n", msg);
      }
#endif

      // Prepare the BIOS instance data before calling the first function
      err = (BiosLoaderError_t)_bios.biosHeader.initParams(_bios.biosParams,
              SystemCoreClock,
              Chip_Clock_GetPeripheralClockRate(),
              bios_wait_us,
              bios_readTimeMs);
      if (err != BiosLoaderError_Ok) {
        break;
      }

      // Setup the mandatory I2C0 interrupt handler after initParams but before all other calls
      NVIC_SetPriority(I2C0_IRQn, (1 << __NVIC_PRIO_BITS) - 1);
      NVIC_ClearPendingIRQ(I2C0_IRQn);
      NVIC_EnableIRQ(I2C0_IRQn);

      _bios.initialized = true;
    } while(0);
  }
  return err;
}

void bios_getMacAddress(char mac[6])
{
  static char cache[6];
  static bool haveIt = false;

  if (!haveIt) {
    if (!_bios.initialized) {
      bios_init();
    }
    if (_bios.initialized) {
      BiosError_t err = _bios.biosHeader.ethernetMac(_bios.biosParams, cache);
      if (err == BiosError_Ok) {
        haveIt = true;
      }
    }

    if (!haveIt) {
      // We always consider the MAC address to be retrieved even though
      // reading is failed. If it wasn't possible to read then the default
      // address will be used.
      cache[0] = 0x00;
      cache[1] = 0x02;
      cache[2] = 0xF7;
      cache[3] = 0xF0;
      cache[4] = 0x00;
      cache[5] = 0x01;
    }
    haveIt = true;
  }

  mac[0] = cache[0];
  mac[1] = cache[1];
  mac[2] = cache[2];
  mac[3] = cache[3];
  mac[4] = cache[4];
  mac[5] = cache[5];
}

bool bios_isKnownSPIFIMemory(uint8_t mfgr, uint8_t devType, uint8_t devID, uint32_t memSize, uint32_t* eraseBlockSize)
{
  BiosError_t err = BiosError_Ok;
  do {
    if (!_bios.initialized) {
      if (bios_init() != BiosLoaderError_Ok) {
        break;
      }
    }

    if (_bios.initialized) {
      bool isKnown = false;
      err = _bios.biosHeader.spifiIsSupported(_bios.biosParams, mfgr,devType,devID,memSize,&isKnown,eraseBlockSize);
      if (err == BiosError_Ok) {
        return isKnown;
      }
    }
  } while(false);
  return false;
}

void bios_getStats(uint8_t* type, uint8_t* major, uint8_t* minor, uint8_t* rev)
{
  *type  = (_bios.stats >> 24) & 0xff;
  *major = (_bios.stats >> 16) & 0xff;
  *minor = (_bios.stats >>  8) & 0xff;
  *rev   = (_bios.stats >>  0) & 0xff;
}

BiosLoaderError_t disp_init(void)
{
  BiosLoaderError_t err = BiosLoaderError_Ok;
  if (!_display.initialized) {
    do {
      if (!_bios.initialized) {
        err = BiosLoaderError_NoInit;
        break;
      }

      err = (BiosLoaderError_t)_bios.biosHeader.displayInit(_bios.biosParams);
      if (err != BiosLoaderError_Ok) {
        break;
      }

      err = (BiosLoaderError_t)_bios.biosHeader.displayInformation(_bios.biosParams,
              &_display.width,
              &_display.height,
              &_display.bpp,
              &_display.landscape,
              &_display.supportedRes,
              &_display.activeRes);
      if (err != BiosLoaderError_Ok) {
        break;
      }

      _display.initialized = true;
    } while(0);
  }
  return err;
}

BiosLoaderError_t disp_powerUp(void* framebuffer, Resolution_t res, FrameRate_t rate)
{
  BiosLoaderError_t err = BiosLoaderError_Ok;
  if (!_display.poweredOn) {
    err = disp_init();
    if (err == BiosLoaderError_Ok) {
      do {
        err = (BiosLoaderError_t)_bios.biosHeader.displayPowerUp(_bios.biosParams, framebuffer, res, rate);

        if (err != BiosLoaderError_Ok) {
          break;
        }

        err = (BiosLoaderError_t)_bios.biosHeader.displayInformation(_bios.biosParams,
                &_display.width,
                &_display.height,
                &_display.bpp,
                &_display.landscape,
                &_display.supportedRes,
                &_display.activeRes);
        if (err != BiosLoaderError_Ok) {
          break;
        }

        _display.poweredOn = true;
      } while(0);
    }
  }
  return err;
}

BiosLoaderError_t disp_powerDown(void)
{
  BiosLoaderError_t err = BiosLoaderError_Ok;
  if (_display.poweredOn) {
    err = (BiosLoaderError_t)_bios.biosHeader.displayPowerDown(_bios.biosParams);
    _display.poweredOn = false;
  }
  return err;
}

BiosLoaderError_t disp_backlight(int percent)
{
  BiosLoaderError_t err = BiosLoaderError_Ok;
  if (!_display.initialized) {
    err = BiosLoaderError_NoInit;
  } else {
    err = (BiosLoaderError_t)_bios.biosHeader.displayBacklight(_bios.biosParams, percent);
  }
  return err;
}

uint16_t disp_width(void)
{
  return _display.width;
}

uint16_t disp_height(void)
{
  return _display.height;
}

uint16_t disp_bytesPerPixel(void)
{
  return _display.bpp;
}

uint32_t disp_fbSize(void)
{
  return _display.width * _display.height * _display.bpp;
}

bool disp_landscape(void)
{
  return _display.width;
}

bool disp_isSupported(Resolution_t res)
{
  return (_display.supportedRes & res);
}

Resolution_t disp_currentResolution(void)
{
  return _display.activeRes;
}

void disp_setFramebuffer(void* buff)
{
  LPC_LCD->UPBASE = (uint32_t)buff;
}

void* disp_swapFramebuffer(void* buff)
{
  uint32_t old = LPC_LCD->UPBASE;
  LPC_LCD->UPBASE = (uint32_t)buff;
  return (void*)old;
}

BiosLoaderError_t touch_init(void)
{
  BiosLoaderError_t result = BiosLoaderError_Ok;
  if (!_touch.initialized) {
    do {
      result = (BiosLoaderError_t)_bios.biosHeader.touchInformation(_bios.biosParams,
                  &_touch.supportsTouch,
                  &_touch.supportsTouchCalibration,
                  &_touch.numFingers);
      if (result != BiosLoaderError_Ok) {
        break;
      }
      _touch.haveInfo = true;

      // is it supported at all?
      if (!_touch.supportsTouch) {
        result = BiosLoaderError_NotSupported;
        break;
      }

      result = (BiosLoaderError_t)_bios.biosHeader.touchInit(_bios.biosParams,
                  touchIrqEnabler,
                  0,
                  touchNewData,
                  0);
      if (result != BiosLoaderError_Ok) {
        break;
      }

      result = (BiosLoaderError_t)_bios.biosHeader.touchPowerUp(_bios.biosParams);
      if (result != BiosLoaderError_Ok) {
        break;
      }

      _touch.mutex = xSemaphoreCreateMutex();
      if (_touch.mutex == NULL) {
        result = BiosLoaderError_RTOS;
        break;
      }
      vSemaphoreCreateBinary(_touch.notifier);
      if (_touch.notifier == NULL) {
        result = BiosLoaderError_RTOS;
        break;
      }

      /* Make sure that the first "take" in vTouchTask blocks */
      xSemaphoreTake( _touch.notifier, portMAX_DELAY );

      /* GUI thread, draw something on the display */
      xTaskCreate(vTouchTask, (signed char *) "vTaskTouch",
                  configMINIMAL_STACK_SIZE, NULL, (tskIDLE_PRIORITY + 1UL),
                  &_touch.touchTask);

      _touch.initialized = true;
    } while(0);
  }
  return result;
}

bool touch_isTouchSupported(void)
{
  if (_touch.haveInfo) {
    return _touch.supportsTouch;
  }
  return false;
}

BiosLoaderError_t touch_read(touch_coordinate_t* coord, int num)
{
  BiosLoaderError_t err = BiosLoaderError_Ok;
  if (!_touch.haveInfo) {
    err = BiosLoaderError_NoInit;
  } else if (num > _touch.numFingers || num < 1) {
    err = BiosLoaderError_InvalidParam;
  } else {
    xSemaphoreTake(_touch.mutex, portMAX_DELAY);
    memcpy(coord, _touch.fingersOut, num*sizeof(touch_coordinate_t));
    xSemaphoreGive(_touch.mutex);
  }
  return err;
}

BiosLoaderError_t touch_info(bool* resistive, int* maxPoints, bool* calibrated)
{
  BiosLoaderError_t err = BiosLoaderError_Ok;
  if (!_touch.haveInfo) {
    err = BiosLoaderError_NoInit;
  } else {
    *maxPoints = _touch.numFingers;
    *calibrated = _touch.supportsTouchCalibration;
  }
  return err;
}

BiosLoaderError_t touch_calibrateStart(void)
{
  BiosLoaderError_t err = BiosLoaderError_Ok;
  if (!_touch.initialized) {
    err = BiosLoaderError_NoInit;
  } else {
    err = (BiosLoaderError_t)_bios.biosHeader.touchCalibrateStart(_bios.biosParams);
  }
  return err;
}

BiosLoaderError_t touch_getNextCalibratePoint(uint16_t* x, uint16_t* y, bool* last)
{
  BiosLoaderError_t err = BiosLoaderError_Ok;
  if (!_touch.initialized) {
    err = BiosLoaderError_NoInit;
  } else {
    err = (BiosLoaderError_t)_bios.biosHeader.touchGetNextCalibPoint(_bios.biosParams, x, y, last);
  }
  return err;
}

BiosLoaderError_t touch_waitForCalibratePoint(bool* morePoints, uint32_t timeout)
{
  BiosLoaderError_t err = BiosLoaderError_Ok;
  if (!_touch.initialized) {
    err = BiosLoaderError_NoInit;
  } else {
    err = (BiosLoaderError_t)_bios.biosHeader.touchWaitForCalibratePoint(_bios.biosParams, morePoints, timeout);
  }
  return err;
}

touchEventFunc touch_setListener(touchEventFunc handler)
{
  touchEventFunc old = NULL;
  if (_touch.initialized) {
    xSemaphoreTake(_touch.mutex, portMAX_DELAY);
    old = _touch.listener;
    _touch.listener = handler;
    xSemaphoreGive(_touch.mutex);
  }
  return old;
}
