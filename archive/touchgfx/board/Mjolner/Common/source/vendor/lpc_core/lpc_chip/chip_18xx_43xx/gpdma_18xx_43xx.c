/*
 * @brief LPC18xx/43xx DMA driver
 *
 * @note
 * Copyright(C) NXP Semiconductors, 2012
 * All rights reserved.
 *
 * @par
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * LPC products.  This software is supplied "AS IS" without any warranties of
 * any kind, and NXP Semiconductors and its licensor disclaim any and
 * all warranties, express or implied, including all implied warranties of
 * merchantability, fitness for a particular purpose and non-infringement of
 * intellectual property rights.  NXP Semiconductors assumes no responsibility
 * or liability for the use of the software, conveys no license or rights under any
 * patent, copyright, mask work right, or any other intellectual property rights in
 * or to any products. NXP Semiconductors reserves the right to make changes
 * in the software without notification. NXP Semiconductors also makes no
 * representation or warranty that such application will be suitable for the
 * specified use without further testing or modification.
 *
 * @par
 * Permission to use, copy, modify, and distribute this software and its
 * documentation is hereby granted, under NXP Semiconductors' and its
 * licensor's relevant copyrights in the software, without fee, provided that it
 * is used in conjunction with NXP Semiconductors microcontrollers.  This
 * copyright, permission, and disclaimer notice must appear in all copies of
 * this code.
 */

#include "chip.h"

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

/* Channel array to monitor free channel */
static DMA_ChannelHandle_t ChannelHandlerArray[GPDMA_NUMBER_CHANNELS];

/* Optimized Peripheral Source and Destination burst size (18xx,43xx) */
static const uint8_t GPDMA_LUTPerBurst[] = {
        GPDMA_BSIZE_4,  /* MEMORY             */
        GPDMA_BSIZE_1,  /* MAT0.0             */
        GPDMA_BSIZE_1,  /* UART0 Tx           */
        GPDMA_BSIZE_1,  /* MAT0.1             */
        GPDMA_BSIZE_1,  /* UART0 Rx           */
        GPDMA_BSIZE_1,  /* MAT1.0             */
        GPDMA_BSIZE_1,  /* UART1 Tx           */
        GPDMA_BSIZE_1,  /* MAT1.1             */
        GPDMA_BSIZE_1,  /* UART1 Rx           */
        GPDMA_BSIZE_1,  /* MAT2.0             */
        GPDMA_BSIZE_1,  /* UART2 Tx           */
        GPDMA_BSIZE_1,  /* MAT2.1             */
        GPDMA_BSIZE_1,  /* UART2 Rx           */
        GPDMA_BSIZE_1,  /* MAT3.0             */
        GPDMA_BSIZE_1,  /* UART3 Tx           */
        0,                              /* SCT timer channel 0*/
        GPDMA_BSIZE_1,  /* MAT3.1             */
        GPDMA_BSIZE_1,  /* UART3 Rx           */
        0,                              /* SCT timer channel 1*/
        GPDMA_BSIZE_4,  /* SSP0 Rx            */
        GPDMA_BSIZE_32, /* I2S channel 0      */
        GPDMA_BSIZE_4,  /* SSP0 Tx            */
        GPDMA_BSIZE_32, /* I2S channel 1      */
        GPDMA_BSIZE_4,  /* SSP1 Rx            */
        GPDMA_BSIZE_4,  /* SSP1 Tx            */
        GPDMA_BSIZE_4,  /* ADC 0              */
        GPDMA_BSIZE_4,  /* ADC 1              */
        GPDMA_BSIZE_1,  /* DAC                */
        GPDMA_BSIZE_32, /* I2S channel 0      */
        GPDMA_BSIZE_32  /* I2S channel 0      */
};

