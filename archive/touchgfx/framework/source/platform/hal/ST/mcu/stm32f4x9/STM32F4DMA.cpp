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
#include <touchgfx/hal/HAL.hpp>
#include <platform/hal/ST/mcu/stm32f4x9/STM32F4DMA.hpp>
#include <touchgfx/lcd/LCD.hpp>
#include <cassert>
#include "stm32f4xx_dma2d.h"

STM32F4DMA::STM32F4DMA()
    : touchgfx::DMA_Interface(dma_queue), dma_queue(queue_storage, sizeof(queue_storage) / sizeof(queue_storage[0])),
      pBlitOp(0), numLines(0), deadTime(0), use32Bit(0)
{}

STM32F4DMA::~STM32F4DMA()
{
}

void STM32F4DMA::initialize()
{
    DMA2D_ITConfig(DMA2D_IT_TC, ENABLE);
}

volatile bool disableChromArt = false;

touchgfx::BlitOperations STM32F4DMA::getBlitCaps()
{
    if (disableChromArt)
    {
        return static_cast<touchgfx::BlitOperations>(0);
    }
    else
    {
        return static_cast<touchgfx::BlitOperations>(touchgfx::BLIT_OP_FILL | touchgfx::BLIT_OP_COPY | touchgfx::BLIT_OP_COPY_WITH_ALPHA | touchgfx::BLIT_OP_COPY_ARGB8888 | touchgfx::BLIT_OP_COPY_ARGB8888_WITH_ALPHA | touchgfx::BLIT_OP_COPY_A4 | touchgfx::BLIT_OP_COPY_A8);
    }
}

void STM32F4DMA::setupDataCopy(const touchgfx::BlitOp& blitOp)
{
    uint32_t dma2dTransferMode = DMA2D_M2M_BLEND;  //DMA2D transfer mode - Default blending.
    uint32_t dma2dColorMode = 0;                   //DMA2D foreground color mode.

    bool blendingImage   = ((blitOp.operation == touchgfx::BLIT_OP_COPY_ARGB8888) || (blitOp.operation == touchgfx::BLIT_OP_COPY_ARGB8888_WITH_ALPHA) || (blitOp.operation == BLIT_OP_COPY_WITH_ALPHA));
    bool blendingText    = ((blitOp.operation == touchgfx::BLIT_OP_COPY_A4)       || (blitOp.operation == touchgfx::BLIT_OP_COPY_A8));

    uint8_t bitDepth = HAL::lcd().bitDepth();

    //Determine color mode and transfer mode
    switch (blitOp.operation)
    {
    case touchgfx::BLIT_OP_COPY_A4:
        dma2dColorMode = CM_A4;
        break;
    case touchgfx::BLIT_OP_COPY_A8:
        dma2dColorMode = CM_A8;
        break;
    case BLIT_OP_COPY_WITH_ALPHA:
        dma2dTransferMode = DMA2D_M2M_BLEND;
        dma2dColorMode = (bitDepth == 16) ? CM_RGB565 : CM_RGB888;
        break;
    case touchgfx::BLIT_OP_COPY_ARGB8888:
    case touchgfx::BLIT_OP_COPY_ARGB8888_WITH_ALPHA:
        dma2dColorMode = CM_ARGB8888;
        break;
    default:
        dma2dTransferMode = DMA2D_M2M;
        dma2dColorMode = (bitDepth == 16) ? CM_RGB565 : CM_RGB888;
        break;
    }

    numLines = blitOp.nLoops;

    DMA2D_InitTypeDef DMA2D_InitStruct;
    NVIC_DisableIRQ(DMA2D_IRQn);

    DMA2D_DeInit();
    DMA2D_InitStruct.DMA2D_Mode = dma2dTransferMode;
    DMA2D_InitStruct.DMA2D_CMode = (bitDepth == 16) ? DMA2D_RGB565 : DMA2D_RGB888;
    DMA2D_InitStruct.DMA2D_OutputGreen = 0;
    DMA2D_InitStruct.DMA2D_OutputBlue = 0;
    DMA2D_InitStruct.DMA2D_OutputRed = 0;
    DMA2D_InitStruct.DMA2D_OutputAlpha = 0;
    DMA2D_InitStruct.DMA2D_OutputMemoryAdd = (unsigned int)blitOp.pDst;
    DMA2D_InitStruct.DMA2D_OutputOffset = blitOp.dstLoopStride - blitOp.nSteps;
    DMA2D_InitStruct.DMA2D_NumberOfLine = blitOp.nLoops;
    DMA2D_InitStruct.DMA2D_PixelPerLine = blitOp.nSteps;
    DMA2D_Init(&DMA2D_InitStruct);
    DMA2D_ITConfig(DMA2D_IT_TC, ENABLE);

    //Configure foreground
    DMA2D_FG_InitTypeDef DMA2D_FG_InitStruct;
    DMA2D_FG_StructInit(&DMA2D_FG_InitStruct);
    DMA2D_FG_InitStruct.DMA2D_FGMA = (unsigned int)blitOp.pSrc;

    DMA2D_FG_InitStruct.DMA2D_FGCM = dma2dColorMode;
    DMA2D_FG_InitStruct.DMA2D_FG_CLUT_CM = CLUT_CM_ARGB8888; // Note: CLUT not used
    DMA2D_FG_InitStruct.DMA2D_FGO = blitOp.srcLoopStride - blitOp.nSteps;

    if (blendingImage || blendingText)
    {
        //Adjust blending for configured alpha value
        if (blitOp.alpha < 255)
        {
            DMA2D_FG_InitStruct.DMA2D_FGPFC_ALPHA_MODE = COMBINE_ALPHA_VALUE;
            DMA2D_FG_InitStruct.DMA2D_FGPFC_ALPHA_VALUE = blitOp.alpha;
        }
        else
        {
            DMA2D_FG_InitStruct.DMA2D_FGPFC_ALPHA_MODE = NO_MODIF_ALPHA_VALUE;
        }

        if (blendingText)
        {
            if (bitDepth == 16)
            {
                DMA2D_FG_InitStruct.DMA2D_FGC_RED   = (((blitOp.color & 0xF800) >> 11) * 255) / 31;
                DMA2D_FG_InitStruct.DMA2D_FGC_GREEN = (((blitOp.color & 0x7E0) >> 5) * 255) / 63;
                DMA2D_FG_InitStruct.DMA2D_FGC_BLUE  = (((blitOp.color & 0x1F)) * 255) / 31;
            }
            else
            {
                uint32_t col = blitOp.color.getColor32();
                DMA2D_FG_InitStruct.DMA2D_FGC_RED   = (col >> 16) & 0xFF;
                DMA2D_FG_InitStruct.DMA2D_FGC_GREEN = (col >> 8) & 0xFF;
                DMA2D_FG_InitStruct.DMA2D_FGC_BLUE  = (col & 0xFF);
            }
        }

        //Configure background
        DMA2D_BG_InitTypeDef DMA2D_BG_InitStruct;
        DMA2D_BG_StructInit(&DMA2D_BG_InitStruct);

        /*!< Initialize the DMA2D background memory address */
        DMA2D_BG_InitStruct.DMA2D_BGMA = DMA2D_InitStruct.DMA2D_OutputMemoryAdd;

        /*!< Initialize the DMA2D background offset */
        DMA2D_BG_InitStruct.DMA2D_BGO = DMA2D_InitStruct.DMA2D_OutputOffset;

        /*!< Initialize the DMA2D background color mode */
        DMA2D_BG_InitStruct.DMA2D_BGCM = (bitDepth == 16) ? CM_RGB565 : CM_RGB888;

        /*!< Initialize the DMA2D background CLUT color mode */
        DMA2D_BG_InitStruct.DMA2D_BG_CLUT_CM = CLUT_CM_ARGB8888; // Note: CLUT not used

        DMA2D_BGConfig(&DMA2D_BG_InitStruct);
    }

    DMA2D_FGConfig(&DMA2D_FG_InitStruct);
    if (deadTime)
    {
        DMA2D_DeadTimeConfig(deadTime, ENABLE);
    }
    NVIC_EnableIRQ(DMA2D_IRQn);

    DMA2D_StartTransfer();
}

