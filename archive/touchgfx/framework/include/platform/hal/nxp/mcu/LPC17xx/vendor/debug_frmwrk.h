/**********************************************************************
* $Id$          debug_frmwrk.h                  2011-06-02
*//**
* @file         debug_frmwrk.h
* @brief        Contains some utilities that used for debugging through UART
* @version      1.0
* @date         02. June. 2011
* @author       NXP MCU SW Application Team
* 
* Copyright(C) 2011, NXP Semiconductor
* All rights reserved.
*
***********************************************************************
* Software that is described herein is for illustrative purposes only
* which provides customers with programming information regarding the
* products. This software is supplied "AS IS" without any warranties.
* NXP Semiconductors assumes no responsibility or liability for the
* use of the software, conveys no license or title under any patent,
* copyright, or mask work right to the product. NXP Semiconductors
* reserves the right to make changes in the software without
* notification. NXP Semiconductors also make no representation or
* warranty that such application will be suitable for the specified
* use without further testing or modification.
**********************************************************************/

#ifndef __DEBUG_FRMWRK_H_
#define __DEBUG_FRMWRK_H_

#include "lpc177x_8x_uart.h"

#define USED_UART_DEBUG_PORT    2

#define NUM_SKIPPED_ALLOWED                     (10)

#define DBG_GETVAL_IN_DEC                       (0)
#define DBG_GETVAL_IN_HEX                       (1)
//#define DBG_GETVAL_IN_CHARS                   (2)

#if (USED_UART_DEBUG_PORT == 0)
#define DEBUG_UART_PORT (LPC_UART_TypeDef *)LPC_UART0
#elif (USED_UART_DEBUG_PORT == 1)
#define DEBUG_UART_PORT (LPC_UART_TypeDef *)LPC_UART1
#elif (USED_UART_DEBUG_PORT == 2)
#define DEBUG_UART_PORT (LPC_UART_TypeDef *)LPC_UART2
#elif (USED_UART_DEBUG_PORT == 3)
#define DEBUG_UART_PORT (LPC_UART_TypeDef *)LPC_UART3
#elif (USED_UART_DEBUG_PORT == 4)
#define DEBUG_UART_PORT (LPC_UART_TypeDef *)LPC_UART4
#else
 #error "Invalid UART port selection!"
#endif

#define _DBG(x)                                                         _db_msg(DEBUG_UART_PORT, x)
#define _DBG_(x)                                                        _db_msg_(DEBUG_UART_PORT, x)

#define _DBC(x)                                                         _db_char(DEBUG_UART_PORT, x)

#define _DBD(x)                                                         _db_dec(DEBUG_UART_PORT, x)
#define _DBD16(x)                                                       _db_dec_16(DEBUG_UART_PORT, x)
#define _DBD32(x)                                                       _db_dec_32(DEBUG_UART_PORT, x)

#define _DBH(x)                                                         _db_hex(DEBUG_UART_PORT, x)
#define _DBH16(x)                                                       _db_hex_16(DEBUG_UART_PORT, x)
#define _DBH32(x)                                                       _db_hex_32(DEBUG_UART_PORT, x)

#define _DBH_(x)                                                        _db_hex_(DEBUG_UART_PORT, x)
#define _DBH16_(x)                                                      _db_hex_16_(DEBUG_UART_PORT, x)
#define _DBH32_(x)                                                      _db_hex_32_(DEBUG_UART_PORT, x)

#define _DG                                                                     _db_get_char(DEBUG_UART_PORT)
#define _DGV(option, numCh, val)                        _db_get_val(DEBUG_UART_PORT, option, numCh, val)

//void  _printf (const  char *format, ...);

extern void (*_db_msg)(LPC_UART_TypeDef *UARTx, const void *s);
extern void (*_db_msg_)(LPC_UART_TypeDef *UARTx, const void *s);
extern void (*_db_char)(LPC_UART_TypeDef *UARTx, uint8_t ch);
extern void (*_db_dec)(LPC_UART_TypeDef *UARTx, uint8_t decn);
extern void (*_db_dec_16)(LPC_UART_TypeDef *UARTx, uint16_t decn);
extern void (*_db_dec_32)(LPC_UART_TypeDef *UARTx, uint32_t decn);
extern void (*_db_hex)(LPC_UART_TypeDef *UARTx, uint8_t hexn);
extern void (*_db_hex_16)(LPC_UART_TypeDef *UARTx, uint16_t hexn);
extern void (*_db_hex_32)(LPC_UART_TypeDef *UARTx, uint32_t hexn);
extern void (*_db_hex_)(LPC_UART_TypeDef *UARTx, uint8_t hexn);
extern void (*_db_hex_16_)(LPC_UART_TypeDef *UARTx, uint16_t hexn);
extern void (*_db_hex_32_)(LPC_UART_TypeDef *UARTx, uint32_t hexn);

extern uint8_t (*_db_get_char)(LPC_UART_TypeDef *UARTx);
extern uint8_t (*_db_get_val)(LPC_UART_TypeDef *UARTx, uint8_t option, uint8_t numCh, uint32_t * val);

uint8_t UARTGetValue (LPC_UART_TypeDef *UARTx, uint8_t option,
                                                                                        uint8_t numCh, uint32_t* val);
void UARTPutChar (LPC_UART_TypeDef *UARTx, uint8_t ch);
void UARTPuts(LPC_UART_TypeDef *UARTx, const void *str);
void UARTPuts_(LPC_UART_TypeDef *UARTx, const void *str);
void UARTPutDec(LPC_UART_TypeDef *UARTx, uint8_t decnum);
void UARTPutDec16(LPC_UART_TypeDef *UARTx, uint16_t decnum);
void UARTPutDec32(LPC_UART_TypeDef *UARTx, uint32_t decnum);
void UARTPutHex (LPC_UART_TypeDef *UARTx, uint8_t hexnum);
void UARTPutHex16 (LPC_UART_TypeDef *UARTx, uint16_t hexnum);
void UARTPutHex32 (LPC_UART_TypeDef *UARTx, uint32_t hexnum);
uint8_t UARTGetChar (LPC_UART_TypeDef *UARTx);
void debug_frmwrk_init(void);

#endif /* __DEBUG_FRMWRK_H_ */
