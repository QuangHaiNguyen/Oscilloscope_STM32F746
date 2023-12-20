/******************************************************************************
 * @file:    LPC177x_8x.h
 * @purpose: CMSIS Cortex-M3 Core Peripheral Access Layer Header File for
 *           NXP LPC177x_8x Device Series
 * @version: V1.10
 * @date:    24. September 2010
 *----------------------------------------------------------------------------
 *
 * @note
 * Copyright (C) 2010 ARM Limited. All rights reserved.
 *
 * @par
 * ARM Limited (ARM) is supplying this software for use with Cortex-M
 * processor based microcontrollers.  This file can be freely distributed
 * within development tools that are supporting such ARM based processors.
 *
 * @par
 * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * ARM SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 ******************************************************************************/

#ifndef __LPC177x_8x_H__
#define __LPC177x_8x_H__

/*
 * ==========================================================================
 * ---------- Interrupt Number Definition -----------------------------------
 * ==========================================================================
 */

typedef enum IRQn
{
/******  Cortex-M3 Processor Exceptions Numbers ***************************************************/
  NonMaskableInt_IRQn           = -14,      /*!< 2 Non Maskable Interrupt                         */
  MemoryManagement_IRQn         = -12,      /*!< 4 Cortex-M3 Memory Management Interrupt          */
  BusFault_IRQn                 = -11,      /*!< 5 Cortex-M3 Bus Fault Interrupt                  */
  UsageFault_IRQn               = -10,      /*!< 6 Cortex-M3 Usage Fault Interrupt                */
  SVCall_IRQn                   = -5,       /*!< 11 Cortex-M3 SV Call Interrupt                   */
  DebugMonitor_IRQn             = -4,       /*!< 12 Cortex-M3 Debug Monitor Interrupt             */
  PendSV_IRQn                   = -2,       /*!< 14 Cortex-M3 Pend SV Interrupt                   */
  SysTick_IRQn                  = -1,       /*!< 15 Cortex-M3 System Tick Interrupt               */

/******  LPC177x_8x Specific Interrupt Numbers ****************************************************/
  WDT_IRQn                      = 0,        /*!< Watchdog Timer Interrupt                         */
  TIMER0_IRQn                   = 1,        /*!< Timer0 Interrupt                                 */
  TIMER1_IRQn                   = 2,        /*!< Timer1 Interrupt                                 */
  TIMER2_IRQn                   = 3,        /*!< Timer2 Interrupt                                 */
  TIMER3_IRQn                   = 4,        /*!< Timer3 Interrupt                                 */
  UART0_IRQn                    = 5,        /*!< UART0 Interrupt                                  */
  UART1_IRQn                    = 6,        /*!< UART1 Interrupt                                  */
  UART2_IRQn                    = 7,        /*!< UART2 Interrupt                                  */
  UART3_IRQn                    = 8,        /*!< UART3 Interrupt                                  */
  PWM1_IRQn                     = 9,        /*!< PWM1 Interrupt                                   */
  I2C0_IRQn                     = 10,       /*!< I2C0 Interrupt                                   */
  I2C1_IRQn                     = 11,       /*!< I2C1 Interrupt                                   */
  I2C2_IRQn                     = 12,       /*!< I2C2 Interrupt                                   */
  SPIFI_IRQn                    = 13,       /*!< SPIFI Interrupt                                  */
  SSP0_IRQn                     = 14,       /*!< SSP0 Interrupt                                   */
  SSP1_IRQn                     = 15,       /*!< SSP1 Interrupt                                   */
  PLL0_IRQn                     = 16,       /*!< PLL0 Lock (Main PLL) Interrupt                   */
  RTC_IRQn                      = 17,       /*!< Real Time Clock Interrupt                        */
  EINT0_IRQn                    = 18,       /*!< External Interrupt 0 Interrupt                   */
  EINT1_IRQn                    = 19,       /*!< External Interrupt 1 Interrupt                   */
  EINT2_IRQn                    = 20,       /*!< External Interrupt 2 Interrupt                   */
  EINT3_IRQn                    = 21,       /*!< External Interrupt 3 Interrupt                   */
  ADC_IRQn                      = 22,       /*!< A/D Converter Interrupt                          */
  BOD_IRQn                      = 23,       /*!< Brown-Out Detect Interrupt                       */
  USB_IRQn                      = 24,       /*!< USB Interrupt                                    */
  CAN_IRQn                      = 25,       /*!< CAN Interrupt                                    */
  DMA_IRQn                      = 26,       /*!< General Purpose DMA Interrupt                    */
  I2S_IRQn                      = 27,       /*!< I2S Interrupt                                    */
  ENET_IRQn                     = 28,       /*!< Ethernet Interrupt                               */
  MCI_IRQn                      = 29,       /*!< SD/MMC card I/F Interrupt                        */
  MCPWM_IRQn                    = 30,       /*!< Motor Control PWM Interrupt                      */
  QEI_IRQn                      = 31,       /*!< Quadrature Encoder Interface Interrupt           */
  PLL1_IRQn                     = 32,       /*!< PLL1 Lock (USB PLL) Interrupt                    */
  USBActivity_IRQn              = 33,       /*!< USB Activity interrupt                           */
  CANActivity_IRQn              = 34,       /*!< CAN Activity interrupt                           */
  UART4_IRQn                    = 35,       /*!< UART4 Interrupt                                  */
  SSP2_IRQn                     = 36,       /*!< SSP2 Interrupt                                   */
  LCD_IRQn                      = 37,       /*!< LCD Interrupt                                    */
  GPIO_IRQn                     = 38,       /*!< GPIO Interrupt                                   */
  PWM0_IRQn                     = 39,       /*!< PWM0 Interrupt                                   */
  KFLASH_IRQn                   = 40,       /*!< KFLASH SHELL Interrupt                           */
} IRQn_Type;


/*
 * ==========================================================================
 * ----------- Processor and Core Peripheral Section ------------------------
 * ==========================================================================
 */

/* Configuration of the Cortex-M3 Processor and Core Peripherals */
#define __MPU_PRESENT             1         /*!< MPU present or not                               */
#define __NVIC_PRIO_BITS          5         /*!< Number of Bits used for Priority Levels          */
#define __Vendor_SysTickConfig    0         /*!< Set to 1 if different SysTick Config is used     */


#include "core_cm3.h"                       /* Cortex-M3 processor and core peripherals           */
#include "system_LPC177x_8x.h"                 /* System Header                                      */


/******************************************************************************/
/*                Device Specific Peripheral registers structures             */
/******************************************************************************/

//#pragma anon_unions

/*------------- System Control (SC) ------------------------------------------*/
typedef struct
{
  __IO uint32_t FLASHCFG;                   /*!< Offset: 0x000 (R/W)  Flash Accelerator Configuration Register */
       uint32_t RESERVED0[31];
  __IO uint32_t PLL0CON;                    /*!< Offset: 0x080 (R/W)  PLL0 Control Register */
  __IO uint32_t PLL0CFG;                    /*!< Offset: 0x084 (R/W)  PLL0 Configuration Register */
  __I  uint32_t PLL0STAT;                   /*!< Offset: 0x088 (R/ )  PLL0 Status Register */
  __O  uint32_t PLL0FEED;                   /*!< Offset: 0x08C ( /W)  PLL0 Feed Register */
       uint32_t RESERVED1[4];
  __IO uint32_t PLL1CON;                    /*!< Offset: 0x0A0 (R/W)  PLL1 Control Register */
  __IO uint32_t PLL1CFG;                    /*!< Offset: 0x0A4 (R/W)  PLL1 Configuration Register */
  __I  uint32_t PLL1STAT;                   /*!< Offset: 0x0A8 (R/ )  PLL1 Status Register */
  __O  uint32_t PLL1FEED;                   /*!< Offset: 0x0AC ( /W)  PLL1 Feed Register */
       uint32_t RESERVED2[4];
  __IO uint32_t PCON;                       /*!< Offset: 0x0C0 (R/W)  Power Control Register */
  __IO uint32_t PCONP;                      /*!< Offset: 0x0C4 (R/W)  Power Control for Peripherals Register */
       uint32_t RESERVED3[14];
  __IO uint32_t EMCCLKSEL;                  /*!< Offset: 0x100 (R/W)  External Memory Controller Clock Selection Register */
  __IO uint32_t CCLKSEL;                    /*!< Offset: 0x104 (R/W)  CPU Clock Selection Register */
  __IO uint32_t USBCLKSEL;                  /*!< Offset: 0x108 (R/W)  USB Clock Selection Register */
  __IO uint32_t CLKSRCSEL;                  /*!< Offset: 0x10C (R/W)  Clock Source Select Register */
  __IO uint32_t CANSLEEPCLR;                /*!< Offset: 0x110 (R/W)  CAN Sleep Clear Register */
  __IO uint32_t CANWAKEFLAGS;               /*!< Offset: 0x114 (R/W)  CAN Wake-up Flags Register */
       uint32_t RESERVED4[10];
  __IO uint32_t EXTINT;                     /*!< Offset: 0x140 (R/W)  External Interrupt Flag Register */
       uint32_t RESERVED5[1];
  __IO uint32_t EXTMODE;                    /*!< Offset: 0x148 (R/W)  External Interrupt Mode Register */
  __IO uint32_t EXTPOLAR;                   /*!< Offset: 0x14C (R/W)  External Interrupt Polarity Register */
       uint32_t RESERVED6[12];
  __IO uint32_t RSID;                       /*!< Offset: 0x180 (R/W)  Reset Source Identification Register */
       uint32_t RESERVED7[7];
  __IO uint32_t SCS;                        /*!< Offset: 0x1A0 (R/W)  System Controls and Status Register */
  __IO uint32_t IRCTRIM;                /* Clock Dividers                     */
  __IO uint32_t PCLKSEL;                    /*!< Offset: 0x1A8 (R/W)  Peripheral Clock Selection Register */
       uint32_t RESERVED8[2];
  __IO uint32_t SPIFISEL;
  __IO uint32_t LCD_CFG;                    /*!< Offset: 0x1B8 (R/W)  LCD Configuration and clocking control Register */
       uint32_t RESERVED9[1];
  __IO uint32_t USBIntSt;                   /*!< Offset: 0x1C0 (R/W)  USB Interrupt Status Register */
  __IO uint32_t DMAREQSEL;                  /*!< Offset: 0x1C4 (R/W)  DMA Request Select Register */
  __IO uint32_t CLKOUTCFG;                  /*!< Offset: 0x1C8 (R/W)  Clock Output Configuration Register */
       uint32_t RESERVED10[4];
  __IO uint32_t EMCDLYCTL;                  /*!< Offset: 0x1D8 (R/W)  SDRAM programmable delays          */
  __IO uint32_t EMCCAL;                     /*!< Offset: 0x1DC (R/W)  Calibration of programmable delays */
 } LPC_SC_TypeDef;