/* Optimized Peripheral Source and Destination transfer width (18xx,43xx) */
static const uint8_t GPDMA_LUTPerWid[] = {
        GPDMA_WIDTH_WORD,       /* MEMORY             */
        GPDMA_WIDTH_WORD,       /* MAT0.0             */
        GPDMA_WIDTH_BYTE,       /* UART0 Tx           */
        GPDMA_WIDTH_WORD,       /* MAT0.1             */
        GPDMA_WIDTH_BYTE,       /* UART0 Rx           */
        GPDMA_WIDTH_WORD,       /* MAT1.0             */
        GPDMA_WIDTH_BYTE,       /* UART1 Tx           */
        GPDMA_WIDTH_WORD,       /* MAT1.1             */
        GPDMA_WIDTH_BYTE,       /* UART1 Rx           */
        GPDMA_WIDTH_WORD,       /* MAT2.0             */
        GPDMA_WIDTH_BYTE,       /* UART2 Tx           */
        GPDMA_WIDTH_WORD,       /* MAT2.1             */
        GPDMA_WIDTH_BYTE,       /* UART2 Rx           */
        GPDMA_WIDTH_WORD,       /* MAT3.0             */
        GPDMA_WIDTH_BYTE,       /* UART3 Tx           */
        0,                                      /* SCT timer channel 0*/
        GPDMA_WIDTH_WORD,       /* MAT3.1             */
        GPDMA_WIDTH_BYTE,       /* UART3 Rx           */
        0,                                      /* SCT timer channel 1*/
        GPDMA_WIDTH_BYTE,       /* SSP0 Rx            */
        GPDMA_WIDTH_WORD,       /* I2S channel 0      */
        GPDMA_WIDTH_BYTE,       /* SSP0 Tx            */
        GPDMA_WIDTH_WORD,       /* I2S channel 1      */
        GPDMA_WIDTH_BYTE,       /* SSP1 Rx            */
        GPDMA_WIDTH_BYTE,       /* SSP1 Tx            */
        GPDMA_WIDTH_WORD,       /* ADC 0              */
        GPDMA_WIDTH_WORD,       /* ADC 1              */
        GPDMA_WIDTH_WORD,       /* DAC                */
        GPDMA_WIDTH_WORD,       /* I2S channel 0      */
        GPDMA_WIDTH_WORD/* I2S channel 0      */
};

/* Lookup Table of Connection Type matched with (18xx,43xx) Peripheral Data (FIFO) register base address */
volatile static const void *GPDMA_LUTPerAddr[] = {
        NULL,                                                   /* MEMORY             */
        (&LPC_TIMER0->MR),                              /* MAT0.0             */
        (&LPC_USART0-> /*RBTHDLR.*/ THR),       /* UART0 Tx           */
        ((uint32_t *) &LPC_TIMER0->MR + 1),     /* MAT0.1             */
        (&LPC_USART0-> /*RBTHDLR.*/ RBR),       /* UART0 Rx           */
        (&LPC_TIMER1->MR),                              /* MAT1.0             */
        (&LPC_UART1-> /*RBTHDLR.*/ THR),/* UART1 Tx           */
        ((uint32_t *) &LPC_TIMER1->MR + 1),     /* MAT1.1             */
        (&LPC_UART1-> /*RBTHDLR.*/ RBR),/* UART1 Rx           */
        (&LPC_TIMER2->MR),                              /* MAT2.0             */
        (&LPC_USART2-> /*RBTHDLR.*/ THR),       /* UART2 Tx           */
        ((uint32_t *) &LPC_TIMER2->MR + 1),     /* MAT2.1             */
        (&LPC_USART2-> /*RBTHDLR.*/ RBR),       /* UART2 Rx           */
        (&LPC_TIMER3->MR),                              /* MAT3.0             */
        (&LPC_USART3-> /*RBTHDLR.*/ THR),       /* UART3 Tx           */
        0,                                                              /* SCT timer channel 0*/
        ((uint32_t *) &LPC_TIMER3->MR + 1),     /* MAT3.1             */
        (&LPC_USART3-> /*RBTHDLR.*/ RBR),       /* UART3 Rx           */
        0,                                                              /* SCT timer channel 1*/
        (&LPC_SSP0->DR),                                /* SSP0 Rx            */
        (&LPC_I2S0->TXFIFO),                    /* I2S channel 0      */
        (&LPC_SSP0->DR),                                /* SSP0 Tx            */
        (&LPC_I2S0->RXFIFO),                    /* I2S channel 1      */
        (&LPC_SSP1->DR),                                /* SSP1 Rx            */
        (&LPC_SSP1->DR),                                /* SSP1 Tx            */
        (&LPC_ADC0->GDR),                               /* ADC 0              */
        (&LPC_ADC1->GDR),                               /* ADC 1              */
        (&LPC_DAC->CR),                                 /* DAC                */
        (&LPC_I2S0->TXFIFO),                    /* I2S channel 0      */
        (&LPC_I2S0->RXFIFO)                             /* I2S channel 0      */
};

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Private functions
 ****************************************************************************/

