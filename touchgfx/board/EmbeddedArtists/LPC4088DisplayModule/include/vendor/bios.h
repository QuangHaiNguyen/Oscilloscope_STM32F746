#ifndef BIOS_H
#define BIOS_H

#include <stdint.h>
#include <stdbool.h>

#define BIOS_MAGIC  0xEA0123EA
#define BIOS_VER      0x000500  // MAJOR.MINOR.BUILD
#define BIOS_VER_MASK 0xffffff

typedef enum {
    BiosError_Ok            =  0,
    BiosError_ConfigError   =  1,
    BiosError_WrongBPP      =  2,
    BiosError_InvalidParam  =  3,
    BiosError_NoInit        =  4,
    BiosError_Calibration   =  5,
    BiosError_Timeout       =  6,
    BiosError_CommError     =  7,
    BiosError_NotSupported  =  8,
} BiosError_t;

typedef enum {
    Res_16bit_rgb565 = 1<<0,
    Res_18bit_rgb666 = 1<<1,
    Res_24bit_rgb888 = 1<<2,
} Resolution_t;

typedef enum {
    FrameRate_Low,
    FrameRate_Normal,
    FrameRate_High,
} FrameRate_t;

typedef struct {
    uint16_t x;
    uint16_t y;
    uint16_t z;
} touch_coordinate_t;

typedef enum {
    TOUCH_IRQ_RISING_EDGE,
    TOUCH_IRQ_FALLING_EDGE,
    TOUCH_IRQ_HIGH_LEVEL,
    TOUCH_IRQ_LOW_LEVEL,
} touch_irq_trigger_t;

typedef void (*delayUsFunc)(int us);
typedef uint32_t (*readTimeMsFunc)(void);

typedef BiosError_t (*initParamFunc)(void* data, uint32_t SystemCoreClock, uint32_t PeripheralClock, delayUsFunc delay, readTimeMsFunc readMs);
typedef BiosError_t (*simpleFunc)(void* data);
typedef BiosError_t (*macFunc)(void* data, char* mac);

typedef BiosError_t (*powerUpFunc)(void* data, void* framebuffer, Resolution_t res, FrameRate_t rate);
typedef BiosError_t (*backlightFunc)(void* data, int percent);
typedef BiosError_t (*infoFuncD)(void* data,
                                 uint16_t* width,
                                 uint16_t* height,
                                 uint16_t* bytesPerPixel,
                                 bool* landscape,
                                 uint16_t* supportedResolutions,
                                 Resolution_t* currentResolution);
typedef BiosError_t (*infoFuncT)(void* data,
                                 bool* supportsTouch,
                                 bool* supportsCalibration,
                                 uint8_t* numPoints);

typedef void (*touchIrqFunc)(uint32_t arg, bool enable, touch_irq_trigger_t trigger);
typedef void (*touchNewDataFunc)(uint32_t arg, touch_coordinate_t* coords, int num);
typedef BiosError_t (*touchInitFunc)(void* data, 
                                     touchIrqFunc irqEnabler, uint32_t enablerArg, 
                                     touchNewDataFunc newData, uint32_t newDataArg);
typedef BiosError_t (*readFunc)(void* data, touch_coordinate_t* coords, int num);
typedef BiosError_t (*nextFunc)(void* data, uint16_t* x, uint16_t* y, bool* last);
typedef BiosError_t (*waitCalibFunc)(void* data, bool* morePoints, uint32_t timeoutMs);
typedef BiosError_t (*spifiFunc)(void* data, uint8_t mfgr, uint8_t devType, uint8_t devID, uint32_t memSize, bool* known, uint32_t* eraseBlockSize);

typedef struct {
  initParamFunc initParams;
  simpleFunc    i2cIRQHandler;
  spifiFunc     spifiIsSupported;
  macFunc       ethernetMac;

  simpleFunc    displayInit;
  powerUpFunc   displayPowerUp;
  simpleFunc    displayPowerDown;
  backlightFunc displayBacklight;
  infoFuncD     displayInformation;

  touchInitFunc touchInit;
  simpleFunc    touchPowerUp;
  simpleFunc    touchPowerDown;
  readFunc      touchRead;
  simpleFunc    touchCalibrateStart;
  nextFunc      touchGetNextCalibPoint;
  waitCalibFunc touchWaitForCalibratePoint;
  simpleFunc    touchIrqHandler;
  infoFuncT     touchInformation;

} bios_header_t;

typedef struct {
  uint32_t magic;
  uint32_t size;
  uint32_t crc;
  uint32_t version;
  uint32_t paramSize;
  uint32_t headerSize;
  bios_header_t header;
} file_header_t;

#endif /* BIOS_H */