/*------------- Pin Connect Block (PINCON) -----------------------------------*/
typedef struct
{
  __IO uint32_t P0_0;               /* 0x000 */
  __IO uint32_t P0_1;
  __IO uint32_t P0_2;
  __IO uint32_t P0_3;
  __IO uint32_t P0_4;
  __IO uint32_t P0_5;
  __IO uint32_t P0_6;
  __IO uint32_t P0_7;

  __IO uint32_t P0_8;               /* 0x020 */
  __IO uint32_t P0_9;
  __IO uint32_t P0_10;
  __IO uint32_t P0_11;
  __IO uint32_t P0_12;
  __IO uint32_t P0_13;
  __IO uint32_t P0_14;
  __IO uint32_t P0_15;

  __IO uint32_t P0_16;              /* 0x040 */
  __IO uint32_t P0_17;
  __IO uint32_t P0_18;
  __IO uint32_t P0_19;
  __IO uint32_t P0_20;
  __IO uint32_t P0_21;
  __IO uint32_t P0_22;
  __IO uint32_t P0_23;

  __IO uint32_t P0_24;              /* 0x060 */
  __IO uint32_t P0_25;
  __IO uint32_t P0_26;
  __IO uint32_t P0_27;
  __IO uint32_t P0_28;
  __IO uint32_t P0_29;
  __IO uint32_t P0_30;
  __IO uint32_t P0_31;

  __IO uint32_t P1_0;               /* 0x080 */
  __IO uint32_t P1_1;
  __IO uint32_t P1_2;
  __IO uint32_t P1_3;
  __IO uint32_t P1_4;
  __IO uint32_t P1_5;
  __IO uint32_t P1_6;
  __IO uint32_t P1_7;

  __IO uint32_t P1_8;               /* 0x0A0 */
  __IO uint32_t P1_9;
  __IO uint32_t P1_10;
  __IO uint32_t P1_11;
  __IO uint32_t P1_12;
  __IO uint32_t P1_13;
  __IO uint32_t P1_14;
  __IO uint32_t P1_15;

  __IO uint32_t P1_16;              /* 0x0C0 */
  __IO uint32_t P1_17;
  __IO uint32_t P1_18;
  __IO uint32_t P1_19;
  __IO uint32_t P1_20;
  __IO uint32_t P1_21;
  __IO uint32_t P1_22;
  __IO uint32_t P1_23;

  __IO uint32_t P1_24;              /* 0x0E0 */
  __IO uint32_t P1_25;
  __IO uint32_t P1_26;
  __IO uint32_t P1_27;
  __IO uint32_t P1_28;
  __IO uint32_t P1_29;
  __IO uint32_t P1_30;
  __IO uint32_t P1_31;

  __IO uint32_t P2_0;               /* 0x100 */
  __IO uint32_t P2_1;
  __IO uint32_t P2_2;
  __IO uint32_t P2_3;
  __IO uint32_t P2_4;
  __IO uint32_t P2_5;
  __IO uint32_t P2_6;
  __IO uint32_t P2_7;

  __IO uint32_t P2_8;               /* 0x120 */
  __IO uint32_t P2_9;
  __IO uint32_t P2_10;
  __IO uint32_t P2_11;
  __IO uint32_t P2_12;
  __IO uint32_t P2_13;
  __IO uint32_t P2_14;
  __IO uint32_t P2_15;

  __IO uint32_t P2_16;              /* 0x140 */
  __IO uint32_t P2_17;
  __IO uint32_t P2_18;
  __IO uint32_t P2_19;
  __IO uint32_t P2_20;
  __IO uint32_t P2_21;
  __IO uint32_t P2_22;
  __IO uint32_t P2_23;

  __IO uint32_t P2_24;              /* 0x160 */
  __IO uint32_t P2_25;
  __IO uint32_t P2_26;
  __IO uint32_t P2_27;
  __IO uint32_t P2_28;
  __IO uint32_t P2_29;
  __IO uint32_t P2_30;
  __IO uint32_t P2_31;

  __IO uint32_t P3_0;               /* 0x180 */
  __IO uint32_t P3_1;
  __IO uint32_t P3_2;
  __IO uint32_t P3_3;
  __IO uint32_t P3_4;
  __IO uint32_t P3_5;
  __IO uint32_t P3_6;
  __IO uint32_t P3_7;

  __IO uint32_t P3_8;               /* 0x1A0 */
  __IO uint32_t P3_9;
  __IO uint32_t P3_10;
  __IO uint32_t P3_11;
  __IO uint32_t P3_12;
  __IO uint32_t P3_13;
  __IO uint32_t P3_14;
  __IO uint32_t P3_15;

  __IO uint32_t P3_16;              /* 0x1C0 */
  __IO uint32_t P3_17;
  __IO uint32_t P3_18;
  __IO uint32_t P3_19;
  __IO uint32_t P3_20;
  __IO uint32_t P3_21;
  __IO uint32_t P3_22;
  __IO uint32_t P3_23;

  __IO uint32_t P3_24;              /* 0x1E0 */
  __IO uint32_t P3_25;
  __IO uint32_t P3_26;
  __IO uint32_t P3_27;
  __IO uint32_t P3_28;
  __IO uint32_t P3_29;
  __IO uint32_t P3_30;
  __IO uint32_t P3_31;

  __IO uint32_t P4_0;               /* 0x200 */
  __IO uint32_t P4_1;
  __IO uint32_t P4_2;
  __IO uint32_t P4_3;
  __IO uint32_t P4_4;
  __IO uint32_t P4_5;
  __IO uint32_t P4_6;
  __IO uint32_t P4_7;

  __IO uint32_t P4_8;               /* 0x220 */
  __IO uint32_t P4_9;
  __IO uint32_t P4_10;
  __IO uint32_t P4_11;
  __IO uint32_t P4_12;
  __IO uint32_t P4_13;
  __IO uint32_t P4_14;
  __IO uint32_t P4_15;

  __IO uint32_t P4_16;              /* 0x240 */
  __IO uint32_t P4_17;
  __IO uint32_t P4_18;
  __IO uint32_t P4_19;
  __IO uint32_t P4_20;
  __IO uint32_t P4_21;
  __IO uint32_t P4_22;
  __IO uint32_t P4_23;

  __IO uint32_t P4_24;              /* 0x260 */
  __IO uint32_t P4_25;
  __IO uint32_t P4_26;
  __IO uint32_t P4_27;
  __IO uint32_t P4_28;
  __IO uint32_t P4_29;
  __IO uint32_t P4_30;
  __IO uint32_t P4_31;

  __IO uint32_t P5_0;               /* 0x280 */
  __IO uint32_t P5_1;
  __IO uint32_t P5_2;
  __IO uint32_t P5_3;
  __IO uint32_t P5_4;               /* 0x290 */
} LPC_IOCON_TypeDef;

/*------------- General Purpose Input/Output (GPIO) --------------------------*/
typedef struct
{
  __IO uint32_t DIR;                        /*!< Offset: 0x000 (R/W)  GPIO port Direction Register */
       uint32_t RESERVED0[3];
  __IO uint32_t MASK;                       /*!< Offset: 0x010 (R/W)  GPIO port Mask Register */
  __IO uint32_t PIN;                        /*!< Offset: 0x014 (R/W)  GPIO port Pin value Register */
  __IO uint32_t SET;                        /*!< Offset: 0x018 (R/W)  GPIO port output Set Register */
  __O  uint32_t CLR;                        /*!< Offset: 0x01C ( /W)  GPIO port output Clear Register */
} LPC_GPIO_TypeDef;

typedef struct
{
  __I  uint32_t IntStatus;                  /*!< Offset: 0x000 (R/ )  GPIO overall Interrupt Status Register */
  __I  uint32_t IO0IntStatR;                /*!< Offset: 0x004 (R/ )  GPIO Interrupt Status Register 0 for Rising edge */
  __I  uint32_t IO0IntStatF;                /*!< Offset: 0x008 (R/ )  GPIO Interrupt Status Register 0 for Falling edge */
  __O  uint32_t IO0IntClr;                  /*!< Offset: 0x00C (R/W)  GPIO Interrupt Clear  Register 0 */
  __IO uint32_t IO0IntEnR;                  /*!< Offset: 0x010 ( /W)  GPIO Interrupt Enable Register 0 for Rising edge */
  __IO uint32_t IO0IntEnF;                  /*!< Offset: 0x014 (R/W)  GPIO Interrupt Enable Register 0 for Falling edge */
       uint32_t RESERVED0[3];
  __I  uint32_t IO2IntStatR;                /*!< Offset: 0x000 (R/ )  GPIO Interrupt Status Register 2 for Rising edge */
  __I  uint32_t IO2IntStatF;                /*!< Offset: 0x000 (R/ )  GPIO Interrupt Status Register 2 for Falling edge */
  __O  uint32_t IO2IntClr;                  /*!< Offset: 0x000 ( /W)  GPIO Interrupt Clear  Register 2 */
  __IO uint32_t IO2IntEnR;                  /*!< Offset: 0x000 (R/W)  GPIO Interrupt Enable Register 2 for Rising edge */
  __IO uint32_t IO2IntEnF;                  /*!< Offset: 0x000 (R/W)  GPIO Interrupt Enable Register 2 for Falling edge */
} LPC_GPIOINT_TypeDef;

/*------------- Timer (TIM) --------------------------------------------------*/
typedef struct
{
  __IO uint32_t IR;                         /*!< Offset: 0x000 (R/W)  Interrupt Register */
  __IO uint32_t TCR;                        /*!< Offset: 0x004 (R/W)  Timer Control Register */
  __IO uint32_t TC;                         /*!< Offset: 0x008 (R/W)  Timer Counter Register */
  __IO uint32_t PR;                         /*!< Offset: 0x00C (R/W)  Prescale Register */
  __IO uint32_t PC;                         /*!< Offset: 0x010 (R/W)  Prescale Counter Register */
  __IO uint32_t MCR;                        /*!< Offset: 0x014 (R/W)  Match Control Register */
  __IO uint32_t MR0;                        /*!< Offset: 0x018 (R/W)  Match Register 0 */
  __IO uint32_t MR1;                        /*!< Offset: 0x01C (R/W)  Match Register 1 */
  __IO uint32_t MR2;                        /*!< Offset: 0x020 (R/W)  Match Register 2 */
  __IO uint32_t MR3;                        /*!< Offset: 0x024 (R/W)  Match Register 3 */
  __IO uint32_t CCR;                        /*!< Offset: 0x028 (R/W)  Capture Control Register */
  __I  uint32_t CR0;                        /*!< Offset: 0x02C (R/ )  Capture Register 0 */
  __I  uint32_t CR1;                        /*!< Offset: 0x030 (R/ )  Capture Register */
       uint32_t RESERVED0[2];
  __IO uint32_t EMR;                        /*!< Offset: 0x03C (R/W)  External Match Register */
       uint32_t RESERVED1[12];
  __IO uint32_t CTCR;                       /*!< Offset: 0x070 (R/W)  Count Control Register */
} LPC_TIM_TypeDef;

/*------------- Pulse-Width Modulation (PWM) ---------------------------------*/
typedef struct
{
  __IO uint32_t IR;                         /*!< Offset: 0x000 (R/W)  Interrupt Register */
  __IO uint32_t TCR;                        /*!< Offset: 0x004 (R/W)  Timer Control Register. Register */
  __IO uint32_t TC;                         /*!< Offset: 0x008 (R/W)  Timer Counter Register */
  __IO uint32_t PR;                         /*!< Offset: 0x00C (R/W)  Prescale Register */
  __IO uint32_t PC;                         /*!< Offset: 0x010 (R/W)  Prescale Counter Register */
  __IO uint32_t MCR;                        /*!< Offset: 0x014 (R/W)  Match Control Register */
  __IO uint32_t MR0;                        /*!< Offset: 0x018 (R/W)  Match Register 0 */
  __IO uint32_t MR1;                        /*!< Offset: 0x01C (R/W)  Match Register 1 */
  __IO uint32_t MR2;                        /*!< Offset: 0x020 (R/W)  Match Register 2 */
  __IO uint32_t MR3;                        /*!< Offset: 0x024 (R/W)  Match Register 3 */
  __IO uint32_t CCR;                        /*!< Offset: 0x028 (R/W)  Capture Control Register */
  __I  uint32_t CR0;                        /*!< Offset: 0x02C (R/ )  Capture Register 0 */
  __I  uint32_t CR1;                        /*!< Offset: 0x030 (R/ )  Capture Register 1 */
  __I  uint32_t CR2;                        /*!< Offset: 0x034 (R/ )  Capture Register 2 */
  __I  uint32_t CR3;                        /*!< Offset: 0x038 (R/ )  Capture Register 3 */
       uint32_t RESERVED0;
  __IO uint32_t MR4;                        /*!< Offset: 0x040 (R/W)  Match Register 4 */
  __IO uint32_t MR5;                        /*!< Offset: 0x044 (R/W)  Match Register 5 */
  __IO uint32_t MR6;                        /*!< Offset: 0x048 (R/W)  Match Register 6 */
  __IO uint32_t PCR;                        /*!< Offset: 0x04C (R/W)  PWM Control Register */
  __IO uint32_t LER;                        /*!< Offset: 0x050 (R/W)  Load Enable Register */
       uint32_t RESERVED1[7];
  __IO uint32_t CTCR;                       /*!< Offset: 0x070 (R/W)  Count Control Register */
} LPC_PWM_TypeDef;

/*------------- Universal Asynchronous Receiver Transmitter (UART) -----------*/
typedef struct
{
  union {
  __I  uint8_t  RBR;                        /*!< Offset: 0x000 (R/ )  Receiver Buffer Register */
  __O  uint8_t  THR;                        /*!< Offset: 0x000 ( /W)  Transmit Holding Register */
  __IO uint8_t  DLL;                        /*!< Offset: 0x000 (R/W)  Divisor Latch LSB Register */
       uint32_t RESERVED0;
  };
  union {
  __IO uint8_t  DLM;                        /*!< Offset: 0x004 (R/W)  Divisor Latch MSB Register */
  __IO uint32_t IER;                        /*!< Offset: 0x004 (R/W)  Interrupt Enable Register */
  };
  union {
  __I  uint32_t IIR;                        /*!< Offset: 0x008 (R/ )  Interrupt ID Register */
  __O  uint8_t  FCR;                        /*!< Offset: 0x008 ( /W)  FIFO Control Register */
  };
  __IO uint8_t  LCR;                        /*!< Offset: 0x00C (R/W)  Line Control Register */
       uint8_t  RESERVED1[7];
  __I  uint8_t  LSR;                        /*!< Offset: 0x014 (R/ )  Line Control Register */
       uint8_t  RESERVED2[7];
  __IO uint8_t  SCR;                        /*!< Offset: 0x01C (R/W)  Scratch Pad Register */
       uint8_t  RESERVED3[3];
  __IO uint32_t ACR;                        /*!< Offset: 0x020 (R/W)  Auto-baud Control Register */
  __IO uint8_t  ICR;                        /*!< Offset: 0x024 (R/W)  IrDA Control Register */
       uint8_t  RESERVED4[3];
  __IO uint8_t  FDR;                        /*!< Offset: 0x028 (R/W)  Fractional Divider Register */
       uint8_t  RESERVED5[7];
  __IO uint8_t  TER;                        /*!< Offset: 0x030 (R/W)  Transmit Enable Register */
} LPC_UART_TypeDef;