/* Control which set of peripherals is connected to the DMA controller */
static uint8_t DMAMUX_Config(uint32_t gpdma_peripheral_connection_number)
{
        uint8_t function, channel;

        switch (gpdma_peripheral_connection_number) {
        case GPDMA_CONN_MAT0_0:
                function = 0;
                channel = 1;
                break;

        case GPDMA_CONN_UART0_Tx:
                function = 1;
                channel = 1;
                break;

        case GPDMA_CONN_MAT0_1:
                function = 0;
                channel = 2;
                break;

        case GPDMA_CONN_UART0_Rx:
                function = 1;
                channel = 2;
                break;

        case GPDMA_CONN_MAT1_0:
                function = 0;
                channel = 3;
                break;

        case GPDMA_CONN_UART1_Tx:
                function = 1;
                channel = 3;
                break;

        case GPDMA_CONN_MAT1_1:
                function = 0;
                channel = 4;
                break;

        case GPDMA_CONN_UART1_Rx:
                function = 1;
                channel = 4;
                break;

        case GPDMA_CONN_MAT2_0:
                function = 0;
                channel = 5;
                break;

        case GPDMA_CONN_UART2_Tx:
                function = 1;
                channel = 5;
                break;

        case GPDMA_CONN_MAT2_1:
                function = 0;
                channel = 6;
                break;

        case GPDMA_CONN_UART2_Rx:
                function = 1;
                channel = 6;
                break;

        case GPDMA_CONN_MAT3_0:
                function = 0;
                channel = 7;
                break;

        case GPDMA_CONN_UART3_Tx:
                function = 1;
                channel = 7;
                break;

        case GPDMA_CONN_SCT_0:
                function = 2;
                channel = 7;
                break;

        case GPDMA_CONN_MAT3_1:
                function = 0;
                channel = 8;
                break;

        case GPDMA_CONN_UART3_Rx:
                function = 1;
                channel = 8;
                break;

        case GPDMA_CONN_SCT_1:
                function = 2;
                channel = 8;
                break;

        case GPDMA_CONN_SSP0_Rx:
                function = 0;
                channel = 9;
                break;

        case GPDMA_CONN_I2S_Tx_Channel_0:
        case GPDMA_CONN_I2S_Rx_Channel_0:
                function = 1;
                channel = 9;
                break;

        case GPDMA_CONN_SSP0_Tx:
                function = 0;
                channel = 10;
                break;

        case GPDMA_CONN_I2S_Tx_Channel_1:
        case GPDMA_CONN_I2S_Rx_Channel_1:
                function = 1;
                channel = 10;
                break;

        case GPDMA_CONN_SSP1_Rx:
                function = 0;
                channel = 11;
                break;

        case GPDMA_CONN_SSP1_Tx:
                function = 0;
                channel = 12;
                break;

        case GPDMA_CONN_ADC_0:
                function = 0;
                channel = 13;
                break;

        case GPDMA_CONN_ADC_1:
                function = 0;
                channel = 14;
                break;

        case GPDMA_CONN_DAC:
                function = 0;
                channel = 15;
                break;

        default:
                function = 3;
                channel = 15;
                break;
        }
        /* Set select function to dmamux register */
        if (0 != gpdma_peripheral_connection_number) {
                uint32_t temp;
                temp = LPC_CREG->DMAMUX & (~(0x03 << (2 * channel)));
                LPC_CREG->DMAMUX = temp | (function << (2 * channel));
        }
        return channel;
}

