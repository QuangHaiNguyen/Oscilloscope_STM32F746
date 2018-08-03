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

#include "board.h"
#include "string.h"
#include <stdarg.h>

#include "retarget.h"

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

#if defined(CHIP_LPC177X_8X)
  #define LCD_SSP_CTRL                       LPC_SSP0
#elif defined(CHIP_LPC40XX)
  #define LCD_SSP_CTRL                       LPC_SSP2
#endif


#define BUTTONS_BUTTON1_GPIO_PORT_NUM           2
#define BUTTONS_BUTTON1_GPIO_BIT_NUM            10


#define NUM_LEDS  (sizeof(ledports)/sizeof(ledports[0]))

static const uint8_t ledports[] = {1, 0, 1, 2};
static const uint8_t ledpins[] = {18, 13, 13, 19};
static const uint8_t ledinverted[] = {true, true, false, false};

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/* System oscillator rate and RTC oscillator rate */
const uint32_t OscRateIn = 12000000;
const uint32_t RTCOscRateIn = 32768;

/*****************************************************************************
 * Private functions
 ****************************************************************************/

///* Very simple (inaccurate) delay function */
//static void delay(uint32_t i)
//{
//	while (i--) {}
//}

///* Delay in miliseconds  (cclk = 120MHz) */
//static void delayMs(uint32_t ms)
//{
//	delay(ms * 40000);
//}

/* Initializes board LED(s) */
static void Board_LED_Init(void)
{
	int i;

	/* Setup port direction and initial output state */
	for (i = 0; i < NUM_LEDS; i++) {
		Chip_GPIO_WriteDirBit(LPC_GPIO, ledports[i], ledpins[i], true);
		Board_LED_Set(i, false);
	}
}

/*****************************************************************************
 * Public functions
 ****************************************************************************/

/* Initialize UART pins */
void Board_UART_Init(LPC_USART_T *pUART)
{
	if (pUART == LPC_UART0) {
		/*
		 * Initialize UART0 pin connect
		 * P0.2: TXD
		 * P0.3: RXD
		 */
		Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 2, (IOCON_FUNC1 | IOCON_MODE_INACT));
		Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 3, (IOCON_FUNC1 | IOCON_MODE_INACT));
	}
	else if (pUART == LPC_UART2) {
		/* Initialize UART2 pin connect */
#if defined(CHIP_LPC40XX)
		Chip_IOCON_PinMuxSet(LPC_IOCON, 4, 22, (IOCON_FUNC2 | IOCON_MODE_INACT));
		Chip_IOCON_PinMuxSet(LPC_IOCON, 4, 23, (IOCON_FUNC2 | IOCON_MODE_INACT));
#else
		Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 10, (IOCON_FUNC1 | IOCON_MODE_INACT));
		Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 11, (IOCON_FUNC1 | IOCON_MODE_INACT));
#endif
	}
}

/* Initialize debug output via UART for board */
void Board_Debug_Init(void)
{
#if defined(DEBUG_UART)
	Board_UART_Init(DEBUG_UART);

	Chip_UART_Init(DEBUG_UART);
	Chip_UART_SetBaud(DEBUG_UART, 115200);
	Chip_UART_ConfigData(DEBUG_UART, UART_LCR_WLEN8 | UART_LCR_SBS_1BIT | UART_LCR_PARITY_DIS);

	/* Enable UART Transmit */
	Chip_UART_TXEnable(DEBUG_UART);
#endif
}

/* Sends a character on the UART */
void Board_UARTPutChar(char ch)
{
#if defined(DEBUG_UART)
	while ((Chip_UART_ReadLineStatus(DEBUG_UART) & UART_LSR_THRE) == 0) {}
	Chip_UART_SendByte(DEBUG_UART, (uint8_t) ch);
#endif
}

/* Gets a character from the UART, returns EOF if no character is ready */
int Board_UARTGetChar(void)
{
#if defined(DEBUG_UART)
	if (Chip_UART_ReadLineStatus(DEBUG_UART) & UART_LSR_RDR) {
		return (int) Chip_UART_ReadByte(DEBUG_UART);
	}
#endif
	return EOF;
}

/* Outputs a string on the debug UART */
void Board_UARTPutSTR(char *str)
{
#if defined(DEBUG_UART)
	while (*str != '\0') {
		Board_UARTPutChar(*str++);
	}
#endif
}

