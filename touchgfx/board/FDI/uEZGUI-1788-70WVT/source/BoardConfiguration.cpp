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

#include <platform/hal/nxp/mcu/LPC17xx/NXP1788HAL.hpp>
#include <platform/hal/nxp/mcu/LPC17xx/NXP1788DMA.hpp>
#include <platform/driver/lcd/LCD16bpp.hpp>

#include "bsp/ResistiveTouchController.hpp"
#include "vendor/FDI_LCD.hpp"
#include "vendor/FDI_RTC.hpp"
#include "vendor/FDI_Touch.hpp"
#include "vendor/FDI_Memory.hpp"

#include "LPC1788.h"

extern "C"
{
#include "lpc177x_8x_pinsel.h"
#include "lpc177x_8x_gpio.h"
}

static const uint16_t DISPLAY_NATIVE_WIDTH  = 800;
static const uint16_t DISPLAY_NATIVE_HEIGHT = 480;

//Linked list memory for scatter-gather DMA. Must contain same number of elements as the height of the display.
GPDMA_LLI_Type lliMemory[DISPLAY_NATIVE_HEIGHT];

NXP1788DMA dma(lliMemory);
ResistiveTouchController tc;
LCD16bpp display;

namespace touchgfx
{
void touchgfx_init()
{
    HAL& hal = touchgfx_generic_init<NXP1788HAL>(dma, display, tc, DISPLAY_NATIVE_WIDTH, DISPLAY_NATIVE_HEIGHT, 0, 0);
    hal.setFrameBufferStartAddress((uint16_t*)0xA0000000);
    hal.setFingerSize(1);

    // By default frame rate compensation is off.
    // Enable frame rate compensation to smooth out animations in case there is periodic slow frame rates.
    hal.setFrameRateCompensation(false);
}
void hw_init()
{
    FDI_SDRAM_Init();
    FDI_NOR_Init();
    FDI_LCD_Init();
    FDI_Touch_Init();
    FDI_RTC_Init();

    //SPKR
    PINSEL_ConfigPin(2, 1, 0);
    PINSEL_SetPinMode(2, 1, PINSEL_BASICMODE_PLAINOUT);
    GPIO_SetDir(2, (1 << 1), 1);
    GPIO_ClearValue(2, (1 << 1));

    //AMP_MODE
    PINSEL_ConfigPin(2, 5, 0);
    PINSEL_SetPinMode(2, 5, PINSEL_BASICMODE_PLAINOUT);
    GPIO_SetDir(2, (1 << 5), 1);
    GPIO_ClearValue(2, (1 << 5));
}
}