void STM32F4DMA::setupDataFill(const touchgfx::BlitOp& blitOp)
{
    uint8_t bitDepth = HAL::lcd().bitDepth();
    uint32_t dma2dTransferMode = DMA2D_R2M;
    uint32_t dma2dColorMode = (bitDepth == 16) ? CM_RGB565 : CM_RGB888;

    DMA2D_InitTypeDef DMA2D_InitStruct;
    NVIC_DisableIRQ(DMA2D_IRQn);

    DMA2D_DeInit();
    DMA2D_InitStruct.DMA2D_Mode = dma2dTransferMode;
    DMA2D_InitStruct.DMA2D_CMode = (bitDepth == 16) ? DMA2D_RGB565 : DMA2D_RGB888;

    uint32_t red = 0;
    uint32_t green = 0;
    uint32_t blue = 0;
    uint32_t alpha = blitOp.alpha;;

    if (bitDepth == 16)
    {
        // Do not scale to [0..255] according to DMA2D_InitTypeDef
        red   = (blitOp.color & 0xF800) >> 11;
        green = (blitOp.color & 0x7E0) >> 5;
        blue  = (blitOp.color & 0x1F);
    }
    else
    {
        red   = (blitOp.color.getColor32() & 0xFF0000) >> 16;
        green = (blitOp.color.getColor32() & 0xFF00) >> 8;
        blue  = blitOp.color.getColor32() & 0xFF;
    }

    DMA2D_InitStruct.DMA2D_OutputGreen = green;
    DMA2D_InitStruct.DMA2D_OutputBlue = blue;
    DMA2D_InitStruct.DMA2D_OutputRed = red;
    DMA2D_InitStruct.DMA2D_OutputAlpha = alpha;
    DMA2D_InitStruct.DMA2D_OutputMemoryAdd = (unsigned int)blitOp.pDst;
    DMA2D_InitStruct.DMA2D_OutputOffset = blitOp.dstLoopStride - blitOp.nSteps;
    DMA2D_InitStruct.DMA2D_NumberOfLine = blitOp.nLoops;
    DMA2D_InitStruct.DMA2D_PixelPerLine = blitOp.nSteps;
    DMA2D_Init(&DMA2D_InitStruct);
    DMA2D_ITConfig(DMA2D_IT_TC, ENABLE);

    //Configure foreground
    DMA2D_FG_InitTypeDef DMA2D_FG_InitStruct;
    DMA2D_FG_StructInit(&DMA2D_FG_InitStruct);
    DMA2D_FG_InitStruct.DMA2D_FGMA = 0;

    DMA2D_FG_InitStruct.DMA2D_FGCM = dma2dColorMode;
    DMA2D_FG_InitStruct.DMA2D_FG_CLUT_CM = CLUT_CM_ARGB8888; //Note: CLUT not used

    DMA2D_FGConfig(&DMA2D_FG_InitStruct);
    if (deadTime)
    {
        DMA2D_DeadTimeConfig(deadTime, ENABLE);
    }
    NVIC_EnableIRQ(DMA2D_IRQn);

    DMA2D_StartTransfer();
}
