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
#include <platform/hal/nxp/mcu/LPC17xx/NXP1788DMA.hpp>

#include <cassert>

#include "LPC1788.h"


NXP1788DMA::NXP1788DMA(GPDMA_LLI_Type* lliMemory)
    : DMA_Interface(dma_queue), MyLLI(lliMemory), dma_queue(queue_storage, (sizeof(queue_storage) / sizeof(BlitOp))),
      pBlitOp(0), numLines(0), use32Bit(0)
{}


NXP1788DMA::~NXP1788DMA()
{
}


void NXP1788DMA::initialize()
{
}


volatile int linecount = 0;
volatile int completedcount = 0;
volatile int dmabug = 0;


void NXP1788DMA::signalDMAInterrupt(void)
{
    if (LPC_GPDMA->IntTCStat & 1)
    {
        LPC_GPDMA->IntTCClear = 1;
        linecount++;
        if (((LPC_GPDMA->IntStat & 1) == 0) && LPC_GPDMACH0->CLLI == 0)
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
            //      if (!numLines)
            //        executeCompleted();
        }
    }
    if (LPC_GPDMA->IntErrStat & 1)
    {
        dmabug = 1;
    }
}


void NXP1788DMA::setupDataCopy(const BlitOp& blitOp)
{
    numLines = blitOp.nLoops;
    while (LPC_GPDMA->EnbldChns) ;
    GPDMA_Channel_CFG_Type GPDMACfg;

    use32Bit = (blitOp.nSteps % 2) == 0 &&          // Pixels pr line is even
               ((unsigned long)blitOp.pSrc & 3) == 0 && // src-address is 32bit-aligned
               ((unsigned long)blitOp.pDst & 3) == 0 && // dst-address us 32bit-aligned
               (blitOp.srcLoopStride % 2) == 0 &&   // src-stride is even
               (blitOp.dstLoopStride % 2) == 0;     // dst-stride is even


    completedcount = 0;
    /* GPDMA block section -------------------------------------------- */
    /* Disable GPDMA interrupt */
    NVIC_DisableIRQ(DMA_IRQn);
    /* preemption = 1, sub-priority = 1 */
    //NVIC_SetPriority(DMA_IRQn, ((0x01<<3)|0x01));
    /* Initialize GPDMA controller */
    GPDMA_Init();
    /* Setup GPDMA channel --------------------------------*/
    /* channel 0 */
    GPDMACfg.ChannelNum = 0;
    /* Source memory */
    GPDMACfg.SrcMemAddr = (uint32_t)blitOp.pSrc;
    /* Destination memory */
    GPDMACfg.DstMemAddr = (unsigned int)blitOp.pDst;
    /* Source connection - unused */
    GPDMACfg.SrcConn = 0;
    /* Destination connection - unused */
    GPDMACfg.DstConn = 0;
    /* Transfer type */
    GPDMACfg.TransferType = GPDMA_TRANSFERTYPE_M2M;

    if (use32Bit)
    {
        /* Transfer size */
        GPDMACfg.TransferSize = blitOp.nSteps / 2;
        /* Transfer width */
        GPDMACfg.TransferWidth = GPDMA_WIDTH_WORD;//0;
    }
    else
    {
        /* Transfer size */
        GPDMACfg.TransferSize = blitOp.nSteps;// * 2;
        /* Transfer width */
        GPDMACfg.TransferWidth = GPDMA_WIDTH_HALFWORD;//0;
    }

    GPDMACfg.DMALLI = (blitOp.nLoops > 1 ? (unsigned int) &MyLLI[0] : 0);

    int i = 0;
    unsigned int dest = GPDMACfg.DstMemAddr;
    unsigned int src = GPDMACfg.SrcMemAddr;

    for (; i < blitOp.nLoops - 1; i++)
    {
        dest += blitOp.dstLoopStride * 2;
        src += blitOp.srcLoopStride * 2;
        MyLLI[i].DstAddr = dest;
        MyLLI[i].SrcAddr = src;
        //MyLLI[i].Control = 0x8C024000 | blitOp.nSteps * 2; // 8-bit
        if (use32Bit)
        {
            //MyLLI[i].Control =  0x8C489000 | blitOp.nSteps/2; // <-- 32bit

            MyLLI[i].Control =
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
            MyLLI[i].Control = 0x8C249000 | blitOp.nSteps; // 16-bit
        }

        if (i == blitOp.nLoops - 2)
        {
            MyLLI[i].NextLLI = 0;
        }
        else
        {
            MyLLI[i].NextLLI = (unsigned int)&MyLLI[i + 1];
        }
    }

    /* Setup channel with given parameter */
    NVIC_EnableIRQ(DMA_IRQn);

    GPDMA_Setup(&GPDMACfg);

    GPDMA_ChannelCmd(0, ENABLE);
}

uint16_t DMAFillColor16;
uint32_t DMAFillColor32;

