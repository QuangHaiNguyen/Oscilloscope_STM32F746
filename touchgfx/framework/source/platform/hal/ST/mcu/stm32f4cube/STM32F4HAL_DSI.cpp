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
#include <touchgfx/hal/OSWrappers.hpp>
#include <platform/hal/ST/mcu/stm32f4cube/STM32F4HAL_DSI.hpp>
#include <platform/hal/ST/mcu/stm32f4cube/STM32F4DMA.hpp>
#include <touchgfx/lcd/LCD.hpp>
#include <touchgfx/hal/GPIO.hpp>
#include <string.h>
#include <stdio.h>

/**
 * About this implementation:
 * This class is for use ONLY with the DSI peripheral. If you have a regular RGB interface display, use the STM32F4HAL.cpp class instead.
 *
 * This implementation assumes that the DSI is configured to be in adapted command mode, with tearing effect set to external pin.
 * Display will only be updated when there has actually been changes to the frame buffer.
 */

extern "C"
{
#include "stm32f4xx.h"
#include "stm32f4xx_hal_dsi.h"
#include "stm32f4xx_hal_ltdc.h"
#include "stm32f4xx_hal_gpio.h"
    extern DSI_HandleTypeDef hdsi_eval;

    /* Request tear interrupt at specific scanline. Implemented in BoardConfiguration.cpp */
    void LCD_ReqTear();

    /* Configures display to update indicated region of the screen (200pixel wide chunks) - 16bpp mode */
    void LCD_SetUpdateRegion(int idx);

    /* Configures display to update left half of the screen. Implemented in BoardConfiguration.cpp  - 24bpp mode*/
    void LCD_SetUpdateRegionLeft();

    /* Configures display to update right half of the screen. Implemented in BoardConfiguration.cpp - 24bpp mode*/
    void LCD_SetUpdateRegionRight();
}

volatile bool displayRefreshing = false;
volatile bool refreshRequested = true;
static bool doubleBufferingEnabled = false;
static uint16_t* currFbBase = 0;
static int updateRegion = 0;
static uint16_t bitDepth = 0;

uint16_t* STM32F4HAL_DSI::getTFTFrameBuffer() const
{
    return currFbBase;
}

void STM32F4HAL_DSI::setFrameBufferStartAddress(void* adr, uint16_t depth, bool useDoubleBuffering, bool useAnimationStorage)
{
    // Make note of whether we are using double buffering.
    doubleBufferingEnabled = useDoubleBuffering;
    currFbBase = (uint16_t*)adr;
    bitDepth = depth;
    HAL::setFrameBufferStartAddress(adr, depth, useDoubleBuffering, useAnimationStorage);
}

void STM32F4HAL_DSI::setTFTFrameBuffer(uint16_t* adr)
{
    if (doubleBufferingEnabled)
    {
        __HAL_DSI_WRAPPER_DISABLE(&hdsi_eval);
        LTDC_Layer1->CFBAR = (uint32_t)adr;
        LTDC->SRCR |= LTDC_SRCR_IMR;
        currFbBase = adr;
        __HAL_DSI_WRAPPER_ENABLE(&hdsi_eval);
    }
}

void STM32F4HAL_DSI::configureInterrupts()
{
    // These two priorities MUST be EQUAL, and MUST be functionally lower than RTOS scheduler interrupts.
    NVIC_SetPriority(DMA2D_IRQn, 7);
    NVIC_SetPriority(DSI_IRQn, 7);
}

/* Enable LCD line interrupt, when entering video (active) area */
void STM32F4HAL_DSI::enableLCDControllerInterrupt()
{
    LCD_ReqTear();

    __HAL_DSI_CLEAR_FLAG(&hdsi_eval, DSI_IT_ER);
    __HAL_DSI_CLEAR_FLAG(&hdsi_eval, DSI_IT_TE);
    __HAL_DSI_ENABLE_IT(&hdsi_eval, DSI_IT_TE);
    __HAL_DSI_ENABLE_IT(&hdsi_eval, DSI_IT_ER);

}

void STM32F4HAL_DSI::disableInterrupts()
{
    NVIC_DisableIRQ(DMA2D_IRQn);
    NVIC_DisableIRQ(DSI_IRQn);
}

void STM32F4HAL_DSI::enableInterrupts()
{
    NVIC_EnableIRQ(DMA2D_IRQn);
    NVIC_EnableIRQ(DSI_IRQn);
}

bool STM32F4HAL_DSI::beginFrame()
{
    refreshRequested = false;
    return HAL::beginFrame();
}

void STM32F4HAL_DSI::endFrame()
{
    HAL::endFrame();
    if (frameBufferUpdatedThisFrame)
    {
        refreshRequested = true;
    }
}

