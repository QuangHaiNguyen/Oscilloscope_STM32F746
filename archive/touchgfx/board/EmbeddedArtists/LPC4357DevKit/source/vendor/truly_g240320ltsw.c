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


#include "lpc_types.h"
#include "lpc43xx_scu.h"
#include "lpc43xx_ssp.h"
#include "lpc43xx_timer.h"

#include "lcd_driver.h"
#include "lcd_params.h"
#include "truly_display.h"

/******************************************************************************
 * Defines and typedefs
 *****************************************************************************/

#define CS_ON    (LPC_GPIO_PORT->CLR[7] |= (1UL << 16)) // pF.1 (gpio7[16]) -> low
#define CS_OFF   (LPC_GPIO_PORT->SET[7] |= (1UL << 16)) // pF.1 (gpio7[16]) -> high
#define DC_CMD    (LPC_GPIO_PORT->CLR[2] |= (1UL << 0)) // p4.0 (gpio2[0]) -> low
#define DC_DATA   (LPC_GPIO_PORT->SET[2] |= (1UL << 0)) // p4.0 (gpio2[0]) -> high

#define SSP_PORT  (LPC_SSP0)
#define SSP_CLOCK 3000000

/******************************************************************************
 * External global variables
 *****************************************************************************/

/******************************************************************************
 * Local variables
 *****************************************************************************/

static const LCD_PARAM_T truly_g240320ltsw =
{
   28,       /* Horizontal back porch */
   10,       /* Horizontal front porch */
    2,       /* HSYNC pulse width */
  240,       /* Pixels per line */
    2,       /* Vertical back porch */
    1,       /* Vertical front porch */
    2,       /* VSYNC pulse width */
  320,       /* Lines per panel */
    0,        /* Invert output enable */
    1,        /* Do not invert panel clock */
    1,        /* Invert HSYNC */
    1,        /* Invert VSYNC */
    1,        /* AC bias frequency (not used) */
   16,        /* Bits per pixel */
  8200000,    /* Optimal clock rate (Hz) between 1-8.22 MHz according to SSD1289 datasheet */
  TFT,        /* LCD panel type */
    0,        /* Single panel display */
};



/******************************************************************************
 * Local Functions
 *****************************************************************************/