typedef struct
{
  union {
  __I  uint8_t  RBR;                        /*!< Offset: 0x000 (R/ )  Receiver Buffer Register */
  __O  uint8_t  THR;                        /*!< Offset: 0x000 ( /W)  Transmit Holding Register */
  __IO uint8_t  DLL;                        /*!< Offset: 0x000 (R/W)  Divisor Latch LSB Register */
       uint32_t RESERVED0;
  };
  union {
  __IO uint8_t  DLM;                        /*!< Offset: 0x004 (R/W)  Divisor Latch MSB Register */
  __IO uint32_t IER;                        /*!< Offset: 0x004 (R/W)  Interrupt Enable Register */
  };
  union {
  __I  uint32_t IIR;                        /*!< Offset: 0x008 (R/ )  Interrupt ID Register */
  __O  uint8_t  FCR;                        /*!< Offset: 0x008 ( /W)  FIFO Control Register */
  };
  __IO uint8_t  LCR;                        /*!< Offset: 0x00C (R/W)  Line Control Register */
       uint8_t  RESERVED1[7];
  __I  uint8_t  LSR;                        /*!< Offset: 0x014 (R/ )  Line Control Register */
       uint8_t  RESERVED2[7];
  __IO uint8_t  SCR;                        /*!< Offset: 0x01C (R/W)  Scratch Pad Register */
       uint8_t  RESERVED3[3];
  __IO uint32_t ACR;                        /*!< Offset: 0x020 (R/W)  Auto-baud Control Register */
  __IO uint8_t  ICR;                        /*!< Offset: 0x024 (R/W)  IrDA Control Register */
       uint8_t  RESERVED4[3];
  __IO uint8_t  FDR;                        /*!< Offset: 0x028 (R/W)  Fractional Divider Register */
       uint8_t  RESERVED5[7];
  __IO uint8_t  TER;                        /*!< Offset: 0x030 (R/W)  Transmit Enable Register */
} LPC_UART0_TypeDef;

typedef struct
{
  union {
  __I  uint8_t  RBR;                        /*!< Offset: 0x000 (R/ )  Receiver Buffer Register */
  __O  uint8_t  THR;                        /*!< Offset: 0x000 ( /W)  Transmit Holding Register */
  __IO uint8_t  DLL;                        /*!< Offset: 0x000 (R/W)  Divisor Latch LSB Register */
       uint32_t RESERVED0;
  };
  union {
  __IO uint8_t  DLM;                        /*!< Offset: 0x004 (R/W)  Divisor Latch MSB Register */
  __IO uint32_t IER;                        /*!< Offset: 0x004 (R/W)  Interrupt Enable Register */
  };
  union {
  __I  uint32_t IIR;                        /*!< Offset: 0x008 (R/ )  Interrupt ID Register */
  __O  uint8_t  FCR;                        /*!< Offset: 0x008 ( /W)  FIFO Control Register */
  };
  __IO uint8_t  LCR;                        /*!< Offset: 0x00C (R/W)  Line Control Register */
       uint8_t  RESERVED1[3];
  __IO uint8_t  MCR;                        /*!< Offset: 0x010 (R/W)  Modem Control Register */
       uint8_t  RESERVED2[3];
  __I  uint8_t  LSR;                        /*!< Offset: 0x014 (R/ )  Line Control Register */
       uint8_t  RESERVED3[3];
  __I  uint8_t  MSR;                        /*!< Offset: 0x018 (R/ )  Modem Control Register */
       uint8_t  RESERVED4[3];
  __IO uint8_t  SCR;                        /*!< Offset: 0x01C (R/W)  Scratch Pad Register */
       uint8_t  RESERVED5[3];
  __IO uint32_t ACR;                        /*!< Offset: 0x020 (R/W)  Auto-baud Control Register */
       uint32_t RESERVED6;
  __IO uint8_t  FDR;                        /*!< Offset: 0x028 (R/W)  Fractional Divider Register */
       uint8_t  RESERVED7[7];
  __IO uint8_t  TER;                        /*!< Offset: 0x030 (R/W)  Transmit Enable Register */
       uint8_t  RESERVED8[27];
  __IO uint8_t  RS485CTRL;                  /*!< Offset: 0x04C (R/W)  RS-485/EIA-485 Control Register */
       uint8_t  RESERVED9[3];
  __IO uint8_t  ADRMATCH;                   /*!< Offset: 0x050 (R/W)  RS-485/EIA-485 address match Register */
       uint8_t  RESERVED10[3];
  __IO uint8_t  RS485DLY;                   /*!< Offset: 0x054 (R/W)  RS-485/EIA-485 direction control delay Register */
} LPC_UART1_TypeDef;

/*------------- Synchronous Serial Communication (SSP) -----------------------*/
typedef struct
{
  __IO uint32_t CR0;                        /*!< Offset: 0x000 (R/W)  Control Register 0 */
  __IO uint32_t CR1;                        /*!< Offset: 0x004 (R/W)  Control Register 1 */
  __IO uint32_t DR;                         /*!< Offset: 0x008 (R/W)  Data Register */
  __I  uint32_t SR;                         /*!< Offset: 0x00C (R/ )  Status Register */
  __IO uint32_t CPSR;                       /*!< Offset: 0x010 (R/W)  Clock Prescale Register */
  __IO uint32_t IMSC;                       /*!< Offset: 0x014 (R/W)  Interrupt Mask Set and Clear Register */
  __IO uint32_t RIS;                        /*!< Offset: 0x018 (R/W)  Raw Interrupt Status Register */
  __IO uint32_t MIS;                        /*!< Offset: 0x01C (R/W)  Masked Interrupt Status Register */
  __IO uint32_t ICR;                        /*!< Offset: 0x020 (R/W)  SSPICR Interrupt Clear Register */
  __IO uint32_t DMACR;                      /*!< Offset: 0x024 (R/W)  DMA Control Register */
} LPC_SSP_TypeDef;

/*------------- Inter-Integrated Circuit (I2C) -------------------------------*/
typedef struct
{
  __IO uint32_t CONSET;                     /*!< Offset: 0x000 (R/W)  I2C Control Set Register */
  __I  uint32_t STAT;                       /*!< Offset: 0x004 (R/ )  I2C Status Register */
  __IO uint32_t DAT;                        /*!< Offset: 0x008 (R/W)  I2C Data Register */
  __IO uint32_t ADR0;                       /*!< Offset: 0x00C (R/W)  I2C Slave Address Register 0 */
  __IO uint32_t SCLH;                       /*!< Offset: 0x010 (R/W)  SCH Duty Cycle Register High Half Word */
  __IO uint32_t SCLL;                       /*!< Offset: 0x014 (R/W)  SCL Duty Cycle Register Low Half Word */
  __O  uint32_t CONCLR;                     /*!< Offset: 0x018 (R/W)  I2C Control Clear Register */
  __IO uint32_t MMCTRL;                     /*!< Offset: 0x01C (R/W)  Monitor mode control register */
  __IO uint32_t ADR1;                       /*!< Offset: 0x020 (R/W)  I2C Slave Address Register 1 */
  __IO uint32_t ADR2;                       /*!< Offset: 0x024 (R/W)  I2C Slave Address Register 2 */
  __IO uint32_t ADR3;                       /*!< Offset: 0x028 (R/W)  I2C Slave Address Register 3 */
  __I  uint32_t DATA_BUFFER;                /*!< Offset: 0x02C (R/ )  Data buffer Register */
  __IO uint32_t MASK0;                      /*!< Offset: 0x030 (R/W)  I2C Slave address mask register 0 */
  __IO uint32_t MASK1;                      /*!< Offset: 0x034 (R/W)  I2C Slave address mask register 1 */
  __IO uint32_t MASK2;                      /*!< Offset: 0x038 (R/W)  I2C Slave address mask register 2 */
  __IO uint32_t MASK3;                      /*!< Offset: 0x03C (R/W)  I2C Slave address mask register 3 */
} LPC_I2C_TypeDef;

/*------------- Inter IC Sound (I2S) -----------------------------------------*/
typedef struct
{
  __IO uint32_t DAO;                        /*!< Offset: 0x000 (R/W)  Digital Audio Output Register */
  __IO uint32_t DAI;                        /*!< Offset: 0x004 (R/W)  Digital Audio Input Register */
  __O  uint32_t TXFIFO;                     /*!< Offset: 0x008 ( /W)  Transmit FIFO */
  __I  uint32_t RXFIFO;                     /*!< Offset: 0x00C (R/ )  Receive FIFO */
  __I  uint32_t STATE;                      /*!< Offset: 0x010 (R/W)  Status Feedback Register */
  __IO uint32_t DMA1;                       /*!< Offset: 0x014 (R/W)  DMA Configuration Register 1 */
  __IO uint32_t DMA2;                       /*!< Offset: 0x018 (R/W)  DMA Configuration Register 2 */
  __IO uint32_t IRQ;                        /*!< Offset: 0x01C (R/W)  Interrupt Request Control Register */
  __IO uint32_t TXRATE;                     /*!< Offset: 0x020 (R/W)  Transmit reference clock divider Register */
  __IO uint32_t RXRATE;                     /*!< Offset: 0x024 (R/W)  Receive reference clock divider Register */
  __IO uint32_t TXBITRATE;                  /*!< Offset: 0x028 (R/W)  Transmit bit rate divider Register */
  __IO uint32_t RXBITRATE;                  /*!< Offset: 0x02C (R/W)  Receive bit rate divider Register */
  __IO uint32_t TXMODE;                     /*!< Offset: 0x030 (R/W)  Transmit mode control Register */
  __IO uint32_t RXMODE;                     /*!< Offset: 0x034 (R/W)  Receive mode control Register */
} LPC_I2S_TypeDef;


/*------------- Real-Time Clock (RTC) ----------------------------------------*/
typedef struct
{
  __IO uint8_t  ILR;                        /*!< Offset: 0x000 (R/W)  Interrupt Location Register */
       uint8_t  RESERVED0[7];
  __IO uint8_t  CCR;                        /*!< Offset: 0x008 (R/W)  Clock Control Register */
       uint8_t  RESERVED1[3];
  __IO uint8_t  CIIR;                       /*!< Offset: 0x00C (R/W)  Counter Increment Interrupt Register */
       uint8_t  RESERVED2[3];
  __IO uint8_t  AMR;                        /*!< Offset: 0x010 (R/W)  Alarm Mask Register */
       uint8_t  RESERVED3[3];
  __I  uint32_t CTIME0;                     /*!< Offset: 0x014 (R/ )  Consolidated Time Register 0 */
  __I  uint32_t CTIME1;                     /*!< Offset: 0x018 (R/ )  Consolidated Time Register 1 */
  __I  uint32_t CTIME2;                     /*!< Offset: 0x01C (R/ )  Consolidated Time Register 2 */
  __IO uint8_t  SEC;                        /*!< Offset: 0x020 (R/W)  Seconds Counter Register */
       uint8_t  RESERVED4[3];
  __IO uint8_t  MIN;                        /*!< Offset: 0x024 (R/W)  Minutes Register */
       uint8_t  RESERVED5[3];
  __IO uint8_t  HOUR;                       /*!< Offset: 0x028 (R/W)  Hours Register */
       uint8_t  RESERVED6[3];
  __IO uint8_t  DOM;                        /*!< Offset: 0x02C (R/W)  Day of Month Register */
       uint8_t  RESERVED7[3];
  __IO uint8_t  DOW;                        /*!< Offset: 0x030 (R/W)  Day of Week Register */
       uint8_t  RESERVED8[3];
  __IO uint16_t DOY;                        /*!< Offset: 0x034 (R/W)  Day of Year Register */
       uint16_t RESERVED9;
  __IO uint8_t  MONTH;                      /*!< Offset: 0x038 (R/W)  Months Register */
       uint8_t  RESERVED10[3];
  __IO uint16_t YEAR;                       /*!< Offset: 0x03C (R/W)  Years Register */
       uint16_t RESERVED11;
  __IO uint32_t CALIBRATION;                /*!< Offset: 0x040 (R/W)  Calibration Value Register */
  __IO uint32_t GPREG0;                     /*!< Offset: 0x044 (R/W)  General Purpose Register 0 */
  __IO uint32_t GPREG1;                     /*!< Offset: 0x048 (R/W)  General Purpose Register 1 */
  __IO uint32_t GPREG2;                     /*!< Offset: 0x04C (R/W)  General Purpose Register 2 */
  __IO uint32_t GPREG3;                     /*!< Offset: 0x050 (R/W)  General Purpose Register 3 */
  __IO uint32_t GPREG4;                     /*!< Offset: 0x054 (R/W)  General Purpose Register 4 */
  __IO uint8_t  RTC_AUXEN;                  /*!< Offset: 0x058 (R/W)  RTC Auxiliary Enable Register */
       uint8_t  RESERVED12[3];
  __IO uint8_t  RTC_AUX;                    /*!< Offset: 0x05C (R/W)  RTC Auxiliary Control Register */
       uint8_t  RESERVED13[3];
  __IO uint8_t  ALSEC;                      /*!< Offset: 0x060 (R/W)  Alarm value for Seconds */
       uint8_t  RESERVED14[3];
  __IO uint8_t  ALMIN;                      /*!< Offset: 0x064 (R/W)  Alarm value for Minutes */
       uint8_t  RESERVED15[3];
  __IO uint8_t  ALHOUR;                     /*!< Offset: 0x068 (R/W)  Alarm value for Hours */
       uint8_t  RESERVED16[3];
  __IO uint8_t  ALDOM;                      /*!< Offset: 0x06C (R/W)  Alarm value for Day of Month */
       uint8_t  RESERVED17[3];
  __IO uint8_t  ALDOW;                      /*!< Offset: 0x070 (R/W)  Alarm value for Day of Week */
       uint8_t  RESERVED18[3];
  __IO uint16_t ALDOY;                      /*!< Offset: 0x074 (R/W)  Alarm value for Day of Year */
       uint16_t RESERVED19;
  __IO uint8_t  ALMON;                      /*!< Offset: 0x078 (R/W)  Alarm value for Months */
       uint8_t  RESERVED20[3];
  __IO uint16_t ALYEAR;                     /*!< Offset: 0x07C (R/W)  Alarm value for Year */
       uint16_t RESERVED21;
  __IO uint32_t ERSTATUS;                   /*!< Offset: 0x080 (R/W)  Event Recorder Status Register */
  __IO uint32_t ERCONTROL;                  /*!< Offset: 0x084 (R/W)  Event Recorder Control Register */
  __I  uint32_t ERCOUNTERS;                 /*!< Offset: 0x088 (R/ )  Event Recorder Counters Register */
       uint32_t RESERVED22;
  __I  uint32_t ERFIRSTSTAMP0;              /*!< Offset: 0x090 (R/ )  Event Recorder First Stamp Register Channel 0 */
  __I  uint32_t ERFIRSTSTAMP1;              /*!< Offset: 0x094 (R/ )  Event Recorder First Stamp Register Channel 1 */
  __I  uint32_t ERFIRSTSTAMP2;              /*!< Offset: 0x098 (R/ )  Event Recorder First Stamp Register Channel 2 */
       uint32_t RESERVED23;
  __I  uint32_t ERLASTSTAMP0;               /*!< Offset: 0x0A0 (R/ )  Event Recorder Last Stamp Register Channel 0 */
  __I  uint32_t ERLASTSTAMP1;               /*!< Offset: 0x0A4 (R/ )  Event Recorder Last Stamp Register Channel 1 */
  __I  uint32_t ERLASTSTAMP2;               /*!< Offset: 0x0A8 (R/ )  Event Recorder Last Stamp Register Channel 2 */
} LPC_RTC_TypeDef;

