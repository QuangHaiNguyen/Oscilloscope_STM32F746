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

#include <platform/hal/nxp/mcu/LPC40xx/NXP4088HAL.hpp>
#include <platform/hal/nxp/mcu/LPC40xx/NXP4088DMA.hpp>
#include <platform/driver/lcd/LCD16bpp.hpp>
#include <touchgfx/hal/GPIO.hpp>
#include <platform/core/arm/cortex-m/CortexMMCUInstrumentation.hpp>

extern "C" {
#include "bsp/DMTouchController.hpp"
#include "vendor/bios_loader.h"
#include "vendor/display.h"
#include "vendor/touch.h"

#include "board.h"
}

#define MAX_DISPLAY_NATIVE_HEIGHT  (480)

//Linked list memory for scatter-gather DMA. Must contain at least same number of
//elements as the height of the display.
DMA_TransferDescriptor_t lliMemory[MAX_DISPLAY_NATIVE_HEIGHT];

CortexMMCUInstrumentation mcuInst;

NXP4088DMA dma(lliMemory);
DMTouchController tc;
LCD16bpp display;

namespace touchgfx
{
void touchgfx_init()
{
    uint16_t* fb = (uint16_t*)SDRAM_ADDR_FB1;
    uint32_t size = disp_fbSize() * 3; // room for double buffering and animation buffer
    memset(fb, 0x00, size); // make buffer black before stuff is drawn on it
    if (disp_width() == 800)
    {
        disp_powerUp(fb, Res_16bit_rgb565, FrameRate_Low);
    }
    else
    {
        disp_powerUp(fb, Res_16bit_rgb565, FrameRate_Normal);
    }

    GPIO::init();

    if (disp_height() > MAX_DISPLAY_NATIVE_HEIGHT)
    {
        DEBUGOUT("ERROR: The display is %dx%d but MAX_DISPLAY_NATIVE_HEIGHT is %d\n", disp_width(), disp_height(), MAX_DISPLAY_NATIVE_HEIGHT);
        DEBUGSTR("ERROR: Modify MAX_DISPLAY_NATIVE_HEIGHT in "__FILE__"!\n");
        memset(fb, 0xe0, size); // 0xe0e0 is red
        while (1)
        {
        }
    }

    DEBUGOUT("Display powered up, framebuffer 0x%08x\n", (uint32_t)fb);
    HAL& hal = touchgfx_generic_init<NXP4088HAL>(dma, display, tc, disp_width(), disp_height(), 0, 0);
    hal.setFrameBufferStartAddress(fb);
    hal.lockDMAToFrontPorch(false); //DMA is now no longer tied to the TFT controller.
    hal.setFingerSize(1);

    // By default frame rate compensation is off.
    // Enable frame rate compensation to smooth out animations in case there is periodic slow frame rates.
    hal.setFrameRateCompensation(false);

    mcuInst.init();
    hal.setMCUInstrumentation(&mcuInst);
    hal.enableMCULoadCalculation(true);
}
void hw_init()
{
    Board_SystemInit();
    Board_Init();
    do
    {
        BiosLoaderError_t err;
        err = bios_init();
        if (err != BiosLoaderError_Ok)
        {
            DEBUGOUT("Failed to init BIOS, got error %d\n", err);
            break;
        }
        err = disp_init();
        if (err != BiosLoaderError_Ok)
        {
            DEBUGOUT("Failed to init the display, got error %d\n", err);
            break;
        }
        err = touch_init();
        if (err != BiosLoaderError_Ok)
        {
            DEBUGOUT("Failed to init the touch controller, got error %d\n", err);
            break;
        }
        DEBUGSTR("\n\n***********\n\n");
        DEBUGSTR("hw_init() completed!\n");
    }
    while (false);
}
}
