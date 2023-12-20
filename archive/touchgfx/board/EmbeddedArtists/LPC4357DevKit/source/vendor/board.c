/*****************************************************************************
 *
 *   Copyright(C) 2011, Embedded Artists AB
 *   All rights reserved.
 *
 ******************************************************************************
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * products. This software is supplied "AS IS" without any warranties.
 * Embedded Artists AB assumes no responsibility or liability for the
 * use of the software, conveys no license or title under any patent,
 * copyright, or mask work right to the product. Embedded Artists AB
 * reserves the right to make changes in the software without
 * notification. Embedded Artists AB also make no representation or
 * warranty that such application will be suitable for the specified
 * use without further testing or modification.
 *****************************************************************************/


/******************************************************************************
 * Includes
 *****************************************************************************/

#include <string.h>
#include "lpc43xx.h"
#include "lpc43xx_adc.h"
#include "lpc43xx_cgu.h"
#include "lpc43xx_gpio.h"
#include "lpc43xx_scu.h"
#include "lpc43xx_uart.h"
#include "board.h"




/******************************************************************************
 * Defines and typedefs
 *****************************************************************************/

#define CONSOLE_DEV (LPC_USART0)
#define XTAL_FREQ   (12000000)

/******************************************************************************
 * External global variables
 *****************************************************************************/

/******************************************************************************
 * Local variables
 *****************************************************************************/

/* Structure for initial base clock states */
struct CLK_BASE_STATES {
        CGU_ENTITY_T clk;       /* Base clock */
        CGU_ENTITY_T clkin;     /* Base clock source, see UM for allowable sources per base clock */
//      Bool autoblock_enab;    /* Set to true to enable autoblocking on frequency change */
//      Bool powerdn;                   /* Set to true if the base clock is initially powered down */
};


#define CLKINPUT_PD  CGU_CLKSRC_32KHZ_OSC /* LPCOpen has an extra clock here with an invalid value */

static const struct CLK_BASE_STATES InitClkStates[] = {
        {CGU_BASE_SAFE, CGU_CLKSRC_IRC/*, true, false*/},
        {CGU_BASE_APB1, CGU_CLKSRC_PLL1/*, true, false*/},
        {CGU_BASE_APB3, CGU_CLKSRC_PLL1/*, true, false*/},
        {CGU_BASE_LCD, CGU_CLKSRC_PLL1/*, true, false*/},
        {CGU_BASE_USB0, CGU_CLKSRC_PLL0/*, true, true*/},
        {CGU_BASE_PERIPH, CGU_CLKSRC_PLL1/*, true, false*/},
        {CGU_BASE_SPI, CGU_CLKSRC_PLL1/*, true, false*/},
        {CGU_BASE_ADCHS, CGU_CLKSRC_PLL1/*, true, true*/},
        {CGU_BASE_SDIO, CGU_CLKSRC_PLL1/*, true, false*/},
        {CGU_BASE_SSP0, CGU_CLKSRC_PLL1/*, true, false*/},
        {CGU_BASE_SSP1, CGU_CLKSRC_PLL1/*, true, false*/},
        {CGU_BASE_UART0, CGU_CLKSRC_PLL1/*, true, false*/},
        {CGU_BASE_UART1, CGU_CLKSRC_PLL1/*, true, false*/},
        {CGU_BASE_UART2, CGU_CLKSRC_PLL1/*, true, false*/},
        {CGU_BASE_UART3, CGU_CLKSRC_PLL1/*, true, false*/},
        {CGU_BASE_CLKOUT, CLKINPUT_PD/*, true, false*/},
        {CGU_BASE_APLL, CLKINPUT_PD/*, true, false*/},
        {CGU_BASE_OUT0, CLKINPUT_PD/*, true, false*/},
        {CGU_BASE_OUT1, CLKINPUT_PD/*, true, false*/},
};

/******************************************************************************
 * Local Functions
 *****************************************************************************/

static void emc_WaitUS(volatile uint32_t us)
{
  us *= (SystemCoreClock / 1000000) / 3;
  while(us--);
}

/******************************************************************************
 * Public Functions
 *****************************************************************************/

/******************************************************************************
 *
 * Description:
 *   Sets the operating frequency. If the wanted frequency is above 90 then
 *   it has to be increased in two steps according to the Users Manual.
 *   The first step takes it up to the 90-110MHz range and from there it
 *   can be set up to the maximum 204MHz.
 *
 *****************************************************************************/
