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
#include <platform/hal/nxp/mcu/LPC17xx/NXP1788HAL.hpp>
#include <touchgfx/hal/OSWrappers.hpp>
#include <touchgfx/lcd/LCD.hpp>

#include "LPC1788.h"

uint16_t* NXP1788HAL::getTFTFrameBuffer() const
{
    return (uint16_t*)LPC_LCD->UPBASE;
}

void NXP1788HAL::setTFTFrameBuffer(uint16_t* adr)
{
    LPC_LCD->UPBASE = (uint32_t)adr;
}

void NXP1788HAL::configureInterrupts()
{
    NVIC_SetPriority(DMA_IRQn, 9);
    NVIC_SetPriority(LCD_IRQn, 9);
}
void NXP1788HAL::enableLCDControllerInterrupt()
{
    LPC_LCD->INTMSK = 8;
}

void NXP1788HAL::disableInterrupts()
{
    NVIC_DisableIRQ(DMA_IRQn);
    NVIC_DisableIRQ(LCD_IRQn);
}

void NXP1788HAL::enableInterrupts()
{
    NVIC_EnableIRQ(DMA_IRQn);
    NVIC_EnableIRQ(LCD_IRQn);
}


static bool LCD_HasIntPorch()
{
    return ((LPC_LCD->CTRL & 0x1000) > 0);
}

static void LCD_SetNextIntPorch()
{
    uint32_t val = LPC_LCD->CTRL & 0xFFFFCFFF;
    LPC_LCD->CTRL = val | (3 << 12);
}

static void LCD_SetNextIntVideo()
{
    uint32_t val = LPC_LCD->CTRL & 0xFFFFCFFF;
    LPC_LCD->CTRL = val;// | (2<<12);
}


extern "C"
__irq void LCD_IRQHandler(void)
{
    if (LPC_LCD->INTMSK & 8)
    {
        LPC_LCD->INTCLR = 8;

        if (LCD_HasIntPorch())
        {
            LCD_SetNextIntVideo();
            touchgfx::HAL::getInstance()->frontPorchEntered();
        }
        else
        {
            LCD_SetNextIntPorch();
            HAL::getInstance()->vSync();
            touchgfx::OSWrappers::signalVSync();
            // Swap frame buffers immediately instead of waiting for the task to be scheduled in.
            // Note: task will also swap when it wakes up, but that operation is guarded and will not have
            // any effect if already swapped.
            HAL::getInstance()->swapFrameBuffers();
        }
    }
}

extern "C"
__irq void GPDMA_IRQHandler(void)
{
    touchgfx::HAL::getInstance()->signalDMAInterrupt();
}