/* Outputs a formatted string on the debug UART */
void Board_UARTPrintf(const char *format, ...)
{
#if defined(DEBUG_UART)
    char  buffer[128];
    va_list     vArgs;
    va_start(vArgs, format);
    vsnprintf((char *)buffer, 128, (char const *)format, vArgs);
    va_end(vArgs);

    Board_UARTPutSTR(buffer);
#endif
}

/* Sets the state of a board LED to on or off */
void Board_LED_Set(uint8_t LEDNumber, bool On)
{
	if (LEDNumber < NUM_LEDS) {
		if (ledinverted[LEDNumber]) {
            Chip_GPIO_SetPinState(LPC_GPIO, ledports[LEDNumber], ledpins[LEDNumber], !On);
		} else {
            Chip_GPIO_SetPinState(LPC_GPIO, ledports[LEDNumber], ledpins[LEDNumber], On);
		}
	}
}

/* Returns the current state of a board LED */
bool Board_LED_Test(uint8_t LEDNumber)
{
	bool state = false;

	if (LEDNumber < NUM_LEDS) {
		state = Chip_GPIO_GetPinState(LPC_GPIO, ledports[LEDNumber], ledpins[LEDNumber]);
		if (ledinverted[LEDNumber]) {
			state = !state;
		}
	}

	return state;
}

/* Toggles the current state of a board LED */
void Board_LED_Toggle(uint8_t LEDNumber)
{
	if (LEDNumber < NUM_LEDS) {
		Board_LED_Set(LEDNumber, !Board_LED_Test(LEDNumber));
	}
}

/* Returns the MAC address assigned to this board */
void Board_ENET_GetMacADDR(uint8_t *mcaddr)
{
	const uint8_t boardmac[] = {0x00, 0x60, 0x37, 0x12, 0x34, 0x56};

	memcpy(mcaddr, boardmac, 6);
}

/* Initialize pin muxing for SSP interface */
void Board_SSP_Init(LPC_SSP_T *pSSP)
{
	if (pSSP == LPC_SSP1) {
		/* Set up clock and muxing for SSP1 interface */

		/*
		 * Initialize SSP1 pins connect
		 * P0.7: SCK
		 * P0.6: SSEL
		 * P0.8: MISO
		 * P0.9: MOSI
		 */
		Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 7, (IOCON_FUNC2 | IOCON_MODE_INACT | IOCON_DIGMODE_EN));
		Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 6, (IOCON_FUNC2 | IOCON_MODE_INACT | IOCON_DIGMODE_EN));
		Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 8, (IOCON_FUNC2 | IOCON_MODE_INACT | IOCON_DIGMODE_EN));
		Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 9, (IOCON_FUNC2 | IOCON_MODE_INACT | IOCON_DIGMODE_EN));
	}
	else if (pSSP == LPC_SSP2) {
//#if defined(CHIP_LPC40XX)
//		/*
//		 * Initialize SSP0 pins connect
//		 * P5.2: SCK
//		 * P5.3: SSEL
//		 * P5.1: MISO
//		 * P5.0: MOSI
//		 */
//		Chip_IOCON_PinMuxSet(LPC_IOCON, 5, 2, (IOCON_FUNC2 | IOCON_MODE_INACT));
//		Chip_IOCON_PinMuxSet(LPC_IOCON, 5, 3, (IOCON_FUNC2 | IOCON_MODE_INACT));
//		Chip_IOCON_PinMuxSet(LPC_IOCON, 5, 1, (IOCON_FUNC2 | IOCON_MODE_INACT));
//		Chip_IOCON_PinMuxSet(LPC_IOCON, 5, 0, (IOCON_FUNC2 | IOCON_MODE_INACT));
//#endif

	}
	else {
		/* Set up clock and muxing for SSP0 interface */

		/*
		 * Initialize SSP0 pins connect
		 * P1.20: SCK
		 * P0.21: SSEL - uSD/IF  GPIO
		 * P1.2 : SSEL - LCD/IF  GPIO
		 * P1.23: MISO
		 * P1.24: MOSI
		 */
		Chip_IOCON_PinMuxSet(LPC_IOCON, 1, 20, (IOCON_FUNC5 | IOCON_MODE_INACT | IOCON_DIGMODE_EN));
		Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 21, (IOCON_FUNC0 | IOCON_MODE_INACT | IOCON_DIGMODE_EN));
		Chip_IOCON_PinMuxSet(LPC_IOCON, 1,  2, (IOCON_FUNC0 | IOCON_MODE_INACT | IOCON_DIGMODE_EN));
		Chip_IOCON_PinMuxSet(LPC_IOCON, 1, 23, (IOCON_FUNC5 | IOCON_MODE_INACT | IOCON_DIGMODE_EN));
		Chip_IOCON_PinMuxSet(LPC_IOCON, 1, 24, (IOCON_FUNC5 | IOCON_MODE_INACT | IOCON_DIGMODE_EN));
	}
}

