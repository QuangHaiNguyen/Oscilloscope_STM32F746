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
#include "vendor/bios_loader.h"
#include "spifi.h"
#include "spifi_rom_api.h"


/******************************************************************************
 * Defines and typedefs
 *****************************************************************************/

//#define SPIFI_DBG             0

/* 
 * The xSPIFI_ROM_PTR (0x1FFF1FF8) points to an area where the pointers to
 * different drivers in ROM are stored.
 */
typedef struct {
   /*const*/ unsigned p_usbd;     // USBROMD 
   /*const*/ unsigned p_clib;
   /*const*/ unsigned p_cand;
   /*const*/ unsigned p_pwrd;     // PWRROMD
   /*const*/ unsigned p_promd;    // DIVROMD
   /*const*/ SPIFI_RTNS *pSPIFID; // SPIFIROMD
   /*const*/ unsigned p_dev3;
   /*const*/ unsigned p_dev4; 
} ROM;

#define ROM_DRIVERS_PTR ((ROM *)(*((unsigned int *)xSPIFI_ROM_PTR)))
#define IS_ADDR_IN_SPIFI(__addr)  ( (((uint32_t)(__addr)) & 0xff000000) == SPIFI_MEM_BASE )

#define SPIFI_MIN(__a, __b) (((__a) < (__b)) ? (__a) : (__b))

typedef enum {
    SpifiDevice_Spansion_S25FL032,    /* Manufacturer: 0x01, devType: 0x02, devID: 0x15 */
    SpifiDevice_Winbond_W25Q64FV,     /* Manufacturer: 0xEF, devType: 0x40, devID: 0x17 */
    SpifiDevice_Macronix_MX25L6435E,  /* Manufacturer: 0xC2, devType: 0x20, devID: 0x17 */
    SpifiDevice_Macronix_MX25L12835F, /* Manufacturer: 0xC2, devType: 0x20, devID: 0x18 */
    SpifiDevice_SpecifiedInBios,      /* The device is known by the BIOS */
    SpifiDevice_UnknownDevice
} SpifiDevice_t;

/******************************************************************************
 * Local variables
 *****************************************************************************/

static bool              _initialized = false;
static SPIFIobj          _romData = {0};
static const SPIFI_RTNS* _spifi = 0;
static SpifiDevice_t     _device = SpifiDevice_UnknownDevice;
static uint32_t          _memorySize = 0;
static uint32_t          _eraseBlockSize = 0;

/******************************************************************************
 * Private Functions
 *****************************************************************************/

static SpifiError_t spifi_translateError(int err)
{  
  SpifiError_t res;
  
  if (err == 0)
  {
    res = SpifiError_Ok;
  }
  else if ((err >= SpifiError_Uninitialized) && (err <= SpifiError_UnknownError))
  {
    // This is a known error code
    res = (SpifiError_t)err;
  }
  else if ((err >= SpifiError_InternalError) && (err <= SpifiError_EraseConflict))
  {
    // This is a known error code
    res = (SpifiError_t)err;
  }
  else
  {
    // Should never happen :-) as all listed error codes are covered but
    // to be on the safe side and not interpret this as a success, a generic
    // error is set.
    res = SpifiError_UnknownError;
  }
  return res;
}

/******************************************************************************
 * Public Functions
 *****************************************************************************/

SpifiError_t spifi_init(void)
{
  uint32_t spifi_clk_div, spifi_clk_mhz;
  int rc;

  // to avoid warning
  (void)_device;
  (void)_memorySize;

  if (!_initialized) {
    
    // Turn on SPIFI block as it is disabled on reset
    Chip_SPIFI_Init();
      
    // Configure clock source as Main PLL and divider 2 to get 60MHz
    spifi_clk_div = 2;
    LPC_SYSCTL->SPIFICLKSEL = (1<<8) | spifi_clk_div;

    // pinsel for SPIFI
    //LPC_IOCON->P2_7 = 5; /* SPIFI_CSN @ P2.7 */
    //LPC_IOCON->P0_22 = 5; /* SPIFI_CLK @ P0.22 */
    //LPC_IOCON->P0_15 = 5; /* SPIFI_IO2 @ P0.15 */
    //LPC_IOCON->P0_16 = 5; /* SPIFI_IO3 @ P0.16 */
    //LPC_IOCON->P0_17 = 5; /* SPIFI_IO1 @ P0.17 */
    //LPC_IOCON->P0_18 = 5; /* SPIFI_IO0 @ P0.18 */
    
    spifi_clk_mhz = (SystemCoreClock / spifi_clk_div) / 1000000;

    _spifi = ROM_DRIVERS_PTR->pSPIFID;

    /* Typical time tCS is 20 ns min, we give 200 ns to be on safer side */
    rc = _spifi->spifi_init (&_romData, spifi_clk_mhz/5, S_FULLCLK+S_RCVCLK, spifi_clk_mhz);
    if (rc) {
      _spifi = NULL;
      return spifi_translateError(rc);
    }

    /* Make sure it is a tested flash module */
    if ((_romData.mfger == 1) && (_romData.devType == 0x2) && (_romData.devID == 0x15) && (_romData.memSize > 0x100000)) 
    {
      _device = SpifiDevice_Spansion_S25FL032;
      _memorySize = _romData.memSize;
      _eraseBlockSize = 64*1024;
    } 
    else if ((_romData.mfger == 0xef) && (_romData.devType == 0x40) && (_romData.devID == 0x17) && (_romData.memSize > 0x100000))
    {
      _device = SpifiDevice_Winbond_W25Q64FV;
      _memorySize = _romData.memSize;
      _eraseBlockSize = 4*1024;
    } 
    else if ((_romData.mfger == 0xc2) && (_romData.devType == 0x20) && (_romData.devID == 0x17) && (_romData.memSize > 0x100000))
    {
      _device = SpifiDevice_Macronix_MX25L6435E;
      _memorySize = _romData.memSize;
      _eraseBlockSize = 4*1024;
    } 
    else if ((_romData.mfger == 0xc2) && (_romData.devType == 0x20) && (_romData.devID == 0x18) && (_romData.memSize > 0x100000))
    {
      _device = SpifiDevice_Macronix_MX25L12835F;
      _memorySize = _romData.memSize;
      _eraseBlockSize = 4*1024;
    }
    else if (bios_isKnownSPIFIMemory(_romData.mfger, _romData.devType, _romData.devID, _romData.memSize, &_eraseBlockSize))
    {
      /* The BIOS was able to identify the FLASH and we will use the 
       * eraseBlockSize specified in the BIOS.
       */
      _device = SpifiDevice_SpecifiedInBios;
      _memorySize = _romData.memSize;
    }
    else 
    {
      DEBUGSTR("SPIFI::init(): Memory is unknown and may not work as expected\n");
      DEBUGOUT("ID: mfgr 0x%02x, devType 0x%02x, devID 0x%02x, memSize 0x%x\n", 
            _romData.mfger, _romData.devType, _romData.devID, _romData.memSize);
      
      // Asuming it has 64Kb erase blocks (i.e. same setup as the Spansion S25FL032
      _device = SpifiDevice_UnknownDevice;
      _memorySize = _romData.memSize;
      _eraseBlockSize = 64*1024;

      /*
       * If this happens, check the manufacturer and device information
       * and compare with the data sheet for your chip. Also make sure
       * that the sector sizes are the same (i.e. 64KB) for your chip.
       * If everything is the same then add an exception for your chip.
       */
    }
    
    _initialized = true;
  }
  return SpifiError_Ok;
}