static void pinConfig(void)
{
  scu_pinmux(0x4,  2, LCD_PINCONFIG, FUNC2);  /* LCD_VD_3 @ P4.2 - RED0 */
  scu_pinmux(0x8,  7, LCD_PINCONFIG, FUNC3);  /* LCD_VD_4 @ P8.7 - RED1 */
  scu_pinmux(0x8,  6, LCD_PINCONFIG, FUNC3);  /* LCD_VD_5 @ P8.6 - RED2 */
  scu_pinmux(0x8,  5, LCD_PINCONFIG, FUNC3);  /* LCD_VD_6 @ P8.5 - RED3 */
  scu_pinmux(0x8,  4, LCD_PINCONFIG, FUNC3);  /* LCD_VD_7 @ P8.4 - RED4 */
  scu_pinmux(0x4, 10, LCD_PINCONFIG, FUNC2);  /* LCD_VD_10 @ P4.10 - GREEN0 */
  scu_pinmux(0x4,  9, LCD_PINCONFIG, FUNC2);  /* LCD_VD_11 @ P4.9 - GREEN1 */
  scu_pinmux(0x8,  3, LCD_PINCONFIG, FUNC3);  /* LCD_VD_12 @ P8.3 - GREEN2 */
  scu_pinmux(0xB,  6, LCD_PINCONFIG, FUNC2);  /* LCD_VD_13 @ PB.6 - GREEN3 */
  scu_pinmux(0xB,  5, LCD_PINCONFIG, FUNC2);  /* LCD_VD_14 @ PB.5 - GREEN4 */
  scu_pinmux(0xB,  4, LCD_PINCONFIG, FUNC2);  /* LCD_VD_15 @ PB.4 - GREEN5 */
  scu_pinmux(0x7,  1, LCD_PINCONFIG, FUNC3);  /* LCD_VD_19 @ P7.1 - BLUE0 */
  scu_pinmux(0xB,  3, LCD_PINCONFIG, FUNC2);  /* LCD_VD_20 @ PB.3 - BLUE1 */
  scu_pinmux(0xB,  2, LCD_PINCONFIG, FUNC2);  /* LCD_VD_21 @ PB.2 - BLUE2 */
  scu_pinmux(0xB,  1, LCD_PINCONFIG, FUNC2);  /* LCD_VD_22 @ PB.1 - BLUE3 */
  scu_pinmux(0xB,  0, LCD_PINCONFIG, FUNC2);  /* LCD_VD_23 @ PB.0 - BLUE4 */

  scu_pinmux(0x4,  5, LCD_PINCONFIG, FUNC2);  /* LCD_FP @ P4.5 */
  scu_pinmux(0x4,  6, LCD_PINCONFIG, FUNC2);  /* LCD_ENAB @ P4.6 */
  scu_pinmux(0x4,  7, LCD_PINCONFIG, FUNC0);  /* LCD_DCLK @ P4.7 */
  scu_pinmux(0x7,  0, LCD_PINCONFIG, FUNC3);  /* LCD_LE @ P7.0 */
  scu_pinmux(0x7,  6, LCD_PINCONFIG, FUNC3);  /* LCD_LP @ P7.6 */
  scu_pinmux(0x7,  7, LCD_PINCONFIG, FUNC3);  /* LCD_PWR @ P7.7 */
  
  scu_pinmux(0xF,  4, LCD_PINCONFIG, FUNC1);  /* LCD_CLKIN @ PF.4 */

  /* (backlight) pin as output */
  scu_pinmux(0x9,  6, MD_PDN, FUNC0);  /* GPIO4[11] @ P9.6 */
  LPC_GPIO_PORT->DIR[4] |= (1UL << 11);
  LPC_GPIO_PORT->CLR[4] |= (1UL << 11);

  /* output for CS */
  scu_pinmux(0xe, 15, MD_PDN, FUNC4);  /* GPIO7[15] @ PE.15 */
  LPC_GPIO_PORT->DIR[7] |= (1UL << 15);

  /* output for DC */
  scu_pinmux(0x4, 0, MD_PDN, FUNC0);  /* GPIO2[0] @ P4.0 */
  LPC_GPIO_PORT->DIR[2] |= (1UL << 0);

  // PIN config SSP 0
  scu_pinmux(0xf, 0, MD_PLN_FAST, FUNC0); //SSP0_SCK
  scu_pinmux(0xf, 2, MD_PLN_FAST, FUNC2); //SSP0_MISO
  scu_pinmux(0xf, 3, MD_PLN_FAST, FUNC2); //SSP0_MOSI
  
  // CS is controlled via PE.15 and not SSP0_SSEL.
  // Pin PF.1 should be configured as GPIO and set high.
  scu_pinmux(0xf, 1, MD_PLN_FAST, FUNC4); //GPIO7[16]
  LPC_GPIO_PORT->DIR[7] |= (1UL << 16);
  LPC_GPIO_PORT->SET[7] |= (1UL << 16);

  truly_lcd_setBacklight(0);
}

static void
writeToReg(uint16_t addr, uint16_t data)
{
  uint8_t buf[2];
  SSP_DATA_SETUP_Type sspCfg;
  DC_CMD;
  CS_ON;

  buf[0] = 0;
  buf[1] = (addr & 0xff);

  sspCfg.tx_data = buf;
  sspCfg.rx_data = NULL;
  sspCfg.length  = 2; 

  SSP_ReadWrite (SSP_PORT, &sspCfg, SSP_TRANSFER_POLLING);

  CS_OFF;  

  DC_DATA;
  CS_ON;
  buf[0] = (data >> 8);
  buf[1] = (data & 0xff);
  SSP_ReadWrite (SSP_PORT, &sspCfg, SSP_TRANSFER_POLLING);
  CS_OFF; 

  DC_CMD;
  CS_ON;

  buf[0] = (0);
  buf[1] = (0x22);
  SSP_ReadWrite (SSP_PORT, &sspCfg, SSP_TRANSFER_POLLING);
  CS_OFF; 
}

