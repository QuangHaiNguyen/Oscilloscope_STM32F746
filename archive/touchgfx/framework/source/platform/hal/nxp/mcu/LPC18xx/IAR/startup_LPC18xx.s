;/*****************************************************************************
; * @file:    startup_LPC18xx.s
; * @purpose: CMSIS Cortex-M3 Core Device Startup File
; *           for the NXP LPC18xx Device Series
; * @version: V1.00
; * @date:    24. May. 2011
; *----------------------------------------------------------------------------
; *
; * Copyright (C) 2010 ARM Limited. All rights reserved.
; *
; * ARM Limited (ARM) is supplying this software for use with Cortex-Mx
; * processor based microcontrollers.  This file can be freely distributed
; * within development tools that are supporting such ARM based processors.
; *
; * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
; * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
; * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
; * ARM SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
; * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
; *
; ******************************************************************************/


;
; The modules in this file are included in the libraries, and may be replaced
; by any user-defined modules that define the PUBLIC symbol _program_start or
; a user defined start symbol.
; To override the cstartup defined in the library, simply add your modified
; version to the workbench project.
;
; The vector table is normally located at address 0.
; When debugging in RAM, it can be located in RAM, aligned to at least 2^6.
; The name "__vector_table" has special meaning for C-SPY:
; it is where the SP start value is found, and the NVIC vector
; table register (VTOR) is initialized to this address if != 0.
;
; Cortex-M version
;

        MODULE  ?cstartup

        ;; Forward declaration of sections.
        SECTION CSTACK:DATA:NOROOT(3)

        SECTION .intvec:CODE:NOROOT(2)

        EXTERN  __iar_program_start
        EXTERN  SystemInit
        PUBLIC  __vector_table
        PUBLIC  __vector_table_0x1c
        PUBLIC  __Vectors
        PUBLIC  __Vectors_End
        PUBLIC  __Vectors_Size

        DATA

Sign_Value		EQU		0x5A5A5A5A

__vector_table
        DCD     sfe(CSTACK)
        DCD     Reset_Handler

        DCD     NMI_Handler
        DCD     HardFault_Handler
        DCD     MemManage_Handler
        DCD     BusFault_Handler
        DCD     UsageFault_Handler
__vector_table_0x1c
        DCD     Sign_Value
        DCD     0
        DCD     0
        DCD     0
        DCD     SVC_Handler
        DCD     DebugMon_Handler
        DCD     0
        DCD     PendSV_Handler
        DCD     SysTick_Handler

        ; External Interrupts
        DCD		DAC_IRQHandler	 			; 16 D/A Converter
        DCD		EVRT_IRQHandler				; 17 Event Router
        DCD		MyGPDMA_IRQHandler			; 18 General Purpose DMA
        DCD		0				        ; 19 Reserved
        DCD		0				        ; 20 Reserved
        DCD		ETH_IRQHandler				; 21 Ethernet
        DCD		SDIO_IRQHandler				; 22 SD/MMC
        DCD		MyLCD_IRQHandler    	        	; 23 LCD
        DCD		USB0_IRQHandler				; 24 USB0
        DCD		USB1_IRQHandler				; 25 USB1
        DCD		SCT_IRQHandler				; 26 State Configurable Timer
        DCD		RIT_IRQHandler				; 27 Repetitive Interrupt Timer
        DCD		TIMER0_IRQHandler			; 28 Timer0
        DCD		TIMER1_IRQHandler			; 29 Timer1
        DCD		TIMER2_IRQHandler			; 30 Timer2
        DCD		TIMER3_IRQHandler			; 31 Timer3
        DCD		MCPWM_IRQHandler			; 32 Motor Control PWM
        DCD		ADC0_IRQHandler				; 33 A/D Converter 0
        DCD		I2C0_IRQHandler				; 34 I2C0
        DCD		I2C1_IRQHandler				; 35 I2C1
        DCD		0					; 36 Reserved
        DCD		ADC1_IRQHandler				; 37 A/D Converter 1
        DCD		SSP0_IRQHandler				; 38 SSP0
        DCD		SSP1_IRQHandler				; 39 SSP1
        DCD		UART0_IRQHandler			; 40 UART0
        DCD		UART1_IRQHandler			; 41 UART1
        DCD		UART2_IRQHandler			; 42 UART2
        DCD		UART3_IRQHandler			; 43 UART3
        DCD		I2S_IRQHandler				; 44 I2S
        DCD 	        AES_IRQHandler				; 45 AES Engine
        DCD 	        SPIFI_IRQHandler			; 46 SPI Flash Interface
