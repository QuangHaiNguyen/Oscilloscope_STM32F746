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
#include <platform/hal/ST/mcu/stm32f7xx/STM32F7HAL_DSI.hpp>
#include <platform/hal/ST/mcu/stm32f7xx/STM32F7DMA.hpp>
#include <touchgfx/lcd/LCD.hpp>
#include <touchgfx/hal/GPIO.hpp>
#include <string.h>
#include <stdio.h>

//NOTE: Second implementation of the class STM32F7HAL_DSI.

extern "C"
{
#include "stm32f7xx.h"
#include "stm32f7xx_hal_dsi.h"
#include "stm32f7xx_hal_ltdc.h"
#include "stm32f7xx_hal_gpio.h"

    //Generate error if no target is defined
#if !defined(STM32F769I_DISCO)
#error Please specify STM32F769I_DISCO or STM32F769I_EVAL
#endif

    extern DSI_HandleTypeDef hdsi_discovery;
}

volatile bool displayRefreshing = false;
volatile bool refreshRequested = true;
static bool doubleBufferingEnabled = false;
static uint16_t* currFbBase = 0;

uint16_t* STM32F7HAL_DSI::getTFTFrameBuffer() const
{
    return currFbBase;
}

void STM32F7HAL_DSI::setFrameBufferStartAddress(void* adr, uint16_t depth, bool useDoubleBuffering, bool useAnimationStorage)
{
    // Make note of whether we are using double buffering.
    doubleBufferingEnabled = useDoubleBuffering;
    currFbBase = (uint16_t*)adr;
    HAL::setFrameBufferStartAddress(adr, depth, useDoubleBuffering, useAnimationStorage);
}

void STM32F7HAL_DSI::setTFTFrameBuffer(uint16_t* adr)
{
    if (doubleBufferingEnabled)
    {
        __HAL_DSI_WRAPPER_DISABLE(&hdsi_discovery);

        LTDC_Layer1->CFBAR = (uint32_t)adr;
        LTDC->SRCR |= LTDC_SRCR_IMR;
        currFbBase = adr;

        __HAL_DSI_WRAPPER_ENABLE(&hdsi_discovery);
    }
}

void STM32F7HAL_DSI::configureInterrupts()
{
    // These two priorities MUST be EQUAL, and MUST be functionally lower than RTOS scheduler interrupts.
    NVIC_SetPriority(DMA2D_IRQn, 7);
    NVIC_SetPriority(DSI_IRQn, 7);
}

/* Enable LCD line interrupt, when entering video (active) area */
void STM32F7HAL_DSI::enableLCDControllerInterrupt()
{
    __HAL_DSI_CLEAR_FLAG(&hdsi_discovery, DSI_IT_ER);
    __HAL_DSI_CLEAR_FLAG(&hdsi_discovery, DSI_IT_TE);
    __HAL_DSI_ENABLE_IT(&hdsi_discovery, DSI_IT_TE);
    __HAL_DSI_ENABLE_IT(&hdsi_discovery, DSI_IT_ER);

    LTDC->IER = 2;
}

void STM32F7HAL_DSI::disableInterrupts()
{
    NVIC_DisableIRQ(DMA2D_IRQn);
    NVIC_DisableIRQ(DSI_IRQn);
}

void STM32F7HAL_DSI::enableInterrupts()
{
    NVIC_EnableIRQ(DMA2D_IRQn);
    NVIC_EnableIRQ(DSI_IRQn);
    NVIC_EnableIRQ(LTDC_ER_IRQn);
}

bool STM32F7HAL_DSI::beginFrame()
{
    //comment out if using double buffering to transfer on every vsync
    refreshRequested = false;
    return HAL::beginFrame();
}

const bool alwaysRefresh = false;

void STM32F7HAL_DSI::endFrame()
{
    HAL::endFrame();
    if (alwaysRefresh || frameBufferUpdatedThisFrame)
    {
        refreshRequested = true;
    }
}

extern "C"
__irq void DSI_IRQHandler(void)
{
    if (__HAL_DSI_GET_IT_SOURCE(&hdsi_discovery, DSI_IT_TE))
    {
        // Tearing effect interrupt. Occurs periodically (every 15.7 ms on 469 eval/disco boards)
        __HAL_DSI_CLEAR_FLAG(&hdsi_discovery, DSI_IT_TE);
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

            // Transfer pixel data.
            HAL_DSI_Refresh(&hdsi_discovery);
            displayRefreshing = true;
        }
        else
        {
            GPIO::clear(GPIO::VSYNC_FREQ);
        }
    }
    if (__HAL_DSI_GET_IT_SOURCE(&hdsi_discovery, DSI_IT_ER))
    {
        // End-of-refresh interrupt. Meaning one of the 4 regions have been transferred.
        __HAL_DSI_CLEAR_FLAG(&hdsi_discovery, DSI_IT_ER);

        GPIO::clear(GPIO::VSYNC_FREQ);
        displayRefreshing = false;
        if (HAL::getInstance())
        {
            // Signal to the framework that display update has finished.
            HAL::getInstance()->frontPorchEntered();
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
volatile int overrunCnt = 0;
extern "C"
__irq void LTDC_ER_IRQHandler(void)
{
    if (LTDC->ISR & 2)
    {
        LTDC->ICR = 2;
        overrunCnt++;
    }
}