/*------------- Watchdog Timer (WDT) -----------------------------------------*/
typedef struct
{
  __IO uint8_t  MOD;                        /*!< Offset: 0x000 (R/W)  Watchdog mode Register */
       uint8_t  RESERVED0[3];
  __IO uint32_t TC;                         /*!< Offset: 0x004 (R/W)  Watchdog timer constant Register */
  __O  uint8_t  FEED;                       /*!< Offset: 0x008 ( /W)  Watchdog feed sequence Register */
       uint8_t  RESERVED1[3];
  __I  uint32_t TV;                         /*!< Offset: 0x00C (R/ )  Watchdog timer value Register */
       uint32_t RESERVED2;
  __IO uint32_t WARNINT;                    /*!< Offset: 0x010 (R/W)  Watchdog Warning Interrupt compare value Register */
  __IO uint32_t WINDOW;                     /*!< Offset: 0x014 (R/W)  Watchdog Window compare value Register */
} LPC_WDT_TypeDef;

/*------------- Analog-to-Digital Converter (ADC) ----------------------------*/
typedef struct
{
  __IO uint32_t CR;                         /*!< Offset: 0x000 (R/W)  A/D Control Register */
  __IO uint32_t GDR;                        /*!< Offset: 0x004 (R/W)  A/D Global Data Register */
       uint32_t RESERVED0;
  __IO uint32_t INTEN;                      /*!< Offset: 0x00C (R/W)  A/D Interrupt Enable Register */
  __I  uint32_t DR[8];                      /*!< Offset: 0x010 (R/ )  A/D Channel # Data Register */
  __I  uint32_t STAT;                       /*!< Offset: 0x030 (R/ )  A/D Status Register */
  __IO uint32_t ADTRM;                      /*!< Offset: 0x034 (R/W)  ADC trim Register */
} LPC_ADC_TypeDef;

/*------------- Digital-to-Analog Converter (DAC) ----------------------------*/
typedef struct
{
  __IO uint32_t CR;                         /*!< Offset: 0x000 (R/W)  D/A Converter Register */
  __IO uint32_t CTRL;                       /*!< Offset: 0x004 (R/W)  DAC Control register */
  __IO uint32_t CNTVAL;                     /*!< Offset: 0x008 (R/W)  DAC Counter Value Register */
} LPC_DAC_TypeDef;

/*------------- Motor Control Pulse-Width Modulation (MCPWM) -----------------*/
typedef struct
{
  __I  uint32_t CON;                        /*!< Offset: 0x000 (R/ )  PWM Control read address Register */
  __O  uint32_t CON_SET;                    /*!< Offset: 0x004 ( /W)  PWM Control set address Register */
  __O  uint32_t CON_CLR;                    /*!< Offset: 0x008 ( /W)  PWM Control clear address Register */
  __I  uint32_t CAPCON;                     /*!< Offset: 0x00C (R/ )  Capture Control read address Register */
  __O  uint32_t CAPCON_SET;                 /*!< Offset: 0x010 ( /W)  Capture Control set address Register */
  __O  uint32_t CAPCON_CLR;                 /*!< Offset: 0x014 ( /W)  Event Control clear address Register */
  __IO uint32_t TC0;                        /*!< Offset: 0x018 (R/W)  Timer Counter Register, channel 0 */
  __IO uint32_t TC1;                        /*!< Offset: 0x01C (R/W)  Timer Counter Register, channel 1 */
  __IO uint32_t TC2;                        /*!< Offset: 0x020 (R/W)  Timer Counter Register, channel 2 */
  __IO uint32_t LIM0;                       /*!< Offset: 0x024 (R/W)  Limit Register, channel 0 */
  __IO uint32_t LIM1;                       /*!< Offset: 0x028 (R/W)  Limit Register, channel 1 */
  __IO uint32_t LIM2;                       /*!< Offset: 0x02C (R/W)  Limit Register, channel 2 */
  __IO uint32_t MAT0;                       /*!< Offset: 0x030 (R/W)  Match Register, channel 0 */
  __IO uint32_t MAT1;                       /*!< Offset: 0x034 (R/W)  Match Register, channel 1 */
  __IO uint32_t MAT2;                       /*!< Offset: 0x038 (R/W)  Match Register, channel 2 */
  __IO uint32_t DT;                         /*!< Offset: 0x03C (R/W)  Dead time Register */
  __IO uint32_t CP;                         /*!< Offset: 0x040 (R/W)  Commutation Pattern Register */
  __IO uint32_t CAP0;                       /*!< Offset: 0x044 (R/W)  Capture Register, channel 0 */
  __IO uint32_t CAP1;                       /*!< Offset: 0x048 (R/W)  Capture Register, channel 1 */
  __IO uint32_t CAP2;                       /*!< Offset: 0x04C (R/W)  Capture Register, channel 2 */
  __I  uint32_t INTEN;                      /*!< Offset: 0x050 (R/ )  Interrupt Enable read Register */
  __O  uint32_t INTEN_SET;                  /*!< Offset: 0x054 ( /W)  Interrupt Enable set address Register */
  __O  uint32_t INTEN_CLR;                  /*!< Offset: 0x058 ( /W)  Interrupt Enable clear address Register */
  __I  uint32_t CNTCON;                     /*!< Offset: 0x05C (R/ )  Count Control read address Register */
  __O  uint32_t CNTCON_SET;                 /*!< Offset: 0x060 ( /W)  Count Control set address Register */
  __O  uint32_t CNTCON_CLR;                 /*!< Offset: 0x064 ( /W)  Count Control clear address Register */
  __I  uint32_t INTF;                       /*!< Offset: 0x068 (R/ )  Interrupt flags read address Register */
  __O  uint32_t INTF_SET;                   /*!< Offset: 0x06C ( /W)  Interrupt flags set address Register */
  __O  uint32_t INTF_CLR;                   /*!< Offset: 0x070 ( /W)  Interrupt flags clear address Register */
  __O  uint32_t CAP_CLR;                    /*!< Offset: 0x074 ( /W)  Capture clear address Register */
} LPC_MCPWM_TypeDef;

/*------------- Quadrature Encoder Interface (QEI) ---------------------------*/
typedef struct
{
  __O  uint32_t CON;                        /*!< Offset: 0x000 ( /W)  Control Register */
  __I  uint32_t STAT;                       /*!< Offset: 0x004 (R/ )  Encoder Status Register */
  __IO uint32_t CONF;                       /*!< Offset: 0x008 (R/W)  Configuration Register */
  __I  uint32_t POS;                        /*!< Offset: 0x00C (R/ )  Position Register */
  __IO uint32_t MAXPOS;                     /*!< Offset: 0x010 (R/W)  Maximum position Register */
  __IO uint32_t CMPOS0;                     /*!< Offset: 0x014 (R/W)  Position compare Register 0 */
  __IO uint32_t CMPOS1;                     /*!< Offset: 0x018 (R/W)  Position compare Register 1 */
  __IO uint32_t CMPOS2;                     /*!< Offset: 0x01C (R/W)  Position compare Register 2 */
  __I  uint32_t INXCNT;                     /*!< Offset: 0x020 (R/ )  Index count Register */
  __IO uint32_t INXCMP0;                    /*!< Offset: 0x024 (R/W)  Index compare Register 0 */
  __IO uint32_t LOAD;                       /*!< Offset: 0x028 (R/W)  Velocity timer reload Register */
  __I  uint32_t TIME;                       /*!< Offset: 0x02C (R/ )  Velocity timer Register */
  __I  uint32_t VEL;                        /*!< Offset: 0x030 (R/ )  Velocity counter Register */
  __I  uint32_t CAP;                        /*!< Offset: 0x034 (R/ )  Velocity capture Register */
  __IO uint32_t VELCOMP;                    /*!< Offset: 0x038 (R/W)  Velocity compare Register */
  __IO uint32_t FILTERPHA;                  /*!< Offset: 0x03C (R/W)  Digital filter Register on PHA */
  __IO uint32_t FILTERPHB;                  /*!< Offset: 0x040 (R/W)  Digital filter Register on PHB */
  __IO uint32_t FILTERINX;                  /*!< Offset: 0x044 (R/W)  Digital filter Register on IDX */
  __IO uint32_t WINDOW;                     /*!< Offset: 0x048 (R/W)  Index acceptance window Register */
  __IO uint32_t INXCMP1;                    /*!< Offset: 0x04C (R/W)  Index compare Register 1 */
  __IO uint32_t INXCMP2;                    /*!< Offset: 0x050 (R/W)  Index compare Register 2 */
       uint32_t RESERVED0[993];
  __O  uint32_t IEC;                        /*!< Offset: 0xFD8 ( /W)  Interrupt enable clear Register */
  __O  uint32_t IES;                        /*!< Offset: 0xFDC ( /W)  Interrupt enable set Register */
  __I  uint32_t INTSTAT;                    /*!< Offset: 0xFE0 (R/ )  Interrupt status Register */
  __I  uint32_t IE;                         /*!< Offset: 0xFE4 (R/ )  Interrupt enable Register */
  __O  uint32_t CLR;                        /*!< Offset: 0xFE8 ( /W)  Interrupt status clear Register */
  __O  uint32_t SET;                        /*!< Offset: 0xFEC ( /W)  Interrupt status set Register */
} LPC_QEI_TypeDef;