__Vectors_End

__Vectors       EQU   __vector_table
__Vectors_Size 	EQU 	__Vectors_End - __Vectors

 
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Default interrupt handlers.
;;
        THUMB

        PUBWEAK Reset_Handler
        SECTION .text:CODE:REORDER:NOROOT(2)
Reset_Handler
        LDR     R0, =__iar_program_start
        BX      R0

        PUBWEAK NMI_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
NMI_Handler
        B NMI_Handler

        PUBWEAK HardFault_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
HardFault_Handler
        B HardFault_Handler

        PUBWEAK MemManage_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
MemManage_Handler
        B MemManage_Handler

        PUBWEAK BusFault_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
BusFault_Handler
        B BusFault_Handler

        PUBWEAK UsageFault_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
UsageFault_Handler
        B UsageFault_Handler

        PUBWEAK SVC_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
SVC_Handler
        B SVC_Handler

        PUBWEAK DebugMon_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
DebugMon_Handler
        B DebugMon_Handler

        PUBWEAK PendSV_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
PendSV_Handler
        B PendSV_Handler

        PUBWEAK SysTick_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
SysTick_Handler
        B SysTick_Handler

        PUBWEAK DAC_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
DAC_IRQHandler
        B DAC_IRQHandler

        PUBWEAK EVRT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
EVRT_IRQHandler
        B EVRT_IRQHandler

        PUBWEAK MyGPDMA_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
MyGPDMA_IRQHandler
        B MyGPDMA_IRQHandler

        PUBWEAK ETH_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
ETH_IRQHandler
        B ETH_IRQHandler

        PUBWEAK SDIO_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
SDIO_IRQHandler
        B SDIO_IRQHandler

        PUBWEAK MyLCD_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
MyLCD_IRQHandler
        B MyLCD_IRQHandler

        PUBWEAK USB0_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
USB0_IRQHandler
        B USB0_IRQHandler

        PUBWEAK USB1_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
USB1_IRQHandler
        B USB1_IRQHandler

        PUBWEAK SCT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
SCT_IRQHandler
        B SCT_IRQHandler

        PUBWEAK RIT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
RIT_IRQHandler
        B RIT_IRQHandler

        PUBWEAK TIMER0_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
TIMER0_IRQHandler
        B TIMER0_IRQHandler

        PUBWEAK TIMER1_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
TIMER1_IRQHandler
        B TIMER1_IRQHandler

        PUBWEAK TIMER2_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
TIMER2_IRQHandler
        B TIMER2_IRQHandler

        PUBWEAK TIMER3_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
TIMER3_IRQHandler 
        B TIMER3_IRQHandler

        PUBWEAK MCPWM_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
MCPWM_IRQHandler
        B MCPWM_IRQHandler 

        PUBWEAK ADC0_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
ADC0_IRQHandler
        B ADC0_IRQHandler

        PUBWEAK I2C0_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
I2C0_IRQHandler
        B I2C0_IRQHandler

        PUBWEAK I2C1_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
I2C1_IRQHandler
        B I2C1_IRQHandler

        PUBWEAK ADC1_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
ADC1_IRQHandler
        B ADC1_IRQHandler

        PUBWEAK SSP0_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
SSP0_IRQHandler
        B SSP0_IRQHandler

        PUBWEAK SSP1_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
SSP1_IRQHandler
        B SSP1_IRQHandler

        PUBWEAK UART0_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
UART0_IRQHandler
        B UART0_IRQHandler

        PUBWEAK UART1_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
UART1_IRQHandler
        B UART1_IRQHandler

        PUBWEAK UART2_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
UART2_IRQHandler
        B UART2_IRQHandler

        PUBWEAK UART3_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
UART3_IRQHandler
        B UART3_IRQHandler

        PUBWEAK I2S_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
I2S_IRQHandler
        B I2S_IRQHandler

        PUBWEAK AES_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
AES_IRQHandler
        B AES_IRQHandler

        PUBWEAK SPIFI_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
SPIFI_IRQHandler
        B SPIFI_IRQHandler


        END