/* Set up and initialize all required blocks and functions related to the
   board hardware */
void Board_Init(void)
{
	/* Sets up DEBUG UART */
	DEBUGINIT();

	/* Initializes GPIO */
	Chip_GPIO_Init(LPC_GPIO);
	Chip_IOCON_Init(LPC_IOCON);

	/* Initialize LEDs */
	Board_LED_Init();
}

/* Sets up board specific I2C interface */
void Board_I2C_Init(I2C_ID_T id)
{
	switch (id) {
	case I2C0:
		Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 27, IOCON_FUNC1);
		Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 28, IOCON_FUNC1);
		break;

	case I2C1:
		Chip_IOCON_PinMuxSet(LPC_IOCON, 2, 14, (IOCON_FUNC2 | IOCON_MODE_PULLUP | IOCON_OPENDRAIN_EN));
		Chip_IOCON_PinMuxSet(LPC_IOCON, 2, 15, (IOCON_FUNC2 | IOCON_MODE_PULLUP | IOCON_OPENDRAIN_EN));
		break;

	case I2C2:
		Chip_IOCON_PinMuxSet(LPC_IOCON, 2, 30, (IOCON_FUNC2 | IOCON_MODE_PULLUP | IOCON_OPENDRAIN_EN));
		Chip_IOCON_PinMuxSet(LPC_IOCON, 2, 31, (IOCON_FUNC2 | IOCON_MODE_PULLUP | IOCON_OPENDRAIN_EN));
		break;

	default:
		return;
	}
}

void Board_Buttons_Init(void)
{
	Chip_GPIO_WriteDirBit(LPC_GPIO, BUTTONS_BUTTON1_GPIO_PORT_NUM, BUTTONS_BUTTON1_GPIO_BIT_NUM, false);
}

uint32_t Buttons_GetStatus(void)
{
	uint8_t ret = NO_BUTTON_PRESSED;
	if (Chip_GPIO_ReadPortBit(LPC_GPIO, BUTTONS_BUTTON1_GPIO_PORT_NUM, BUTTONS_BUTTON1_GPIO_BIT_NUM) == 0x00) {
		ret |= BUTTONS_BUTTON1;
	}
	return ret;
}

/* Sets up board specific CAN interface */
void Board_CAN_Init(LPC_CAN_T *pCAN)
{
	if (pCAN == LPC_CAN1) {
		Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 0, (IOCON_FUNC1 | IOCON_MODE_INACT));
		Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 1, (IOCON_FUNC1 | IOCON_MODE_INACT));
	}
	else {
		Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 4, (IOCON_FUNC2 | IOCON_MODE_INACT));
		Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 5, (IOCON_FUNC2 | IOCON_MODE_INACT));
	}
}

/* Pin mux for Event Monitor/Recorder */
void Board_RTC_EV_Init(void)
{
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 7, (IOCON_FUNC4 | IOCON_MODE_INACT)); // RTC_EV0
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 8, (IOCON_FUNC4 | IOCON_MODE_INACT)); // RTC_EV1
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 9, (IOCON_FUNC4 | IOCON_MODE_INACT)); // RTC_EV2
}

