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
#include <platform/hal/Freescale/mcu/MK70F12/FreescaleK70DMA.hpp>
#include <touchgfx/hal/GPIO.hpp>

extern "C"
{
#include "common.h"
}

FreescaleK70DMA::FreescaleK70DMA()
    : DMA_Interface(dma_queue), dma_queue(queue_storage, sizeof(queue_storage) / sizeof(queue_storage[0])),
      pBlitOp(0), numLines(0), use32Bit(0)
{

}

FreescaleK70DMA::~FreescaleK70DMA()
{

}

void FreescaleK70DMA::initialize()
{

}

/*
  To setup datacopy where source and destination address offsets are different, we need to use channel linking.
  We need to use some form of dynamic TCD setup since we can only occupy so many of the DMA channels - Is this even possible?

  If we're doing a full image, setup the TCD to no linking and just one major loop transfer.

  For filled rec, source simply points to a color and no offset should be used.
  Could we settle on only getting interrupts from one of the channels?
*/

struct TCD
{
    uint32_t SADDR;
    uint16_t SOFF;
    uint16_t ATTR;
    union
    {
        uint32_t NBYTES_MLNO;
        uint32_t NBYTES_MLOFFNO;
        uint32_t NBYTES_MLOFFYES;
    };
    uint32_t SLAST;
    uint32_t DADDR;
    uint16_t DOFF;
    union
    {
        uint16_t CITER_ELINKNO;
        uint16_t CITER_ELINKYES;
    };
    uint32_t DLAST_SGA;
    uint16_t CSR;
    union
    {
        uint16_t BITER_ELINKNO;
        uint16_t BITER_ELINKYES;
    };
};

//Ensure 32-bytes alignment for transfer descriptors
#pragma data_alignment=32
TCD trDscr[480];

void FreescaleK70DMA::setupDataCopy(const BlitOp& blitOp)
{
    //Use 32-bit if data is properly aligned
    use32Bit = (blitOp.nSteps % 2) == 0 &&          // Pixels pr line is even
               ((unsigned long)blitOp.pSrc & 3) == 0 && // src-address is 32bit-aligned
               ((unsigned long)blitOp.pDst & 3) == 0 && // dst-address us 32bit-aligned
               (blitOp.srcLoopStride % 2) == 0 &&   // src-stride is even
               (blitOp.dstLoopStride % 2) == 0;     // dst-stride is even

    uint32_t srcAddr = (uint32_t)blitOp.pSrc;
    uint32_t dstAddr = (uint32_t)blitOp.pDst;

    //Clear INTMAJOR flag
    DMA_TCD0_CSR &= ~(1 << 1); //Clear INTMAJOR

    //disable interrupt temporarily
    disable_irq(0);

    /***     SETUP TCD 0    ***/

    DMA_TCD0_SADDR = (uint32_t)blitOp.pSrc;
    DMA_TCD0_DADDR = (unsigned int)blitOp.pDst;

    //Data transfer size
    if (use32Bit)
    {
        DMA_TCD0_ATTR = 2  | (2 << 8);
        DMA_TCD0_SOFF = 4;
        DMA_TCD0_DOFF = 4;
    }
    else
    {
        DMA_TCD0_ATTR = 1  | (1 << 8);
        DMA_TCD0_SOFF = 2;
        DMA_TCD0_DOFF = 2;
    }

    //No offsets, because we're either drawing only 1 line, or we're using Scatter-Gather
    DMA_TCD0_SLAST = 0;

    // Number of minor loop iterations in major loop + current iteration count (must be equal)
    DMA_TCD0_BITER_ELINKNO = 1;
    DMA_TCD0_CITER_ELINKNO = 1;

    // Draw more than one line, less than full screen length but not fullscreen - ESG config (Most common scenario)
    if (blitOp.nLoops > 1 && blitOp.nSteps < HAL::DISPLAY_WIDTH)
    {
        DMA_TCD0_NBYTES_MLNO = blitOp.nSteps * 2;           //Bytes to transfer
        DMA_TCD0_CSR &= ~(1 << 7);                          //Clear done bit. This is usually dont when the channel is started.
        DMA_TCD0_CSR |= (1 << 4);                           //Enable ESG
        DMA_TCD0_DLASTSGA = (unsigned int) & (trDscr[0]);   //Set address of first transfer descriptor

        //Create remaining nLoops-1 transfer descriptors
        int i = 0;
        for (; i < blitOp.nLoops - 1; i++)
        {
            srcAddr += (blitOp.srcLoopStride) * 2;
            dstAddr += (blitOp.dstLoopStride) * 2;

            trDscr[i].SADDR = srcAddr;
            trDscr[i].NBYTES_MLNO = blitOp.nSteps * 2;
            trDscr[i].SLAST = 0;
            trDscr[i].DADDR = dstAddr;

            if (use32Bit)
            {
                trDscr[i].ATTR = 2  | (2 << 8); //32 BIT SSIZE, DSIZE
                trDscr[i].DOFF = 4;
                trDscr[i].SOFF = 4;
            }
            else
            {
                trDscr[i].ATTR = 1  | (1 << 8); //16 BIT SSIZE, DSIZE
                trDscr[i].DOFF = 2;
                trDscr[i].SOFF = 2;
            }

            trDscr[i].CITER_ELINKNO = 1;
            trDscr[i].CSR = (1 << 3) | 1; //ESG | START

            //Configuration for last line: Disable ESG and set interrupt flag
            if (i == blitOp.nLoops - 2)
            {
                trDscr[i].DLAST_SGA = 0;
                trDscr[i].CSR &= ~(1 << 4); // Clear Enable ScatterGather flag
                trDscr[i].CSR |= (1 << 1); // Set interrupt on major loop completion
            }
            else
            {
                trDscr[i].DLAST_SGA = (unsigned int) & (trDscr[i + 1]);
                trDscr[i].CSR |= (1 << 4);
            }

            trDscr[i].BITER_ELINKNO = 1;
        }
    }
    //Full screen width transfers - No ESG required. Can transfer a full screen image in a single transfer.
    else if (blitOp.nSteps == HAL::DISPLAY_WIDTH)
    {
        DMA_TCD0_NBYTES_MLNO = blitOp.nSteps * blitOp.nLoops * 2; //Define chunk of data that is HAL::DISPLAY_WIDTH*blitOp.nLoops*2 bytes in size.
        DMA_TCD0_DLASTSGA = 0;
        DMA_TCD0_CSR &= ~(1 << 4); //CLEAR ESG
        DMA_TCD0_CSR  |= (1 << 1); //Enable MAJORINT
    }
    //Single line transfer - No ESG
    else
    {
        DMA_TCD0_NBYTES_MLNO = blitOp.nSteps * 2;
        DMA_TCD0_CSR &= ~(1 << 4); //CLEAR ESG
        DMA_TCD0_CSR |= (1 << 1); //Set INTMAJOR
    }

    //nvic interrupt enable
    enable_irq(0);

    //Enable request signal for channel 0 and start transfer
    DMA_ERQ      |= 1;

    // Initiate transfer
    DMA_TCD0_CSR  |= ((1 << 3) | 1); // AUTO DISABLE REQ | START
}

void FreescaleK70DMA::setupDataFill(const BlitOp& blitOp)
{

}

void FreescaleK70DMA::signalDMAInterrupt()
{
    if (DMA_TCD0_CSR & 0x80)
    {
        executeCompleted();
    }
}
