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
#include <platform/hal/nxp/mcu/LPC40xx/NXP4088DMA.hpp>

#include <cassert>

#include "board.h"


NXP4088DMA::NXP4088DMA(DMA_TransferDescriptor_t* lliMemory)
    : DMA_Interface(dma_queue), MyLLI(lliMemory), dma_queue(queue_storage, (sizeof(queue_storage) / sizeof(BlitOp))),
      pBlitOp(0), numLines(0)
{
}


NXP4088DMA::~NXP4088DMA()
{
}


void NXP4088DMA::initialize()
{
    Chip_GPDMA_Init(LPC_GPDMA);

    /* Setup channel with given parameter */
    NVIC_EnableIRQ(DMA_IRQn);
}


volatile int linecount = 0;
volatile int completedcount = 0;
volatile int dmabug = 0;

void NXP4088DMA::signalDMAInterrupt(void)
{
    if (LPC_GPDMA->INTTCSTAT & 1)
    {
        LPC_GPDMA->INTTCCLEAR = 1;
        linecount++;
        if (((LPC_GPDMA->INTSTAT & 1) == 0) && LPC_GPDMA->CH[0].LLI == 0)
        {
            completedcount++;
            if (completedcount == 1)
            {
                executeCompleted();
            }
        }
        if (numLines > 0)
        {
            numLines--;
        }
    }
    if (LPC_GPDMA->INTERRSTAT & 1)
    {
        LPC_GPDMA->INTERRCLR = 1;
        dmabug = 1;
    }
}


void NXP4088DMA::setupDataCopy(const BlitOp& blitOp)
{
    numLines = blitOp.nLoops;
    while (LPC_GPDMA->ENBLDCHNS) ;
    GPDMA_CH_CFG_T GPDMACfg;

    unsigned use32Bit = (blitOp.nSteps % 2) == 0 && // Pixels pr line is even
                        ((unsigned long)blitOp.pSrc & 3) == 0 && // src-address is 32bit-aligned
                        ((unsigned long)blitOp.pDst & 3) == 0 && // dst-address us 32bit-aligned
                        (blitOp.srcLoopStride % 2) == 0 &&   // src-stride is even
                        (blitOp.dstLoopStride % 2) == 0;     // dst-stride is even

    completedcount = 0;
    linecount = 0;
    GPDMACfg.ChannelNum = 0;
    /* Source memory */
    GPDMACfg.SrcAddr = (uint32_t)blitOp.pSrc;
    /* Destination memory */
    GPDMACfg.DstAddr = (unsigned int)blitOp.pDst;
    /* Transfer type */
    GPDMACfg.TransferType = GPDMA_TRANSFERTYPE_M2M_CONTROLLER_DMA;

    if (use32Bit)
    {
        /* Transfer size */
        GPDMACfg.TransferSize = blitOp.nSteps / 2;
        /* Transfer width */
        GPDMACfg.TransferWidth = GPDMA_WIDTH_WORD;
    }
    else
    {
        /* Transfer size */
        GPDMACfg.TransferSize = blitOp.nSteps;
        /* Transfer width */
        GPDMACfg.TransferWidth = GPDMA_WIDTH_HALFWORD;
    }

    //if (blitOp.nLoops > 1)
    {
        int i = 0;
        unsigned int dest = GPDMACfg.DstAddr;
        unsigned int src = GPDMACfg.SrcAddr;

        for (; i < blitOp.nLoops; i++)
        {
            MyLLI[i].dst = dest;
            MyLLI[i].src = src;
            //MyLLI[i].Control = 0x8C024000 | blitOp.nSteps * 2; // 8-bit
            if (use32Bit)
            {
                //MyLLI[i].Control =  0x8C489000 | blitOp.nSteps/2; // <-- 32bit

                MyLLI[i].ctrl =
                    GPDMA_DMACCxControl_TransferSize(GPDMACfg.TransferSize) \
                    | GPDMA_DMACCxControl_SBSize(GPDMA_BSIZE_32) \
                    | GPDMA_DMACCxControl_DBSize(GPDMA_BSIZE_32) \
                    | GPDMA_DMACCxControl_SWidth(GPDMACfg.TransferWidth) \
                    | GPDMA_DMACCxControl_DWidth(GPDMACfg.TransferWidth) \
                    | GPDMA_DMACCxControl_SI \
                    | GPDMA_DMACCxControl_DI \
                    | GPDMA_DMACCxControl_I;
            }
            else
            {
                MyLLI[i].ctrl = 0x8C249000 | blitOp.nSteps; // 16-bit
            }

            if (i == blitOp.nLoops - 1)
            {
                MyLLI[i].lli = 0;
            }
            else
            {
                MyLLI[i].lli = (unsigned int)&MyLLI[i + 1];
            }
            dest += blitOp.dstLoopStride * 2;
            src += blitOp.srcLoopStride * 2;
        }

        Chip_GPDMA_SGTransfer(LPC_GPDMA,
                              0,                                       /* channel 0 */
                              MyLLI,                                   /* List of transfers */
                              GPDMA_TRANSFERTYPE_M2M_CONTROLLER_DMA);  /* Transfer type */
    }

    /* A single transfer, no LLI needed */
    //else
    //{
    //  /* 32-bit transfer */
    //  Chip_GPDMA_Transfer(LPC_GPDMA,
    //           0,                                       /* channel 0 */
    //           (uint32_t)blitOp.pSrc,                   /* Source memory */
    //           (unsigned int)blitOp.pDst,               /* Destination memory */
    //           GPDMA_TRANSFERTYPE_M2M_CONTROLLER_DMA,   /* Transfer type */
    //           blitOp.nSteps / 2);                      /* Transfer size */
    //}
}