extern "C"
__irq void DSI_IRQHandler(void)
{
    if (__HAL_DSI_GET_IT_SOURCE(&hdsi_eval, DSI_IT_TE))
    {
        // Tearing effect interrupt. Occurs periodically (every 15.7 ms on 469 eval/disco boards)
        __HAL_DSI_CLEAR_FLAG(&hdsi_eval, DSI_IT_TE);
        GPIO::set(GPIO::VSYNC_FREQ);
        HAL::getInstance()->vSync();
        OSWrappers::signalVSync();
        if (!doubleBufferingEnabled && HAL::getInstance())
        {
            // In single buffering, only require that the system waits for display update to be finished if we
            // actually intend to update the display in this frame.
            HAL::getInstance()->lockDMAToFrontPorch(refreshRequested);
        }

        if (refreshRequested && !displayRefreshing)
        {
            // We have an update pending.
            if (doubleBufferingEnabled && HAL::getInstance())
            {
                // Swap frame buffers immediately instead of waiting for the task to be scheduled in.
                // Note: task will also swap when it wakes up, but that operation is guarded and will not have
                // any effect if already swapped.
                HAL::getInstance()->swapFrameBuffers();
            }

            // Update region 0 = first area of display (First quarter for 16bpp, first half for 24bpp)
            updateRegion = 0;

            //Set update region based on bit depth of framebuffer. 16pp or 24bpp.
            if (bitDepth == 24)
            {
                LCD_SetUpdateRegionLeft();
            }
            //Default to 16 bpp
            else
            {
                LCD_SetUpdateRegion(updateRegion);
            }

            // Transfer a quarter screen of pixel data.
            HAL_DSI_Refresh(&hdsi_eval);
            displayRefreshing = true;
        }
        else
        {
            GPIO::clear(GPIO::VSYNC_FREQ);
        }
    }
    if (__HAL_DSI_GET_IT_SOURCE(&hdsi_eval, DSI_IT_ER))
    {
        // End-of-refresh interrupt. Meaning one of the 4 regions have been transferred.
        __HAL_DSI_CLEAR_FLAG(&hdsi_eval, DSI_IT_ER);

        if (bitDepth == 24)
        {
            if (updateRegion == 0)
            {
                // If we transferred the left half, also transfer right half.
                DSI->WCR &= ~(DSI_WCR_DSIEN);
                LTDC_Layer1->CFBAR = ((uint32_t)currFbBase) + ((HAL::FRAME_BUFFER_WIDTH / 2) * 3);
                LTDC->SRCR = (uint32_t)LTDC_SRCR_IMR;
                LCD_SetUpdateRegionRight();
                DSI->WCR |= DSI_WCR_DSIEN;
                updateRegion = 1;
                HAL_DSI_Refresh(&hdsi_eval);
            }
            else
            {
                // Otherwise we are done refreshing.
                DSI->WCR &= ~(DSI_WCR_DSIEN);
                LTDC_Layer1->CFBAR = (uint32_t)currFbBase;
                LTDC->SRCR = (uint32_t)LTDC_SRCR_IMR;
                LCD_SetUpdateRegionLeft();
                DSI->WCR |= DSI_WCR_DSIEN;
                GPIO::clear(GPIO::VSYNC_FREQ);

                displayRefreshing = false;
                if (HAL::getInstance())
                {
                    // Signal to the framework that display update has finished.
                    HAL::getInstance()->frontPorchEntered();
                }
            }
        }
        //Default to 16bpp
        else
        {

            updateRegion++;
            if (updateRegion < 4)
            {
                DSI->WCR &= ~(DSI_WCR_DSIEN);
                LTDC_Layer1->CFBAR = ((uint32_t)currFbBase) + HAL::FRAME_BUFFER_WIDTH / 2 * updateRegion;
                LTDC->SRCR = (uint32_t)LTDC_SRCR_IMR;
                LCD_SetUpdateRegion(updateRegion);
                DSI->WCR |= DSI_WCR_DSIEN;
                HAL_DSI_Refresh(&hdsi_eval);
            }
            else
            {
                DSI->WCR &= ~(DSI_WCR_DSIEN);
                LTDC_Layer1->CFBAR = (uint32_t)currFbBase;
                LTDC->SRCR = (uint32_t)LTDC_SRCR_IMR;
                LCD_SetUpdateRegion(0);
                DSI->WCR |= DSI_WCR_DSIEN;
                GPIO::clear(GPIO::VSYNC_FREQ);

                displayRefreshing = false;
                if (HAL::getInstance())
                {
                    // Signal to the framework that display update has finished.
                    HAL::getInstance()->frontPorchEntered();
                }
            }
        }
    }
}

extern "C"
__irq void DMA2D_IRQHandler(void)
{
    if (DMA2D->ISR & 2)
    {
        DMA2D->IFCR = 2;
        touchgfx::HAL::getInstance()->signalDMAInterrupt();
    }
}