void improved_cgu_init(wanted_freq_t freq)
{
  int i;

  if (freq == WANTED_FREQ_72_MHZ)
  {
    // 72MHz is the default
    CGU_Init();
    return;
  }
  
  /* after boot CPU runs at 96 MHz */
  /* cpu runs from: IRC (12MHz) >> PLL M = 24, FCCO @ 288 MHz direct mode >> IDIVC = 4 >> 96 MHz */

  /* enable the crystal oscillator */
  CGU_SetXTALOSC(XTAL_FREQ);
  CGU_EnableEntity(CGU_CLKSRC_XTAL_OSC, ENABLE);
  emc_WaitUS(250);

  /* connect the cpu to the xtal */
  CGU_EntityConnect(CGU_CLKSRC_XTAL_OSC, CGU_BASE_M4);

  /* connect the PLL to the xtal */
  CGU_EntityConnect(CGU_CLKSRC_XTAL_OSC, CGU_CLKSRC_PLL1);

  /* configure the PLL to 108 MHz */
  CGU_SetPLL1(9);
  emc_WaitUS(50);

  /* enable the PLL */
  CGU_EnableEntity(CGU_CLKSRC_PLL1, ENABLE);

  /* connect to the CPU core */
  CGU_EntityConnect(CGU_CLKSRC_PLL1, CGU_BASE_M4);

  SystemCoreClock = 108000000;

  /* wait one msec */
  emc_WaitUS(1000);

  /* Change the clock to the higher frequency (180 or 204 MHz) without disconnecting the CPU */
  /* uses direct mode */
  CGU_SetPLL1(freq);
  emc_WaitUS(50);

  SystemCoreClock = freq*XTAL_FREQ;

  /* configure the EMC to run at CLK/2 */
  LPC_CCU1->CLK_M4_EMCDIV_CFG |=    (1<<0) |  (1<<5);// Turn on clock / 2
  LPC_CREG->CREG6 |= (1<<16);                        // EMC divided by 2
  LPC_CCU1->CLK_M4_EMC_CFG |= (1<<0);                // Turn on clock

  /* wait one msec */
  emc_WaitUS(1000);

  CGU_UpdateClock();

  /* Setup system base clocks and initial states. This won't enable and
     disable individual clocks, but sets up the base clock sources for
     each individual peripheral clock. */
  for (i = 0; i < (sizeof(InitClkStates) / sizeof(InitClkStates[0])); i++) {
        CGU_EntityConnect(InitClkStates[i].clkin, InitClkStates[i].clk);
  }
}

/******************************************************************************
 *
 * Description:
 *   Initialize the console (UART connected to USB-to-UART bridge)
 *
 *****************************************************************************/
void console_init(void)
{
  // UART Configuration structure variable
        UART_CFG_Type UARTConfigStruct;

  // pinsel for UART
        scu_pinmux(0xF ,10 , MD_PDN, FUNC1);    // PF.10 : UART0_TXD
        scu_pinmux(0xF ,11 , MD_PLN|MD_EZI|MD_ZI, FUNC1);       // PF.11 : UART0_RXD

        UART_ConfigStructInit(&UARTConfigStruct);
  UARTConfigStruct.Baud_rate = 115200;
  UARTConfigStruct.Clock_Speed = 0;

        // Initialize UART0 peripheral with given to corresponding parameter
        UART_Init(CONSOLE_DEV, &UARTConfigStruct);

        // Enable UART Transmit
        UART_TxCmd(CONSOLE_DEV, ENABLE);
}

/******************************************************************************
 *
 * Description:
 *   Send data to the console.
 *
 * Params:
 *   [in] txbuf - buffer containing data to send
 *   [in] buflen - number of bytes to send
 *   [in] flag - indicates blocking or non-blocking transfer
 *
 * Returns:
 *   Number of bytes sent.
 *
 *****************************************************************************/
uint32_t console_send(uint8_t *txbuf, uint32_t buflen,
                TRANSFER_BLOCK_Type flag)
{
        return UART_Send(CONSOLE_DEV, txbuf, buflen, flag);
}

/******************************************************************************
 *
 * Description:
 *   Send a null-terminates string to the console.
 *
 * Params:
 *   [in] str - the string to send
 *
 * Returns:
 *   Number of bytes sent.
 *
 *****************************************************************************/
uint32_t console_sendString(uint8_t *str)
{
        return UART_Send(CONSOLE_DEV, str, strlen((char*)str), BLOCKING);
}

/******************************************************************************
 *
 * Description:
 *   Receive a block of data from the console.
 *
 * Params:
 *   [in] rxbuf - pointer to receive buffer
 *   [in] buflen -
 *
 * Returns:
 *   Number of bytes sent.
 *
 *****************************************************************************/
uint32_t console_receive(uint8_t *rxbuf, uint32_t buflen,
                TRANSFER_BLOCK_Type flag)
{
        return UART_Receive(CONSOLE_DEV, rxbuf, buflen, flag);
}

/******************************************************************************
 *
 * Description:
 *   Initialize the trim potentiometer, i.e. ADC connected to TrimPot on
 *   Base Board.
 *
 *****************************************************************************/
void trimpot_init(void)
{
  // pinsel for AD0.3 on p7.5
        scu_pinmux(     7       ,       5       ,       PDN_DISABLE | PUP_DISABLE       | INBUF_DISABLE,        0       );
  LPC_SCU->ENAIO0 |= (1<<3);
  
  ADC_Init(LPC_ADC0, 400000, 10);

        ADC_IntConfig(LPC_ADC0, ADC_ADINTEN2, DISABLE);
        ADC_ChannelCmd(LPC_ADC0, ADC_CH_TRIMPOT, ENABLE);
}