/*****************************************************************************
 * Public functions
 ****************************************************************************/

/* Initialize the GPDMA */
void Chip_GPDMA_Init(LPC_GPDMA_T *pGPDMA)
{
        uint8_t i;

        Chip_Clock_EnableOpts(CLK_MX_DMA, true, true, 1);
        IP_GPDMA_Init(pGPDMA);
        /* Reset all channels are free */
        for (i = 0; i < GPDMA_NUMBER_CHANNELS; i++) {
                ChannelHandlerArray[i].ChannelStatus = DISABLE;
        }
}

/* Shutdown the GPDMA */
void Chip_GPDMA_DeInit(LPC_GPDMA_T *pGPDMA)
{
        Chip_Clock_Disable(CLK_MX_DMA);
}

/* Stop a stream DMA transfer */
void Chip_DMA_Stop(LPC_GPDMA_T *pGPDMA,
                                   uint8_t ChannelNum)
{
        IP_GPDMA_ChannelCmd(pGPDMA, (ChannelNum), DISABLE);
        if (Chip_GPDMA_IntGetStatus(pGPDMA, GPDMA_STAT_INTTC, ChannelNum)) {
                /* Clear terminate counter Interrupt pending */
                Chip_GPDMA_ClearIntPending(pGPDMA, GPDMA_STATCLR_INTTC, ChannelNum);
        }
        if (Chip_GPDMA_IntGetStatus(pGPDMA, GPDMA_STAT_INTERR, ChannelNum)) {
                /* Clear terminate counter Interrupt pending */
                Chip_GPDMA_ClearIntPending(pGPDMA, GPDMA_STATCLR_INTERR, ChannelNum);
        }
        ChannelHandlerArray[ChannelNum].ChannelStatus = DISABLE;
}

/* The GPDMA stream interrupt status checking */
Status Chip_DMA_Interrupt(LPC_GPDMA_T *pGPDMA,
                                                  uint8_t ChannelNum)
{

        if (Chip_GPDMA_IntGetStatus(pGPDMA, GPDMA_STAT_INT, ChannelNum)) {
                /* Check counter terminal status */
                if (Chip_GPDMA_IntGetStatus(pGPDMA, GPDMA_STAT_INTTC, ChannelNum)) {
                        /* Clear terminate counter Interrupt pending */
                        Chip_GPDMA_ClearIntPending(pGPDMA, GPDMA_STATCLR_INTTC, ChannelNum);
                        return SUCCESS;
                }
                /* Check error terminal status */
                if (Chip_GPDMA_IntGetStatus(pGPDMA, GPDMA_STAT_INTERR, ChannelNum)) {
                        /* Clear error counter Interrupt pending */

                        Chip_GPDMA_ClearIntPending(pGPDMA, GPDMA_STATCLR_INTERR, ChannelNum);
                        return ERROR;
                }
        }
        return ERROR;
}

