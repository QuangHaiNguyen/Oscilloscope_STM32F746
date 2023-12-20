/*
 * @brief Embedded Artists LPC4088 Display Module board file
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

#ifndef __BOARD_H_
#define __BOARD_H_

#include "chip.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup BOARD_EA_LPC4088DM LPC4088 Embedded Artists Display Module support software API functions
 * @ingroup LPCOPEN_40XX_BOARD_EA4088DM
 * The board support software API functions provide some simple abstracted
 * functions used across multiple LPCOpen board examples. See @ref BOARD_COMMON_API
 * for the functions defined by this board support layer.<br>
 * @{
 */

/** @defgroup BOARD_EA_LPC4088DM_OPTIONS BOARD: LPC4088 Embedded Artists Display Module build options
 * This board has options that configure its operation at build-time.<br>
 * @{
 */

/** Define DEBUG_ENABLE to enable IO via the DEBUGSTR, DEBUGOUT, and
    DEBUGIN macros. If not defined, DEBUG* functions will be optimized
    out of the code at build time.
 */
#define DEBUG_ENABLE

/** Define DEBUG_SEMIHOSTING along with DEBUG_ENABLE to enable IO support
    via semihosting. You may need to use a C library that supports
    semihosting with this option.
 */
// #define DEBUG_SEMIHOSTING

/** Board UART used for debug output and input using the DEBUG* macros. This
    is also the port used for Board_UARTPutChar, Board_UARTGetChar, and
    Board_UARTPutSTR functions.
 */
#define DEBUG_UART LPC_UART0

/**
 * @}
 */

/* Board name */
#define BOARD_EA_LPC4088DM

#define USE_RMII

#define FRAMEBUFFER_ADDR        EMC_ADDRESS_DYCS0	/*!< Frame buffer address for LCD */

/**
 * Button defines
 */
#define BUTTONS_BUTTON1     0x01
#define NO_BUTTON_PRESSED   0x00

/**
 * LEDs on the LPC4088 Display Module (DM)
 */
#define LED_DM_1          0
#define LED_DM_2          1
#define LED_DM_3          2
#define LED_DM_4          3


/**
 * @brief	Returns the MAC address assigned to this board
 * @param	mcaddr : Pointer to 6-byte character array to populate with MAC address
 * @return	Nothing
 * @note    Returns the MAC address used by Ethernet
 */
void Board_ENET_GetMacADDR(uint8_t *mcaddr);

/**
 * @brief	Initialize pin muxing for a UART
 * @param	pUART	: Pointer to UART register block for UART pins to init
 * @return	Nothing
 */
void Board_UART_Init(LPC_USART_T *pUART);

/**
 * @brief	Initialize pin muxing for SSP interface
 * @param	pSSP	: Pointer to SSP interface to initialize
 * @return	Nothing
 */
void Board_SSP_Init(LPC_SSP_T *pSSP);

/**
 * @brief	Sets up board specific I2C interface
 * @param	id	: I2C peripheral ID (I2C0, I2C1 or I2C2)
 * @return	Nothing
 */
void Board_I2C_Init(I2C_ID_T id);

/**
 * @brief	Sets up I2C Fast Plus mode
 * @param	id	: Must always be I2C0
 * @return	Nothing
 * @note	This function must be called before calling
 *          Chip_I2C_SetClockRate() to set clock rates above
 *          normal range 100KHz to 400KHz. Only I2C0 supports
 *          this mode. All I2C slaves of I2C0
 *          are connected to P0.27 (SDA0) P0.28 (SCL0) none of
 *          them will be accessible in fast plus mode because in
 *          fast plus mode P5.2 will be SDA0 and P5.3 will be SCL0,
 *          make sure the Fast Mode Plus supporting slave devices
 *          are connected to these pins.
 */
STATIC INLINE void Board_I2C_EnableFastPlus(I2C_ID_T id)
{
	/* Free P0[27] & P0[28] as SDA0 and SCL0 respectively*/
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 27, IOCON_FUNC0);
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 28, IOCON_FUNC0);

	/* Use P5[2] & P5[3] as SDA0 and SCL0 respectively */
	Chip_IOCON_PinMuxSet(LPC_IOCON, 5, 2, IOCON_FUNC5 | IOCON_HIDRIVE_EN);
	Chip_IOCON_PinMuxSet(LPC_IOCON, 5, 3, IOCON_FUNC5 | IOCON_HIDRIVE_EN);
}

/**
 * @brief	Disable I2C Fast Plus mode and enables default mode
 * @param	id	: Must always be I2C0
 * @return	Nothing
 * @sa		Board_I2C_EnableFastPlus()
 */
STATIC INLINE void Board_I2C_DisableFastPlus(I2C_ID_T id)
{
	/* Free P5[2] & P5[3] as SDA0 and SCL0 respectively */
	Chip_IOCON_PinMuxSet(LPC_IOCON, 5, 2, IOCON_FUNC0);
	Chip_IOCON_PinMuxSet(LPC_IOCON, 5, 3, IOCON_FUNC0);

	/* Use P0[27] & P0[28] as SDA0 and SCL0 respectively*/
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 27, IOCON_FUNC1);
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 28, IOCON_FUNC1);
}

/**
 * @brief	Sets up board specific CAN interface
 * @param	pCAN	: Pointer to CAN interface to initialize
 * @return	Nothing
 */
void Board_CAN_Init(LPC_CAN_T *pCAN);

/**
 * @brief	Initialize buttons on the board
 * @return	Nothing
 */
void Board_Buttons_Init(void);

/**
 * @brief	Get button status
 * @return	status of button
 */
uint32_t Buttons_GetStatus(void);

/**
 * @brief	Sets up board specific Event Monitor/Recorder interface
 * @return	Nothing
 */
void Board_RTC_EV_Init(void);

/**
 * @brief	Sets up board specific SDC peripheral
 * @return	Nothing
 */
void Board_SDC_Init(void);

/**
 * @brief	Sets up board for CMP peripheral
 * @return	Nothing
 */
void Board_CMP_Init(void);

/**
 * @brief	Initializes USB device mode pins per board design
 * @param	port	: USB port to be enabled 
 * @return	Nothing
 * @note	Only one of the USB port can be enabled at a given time.
 */
void Board_USBD_Init(uint32_t port);

/**
 * @}
 */

#include "board_api.h"

#ifdef __cplusplus
}
#endif

#endif /* __BOARD_H_ */