/*------------- SD/MMC card Interface (MCI)-----------------------------------*/
typedef struct
{
  __IO uint32_t POWER;                      /*!< Offset: 0x000 (R/W)  Power Control Register */
  __IO uint32_t CLOCK;                      /*!< Offset: 0x004 (R/W)  Clock Control Register */
  __IO uint32_t ARGUMENT;                   /*!< Offset: 0x008 (R/W)  Argument Register */
  __IO uint32_t COMMAND;                    /*!< Offset: 0x00C (R/W)  Command Register */
  __I  uint32_t RESP_CMD;                   /*!< Offset: 0x010 (R/ )  Response Command Register */
  __I  uint32_t RESP0;                      /*!< Offset: 0x014 (R/ )  Response Register 0 */
  __I  uint32_t RESP1;                      /*!< Offset: 0x018 (R/ )  Response Register 1 */
  __I  uint32_t RESP2;                      /*!< Offset: 0x01C (R/ )  Response Register 2 */
  __I  uint32_t RESP3;                      /*!< Offset: 0x020 (R/ )  Response Register 3 */
  __IO uint32_t DATATMR;                    /*!< Offset: 0x024 (R/W)  Data Timer Register */
  __IO uint32_t DATALEN;                    /*!< Offset: 0x028 (R/W)  Data Length Register */
  __IO uint32_t DATACTRL;                   /*!< Offset: 0x02C (R/W)  Data Control Register */
  __I  uint32_t DATACNT;                    /*!< Offset: 0x030 (R/ )  Data Counte Register */
  __I  uint32_t STATUS;                     /*!< Offset: 0x034 (R/ )  Status Register */
  __O  uint32_t CLEAR;                      /*!< Offset: 0x038 ( /W)  Clear Register */
  __IO uint32_t MASK0;                      /*!< Offset: 0x03C (R/W)  Interrupt Mask Register 0 */
       uint32_t RESERVED0[2];
  __I  uint32_t FIFOCNT;                    /*!< Offset: 0x048 (R/ )  FIFO Counter Register */
       uint32_t RESERVED1[13];
  __IO uint32_t FIFO;                       /*!< Offset: 0x080 (R/W)  Data FIFO Register */
} LPC_MCI_TypeDef;

/*------------- Controller Area Network (CAN) --------------------------------*/
typedef struct
{
  __IO uint32_t mask[512];                  /*!< Offset: 0x000 (R/W)  Acceptance Filter RAM */
} LPC_CANAF_RAM_TypeDef;

typedef struct                          /* Acceptance Filter Registers        */
{
  __IO uint32_t AFMR;                       /*!< Offset: 0x000 (R/W)  Acceptance Filter Register */
  __IO uint32_t SFF_sa;                     /*!< Offset: 0x004 (R/W)  Standard Frame Individual Start Address Register */
  __IO uint32_t SFF_GRP_sa;                 /*!< Offset: 0x008 (R/W)  Standard Frame Group Start Address Register */
  __IO uint32_t EFF_sa;                     /*!< Offset: 0x00C (R/W)  Extended Frame Start Address Register */
  __IO uint32_t EFF_GRP_sa;                 /*!< Offset: 0x010 (R/W)  Extended Frame Group Start Address Register */
  __IO uint32_t ENDofTable;                 /*!< Offset: 0x014 (R/W)  End of AF Tables Register */
  __I  uint32_t LUTerrAd;                   /*!< Offset: 0x018 (R/ )  LUT Error Address Register */
  __I  uint32_t LUTerr;                     /*!< Offset: 0x01C (R/ )  LUT Error Register */
  __IO uint32_t FCANIE;                     /*!< Offset: 0x020 (R/W)  Global FullCANInterrupt Enable Register */
  __IO uint32_t FCANIC0;                    /*!< Offset: 0x024 (R/W)  FullCAN Interrupt and Capture Register 0 */
  __IO uint32_t FCANIC1;                    /*!< Offset: 0x028 (R/W)  FullCAN Interrupt and Capture Register 1 */
} LPC_CANAF_TypeDef;

typedef struct                          /* Central Registers                  */
{
  __I  uint32_t TxSR;                       /*!< Offset: 0x000 (R/ )  CAN Central Transmit Status Register */
  __I  uint32_t RxSR;                       /*!< Offset: 0x004 (R/ )  CAN Central Receive Status Register */
  __I  uint32_t MSR;                        /*!< Offset: 0x008 (R/ )  CAN Central Miscellaneous Register */
} LPC_CANCR_TypeDef;

typedef struct                          /* Controller Registers               */
{
  __IO uint32_t MOD;                        /*!< Offset: 0x000 (R/W)  CAN Mode Register */
  __O  uint32_t CMR;                        /*!< Offset: 0x004 ( /W)  CAN Command Register */
  __IO uint32_t GSR;                        /*!< Offset: 0x008 (R/W)  CAN Global Status Register */
  __I  uint32_t ICR;                        /*!< Offset: 0x00C (R/ )  CAN Interrupt and Capture Register */
  __IO uint32_t IER;                        /*!< Offset: 0x010 (R/W)  CAN Interrupt Enable Register */
  __IO uint32_t BTR;                        /*!< Offset: 0x014 (R/W)  CAN Bus Timing Register */
  __IO uint32_t EWL;                        /*!< Offset: 0x018 (R/W)  CAN Error Warning Limit Register */
  __I  uint32_t SR;                         /*!< Offset: 0x01C (R/ )  CAN Status Register */
  __IO uint32_t RFS;                        /*!< Offset: 0x020 (R/W)  CAN Receive Frame Status Register */
  __IO uint32_t RID;                        /*!< Offset: 0x024 (R/W)  CAN Receive Identifier Register */
  __IO uint32_t RDA;                        /*!< Offset: 0x028 (R/W)  CAN Receive Data Register A */
  __IO uint32_t RDB;                        /*!< Offset: 0x02C (R/W)  CAN Receive Data Register B */
  __IO uint32_t TFI1;                       /*!< Offset: 0x030 (R/W)  CAN Transmit Frame Information Register 1 */
  __IO uint32_t TID1;                       /*!< Offset: 0x034 (R/W)  CAN Transmit Identifier Register 1 */
  __IO uint32_t TDA1;                       /*!< Offset: 0x038 (R/W)  CAN Transmit Data Register A 1 */
  __IO uint32_t TDB1;                       /*!< Offset: 0x03C (R/W)  CAN Transmit Data Register B 1 */
  __IO uint32_t TFI2;                       /*!< Offset: 0x040 (R/W)  CAN Transmit Frame Information Register 2 */
  __IO uint32_t TID2;                       /*!< Offset: 0x044 (R/W)  CAN Transmit Identifier Register 2 */
  __IO uint32_t TDA2;                       /*!< Offset: 0x048 (R/W)  CAN Transmit Data Register A 2 */
  __IO uint32_t TDB2;                       /*!< Offset: 0x04C (R/W)  CAN Transmit Data Register B 2 */
  __IO uint32_t TFI3;                       /*!< Offset: 0x050 (R/W)  CAN Transmit Frame Information Register 3 */
  __IO uint32_t TID3;                       /*!< Offset: 0x054 (R/W)  CAN Transmit Identifier Register 3 */
  __IO uint32_t TDA3;                       /*!< Offset: 0x058 (R/W)  CAN Transmit Data Register A 3 */
  __IO uint32_t TDB3;                       /*!< Offset: 0x05C (R/W)  CAN Transmit Data Register B 3 */
} LPC_CAN_TypeDef;

/*------------- General Purpose Direct Memory Access (GPDMA) -----------------*/
typedef struct                          /* Common Registers                   */
{
  __I  uint32_t IntStat;                    /*!< Offset: 0x000 (R/ )  DMA Interrupt Status Register */
  __I  uint32_t IntTCStat;                  /*!< Offset: 0x004 (R/ )  DMA Interrupt Terminal Count Request Status Register */
  __O  uint32_t IntTCClear;                 /*!< Offset: 0x008 ( /W)  DMA Interrupt Terminal Count Request Clear Register */
  __I  uint32_t IntErrStat;                 /*!< Offset: 0x00C (R/ )  DMA Interrupt Error Status Register */
  __O  uint32_t IntErrClr;                  /*!< Offset: 0x010 ( /W)  DMA Interrupt Error Clear Register */
  __I  uint32_t RawIntTCStat;               /*!< Offset: 0x014 (R/ )  DMA Raw Interrupt Terminal Count Status Register */
  __I  uint32_t RawIntErrStat;              /*!< Offset: 0x018 (R/ )  DMA Raw Error Interrupt Status Register */
  __I  uint32_t EnbldChns;                  /*!< Offset: 0x01C (R/ )  DMA Enabled Channel Register */
  __IO uint32_t SoftBReq;                   /*!< Offset: 0x020 (R/W)  DMA Software Burst Request Register */
  __IO uint32_t SoftSReq;                   /*!< Offset: 0x024 (R/W)  DMA Software Single Request Register */
  __IO uint32_t SoftLBReq;                  /*!< Offset: 0x028 (R/W)  DMA Software Last Burst Request Register */
  __IO uint32_t SoftLSReq;                  /*!< Offset: 0x02C (R/W)  DMA Software Last Single Request Register */
  __IO uint32_t Config;                     /*!< Offset: 0x030 (R/W)  DMA Configuration Register */
  __IO uint32_t Sync;                       /*!< Offset: 0x034 (R/W)  DMA Synchronization Register */
} LPC_GPDMA_TypeDef;

typedef struct                          /* Channel Registers                  */
{
  __IO uint32_t CSrcAddr;                   /*!< Offset: 0x000 (R/W)  DMA Channel # Source Address Register */
  __IO uint32_t CDestAddr;                  /*!< Offset: 0x004 (R/W)  DMA Channel # Destination Address Register */
  __IO uint32_t CLLI;                       /*!< Offset: 0x008 (R/W)  DMA Channel # Linked List Item Register */
  __IO uint32_t CControl;                   /*!< Offset: 0x00C (R/W)  DMA Channel # Control Register */
  __IO uint32_t CConfig;                    /*!< Offset: 0x010 (R/W)  DMA Channel # Configuration Register */
} LPC_GPDMACH_TypeDef;