static void ssd1289_init(void)
{
  writeToReg (0x00,0x0001);
  TIM_Waitms(15);
  writeToReg (0x03,0xAEAC);
  writeToReg (0x0C,0x0007);
  writeToReg (0x0D,0x000F);
  writeToReg (0x0E,0x2900);
  writeToReg (0x1E,0x00B3);
  TIM_Waitms(15);
  writeToReg (0x01,0x2B3F);
  writeToReg (0x02,0x0600);
  writeToReg (0x10,0x0000);
  writeToReg (0x11,0x65b0);//60B0  rgb i/r
  TIM_Waitms(20);
  writeToReg (0x05,0x0000);
  writeToReg (0x06,0x0000);
  writeToReg (0x16,0xEF1C);
  writeToReg (0x17,0x0003);
  writeToReg (0x07,0x0233);
  writeToReg (0x0B,0x5312);
  writeToReg (0x0F,0x0000);
  writeToReg (0x1E,0x00AE);
  TIM_Waitms(20);
  writeToReg (0x41,0x0000);
  writeToReg (0x42,0x0000);
  writeToReg (0x48,0x0000);
  writeToReg (0x49,0x013F);
  writeToReg (0x44,0xEF00);
  writeToReg (0x45,0x0000);
  writeToReg (0x46,0x013F);
  writeToReg (0x4A,0x0000);
  writeToReg (0x4B,0x0000);
  TIM_Waitms(20);
  writeToReg (0x30,0x0707);
  writeToReg (0x31,0x0704);
  writeToReg (0x32,0x0204);
  writeToReg (0x33,0x0201);
  writeToReg (0x34,0x0203);
  writeToReg (0x35,0x0204);
  writeToReg (0x36,0x0204);
  writeToReg (0x37,0x0502);
  writeToReg (0x3A,0x0302);
  writeToReg (0x3B,0x0500);
  TIM_Waitms(20);
  writeToReg (0x22,0x0000);
}

/******************************************************************************
 * Public Functions
 *****************************************************************************/

/******************************************************************************
 *
 * Description:
 *    Initialize the display
 *
 *****************************************************************************/
int32_t truly_lcd_open (int32_t framebuf)
{
  int32_t dev = 0;
  SSP_CFG_Type SSP_ConfigStruct;

  pinConfig();

        // initialize SSP configuration structure to default
        SSP_ConfigStructInit(&SSP_ConfigStruct);

  // set clock rate
  SSP_ConfigStruct.ClockRate = SSP_CLOCK;

        // Initialize SSP peripheral with parameter given in structure above
        SSP_Init(SSP_PORT, &SSP_ConfigStruct);

        // Enable SSP peripheral
        SSP_Cmd(SSP_PORT, ENABLE);

  /* turn on backlight -> active low */
  truly_lcd_setBacklight(100);

  dev = lcd_open((int32_t)&truly_g240320ltsw);

  lcd_ioctl(dev, LCD_PWR_ON, 1);
  lcd_ioctl(dev, LCD_SET_BUFFER, framebuf);

  TIM_Waitms(200);

  /* initialize LCD controller */
  ssd1289_init();

  SSP_Cmd(SSP_PORT, DISABLE);

  SSP_DeInit(SSP_PORT);


  return dev;
}

int32_t truly_lcd_close (int32_t devid)
{
  /* turn off backlight  */
  truly_lcd_setBacklight(0);

  return lcd_close(devid);
}

void truly_lcd_setBacklight (uint8_t percent)
{
  /* Don't have any PWM so turn off for 0% and on in all other cases */
  if (percent == 0)
  {
    LPC_GPIO_PORT->CLR[4] |= (1UL << 11);
  }
  else
  {
    LPC_GPIO_PORT->SET[4] |= (1UL << 11);
  }
}