static uint16_t DMAFillColor16;
static uint32_t DMAFillColor32;

void NXP4088DMA::setupDataFill(const BlitOp& blitOp)
{
    numLines = blitOp.nLoops;
    while (LPC_GPDMA->ENBLDCHNS) ;
    GPDMA_CH_CFG_T GPDMACfg;

    unsigned use32Bit = (blitOp.nSteps % 2) == 0 && // Pixels pr line is even
                        ((unsigned long)blitOp.pSrc & 3) == 0 && // src-address is 32bit-aligned
                        ((unsigned long)blitOp.pDst & 3) == 0 && // dst-address us 32bit-aligned
                        (blitOp.srcLoopStride % 2) == 0 &&   // src-stride is even
                        (blitOp.dstLoopStride % 2) == 0;     // dst-stride is even


    completedcount = 0;
    linecount = 0;
    GPDMACfg.ChannelNum = 0;
    /* Destination memory */
    GPDMACfg.DstAddr = (unsigned int)blitOp.pDst;
    /* Transfer type */
    GPDMACfg.TransferType = GPDMA_TRANSFERTYPE_M2M_CONTROLLER_DMA;

    if (use32Bit)
    {
        /* Source memory */
        DMAFillColor32 = blitOp.color << 16 | blitOp.color;
        GPDMACfg.SrcAddr = (uint32_t)&DMAFillColor32;
        /* Transfer size */
        GPDMACfg.TransferSize = blitOp.nSteps / 2;
        /* Transfer width */
        GPDMACfg.TransferWidth = GPDMA_WIDTH_WORD;
    }
    else
    {
        /* Source memory */
        DMAFillColor16 = blitOp.color;
        GPDMACfg.SrcAddr = (uint32_t)&DMAFillColor16;
        /* Transfer size */
        GPDMACfg.TransferSize = blitOp.nSteps;
        /* Transfer width */
        GPDMACfg.TransferWidth = GPDMA_WIDTH_HALFWORD;
    }

    //if (blitOp.nLoops > 1)
    {
        int i = 0;
        unsigned int dest = GPDMACfg.DstAddr;
        unsigned int src = GPDMACfg.SrcAddr;

        for (; i < blitOp.nLoops; i++)
        {
            MyLLI[i].dst = dest;
            MyLLI[i].src = src;

            if (use32Bit)
            {
                MyLLI[i].ctrl =
                    GPDMA_DMACCxControl_TransferSize(GPDMACfg.TransferSize) \
                    | GPDMA_DMACCxControl_SBSize(GPDMA_BSIZE_32) \
                    | GPDMA_DMACCxControl_DBSize(GPDMA_BSIZE_32) \
                    | GPDMA_DMACCxControl_SWidth(GPDMACfg.TransferWidth) \
                    | GPDMA_DMACCxControl_DWidth(GPDMACfg.TransferWidth) \
                    | GPDMA_DMACCxControl_DI \
                    | GPDMA_DMACCxControl_I;
            }
            else
            {
                MyLLI[i].ctrl = 0x88249000 | blitOp.nSteps; // 16-bit
            }

            if (i == blitOp.nLoops - 1)
            {
                MyLLI[i].lli = 0;
            }
            else
            {
                MyLLI[i].lli = (unsigned int)&MyLLI[i + 1];
            }
            dest += blitOp.dstLoopStride * 2;
        }

        Chip_GPDMA_SGTransfer(LPC_GPDMA,
                              0,                                       /* channel 0 */
                              MyLLI,                                   /* List of transfers */
                              GPDMA_TRANSFERTYPE_M2M_CONTROLLER_DMA);  /* Transfer type */
    }

    /* A single transfer, no LLI needed */
    //else
    //{
    //  /* 32-bit transfer */
    //  Chip_GPDMA_Transfer(LPC_GPDMA,
    //           0,                                       /* channel 0 */
    //           (uint32_t)blitOp.pSrc,                   /* Source memory */
    //           (unsigned int)blitOp.pDst,               /* Destination memory */
    //           GPDMA_TRANSFERTYPE_M2M_CONTROLLER_DMA,   /* Transfer type */
    //           blitOp.nSteps / 2);                      /* Transfer size */
    //}
}