/*------------- Universal Serial Bus (USB) -----------------------------------*/
typedef struct
{
  __I  uint32_t HcRevision;                 /*!< Offset: 0x000 (R/ )  Revision Register */
  __IO uint32_t HcControl;                  /*!< Offset: 0x004 (R/W)  Control Register */
  __IO uint32_t HcCommandStatus;            /*!< Offset: 0x008 (R/W)  Command / Status Register */
  __IO uint32_t HcInterruptStatus;          /*!< Offset: 0x00C (R/W)  Interrupt Status Register */
  __IO uint32_t HcInterruptEnable;          /*!< Offset: 0x010 (R/W)  Interrupt Enable Register */
  __IO uint32_t HcInterruptDisable;         /*!< Offset: 0x014 (R/W)  Interrupt Disable Register */
  __IO uint32_t HcHCCA;                     /*!< Offset: 0x018 (R/W)  Host Controller communication Area Register */
  __I  uint32_t HcPeriodCurrentED;          /*!< Offset: 0x01C (R/ )   Register */
  __IO uint32_t HcControlHeadED;            /*!< Offset: 0x020 (R/W)   Register */
  __IO uint32_t HcControlCurrentED;         /*!< Offset: 0x024 (R/W)   Register */
  __IO uint32_t HcBulkHeadED;               /*!< Offset: 0x028 (R/W)   Register */
  __IO uint32_t HcBulkCurrentED;            /*!< Offset: 0x02C (R/W)   Register */
  __I  uint32_t HcDoneHead;                 /*!< Offset: 0x030 (R/ )   Register */
  __IO uint32_t HcFmInterval;               /*!< Offset: 0x034 (R/W)   Register */
  __I  uint32_t HcFmRemaining;              /*!< Offset: 0x038 (R/ )   Register */
  __I  uint32_t HcFmNumber;                 /*!< Offset: 0x03C (R/ )   Register */
  __IO uint32_t HcPeriodicStart;            /*!< Offset: 0x040 (R/W)   Register */
  __IO uint32_t HcLSTreshold;               /*!< Offset: 0x044 (R/W)   Register */
  __IO uint32_t HcRhDescriptorA;            /*!< Offset: 0x048 (R/W)   Register */
  __IO uint32_t HcRhDescriptorB;            /*!< Offset: 0x04C (R/W)   Register */
  __IO uint32_t HcRhStatus;                 /*!< Offset: 0x050 (R/W)   Register */
  __IO uint32_t HcRhPortStatus1;            /*!< Offset: 0x054 (R/W)   Register */
  __IO uint32_t HcRhPortStatus2;            /*!< Offset: 0x05C (R/W)   Register */
       uint32_t RESERVED0[40];
  __I  uint32_t Module_ID;                  /*!< Offset: 0x0FC (R/ )  Module ID / Version Reverence ID Register */
                                            /* USB On-The-Go Registers            */
  __I  uint32_t OTGIntSt;                   /*!< Offset: 0x100 (R/ )  OTG Interrupt Status Register */
  __IO uint32_t OTGIntEn;                   /*!< Offset: 0x104 (R/W)  OTG Interrupt Enable Register */
  __O  uint32_t OTGIntSet;                  /*!< Offset: 0x108 ( /W)  OTG Interrupt Set Register */
  __O  uint32_t OTGIntClr;                  /*!< Offset: 0x10C ( /W)  OTG Interrupt Clear Register */
  __IO uint32_t OTGStCtrl;                  /*!< Offset: 0x110 (R/W)  OTG Status and Control Register */
  __IO uint32_t OTGTmr;                     /*!< Offset: 0x114 (R/W)  OTG Timer Register */
       uint32_t RESERVED1[58];
                                            /* USB Device Interrupt Registers     */
  __I  uint32_t USBDevIntSt;                /*!< Offset: 0x200 (R/ )  USB Device Interrupt Status Register */
  __IO uint32_t USBDevIntEn;                /*!< Offset: 0x204 (R/W)  USB Device Interrupt Enable Register */
  __O  uint32_t USBDevIntClr;               /*!< Offset: 0x208 ( /W)  USB Device Interrupt Clear Register */
  __O  uint32_t USBDevIntSet;               /*!< Offset: 0x20C ( /W)  USB Device Interrupt Set Register */
                                            /* USB Device SIE Command Registers   */
  __O  uint32_t USBCmdCode;                 /*!< Offset: 0x210 (R/W)  USB Command Code Register */
  __I  uint32_t USBCmdData;                 /*!< Offset: 0x214 (R/W)  USB Command Data Register */
                                            /* USB Device Transfer Registers      */
  __I  uint32_t USBRxData;                  /*!< Offset: 0x218 (R/ )  USB Receive Data Register */
  __O  uint32_t USBTxData;                  /*!< Offset: 0x21C ( /W)  USB Transmit Data Register */
  __I  uint32_t USBRxPLen;                  /*!< Offset: 0x220 (R/ )  USB Receive Packet Length Register */
  __O  uint32_t USBTxPLen;                  /*!< Offset: 0x224 ( /W)  USB Transmit Packet Length Register */
  __IO uint32_t USBCtrl;                    /*!< Offset: 0x228 (R/W)  USB Control Register */
  __O  uint32_t USBDevIntPri;               /*!< Offset: 0x22C (R/W)  USB Device Interrupt Priority Register */
                                            /* USB Device Endpoint Interrupt Regs */
  __I  uint32_t USBEpIntSt;                 /*!< Offset: 0x230 (R/ )  USB Endpoint Interrupt Status Register */
  __IO uint32_t USBEpIntEn;                 /*!< Offset: 0x234 (R/W)  USB Endpoint Interrupt Enable Register */
  __O  uint32_t USBEpIntClr;                /*!< Offset: 0x238 ( /W)  USB Endpoint Interrupt Clear Register */
  __O  uint32_t USBEpIntSet;                /*!< Offset: 0x23C ( /W)  USB Endpoint Interrupt Set Register */
  __O  uint32_t USBEpIntPri;                /*!< Offset: 0x240 ( /W)  USB Endpoint Interrupt Priority Register */
                                            /* USB Device Endpoint Realization Reg*/
  __IO uint32_t USBReEp;                    /*!< Offset: 0x244 (R/W)  USB Realize Endpoint Register */
  __O  uint32_t USBEpInd;                   /*!< Offset: 0x248 ( /W)  USB Endpoint Index Register */
  __IO uint32_t USBMaxPSize;                /*!< Offset: 0x24C (R/W)  USB MaxPacketSize Register */
                                            /* USB Device DMA Registers           */
  __I  uint32_t USBDMARSt;                  /*!< Offset: 0x250 (R/ )  USB DMA Request Status Register */
  __O  uint32_t USBDMARClr;                 /*!< Offset: 0x254 ( /W)  USB DMA Request Clear Register */
  __O  uint32_t USBDMARSet;                 /*!< Offset: 0x258 ( /W)  USB DMA Request Set Register */
       uint32_t RESERVED2[9];
  __IO uint32_t USBUDCAH;                   /*!< Offset: 0x280 (R/W)  USB UDCA Head Register */
  __I  uint32_t USBEpDMASt;                 /*!< Offset: 0x284 (R/ )  USB EP DMA Status Register */
  __O  uint32_t USBEpDMAEn;                 /*!< Offset: 0x288 ( /W)  USB EP DMA Enable Register */
  __O  uint32_t USBEpDMADis;                /*!< Offset: 0x28C ( /W)  USB EP DMA Disable Register */
  __I  uint32_t USBDMAIntSt;                /*!< Offset: 0x290 (R/ )  USB DMA Interrupt Status Register */
  __IO uint32_t USBDMAIntEn;                /*!< Offset: 0x294 (R/W)  USB DMA Interrupt Enable Register */
       uint32_t RESERVED3[2];
  __I  uint32_t USBEoTIntSt;                /*!< Offset: 0x2A0 (R/ )  USB End of Transfer Interrupt Status Register */
  __O  uint32_t USBEoTIntClr;               /*!< Offset: 0x2A4 ( /W)  USB End of Transfer Interrupt Clear Register */
  __O  uint32_t USBEoTIntSet;               /*!< Offset: 0x2A8 ( /W)  USB End of Transfer Interrupt Set Register */
  __I  uint32_t USBNDDRIntSt;               /*!< Offset: 0x2AC (R/ )  USB New DD Request Interrupt Status Register */
  __O  uint32_t USBNDDRIntClr;              /*!< Offset: 0x2B0 ( /W)  USB New DD Request Interrupt Clear Register */
  __O  uint32_t USBNDDRIntSet;              /*!< Offset: 0x2B4 ( /W)  USB New DD Request Interrupt Set Register */
  __I  uint32_t USBSysErrIntSt;             /*!< Offset: 0x2B8 (R/ )  USB System Error Interrupt Status Register */
  __O  uint32_t USBSysErrIntClr;            /*!< Offset: 0x2BC ( /W)  USB System Error Interrupt Clear Register */
  __O  uint32_t USBSysErrIntSet;            /*!< Offset: 0x2C0 ( /W)  USB System Error Interrupt Set Register */
       uint32_t RESERVED4[15];
                                            /* USB OTG I2C Registers              */
  union {
  __I  uint32_t I2C_RX;                     /*!< Offset: 0x300 (R/ )  OTG I2C Receive Register */
  __O  uint32_t I2C_TX;                     /*!< Offset: 0x300 ( /W)  OTG I2C Transmit Register */
  };
  __I  uint32_t I2C_STS;                    /*!< Offset: 0x304 (R/ )  OTG I2C Status Register */
  __IO uint32_t I2C_CTL;                    /*!< Offset: 0x308 (R/W)  OTG I2C Control Register */
  __IO uint32_t I2C_CLKHI;                  /*!< Offset: 0x30C (R/W)  OTG I2C Clock High Register */
  __O  uint32_t I2C_CLKLO;                  /*!< Offset: 0x310 ( /W)  OTG I2C Clock Low Register */
       uint32_t RESERVED5[824];
                                            /* USB Clock Control Registers        */
  union {
  __IO uint32_t USBClkCtrl;                 /*!< Offset: 0xFF4 (R/W)  OTG clock controller Register */
  __IO uint32_t OTGClkCtrl;                 /*!< Offset: 0xFF4 (R/W)  USB clock controller Register */
  };
  union {
  __I  uint32_t USBClkSt;                   /*!< Offset: 0xFF8 (R/ )  OTG clock status Register */
  __I  uint32_t OTGClkSt;                   /*!< Offset: 0xFF8 (R/ )  USB clock status Register */
  };
} LPC_USB_TypeDef;

/*------------- Ethernet Media Access Controller (EMAC) ----------------------*/
typedef struct
{
  __IO uint32_t MAC1;                       /*!< Offset: 0x000 (R/W)  MAC Configuration Register 1 */
  __IO uint32_t MAC2;                       /*!< Offset: 0x004 (R/W)  MAC Configuration Register 2 */
  __IO uint32_t IPGT;                       /*!< Offset: 0x008 (R/W)  Back-to-Back Inter-Packet-Gap Register */
  __IO uint32_t IPGR;                       /*!< Offset: 0x00C (R/W)  Non Back-to-Back Inter-Packet-Gap Register */
  __IO uint32_t CLRT;                       /*!< Offset: 0x010 (R/W)  Collision Window / Retry Register */
  __IO uint32_t MAXF;                       /*!< Offset: 0x014 (R/W)  Maximum Frame Register */
  __IO uint32_t SUPP;                       /*!< Offset: 0x018 (R/W)  PHY Support Register */
  __IO uint32_t TEST;                       /*!< Offset: 0x01C (R/W)  Test Register */
  __IO uint32_t MCFG;                       /*!< Offset: 0x020 (R/W)  MII Mgmt Configuration Register */
  __IO uint32_t MCMD;                       /*!< Offset: 0x024 (R/W)  MII Mgmt Command Register */
  __IO uint32_t MADR;                       /*!< Offset: 0x028 (R/W)  MII Mgmt Address Register */
  __O  uint32_t MWTD;                       /*!< Offset: 0x02C ( /W)  MII Mgmt Write Data Register */
  __I  uint32_t MRDD;                       /*!< Offset: 0x030 (R/ )  MII Mgmt Read Data Register */
  __I  uint32_t MIND;                       /*!< Offset: 0x034 (R/ )  MII Mgmt Indicators Register */
       uint32_t RESERVED0[2];
  __IO uint32_t SA0;                        /*!< Offset: 0x040 (R/W)  Station Address 0 Register */
  __IO uint32_t SA1;                        /*!< Offset: 0x044 (R/W)  Station Address 1 Register */
  __IO uint32_t SA2;                        /*!< Offset: 0x048 (R/W)  Station Address 2 Register */
       uint32_t RESERVED1[45];
  __IO uint32_t Command;                    /*!< Offset: 0x100 (R/W)  Command Register */
  __I  uint32_t Status;                     /*!< Offset: 0x104 (R/ )  Status Register */
  __IO uint32_t RxDescriptor;               /*!< Offset: 0x108 (R/W)  Receive Descriptor Base Address Register */
  __IO uint32_t RxStatus;                   /*!< Offset: 0x10C (R/W)  Receive Status Base Address Register */
  __IO uint32_t RxDescriptorNumber;         /*!< Offset: 0x110 (R/W)  Receive Number of Descriptors Register */
  __I  uint32_t RxProduceIndex;             /*!< Offset: 0x114 (R/ )  Receive Produce Index Register */
  __IO uint32_t RxConsumeIndex;             /*!< Offset: 0x118 (R/W)  Receive Consume Index Register */
  __IO uint32_t TxDescriptor;               /*!< Offset: 0x11C (R/W)  Transmit Descriptor Base Address Register */
  __IO uint32_t TxStatus;                   /*!< Offset: 0x120 (R/W)  Transmit Status Base Address Register */
  __IO uint32_t TxDescriptorNumber;         /*!< Offset: 0x124 (R/W)  Transmit Number of Descriptors Register */
  __IO uint32_t TxProduceIndex;             /*!< Offset: 0x128 (R/W)  Transmit Produce Index Register */
  __I  uint32_t TxConsumeIndex;             /*!< Offset: 0x12C (R/ )  Transmit Consume Index Register */
       uint32_t RESERVED2[10];
  __I  uint32_t TSV0;                       /*!< Offset: 0x158 (R/ )  Transmit Status Vector 0 Register */
  __I  uint32_t TSV1;                       /*!< Offset: 0x15C (R/ )  Transmit Status Vector 1 Register */
  __I  uint32_t RSV;                        /*!< Offset: 0x160 (R/ )  Receive Status Vector Register */
       uint32_t RESERVED3[3];
  __IO uint32_t FlowControlCounter;         /*!< Offset: 0x170 (R/W)  Flow Control Counter Register */
  __I  uint32_t FlowControlStatus;          /*!< Offset: 0x174 (R/ )  Flow Control Status egister */
       uint32_t RESERVED4[34];
  __IO uint32_t RxFilterCtrl;               /*!< Offset: 0x200 (R/W)  Receive Filter Control Register */
  __I  uint32_t RxFilterWoLStatus;          /*!< Offset: 0x204 (R/ )  Receive Filter WoL Status Register */
  __O  uint32_t RxFilterWoLClear;           /*!< Offset: 0x208 ( /W)  Receive Filter WoL Clear Register */
       uint32_t RESERVED5;
  __IO uint32_t HashFilterL;                /*!< Offset: 0x210 (R/W)  Hash Filter Table LSBs Register */
  __IO uint32_t HashFilterH;                /*!< Offset: 0x214 (R/W)  Hash Filter Table MSBs Register */
       uint32_t RESERVED6[882];
  __I  uint32_t IntStatus;                  /*!< Offset: 0xFE0 (R/ )  Interrupt Status Register */
  __IO uint32_t IntEnable;                  /*!< Offset: 0xFE4 (R/W)  Interrupt Enable Register */
  __O  uint32_t IntClear;                   /*!< Offset: 0xFE8 ( /W)  Interrupt Clear Register */
  __O  uint32_t IntSet;                     /*!< Offset: 0xFEC ( /W)  Interrupt Set Register */
       uint32_t RESERVED7;
  __IO uint32_t PowerDown;                  /*!< Offset: 0xFF4 (R/W)  Power-Down Register */
} LPC_EMAC_TypeDef;