int Chip_DMA_InitChannelCfg(LPC_GPDMA_T *pGPDMA,
                                                        GPDMA_Channel_CFG_T *GPDMACfg,
                                                        uint8_t  ChannelNum,
                                                        uint32_t src,
                                                        uint32_t dst,
                                                        uint32_t Size,
                                                        IP_GPDMA_FLOW_CONTROL_T TransferType)
{
        int rval = -1;
        GPDMACfg->ChannelNum = ChannelNum;
        GPDMACfg->TransferType = TransferType;
        GPDMACfg->TransferSize = Size;

        switch (TransferType) {
        case GPDMA_TRANSFERTYPE_M2M_CONTROLLER_DMA:
                GPDMACfg->SrcAddr = (uint32_t) src;
                GPDMACfg->DstAddr = (uint32_t) dst;
                rval = 3;
                GPDMACfg->TransferWidth = GPDMA_WIDTH_WORD;
                GPDMACfg->TransferSize = Size / 4;
                break;

        case GPDMA_TRANSFERTYPE_M2P_CONTROLLER_DMA:
        case GPDMA_TRANSFERTYPE_M2P_CONTROLLER_PERIPHERAL:
                GPDMACfg->SrcAddr = (uint32_t) src;
                rval = 1;
                GPDMACfg->DstAddr = (uint32_t) GPDMA_LUTPerAddr[dst];
                break;

        case GPDMA_TRANSFERTYPE_P2M_CONTROLLER_DMA:
        case GPDMA_TRANSFERTYPE_P2M_CONTROLLER_PERIPHERAL:
                GPDMACfg->SrcAddr = (uint32_t) GPDMA_LUTPerAddr[src];
                GPDMACfg->DstAddr = (uint32_t) dst;
                rval = 2;
                break;

        case GPDMA_TRANSFERTYPE_P2P_CONTROLLER_DMA:
        case GPDMA_TRANSFERTYPE_P2P_CONTROLLER_DestPERIPHERAL:
        case GPDMA_TRANSFERTYPE_P2P_CONTROLLER_SrcPERIPHERAL:
                GPDMACfg->SrcAddr = (uint32_t) GPDMA_LUTPerAddr[src];
                GPDMACfg->DstAddr = (uint32_t) GPDMA_LUTPerAddr[dst];
                rval = 0;
                break;

        default:
                break;
        }
        return rval;
}

/* Do a DMA transfer M2M, M2P,P2M or P2P */
Status Chip_DMA_Transfer(LPC_GPDMA_T *pGPDMA,
                                                 uint8_t ChannelNum,
                                                 uint32_t src,
                                                 uint32_t dst,
                                                 IP_GPDMA_FLOW_CONTROL_T TransferType,
                                                 uint32_t Size)
{
        GPDMA_Channel_CFG_T GPDMACfg;
        uint8_t SrcPeripheral = 0, DstPeripheral = 0;
        uint32_t cwrd;
        int ret;

        ret = Chip_DMA_InitChannelCfg(pGPDMA, &GPDMACfg, ChannelNum, src, dst, Size, TransferType);
        if (ret < 0) {
                return ERROR;
        }

        /* Adjust src/dst index if they are memory */
        if (ret & 1) {
                src = 0;
        }
        else {
                SrcPeripheral = DMAMUX_Config(src);
        }

        if (ret & 2) {
                dst = 0;
        }
        else {
                DstPeripheral = DMAMUX_Config(dst);
        }

        cwrd = IP_GPDMA_MakeCtrlWord(&GPDMACfg,
                                                                 (uint32_t) GPDMA_LUTPerBurst[src],
                                                                 (uint32_t) GPDMA_LUTPerBurst[dst],
                                                                 (uint32_t) GPDMA_LUTPerWid[src],
                                                                 (uint32_t) GPDMA_LUTPerWid[dst]);
        if (IP_GPDMA_Setup(pGPDMA, &GPDMACfg, cwrd, 0, SrcPeripheral, DstPeripheral) == ERROR) {
                return ERROR;
        }

        /* Start the Channel */
        IP_GPDMA_ChannelCmd(pGPDMA, ChannelNum, ENABLE);
        return SUCCESS;
}

