/******************************************************************************
 *
 * @brief     This file is part of the TouchGFX 4.7.0 evaluation distribution.
 *
 * @author    Draupner Graphics A/S <http://www.touchgfx.com>
 *
 ******************************************************************************
 *
 * @section Copyright
 *
 * Copyright (C) 2014-2016 Draupner Graphics A/S <http://www.touchgfx.com>.
 * All rights reserved.
 *
 * TouchGFX is protected by international copyright laws and the knowledge of
 * this source code may not be used to write a similar product. This file may
 * only be used in accordance with a license and should not be re-
 * distributed in any way without the prior permission of Draupner Graphics.
 *
 * This is licensed software for evaluation use, any use must strictly comply
 * with the evaluation license agreement provided with delivery of the
 * TouchGFX software.
 *
 * The evaluation license agreement can be seen on www.touchgfx.com
 *
 * @section Disclaimer
 *
 * DISCLAIMER OF WARRANTY/LIMITATION OF REMEDIES: Draupner Graphics A/S has
 * no obligation to support this software. Draupner Graphics A/S is providing
 * the software "AS IS", with no express or implied warranties of any kind,
 * including, but not limited to, any implied warranties of merchantability
 * or fitness for any particular purpose or warranties against infringement
 * of any proprietary rights of a third party.
 *
 * Draupner Graphics A/S can not be held liable for any consequential,
 * incidental, or special damages, or any other relief, or for any claim by
 * any third party, arising from your use of this software.
 *
 *****************************************************************************/
#include <common/TouchGFXInit.hpp>
#include <touchgfx/hal/BoardConfiguration.hpp>

#include <platform/hal/nxp/mcu/LPC43xx/NXP43XXDMA.hpp>
#include <platform/hal/nxp/mcu/LPC43xx/NXP43XXHAL.hpp>

#include <touchgfx/hal/OSWrappers.hpp>
#include <touchgfx/lcd/LCD.hpp>
#include <touchgfx/hal/LED.hpp>
#include <touchgfx/hal/Buttons.hpp>
#include <touchgfx/hal/GPIO.hpp>
#include <stdio.h>
#include <string.h>

//driver includes
#include "platform/driver/touch/ST1232TouchController.hpp"
#include "platform/hal/nxp/mcu/LPC43xx/driver/i2c/I2C_LPC43xx.hpp"
#include "KeySampler.hpp"
#include <platform/core/arm/cortex-m/CortexMMCUInstrumentation.hpp>
#include <platform/driver/lcd/LCD16bpp.hpp>
#include "led_config.h"

/* SDRAM Address Base for DYCS0*/
#define SDRAM_BASE              0x28000000

extern "C"
{
    void Board_Init();
}

volatile int overrideBlitCaps = 0;

//Native display width and height
static const uint16_t DISPLAY_NATIVE_WIDTH  = 640;
static const uint16_t DISPLAY_NATIVE_HEIGHT = 480;

//Linked list memory for scatter-gather DMA. Must contain same number of elements as the height of the display.
GPDMA_LLI_Type lliMemory[DISPLAY_NATIVE_HEIGHT];
//16bpp rendering mode
LCD16bpp display;
//Initialize DMA driver with native display height
NXP43XXDMA dma(lliMemory);
//Initialize I2c channel 0
I2C_LPC43xx i2c(0);
//Define touch controller with dimensions of display
ST1232TouchController tc(i2c, DISPLAY_NATIVE_WIDTH, DISPLAY_NATIVE_HEIGHT, FLIP_X_AXIS);
//Keysampler for buttons
KeySampler btnCtrl;
//MCU instrumentation for performance monitoring
CortexMMCUInstrumentation mcuInstr;

namespace touchgfx
{
void touchgfx_init()
{
    //Instantiate HAL
    HAL& hal = touchgfx_generic_init<NXP43XXHAL>(dma, display, tc, DISPLAY_NATIVE_WIDTH, DISPLAY_NATIVE_HEIGHT, (uint16_t*)(SDRAM_BASE + (0xBB800 * 3)), 0xFFFFFFFF);
    hal.setFrameBufferStartAddress((uint16_t*)SDRAM_BASE);
    hal.setButtonController(&btnCtrl);
    hal.setTouchSampleRate(2);
    hal.setFingerSize(1);

    // By default frame rate compensation is off.
    // Enable frame rate compensation to smooth out animations in case there is periodic slow frame rates.
    hal.setFrameRateCompensation(false);

    // This platform can handle simultaneous DMA and TFT accesses to SDRAM, so disable lock to increase performance.
    hal.lockDMAToFrontPorch(false);

    //Set MCU instrumentation and Load calculation
    mcuInstr.init();
    hal.setMCUInstrumentation(&mcuInstr);
    hal.enableMCULoadCalculation(true);
}

void hw_init()
{
    //Initialize board
    Board_Init();

    // Init LEDs
    touchgfx::LED::init();

    //Turn on LEDs
    touchgfx::LED::on(M4_RED);
    touchgfx::LED::on(M4_YELLOW);
    touchgfx::LED::on(M4_GREEN);
    touchgfx::LED::on(M0_RED);
    touchgfx::LED::on(M0_GREEN);

    //For performance measurement purposes
    GPIO::init();
}
}