/*------------- LCD controller (LCD) -----------------------------------------*/
typedef struct
{
  __IO uint32_t TIMH;                       /*!< Offset: 0x000 (R/W)  Horizontal Timing Control Register */
  __IO uint32_t TIMV;                       /*!< Offset: 0x004 (R/W)  Vertical Timing Control Register */
  __IO uint32_t POL;                        /*!< Offset: 0x008 (R/W)  Clock and Signal Polarity Control Register */
  __IO uint32_t LE;                         /*!< Offset: 0x00C (R/W)  Line End Control Register */
  __IO uint32_t UPBASE;                     /*!< Offset: 0x010 (R/W)  Upper Panel Frame Base Address Register */
  __IO uint32_t LPBASE;                     /*!< Offset: 0x014 (R/W)  Lower Panel Frame Base Address Register */
  __IO uint32_t CTRL;                       /*!< Offset: 0x018 (R/W)  LCD Control Register */
  __IO uint32_t INTMSK;                     /*!< Offset: 0x01C (R/W)  Interrupt Mask Register */
  __I  uint32_t INTRAW;                     /*!< Offset: 0x020 (R/ )  Raw Interrupt Status Register */
  __I  uint32_t INTSTAT;                    /*!< Offset: 0x024 (R/ )  Masked Interrupt Status Register */
  __O  uint32_t INTCLR;                     /*!< Offset: 0x028 ( /W)  Interrupt Clear Register */
  __I  uint32_t UPCURR;                     /*!< Offset: 0x02C (R/ )  Upper Panel Current Address Value Register */
  __I  uint32_t LPCURR;                     /*!< Offset: 0x030 (R/ )  Lower Panel Current Address Value Register */
       uint32_t RESERVED0[115];
  __IO uint32_t PAL[128];                   /*!< Offset: 0x200 (R/W)  256x16-bit Color Palette Register */
       uint32_t RESERVED1[256];
  __IO uint32_t CRSR_IMG[256];              /*!< Offset: 0x800 (R/W)  Cursor Image Register */
  __IO uint32_t CRSR_CTRL;                  /*!< Offset: 0xC00 (R/W)  Cursor Control Register */
  __IO uint32_t CRSR_CFG;                   /*!< Offset: 0xC04 (R/W)  Cursor Configuration Register */
  __IO uint32_t CRSR_PAL0;                  /*!< Offset: 0xC08 (R/W)  Cursor Palette Register 0 */
  __IO uint32_t CRSR_PAL1;                  /*!< Offset: 0xC0C (R/W)  Cursor Palette Register 1 */
  __IO uint32_t CRSR_XY;                    /*!< Offset: 0xC10 (R/W)  Cursor XY Position Register */
  __IO uint32_t CRSR_CLIP;                  /*!< Offset: 0xC14 (R/W)  Cursor Clip Position Register */
       uint32_t RESERVED2[2];
  __IO uint32_t CRSR_INTMSK;                /*!< Offset: 0xC20 (R/W)  Cursor Interrupt Mask Register */
  __O  uint32_t CRSR_INTCLR;                /*!< Offset: 0xC24 ( /W)  Cursor Interrupt Clear Register */
  __I  uint32_t CRSR_INTRAW;                /*!< Offset: 0xC28 (R/ )  Cursor Raw Interrupt Status Register */
  __I  uint32_t CRSR_INTSTAT;               /*!< Offset: 0xC2C (R/ )  Cursor Masked Interrupt Status Register */
} LPC_LCD_TypeDef;

/*------------- External Memory Controller (EMC) -----------------------------*/
typedef struct
{
  __IO uint32_t Control;                    /*!< Offset: 0x000 (R/W)  EMC Control Register */
  __I  uint32_t Status;                     /*!< Offset: 0x004 (R/ )  EMC Status Register */
  __IO uint32_t Config;                     /*!< Offset: 0x008 (R/W)  EMC Configuration Register */
       uint32_t RESERVED0[5];
  __IO uint32_t DynamicControl;             /*!< Offset: 0x020 (R/W)  Dynamic Memory Control Register */
  __IO uint32_t DynamicRefresh;             /*!< Offset: 0x024 (R/W)  Dynamic Memory Refresh Timer Register */
  __IO uint32_t DynamicReadConfig;          /*!< Offset: 0x028 (R/W)  Dynamic Memory Read Configuration Register */
       uint32_t RESERVED1[1];
  __IO uint32_t DynamicRP;                  /*!< Offset: 0x030 (R/W)  Dynamic Memory Precharge Command Period Register */
  __IO uint32_t DynamicRAS;                 /*!< Offset: 0x034 (R/W)  Dynamic Memory Active to Precharge Command Period Register */
  __IO uint32_t DynamicSREX;                /*!< Offset: 0x038 (R/W)  Dynamic Memory Self-refresh Exit Time Register */
  __IO uint32_t DynamicAPR;                 /*!< Offset: 0x03C (R/W)  Dynamic Memory Last Data Out to Active Time Register */
  __IO uint32_t DynamicDAL;                 /*!< Offset: 0x040 (R/W)  Dynamic Memory Data-in to Active Command Time Register */
  __IO uint32_t DynamicWR;                  /*!< Offset: 0x044 (R/W)  Dynamic Memory Write Recovery Time Register */
  __IO uint32_t DynamicRC;                  /*!< Offset: 0x048 (R/W)  Dynamic Memory Active to Active Command Period Register */
  __IO uint32_t DynamicRFC;                 /*!< Offset: 0x04C (R/W)  Dynamic Memory Auto-refresh Period Register */
  __IO uint32_t DynamicXSR;                 /*!< Offset: 0x050 (R/W)  Dynamic Memory Exit Self-refresh Register */
  __IO uint32_t DynamicRRD;                 /*!< Offset: 0x054 (R/W)  Dynamic Memory Active Bank A to Active Bank B Time Register */
  __IO uint32_t DynamicMRD;                 /*!< Offset: 0x058 (R/W)  Dynamic Memory Load Mode register to Active Command Time Register */
       uint32_t RESERVED2[9];
  __IO uint32_t StaticExtendedWait;         /*!< Offset: 0x080 (R/W)  Static Memory Extended Wait Register */
       uint32_t RESERVED3[31];
  __IO uint32_t DynamicConfig0;             /*!< Offset: 0x100 (R/W)  Dynamic Memory Configuration Register 0 */
  __IO uint32_t DynamicRasCas0;             /*!< Offset: 0x104 (R/W)  Dynamic Memory RAS & CAS Delay Register 0 */
       uint32_t RESERVED4[6];
  __IO uint32_t DynamicConfig1;             /*!< Offset: 0x120 (R/W)  Dynamic Memory Configuration Register 1 */
  __IO uint32_t DynamicRasCas1;             /*!< Offset: 0x124 (R/W)  Dynamic Memory RAS & CAS Delay Register 1 */
       uint32_t RESERVED5[6];
  __IO uint32_t DynamicConfig2;             /*!< Offset: 0x140 (R/W)  Dynamic Memory Configuration Register 2 */
  __IO uint32_t DynamicRasCas2;             /*!< Offset: 0x144 (R/W)  Dynamic Memory RAS & CAS Delay Register 2 */
       uint32_t RESERVED6[6];
  __IO uint32_t DynamicConfig3;             /*!< Offset: 0x160 (R/W)  Dynamic Memory Configuration Register 3 */
  __IO uint32_t DynamicRasCas3;             /*!< Offset: 0x164 (R/W)  Dynamic Memory RAS & CAS Delay Register 3 */
       uint32_t RESERVED7[38];
  __IO uint32_t StaticConfig0;              /*!< Offset: 0x200 (R/W)  Static Memory Configuration Register 0 */
  __IO uint32_t StaticWaitWen0;             /*!< Offset: 0x204 (R/W)  Static Memory Write Enable Delay Register 0 */
  __IO uint32_t StaticWaitOen0;             /*!< Offset: 0x208 (R/W)  Static Memory Output Enable Delay Register 0 */
  __IO uint32_t StaticWaitRd0;              /*!< Offset: 0x20C (R/W)  Static Memory Read Delay Register 0 */
  __IO uint32_t StaticWaitPage0;            /*!< Offset: 0x210 (R/W)  Static Memory Page Mode Read Delay Register 0 */
  __IO uint32_t StaticWaitWr0;              /*!< Offset: 0x214 (R/W)  Static Memory Write Delay Register 0 */
  __IO uint32_t StaticWaitTurn0;            /*!< Offset: 0x218 (R/W)  Static Memory Turn Round Delay Register 0 */
       uint32_t RESERVED8[1];
  __IO uint32_t StaticConfig1;              /*!< Offset: 0x220 (R/W)  Static Memory Configuration Register 1 */
  __IO uint32_t StaticWaitWen1;             /*!< Offset: 0x224 (R/W)  Static Memory Write Enable Delay Register 1 */
  __IO uint32_t StaticWaitOen1;             /*!< Offset: 0x228 (R/W)  Static Memory Output Enable Delay Register 1 */
  __IO uint32_t StaticWaitRd1;              /*!< Offset: 0x22C (R/W)  Static Memory Read Delay Register 1 */
  __IO uint32_t StaticWaitPage1;            /*!< Offset: 0x230 (R/W)  Static Memory Page Mode Read Delay Register 1 */
  __IO uint32_t StaticWaitWr1;              /*!< Offset: 0x234 (R/W)  Static Memory Write Delay Register 1 */
  __IO uint32_t StaticWaitTurn1;            /*!< Offset: 0x238 (R/W)  Static Memory Turn Round Delay Register 1 */
       uint32_t RESERVED9[1];
  __IO uint32_t StaticConfig2;              /*!< Offset: 0x240 (R/W)  Static Memory Configuration Register 2 */
  __IO uint32_t StaticWaitWen2;             /*!< Offset: 0x244 (R/W)  Static Memory Write Enable Delay Register 2 */
  __IO uint32_t StaticWaitOen2;             /*!< Offset: 0x248 (R/W)  Static Memory Output Enable Delay Register 2 */
  __IO uint32_t StaticWaitRd2;              /*!< Offset: 0x24C (R/W)  Static Memory Read Delay Register 2 */
  __IO uint32_t StaticWaitPage2;            /*!< Offset: 0x250 (R/W)  Static Memory Page Mode Read Delay Register 2 */
  __IO uint32_t StaticWaitWr2;              /*!< Offset: 0x254 (R/W)  Static Memory Write Delay Register 2 */
  __IO uint32_t StaticWaitTurn2;            /*!< Offset: 0x258 (R/W)  Static Memory Turn Round Delay Register 2 */
       uint32_t RESERVED10[1];
  __IO uint32_t StaticConfig3;              /*!< Offset: 0x260 (R/W)  Static Memory Configuration Register 3 */
  __IO uint32_t StaticWaitWen3;             /*!< Offset: 0x264 (R/W)  Static Memory Write Enable Delay Register 3 */
  __IO uint32_t StaticWaitOen3;             /*!< Offset: 0x268 (R/W)  Static Memory Output Enable Delay Register 3 */
  __IO uint32_t StaticWaitRd3;              /*!< Offset: 0x26C (R/W)  Static Memory Read Delay Register 3 */
  __IO uint32_t StaticWaitPage3;            /*!< Offset: 0x270 (R/W)  Static Memory Page Mode Read Delay Register 3 */
  __IO uint32_t StaticWaitWr3;              /*!< Offset: 0x274 (R/W)  Static Memory Write Delay Register 3 */
  __IO uint32_t StaticWaitTurn3;            /*!< Offset: 0x278 (R/W)  Static Memory Turn Round Delay Register 3 */
} LPC_EMC_TypeDef;

/*------------- CRC Engine (CRC) -----------------------------------------*/
typedef struct
{
  __IO uint32_t MODE;                       /*!< Offset: 0x000 (R/W)  CRC mode Register */
  __IO uint32_t SEED;                       /*!< Offset: 0x004 (R/W)  CRC seed Register */
  union {
      __I  uint32_t SUM;                    /*!< Offset: 0x008 (R/ )  CRC checksum Register */
      __O  uint32_t WR_DATA_32;             /*!< Offset: 0x008 ( /W)  CRC data Register DWORD access */
    struct {
      __O  uint16_t WR_DATA_16;             /*!< Offset: 0x008 ( /W)  CRC data Register WORD access */
           uint16_t RESERVED_16;
    };
    struct {
      __O  uint8_t WR_DATA_8;               /*!< Offset: 0x008 ( /W)  CRC data Register Byte access */
           uint8_t RESERVED_8[3];
    };
  };
} LPC_CRC_TypeDef;

/*------------- SPIFI --------------------------------------------------------*/
typedef struct
{
  __IO uint32_t Control;
  __IO uint32_t Command;
  __IO uint32_t Address;
  __IO uint32_t IntermediateData;
  __IO uint32_t AddressIntermediate;
  __IO uint32_t Data;
  __IO uint32_t MemoryCommand;
  __I  uint32_t Status;
} LPC_SPIFI_TypeDef;