Status Chip_DMA_PrepareDescriptor(LPC_GPDMA_T *pGPDMA,
                                                                  DMA_TransferDescriptor_t *DMADescriptor,
                                                                  uint32_t src,
                                                                  uint32_t dst,
                                                                  uint32_t Size,
                                                                  IP_GPDMA_FLOW_CONTROL_T TransferType,
                                                                  const DMA_TransferDescriptor_t *NextDescriptor)
{
        int ret;
        GPDMA_Channel_CFG_T GPDMACfg;

        ret = Chip_DMA_InitChannelCfg(pGPDMA, &GPDMACfg, 0, src, dst, Size, TransferType);
        if (ret < 0) {
                return ERROR;
        }

        /* Adjust src/dst index if they are memory */
        if (ret & 1) {
                src = 0;
        }

        if (ret & 2) {
                dst = 0;
        }

        DMADescriptor->src  = GPDMACfg.SrcAddr;
        DMADescriptor->dst  = GPDMACfg.DstAddr;
        DMADescriptor->lli  = (uint32_t) NextDescriptor;
        DMADescriptor->ctrl = IP_GPDMA_MakeCtrlWord(&GPDMACfg,
                                                                                                (uint32_t) GPDMA_LUTPerBurst[src],
                                                                                                (uint32_t) GPDMA_LUTPerBurst[dst],
                                                                                                (uint32_t) GPDMA_LUTPerWid[src],
                                                                                                (uint32_t) GPDMA_LUTPerWid[dst]);

        /* By default set interrupt only for last transfer */
        if (NextDescriptor) {
                DMADescriptor->ctrl &= ~GPDMA_DMACCxControl_I;
        }

        return SUCCESS;
}

/* Do a DMA scatter-gather transfer M2M, M2P,P2M or P2P using DMA descriptors */
Status Chip_DMA_SGTransfer(LPC_GPDMA_T *pGPDMA,
                                                   uint8_t ChannelNum,
                                                   const DMA_TransferDescriptor_t *DMADescriptor,
                                                   IP_GPDMA_FLOW_CONTROL_T TransferType)
{
        const DMA_TransferDescriptor_t *dsc = DMADescriptor;
        GPDMA_Channel_CFG_T GPDMACfg;
        uint8_t SrcPeripheral = 0, DstPeripheral = 0;
        uint32_t src = DMADescriptor->src, dst = DMADescriptor->dst;
        int ret;

        ret = Chip_DMA_InitChannelCfg(pGPDMA, &GPDMACfg, ChannelNum, src, dst, 0, TransferType);
        if (ret < 0) {
                return ERROR;
        }

        /* Adjust src/dst index if they are memory */
        if (ret & 1) {
                src = 0;
        }
        else {
                SrcPeripheral = DMAMUX_Config(src);
        }

        if (ret & 2) {
                dst = 0;
        }
        else {
                DstPeripheral = DMAMUX_Config(dst);
        }

        if (IP_GPDMA_Setup(pGPDMA, &GPDMACfg, dsc->ctrl, dsc->lli, SrcPeripheral, DstPeripheral) == ERROR) {
                return ERROR;
        }

        /* Start the Channel */
        IP_GPDMA_ChannelCmd(pGPDMA, ChannelNum, ENABLE);
        return SUCCESS;
}

/* Get a free GPDMA channel for one DMA connection */
uint8_t Chip_DMA_GetFreeChannel(LPC_GPDMA_T *pGPDMA,
                                                                uint32_t PeripheralConnection_ID)
{
        uint8_t temp = 0;
        for (temp = 0; temp < GPDMA_NUMBER_CHANNELS; temp++) {
                if (!Chip_GPDMA_IntGetStatus(pGPDMA, GPDMA_STAT_ENABLED_CH,
                                                                         temp) && (ChannelHandlerArray[temp].ChannelStatus == DISABLE)) {
                        ChannelHandlerArray[temp].ChannelStatus = ENABLE;
                        return temp;
                }
        }
        return 0;
}