void NXP1788DMA::setupDataFill(const BlitOp& blitOp)
{
    numLines = blitOp.nLoops;
    while (LPC_GPDMA->EnbldChns) ;
    GPDMA_Channel_CFG_Type GPDMACfg;

    use32Bit = (blitOp.nSteps % 2) == 0 &&          // Pixels pr line is even
               ((unsigned long)blitOp.pSrc & 3) == 0 && // src-address is 32bit-aligned
               ((unsigned long)blitOp.pDst & 3) == 0 && // dst-address us 32bit-aligned
               (blitOp.srcLoopStride % 2) == 0 &&   // src-stride is even
               (blitOp.dstLoopStride % 2) == 0;     // dst-stride is even

    completedcount = 0;
    /* GPDMA block section -------------------------------------------- */
    /* Disable GPDMA interrupt */
    NVIC_DisableIRQ(DMA_IRQn);
    /* preemption = 1, sub-priority = 1 */
    //NVIC_SetPriority(DMA_IRQn, ((0x01<<3)|0x01));
    /* Initialize GPDMA controller */
    GPDMA_Init();
    /* Setup GPDMA channel --------------------------------*/
    /* channel 0 */
    GPDMACfg.ChannelNum = 0;

    /* Source memory */
    if (use32Bit)
    {
        DMAFillColor32 = blitOp.color << 16 | blitOp.color;
        GPDMACfg.SrcMemAddr = (uint32_t)&DMAFillColor32;
    }
    else
    {
        DMAFillColor16 = blitOp.color;
        GPDMACfg.SrcMemAddr = (uint32_t)&DMAFillColor16;
    }
    /* Destination memory */
    GPDMACfg.DstMemAddr = (unsigned int)blitOp.pDst;
    /* Source connection - unused */
    GPDMACfg.SrcConn = 0;
    /* Destination connection - unused */
    GPDMACfg.DstConn = 0;
    /* Transfer type */
    GPDMACfg.TransferType = GPDMA_TRANSFERTYPE_M2M;

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

    GPDMACfg.DMALLI = (blitOp.nLoops > 1 ? (unsigned int) &MyLLI[0] : 0);

    int i = 0;
    unsigned int dest = GPDMACfg.DstMemAddr;
    unsigned int src = GPDMACfg.SrcMemAddr;

    for (; i < blitOp.nLoops - 1; i++)
    {
        dest += blitOp.dstLoopStride * 2;
        MyLLI[i].DstAddr = dest;
        MyLLI[i].SrcAddr = src;

        if (use32Bit)
        {
            //16-bit specific control-register setup
            MyLLI[i].Control =
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
            //16-bit specific control-register setup
            MyLLI[i].Control = 0x88249000 | blitOp.nSteps;
        }

        if (i == blitOp.nLoops - 2)
        {
            MyLLI[i].NextLLI = 0;
        }
        else
        {
            MyLLI[i].NextLLI = (unsigned int)&MyLLI[i + 1];
        }
    }

    /* Setup channel */
    // For M2M - The vendor method GPDMA_Setup supports only GPDMA_TRANSFERTYPE_M2M_CONTROLLER_DMA with enabled source increment. Manual setup below:
    //GPDMA_Setup(&GPDMACfg);

    LPC_GPDMACH_TypeDef* pDMAch;
    uint8_t SrcPeripheral = 0, DestPeripheral = 0;

    //Check if channel is available
    assert(!(LPC_GPDMA->EnbldChns & (GPDMA_DMACEnbldChns_Ch(GPDMACfg.ChannelNum))) && "DMA Channel not avaiable");

    // Get Channel pointer for channel 0
    pDMAch = (LPC_GPDMACH_TypeDef*) LPC_GPDMACH0;

    // Reset the Interrupt status
    LPC_GPDMA->IntTCClear = GPDMA_DMACIntTCClear_Ch(GPDMACfg.ChannelNum);
    LPC_GPDMA->IntErrClr = GPDMA_DMACIntErrClr_Ch(GPDMACfg.ChannelNum);

    // Clear DMA configure
    pDMAch->CControl = 0x00;
    pDMAch->CConfig = 0x00;

    /* Assign Linker List Item value */
    pDMAch->CLLI = GPDMACfg.DMALLI;

    // Assign physical source and destination address
    pDMAch->CSrcAddr = GPDMACfg.SrcMemAddr;
    pDMAch->CDestAddr = GPDMACfg.DstMemAddr;
    pDMAch->CControl
        = GPDMA_DMACCxControl_TransferSize(GPDMACfg.TransferSize) \
          | GPDMA_DMACCxControl_SBSize(GPDMA_BSIZE_32) \
          | GPDMA_DMACCxControl_DBSize(GPDMA_BSIZE_32) \
          | GPDMA_DMACCxControl_SWidth(GPDMACfg.TransferWidth) \
          | GPDMA_DMACCxControl_DWidth(GPDMACfg.TransferWidth) \
          | GPDMA_DMACCxControl_DI \
          | GPDMA_DMACCxControl_I;

    /* Enable DMA channels, little endian */
    LPC_GPDMA->Config = GPDMA_DMACConfig_E;
    while (!(LPC_GPDMA->Config & GPDMA_DMACConfig_E));

    // Configure DMA Channel, enable Error Counter and Terminate counter
    pDMAch->CConfig = GPDMA_DMACCxConfig_IE | GPDMA_DMACCxConfig_ITC /*| GPDMA_DMACCxConfig_E*/ \
                      | GPDMA_DMACCxConfig_TransferType((uint32_t)GPDMACfg.TransferType) \
                      | GPDMA_DMACCxConfig_SrcPeripheral(SrcPeripheral) \
                      | GPDMA_DMACCxConfig_DestPeripheral(DestPeripheral);

    //Re-enable interrupt
    NVIC_EnableIRQ(DMA_IRQn);

    //Enable channel 0
    GPDMA_ChannelCmd(0, ENABLE);
}