/*------------- EEPROM Controller (EEPROM) -----------------------------------*/
typedef struct
{
  __IO uint32_t CMD;                        /*!< Offset: 0x080 (R/W)  EEPROM command Register */
  __IO uint32_t ADDR;                       /*!< Offset: 0x084 (R/W)  EEPROM address Register */
  __O  uint32_t WDATA;                      /*!< Offset: 0x088 ( /W)  EEPROM write data Register */
  __I  uint32_t RDATA;                      /*!< Offset: 0x08C (R/ )  EEPROM read data Register */
  __IO uint32_t WSTATE;                     /*!< Offset: 0x090 (R/W)  EEPROM wait state Register */
  __IO uint32_t CLKDIV;                     /*!< Offset: 0x094 (R/W)  EEPROM clock divider Register */
  __IO uint32_t PWRDWN;                     /*!< Offset: 0x098 (R/W)  EEPROM power-down Register */
       uint32_t RESERVED1[975];
  __IO uint32_t INTENCLR;                   /*!< Offset: 0xFD8 (R/W)  EEPROM interrupt enable clear Register */
  __IO uint32_t INTENSET;                   /*!< Offset: 0xFDC (R/W)  EEPROM power-down Register */
  __IO uint32_t INTSTAT;                    /*!< Offset: 0xFE0 (R/W)  EEPROM power-down Register */
  __IO uint32_t INTEN;                      /*!< Offset: 0xFE4 (R/W)  EEPROM power-down Register */
  __IO uint32_t INTSTATCLR;                 /*!< Offset: 0xFE8 (R/W)  EEPROM power-down Register */
  __IO uint32_t INTSTATSET;                 /*!< Offset: 0xFEC (R/W)  EEPROM power-down Register */
} LPC_EEPROM_TypeDef;

//#pragma no_anon_unions


/******************************************************************************/
/*                         Peripheral memory map                              */
/******************************************************************************/
/* Base addresses                                                             */
#define LPC_FLASH_BASE        (0x00000000UL)
#define LPC_RAM_BASE          (0x10000000UL)
#define LPC_APB0_BASE         (0x40000000UL)
#define LPC_APB1_BASE         (0x40080000UL)
#define LPC_AHB_BASE          (0x20080000UL)
#define LPC_CM3_BASE          (0xE0000000UL)

/* APB0 peripherals                                                           */
#define LPC_WDT_BASE          (LPC_APB0_BASE + 0x00000)
#define LPC_TIM0_BASE         (LPC_APB0_BASE + 0x04000)
#define LPC_TIM1_BASE         (LPC_APB0_BASE + 0x08000)
#define LPC_UART0_BASE        (LPC_APB0_BASE + 0x0C000)
#define LPC_UART1_BASE        (LPC_APB0_BASE + 0x10000)
#define LPC_PWM0_BASE         (LPC_APB0_BASE + 0x14000)
#define LPC_PWM1_BASE         (LPC_APB0_BASE + 0x18000)
#define LPC_I2C0_BASE         (LPC_APB0_BASE + 0x1C000)
#define LPC_RTC_BASE          (LPC_APB0_BASE + 0x24000)
#define LPC_GPIOINT_BASE      (LPC_APB0_BASE + 0x28080)
#define LPC_IOCON_BASE        (LPC_APB0_BASE + 0x2C000)
#define LPC_SSP1_BASE         (LPC_APB0_BASE + 0x30000)
#define LPC_ADC_BASE          (LPC_APB0_BASE + 0x34000)
#define LPC_CANAF_RAM_BASE    (LPC_APB0_BASE + 0x38000)
#define LPC_CANAF_BASE        (LPC_APB0_BASE + 0x3C000)
#define LPC_CANCR_BASE        (LPC_APB0_BASE + 0x40000)
#define LPC_CAN1_BASE         (LPC_APB0_BASE + 0x44000)
#define LPC_CAN2_BASE         (LPC_APB0_BASE + 0x48000)
#define LPC_I2C1_BASE         (LPC_APB0_BASE + 0x5C000)

/* APB1 peripherals                                                           */
#define LPC_SSP0_BASE         (LPC_APB1_BASE + 0x08000)
#define LPC_DAC_BASE          (LPC_APB1_BASE + 0x0C000)
#define LPC_TIM2_BASE         (LPC_APB1_BASE + 0x10000)
#define LPC_TIM3_BASE         (LPC_APB1_BASE + 0x14000)
#define LPC_UART2_BASE        (LPC_APB1_BASE + 0x18000)
#define LPC_UART3_BASE        (LPC_APB1_BASE + 0x1C000)
#define LPC_I2C2_BASE         (LPC_APB1_BASE + 0x20000)
#define LPC_UART4_BASE        (LPC_APB1_BASE + 0x24000)
#define LPC_I2S_BASE          (LPC_APB1_BASE + 0x28000)
#define LPC_SSP2_BASE         (LPC_APB1_BASE + 0x2C000)
#define LPC_MCPWM_BASE        (LPC_APB1_BASE + 0x38000)
#define LPC_QEI_BASE          (LPC_APB1_BASE + 0x3C000)
#define LPC_MCI_BASE          (LPC_APB1_BASE + 0x40000)
#define LPC_SC_BASE           (LPC_APB1_BASE + 0x7C000)

/* AHB peripherals                                                            */
#define LPC_GPDMA_BASE        (LPC_AHB_BASE  + 0x00000)
#define LPC_GPDMACH0_BASE     (LPC_AHB_BASE  + 0x00100)
#define LPC_GPDMACH1_BASE     (LPC_AHB_BASE  + 0x00120)
#define LPC_GPDMACH2_BASE     (LPC_AHB_BASE  + 0x00140)
#define LPC_GPDMACH3_BASE     (LPC_AHB_BASE  + 0x00160)
#define LPC_GPDMACH4_BASE     (LPC_AHB_BASE  + 0x00180)
#define LPC_GPDMACH5_BASE     (LPC_AHB_BASE  + 0x001A0)
#define LPC_GPDMACH6_BASE     (LPC_AHB_BASE  + 0x001C0)
#define LPC_GPDMACH7_BASE     (LPC_AHB_BASE  + 0x001E0)
#define LPC_EMAC_BASE         (LPC_AHB_BASE  + 0x04000)
#define LPC_LCD_BASE          (LPC_AHB_BASE  + 0x08000)
#define LPC_USB_BASE          (LPC_AHB_BASE  + 0x0C000)
#define LPC_CRC_BASE          (LPC_AHB_BASE  + 0x10000)
#define LPC_SPIFI_BASE        (LPC_AHB_BASE  + 0x14000)
#define LPC_GPIO0_BASE        (LPC_AHB_BASE  + 0x18000)
#define LPC_GPIO1_BASE        (LPC_AHB_BASE  + 0x18020)
#define LPC_GPIO2_BASE        (LPC_AHB_BASE  + 0x18040)
#define LPC_GPIO3_BASE        (LPC_AHB_BASE  + 0x18060)
#define LPC_GPIO4_BASE        (LPC_AHB_BASE  + 0x18080)
#define LPC_GPIO5_BASE        (LPC_AHB_BASE  + 0x180A0)
#define LPC_EMC_BASE          (LPC_AHB_BASE  + 0x1C000)

#define LPC_EEPROM_BASE       (LPC_FLASH_BASE+ 0x200080)


/******************************************************************************/
/*                         Peripheral declaration                             */
/******************************************************************************/
#define LPC_SC                ((LPC_SC_TypeDef        *) LPC_SC_BASE       )
#define LPC_WDT               ((LPC_WDT_TypeDef       *) LPC_WDT_BASE      )
#define LPC_TIM0              ((LPC_TIM_TypeDef       *) LPC_TIM0_BASE     )
#define LPC_TIM1              ((LPC_TIM_TypeDef       *) LPC_TIM1_BASE     )
#define LPC_TIM2              ((LPC_TIM_TypeDef       *) LPC_TIM2_BASE     )
#define LPC_TIM3              ((LPC_TIM_TypeDef       *) LPC_TIM3_BASE     )
#define LPC_UART0             ((LPC_UART_TypeDef      *) LPC_UART0_BASE    )
#define LPC_UART1             ((LPC_UART1_TypeDef     *) LPC_UART1_BASE    )
#define LPC_UART2             ((LPC_UART_TypeDef      *) LPC_UART2_BASE    )
#define LPC_UART3             ((LPC_UART_TypeDef      *) LPC_UART3_BASE    )
#define LPC_UART4             ((LPC_UART_TypeDef      *) LPC_UART4_BASE    )
#define LPC_PWM0              ((LPC_PWM_TypeDef       *) LPC_PWM0_BASE     )
#define LPC_PWM1              ((LPC_PWM_TypeDef       *) LPC_PWM1_BASE     )
#define LPC_I2C0              ((LPC_I2C_TypeDef       *) LPC_I2C0_BASE     )
#define LPC_I2C1              ((LPC_I2C_TypeDef       *) LPC_I2C1_BASE     )
#define LPC_I2C2              ((LPC_I2C_TypeDef       *) LPC_I2C2_BASE     )
#define LPC_I2S               ((LPC_I2S_TypeDef       *) LPC_I2S_BASE      )
#define LPC_RTC               ((LPC_RTC_TypeDef       *) LPC_RTC_BASE      )
#define LPC_GPIOINT           ((LPC_GPIOINT_TypeDef   *) LPC_GPIOINT_BASE  )
#define LPC_IOCON             ((LPC_IOCON_TypeDef     *) LPC_IOCON_BASE    )
#define LPC_SSP0              ((LPC_SSP_TypeDef       *) LPC_SSP0_BASE     )
#define LPC_SSP1              ((LPC_SSP_TypeDef       *) LPC_SSP1_BASE     )
#define LPC_SSP2              ((LPC_SSP_TypeDef       *) LPC_SSP2_BASE     )
#define LPC_ADC               ((LPC_ADC_TypeDef       *) LPC_ADC_BASE      )
#define LPC_DAC               ((LPC_DAC_TypeDef       *) LPC_DAC_BASE      )
#define LPC_CANAF_RAM         ((LPC_CANAF_RAM_TypeDef *) LPC_CANAF_RAM_BASE)
#define LPC_CANAF             ((LPC_CANAF_TypeDef     *) LPC_CANAF_BASE    )
#define LPC_CANCR             ((LPC_CANCR_TypeDef     *) LPC_CANCR_BASE    )
#define LPC_CAN1              ((LPC_CAN_TypeDef       *) LPC_CAN1_BASE     )
#define LPC_CAN2              ((LPC_CAN_TypeDef       *) LPC_CAN2_BASE     )
#define LPC_MCPWM             ((LPC_MCPWM_TypeDef     *) LPC_MCPWM_BASE    )
#define LPC_QEI               ((LPC_QEI_TypeDef       *) LPC_QEI_BASE      )
#define LPC_MCI               ((LPC_MCI_TypeDef       *) LPC_MCI_BASE      )
#define LPC_GPDMA             ((LPC_GPDMA_TypeDef     *) LPC_GPDMA_BASE    )
#define LPC_GPDMACH0          ((LPC_GPDMACH_TypeDef   *) LPC_GPDMACH0_BASE )
#define LPC_GPDMACH1          ((LPC_GPDMACH_TypeDef   *) LPC_GPDMACH1_BASE )
#define LPC_GPDMACH2          ((LPC_GPDMACH_TypeDef   *) LPC_GPDMACH2_BASE )
#define LPC_GPDMACH3          ((LPC_GPDMACH_TypeDef   *) LPC_GPDMACH3_BASE )
#define LPC_GPDMACH4          ((LPC_GPDMACH_TypeDef   *) LPC_GPDMACH4_BASE )
#define LPC_GPDMACH5          ((LPC_GPDMACH_TypeDef   *) LPC_GPDMACH5_BASE )
#define LPC_GPDMACH6          ((LPC_GPDMACH_TypeDef   *) LPC_GPDMACH6_BASE )
#define LPC_GPDMACH7          ((LPC_GPDMACH_TypeDef   *) LPC_GPDMACH7_BASE )
#define LPC_EMAC              ((LPC_EMAC_TypeDef      *) LPC_EMAC_BASE     )
#define LPC_LCD               ((LPC_LCD_TypeDef       *) LPC_LCD_BASE      )
#define LPC_USB               ((LPC_USB_TypeDef       *) LPC_USB_BASE      )
#define LPC_GPIO0             ((LPC_GPIO_TypeDef      *) LPC_GPIO0_BASE    )
#define LPC_GPIO1             ((LPC_GPIO_TypeDef      *) LPC_GPIO1_BASE    )
#define LPC_GPIO2             ((LPC_GPIO_TypeDef      *) LPC_GPIO2_BASE    )
#define LPC_GPIO3             ((LPC_GPIO_TypeDef      *) LPC_GPIO3_BASE    )
#define LPC_GPIO4             ((LPC_GPIO_TypeDef      *) LPC_GPIO4_BASE    )
#define LPC_GPIO5             ((LPC_GPIO_TypeDef      *) LPC_GPIO5_BASE    )
#define LPC_EMC               ((LPC_EMC_TypeDef       *) LPC_EMC_BASE      )
#define LPC_CRC               ((LPC_CRC_TypeDef       *) LPC_CRC_BASE      )
#define LPC_SPIFI             ((LPC_SPIFI_TypeDef     *) LPC_SPIFI_BASE    )
#define LPC_EEPROM            ((LPC_EEPROM_TypeDef    *) LPC_EEPROM_BASE   )

#endif  // __LPC177x_8x_H__