/* Setup board for SDC interface */
void Board_SDC_Init(void)
{
	Chip_IOCON_PinMuxSet(LPC_IOCON, 1, 2, (IOCON_FUNC2 | IOCON_MODE_INACT));
	Chip_IOCON_PinMuxSet(LPC_IOCON, 1, 3, (IOCON_FUNC2 | IOCON_MODE_INACT));
#ifdef CHIP_LPC40XX
	Chip_IOCON_PinMuxSet(LPC_IOCON, 1, 5, (IOCON_FUNC2 | IOCON_MODE_INACT | IOCON_DIGMODE_EN));
	Chip_IOCON_PinMuxSet(LPC_IOCON, 1, 6, (IOCON_FUNC2 | IOCON_MODE_INACT | IOCON_DIGMODE_EN));
	Chip_IOCON_PinMuxSet(LPC_IOCON, 1, 7, (IOCON_FUNC2 | IOCON_MODE_INACT | IOCON_DIGMODE_EN));
#else
	Chip_IOCON_PinMuxSet(LPC_IOCON, 1, 5, (IOCON_FUNC2 | IOCON_MODE_INACT));
	Chip_IOCON_PinMuxSet(LPC_IOCON, 1, 6, (IOCON_FUNC2 | IOCON_MODE_INACT));
	Chip_IOCON_PinMuxSet(LPC_IOCON, 1, 7, (IOCON_FUNC2 | IOCON_MODE_INACT));
#endif
	Chip_IOCON_PinMuxSet(LPC_IOCON, 1, 11, (IOCON_FUNC2 | IOCON_MODE_INACT));
	Chip_IOCON_PinMuxSet(LPC_IOCON, 1, 12, (IOCON_FUNC2 | IOCON_MODE_INACT));
}

#ifdef CHIP_LPC40XX
/* Setup board for CMP interface */
void Board_CMP_Init(void)
{
	/* CMP1_IN0 @ P1.7  */
	Chip_IOCON_PinMuxSet(LPC_IOCON, 1, 7, (IOCON_FUNC5 | IOCON_MODE_INACT | IOCON_ADMODE_EN));
}

/* Setup board for SPIFI interface */
void Board_SPIFI_Init(void)
{
	/* SPIFI_IO[2] */
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 15, (IOCON_FUNC5 | IOCON_MODE_INACT));
	/* SPIFI_IO[3] */
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 16, (IOCON_FUNC5 | IOCON_MODE_INACT));
	/* SPIFI_IO[1] */
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 17, (IOCON_FUNC5 | IOCON_MODE_INACT));
	/* SPIFI_IO[0] */
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 18, (IOCON_FUNC5 | IOCON_MODE_INACT));
	/* SPIFI_CLK */
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 22, (IOCON_FUNC5 | IOCON_MODE_INACT));
	/* SPIFI_CS */
	Chip_IOCON_PinMuxSet(LPC_IOCON, 2, 7, (IOCON_FUNC5 | IOCON_MODE_INACT));
}

#endif

void Board_USBD_Init(uint32_t port)
{
	/* On the EA LPC40xx board leave VBUS at default setting. It's not connected on the board. */
	//Chip_IOCON_PinMux(LPC_IOCON, 1, 30, IOCON_MODE_INACT, IOCON_FUNC2); /* USB VBUS */
	
	if (port == 1) {
		Chip_IOCON_PinMux(LPC_IOCON, 0, 29, IOCON_MODE_INACT, IOCON_FUNC1);	/* P0.29 D1+, P0.30 D1- */
		Chip_IOCON_PinMux(LPC_IOCON, 0, 30, IOCON_MODE_INACT, IOCON_FUNC1);
		LPC_USB->USBClkCtrl = 0x12;                /* Dev, AHB clock enable */
		while ((LPC_USB->USBClkSt & 0x12) != 0x12); 
	} else {
		Chip_IOCON_PinMux(LPC_IOCON, 0, 31, IOCON_MODE_INACT, IOCON_FUNC1);	/* P0.31 D2+, D2- */
		Chip_IOCON_PinMux(LPC_IOCON, 0, 14, IOCON_MODE_INACT, IOCON_FUNC3); /* USB_CONNECT2 */
		//Chip_IOCON_PinMux(LPC_IOCON, 0, 13, IOCON_MODE_INACT, IOCON_FUNC1); /* USB_UP_LED2 */
		
		LPC_USB->USBClkCtrl = 0x1A;                /* Dev, AHB clock enable */
		while ((LPC_USB->USBClkSt & 0x1A) != 0x1A); 
		/* Port Select register when USB device is configured. */
		LPC_USB->StCtrl = 0x3; 
	}
}
