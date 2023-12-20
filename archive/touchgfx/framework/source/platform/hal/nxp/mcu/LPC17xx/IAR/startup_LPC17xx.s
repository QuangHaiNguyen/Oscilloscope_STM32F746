;/*****************************************************************************
; * @file:    startup_LPC17xx.s
; * @purpose: CMSIS Cortex-M3 Core Device Startup File
; *           for the NXP LPC17xx Device Series
; * @version: V1.02
; * @date:    31. July 2009
; *----------------------------------------------------------------------------
; *
; * Copyright (C) 2009 ARM Limited. All rights reserved.
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

 ;       INCLUDE Config.h

        MODULE  ?cstartup

        ;; Forward declaration of sections.
        SECTION CSTACK:DATA:NOROOT(3)

        SECTION .intvec:CODE:NOROOT(2)
	
        EXTERN  __iar_program_start
        EXTERN  SystemInit
        EXTERN  vSafeRTOSSVCHandler
        EXTERN  vSafeRTOSPendSVHandler
        EXTERN  vPortSysTickHandler
        EXTERN  HardFault_HandlerC
		
        PUBLIC  __vector_table
        PUBLIC  __vector_table_0x1c
        PUBLIC  __Vectors
        PUBLIC  __Vectors_End
        PUBLIC  __Vectors_Size

		IMPORT xPortSysTickHandler
		IMPORT vPortSVCHandler
		IMPORT xPortPendSVHandler


        DATA

__vector_table
        DCD     sfe(CSTACK)
        DCD     Reset_Handler

        DCD     NMI_Handler
        DCD     HardFault_Handler
        DCD     MemManage_Handler
        DCD     BusFault_Handler
        DCD     UsageFault_Handler
__vector_table_0x1c
        DCD     0
        DCD     0
        DCD     0
        DCD     0
        DCD     SVC_Handler           ;SVC_Handler
        DCD     DebugMon_Handler
        DCD     0
        DCD     PendSV_Handler        ;PendSV_Handler
        DCD     SysTick_Handler       ;SysTick_Handler


        ; External Interrupts
        DCD     WDT_IRQHandler            ; 16: Watchdog Timer
        DCD     TIMER0_IRQHandler         ; 17: Timer0
        DCD     TIMER1_IRQHandler         ; 18: Timer1
        DCD     TIMER2_IRQHandler         ; 19: Timer2
        DCD     TIMER3_IRQHandler         ; 20: Timer3
        DCD     MyUART0_IRQHandler          ; 21: UART0
        DCD     UART1_IRQHandler          ; 22: UART1
        DCD     UART2_IRQHandler          ; 23: UART2
        DCD     UART3_IRQHandler          ; 24: UART3
        DCD     PWM1_IRQHandler           ; 25: PWM1
        DCD     I2C0_IRQHandler           ; 26: I2C0
        DCD     I2C1_IRQHandler           ; 27: I2C1
        DCD     I2C2_IRQHandler           ; 28: I2C2
        DCD     SPIFI_IRQHandler          ; 29: SPIFI
        DCD     SSP0_IRQHandler           ; 30: SSP0
        DCD     SSP1_IRQHandler           ; 31: SSP1
        DCD     PLL0_IRQHandler           ; 32: PLL0 Lock (Main PLL)
        DCD     RTC_IRQHandler            ; 33: Real Time Clock
        DCD     EINT0_IRQHandler          ; 34: External Interrupt 0
        DCD     EINT1_IRQHandler          ; 35: External Interrupt 1
        DCD     EINT2_IRQHandler          ; 36: External Interrupt 2
        DCD     EINT3_IRQHandler          ; 37: External Interrupt 3
        DCD     ADC_IRQHandler            ; 38: A/D Converter
        DCD     BOD_IRQHandler            ; 39: Brown-Out Detect
        DCD     USB_IRQHandler            ; 40: USB
        DCD     CAN_IRQHandler            ; 41: CAN
        DCD     GPDMA_IRQHandler          ; 42: General Purpose DMA
        DCD     I2S_IRQHandler            ; 43: I2S
        DCD     ENET_IRQHandler           ; 44: Ethernet
        DCD     MCI_IRQHandler            ; 45: MCI
        DCD     MCPWM_IRQHandler          ; 46: Motor Control PWM
        DCD     QEI_IRQHandler            ; 47: Quadrature Encoder Interface
        DCD     PLL1_IRQHandler           ; 48: PLL1 Lock (USB PLL)
        DCD     USBACT_IRQHandler         ; 49: USB Activity
        DCD     CANACT_IRQHandler         ; 50: CAN Activity
        DCD     UART4_IRQHandler          ; 51: UART4
        DCD     SSP2_IRQHandler           ; 52: SSP2
        DCD     LCD_IRQHandler          ; 53: LCD
        DCD     MyGPIO_IRQHandler           ; 54: GPIO
        DCD     PWM0_IRQHandler           ; 55: PWM0
        DCD     KFLASH_IRQHandler         ; 56: KFLASH
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
        LDR     R0, =__vector_table
        LDR     R1, =0xE000ED08 ; VTOR
        STR     R0, [R1]
        LDR     R0, =SystemInit
        BLX     R0
        LDR     R0, =__iar_program_start
        BX      R0

        PUBWEAK NMI_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
NMI_Handler
        mov r0, #4
        b NMI_Handler

        PUBWEAK HardFault_Handler
        SECTION .text:CODE:REORDER:NOROOT(1) 
HardFault_Handler
        ;mov r0, #5
        ;b UEZBSP_FatalError
        TST LR, #4
        ITE EQ
        MRSEQ r0, MSP
        MRSNE R0, PSP
        b HardFault_Handler

        PUBWEAK MemManage_Handler 
        SECTION .text:CODE:REORDER:NOROOT(1)
MemManage_Handler
        mov r0, #6
        b MemManage_Handler

        PUBWEAK BusFault_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
BusFault_Handler
        mov r0, #7
        b BusFault_Handler

        PUBWEAK UsageFault_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
UsageFault_Handler
        mov r0, #8
        b UsageFault_Handler

        PUBWEAK SVC_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
SVC_Handler
        mov r0, #9
        b SVC_Handler

        PUBWEAK DebugMon_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
DebugMon_Handler
        mov r0, #10
        b DebugMon_Handler

        PUBWEAK PendSV_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
PendSV_Handler
        mov r0, #11
        b PendSV_Handler

        PUBWEAK SysTick_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
SysTick_Handler
        B SysTick_Handler

        PUBWEAK WDT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
WDT_IRQHandler
        B WDT_IRQHandler

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

        PUBWEAK MyUART0_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
MyUART0_IRQHandler
        B MyUART0_IRQHandler

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

        PUBWEAK PWM1_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
PWM1_IRQHandler
        B PWM1_IRQHandler

        PUBWEAK I2C0_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
I2C0_IRQHandler
        B I2C0_IRQHandler

        PUBWEAK I2C1_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
I2C1_IRQHandler
        B I2C1_IRQHandler

        PUBWEAK I2C2_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
I2C2_IRQHandler
        B I2C2_IRQHandler

        PUBWEAK SPIFI_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
SPIFI_IRQHandler
        B SPIFI_IRQHandler

        PUBWEAK SSP0_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
SSP0_IRQHandler
        B SSP0_IRQHandler

        PUBWEAK SSP1_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
SSP1_IRQHandler
        B SSP1_IRQHandler

        PUBWEAK PLL0_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
PLL0_IRQHandler
        B PLL0_IRQHandler

        PUBWEAK RTC_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
RTC_IRQHandler
        B RTC_IRQHandler

        PUBWEAK EINT0_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
EINT0_IRQHandler
        B EINT0_IRQHandler

        PUBWEAK EINT1_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
EINT1_IRQHandler
        B EINT1_IRQHandler

        PUBWEAK EINT2_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
EINT2_IRQHandler
        B EINT2_IRQHandler

        PUBWEAK EINT3_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
EINT3_IRQHandler
        B EINT3_IRQHandler

        PUBWEAK ADC_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
ADC_IRQHandler
        B ADC_IRQHandler

        PUBWEAK BOD_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
BOD_IRQHandler
        B BOD_IRQHandler

        PUBWEAK USB_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
USB_IRQHandler
        B USB_IRQHandler

        PUBWEAK CAN_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
CAN_IRQHandler
        B CAN_IRQHandler

        PUBWEAK GPDMA_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
GPDMA_IRQHandler
        B GPDMA_IRQHandler

        PUBWEAK I2S_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
I2S_IRQHandler
        B I2S_IRQHandler

        PUBWEAK ENET_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
ENET_IRQHandler
        B ENET_IRQHandler

        PUBWEAK MCI_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
MCI_IRQHandler
        B MCI_IRQHandler

        PUBWEAK MCPWM_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
MCPWM_IRQHandler
        B MCPWM_IRQHandler

        PUBWEAK QEI_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
QEI_IRQHandler
        B QEI_IRQHandler

        PUBWEAK PLL1_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
PLL1_IRQHandler
        B PLL1_IRQHandler

        PUBWEAK USBACT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
USBACT_IRQHandler
        B USBACT_IRQHandler

        PUBWEAK CANACT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
CANACT_IRQHandler
        B CANACT_IRQHandler

        PUBWEAK UART4_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
UART4_IRQHandler
        B UART4_IRQHandler

        PUBWEAK SSP2_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
SSP2_IRQHandler
        B SSP2_IRQHandler

        PUBWEAK LCD_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
LCD_IRQHandler
        B LCD_IRQHandler

        PUBWEAK MyGPIO_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
MyGPIO_IRQHandler
        B MyGPIO_IRQHandler

        PUBWEAK PWM0_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
PWM0_IRQHandler
        B PWM0_IRQHandler

        PUBWEAK KFLASH_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
KFLASH_IRQHandler
        B KFLASH_IRQHandler

        END
