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
#include "io_config.h"
#include "lpc43xx.h"
#include "lpc43xx_scu.h"
#include "lpc43xx_ssp.h"
#include "lpc43xx_gpio.h"
#include "lpc43xx_rgu.h"
#include "lpc43xx_lcd.h"

// 7.0"
#if defined(DISPLAY_UMSH_8596MD_T) //7.0"
  const LCD_CFG_Type LCD_Cfg = 
  {
    .screen_width         = 800,                          /* Pixels per line */
    .screen_height        = 480,                          /* Lines per panel */
    .horizontal_porch     = { .front = 2, .back = 49},    /* porch setting for horizontal */
    .vertical_porch       = { .front = 2, .back = 2},     /* porch setting for vertical */
    .HSync_pulse_width    = 73,                           /* HSYNC pulse width in clocks */
    .VSync_pulse_width    = 45,                           /* VSYNC pulse width in clocks */
    .ac_bias_frequency    = 0,                            /* AC bias frequency in clocks */
    .HSync_pol            = LCD_SIGNAL_ACTIVE_HIGH,       /* HSYNC polarity */
    .VSync_pol            = LCD_SIGNAL_ACTIVE_HIGH,       /* VSYNC polarity */
    .panel_clk_edge       = LCD_CLK_FALLING,              /* Panel Clock Edge Polarity */    
    .OE_pol               = LCD_SIGNAL_ACTIVE_HIGH,       /* Output Enable polarity */
    .line_end_delay       = 10,                           /* 0 if not used */
    .bits_per_pixel       = LCD_BPP16_565,                /* Maximum bits per pixel the display supports */
    .lcd_panel_type       = LCD_TFT,                      /* LCD panel type */
    .corlor_format        = LCD_COLOR_FORMAT_BGR,         /* BGR or RGB */
    .dual_panel           = FALSE                         /* Dual panel, TRUE = dual panel display */
  };
#elif defined(DISPLAY_UMSH_8247MD_T) //5.7"
  const LCD_CFG_Type LCD_Cfg = 
  {
    .screen_width         = 640,                          /* Pixels per line */
    .screen_height        = 480,                          /* Lines per panel */
    .horizontal_porch     = { .front = 2, .back = 49},    /* porch setting for horizontal */
    .vertical_porch       = { .front = 0x40, .back = 2},     /* porch setting for vertical */
    .HSync_pulse_width    = 0x39,                           /* HSYNC pulse width in clocks */ 
    .VSync_pulse_width    = 45,                           /* VSYNC pulse width in clocks */
    .ac_bias_frequency    = 0,                            /* AC bias frequency in clocks */
    .HSync_pol            = LCD_SIGNAL_ACTIVE_HIGH,       /* HSYNC polarity */
    .VSync_pol            = LCD_SIGNAL_ACTIVE_HIGH,       /* VSYNC polarity */
    .panel_clk_edge       = LCD_CLK_FALLING,              /* Panel Clock Edge Polarity */    
    .OE_pol               = LCD_SIGNAL_ACTIVE_HIGH,       /* Output Enable polarity */
    .line_end_delay       = 10,                           /* 0 if not used */
    .bits_per_pixel       = LCD_BPP16_565,                /* Maximum bits per pixel the display supports */
    .lcd_panel_type       = LCD_TFT,                      /* LCD panel type */
    .corlor_format        = LCD_COLOR_FORMAT_BGR,         /* BGR or RGB */
    .dual_panel           = FALSE                         /* Dual panel, TRUE = dual panel display */
  };
#elif defined(DISPLAY_UMSH_8253MD_3T) //4.3"
const LCD_CFG_Type LCD_Cfg = 
{
  .screen_width         = 480,                          /* Pixels per line */
  .screen_height        = 272,                          /* Lines per panel */
  .horizontal_porch     = { .front = 2, .back = 2},     /* porch setting for horizontal */
  .vertical_porch       = { .front = 2, .back = 2},     /* porch setting for vertical */
  .HSync_pulse_width    = 41,                           /* HSYNC pulse width in clocks */
  .VSync_pulse_width    = 10,                           /* VSYNC pulse width in clocks */
  .ac_bias_frequency    = 0,                            /* AC bias frequency in clocks */
  .HSync_pol            = LCD_SIGNAL_ACTIVE_LOW,        /* HSYNC polarity */
  .VSync_pol            = LCD_SIGNAL_ACTIVE_LOW,        /* VSYNC polarity */
  .panel_clk_edge       = LCD_CLK_RISING,               /* Panel Clock Edge Polarity */    
  .OE_pol               = LCD_SIGNAL_ACTIVE_HIGH,       /* Output Enable polarity */
  .line_end_delay       = 10,                           /* 0 if not used */
  .bits_per_pixel       = LCD_BPP16_565,                /* Maximum bits per pixel the display supports */
  .lcd_panel_type       = LCD_TFT,                      /* LCD panel type */
  .corlor_format        = LCD_COLOR_FORMAT_BGR,         /* BGR or RGB */
  .dual_panel           = FALSE                         /* Dual panel, TRUE = dual panel display */
};
#endif

static void LCD_WriteReg(uint16_t addr, uint16_t data)
{
  uint8_t buf[4];
  
  buf[0] = 0x70;
  buf[1] = (addr >> 8);
  buf[2] = (addr & 0xFF);
  
  SSP_DATA_SETUP_Type ssp_data =
  {
    .tx_data = buf,
    .tx_cnt = 0,
    .rx_data = 0,
    .rx_cnt = 0,
    .length = 3,
    .status = 0,
  };
  
  GPIO_ClearValue(0x7, 25);
  SSP_ReadWrite(LPC_SSP0, &ssp_data, SSP_TRANSFER_POLLING);
  GPIO_SetValue(0x7, 25);
  
  buf[0] = 0x72;
  buf[1] = (data >> 8);
  buf[2] = (data & 0xFF);
  
  GPIO_ClearValue(0x7, 25);
  SSP_ReadWrite(LPC_SSP0, &ssp_data, SSP_TRANSFER_POLLING);
  GPIO_SetValue(0x7, 25);  
}

static uint16_t LCD_ReadStatus(void)
{
  uint8_t opcode;
  uint8_t buf[4];
  
  opcode = 0x71;

  SSP_DATA_SETUP_Type ssp_data =
  {
    .tx_data = &opcode,
    .tx_cnt = 0,
    .rx_data = buf,
    .rx_cnt = 0,
    .length = 1,
    .status = 0,
  };
  
  GPIO_SetValue(0x7, 25);
  SSP_ReadWrite(LPC_SSP0, &ssp_data, SSP_TRANSFER_POLLING);
  GPIO_SetValue(0x7, 25);
    
  return ( buf[0] << 8 ) | buf[1];
}

static void delay(uint32_t ms)
{
  uint32_t i;
  for (i = 0; i < 100 * ms; i++) {}
}

static void LCD_Initialize()
{
  //Reset LCD
  RGU_SoftReset(RGU_SIG_LCD);
  while(RGU_GetSignalStatus(RGU_SIG_LCD)) {}
  
  //Reset PIN 
  GPIO_SetDir(0x6, 1<<17, IO_INPUT);
  
  // CS PIN 
  scu_pinmux(0xF, 11, LCD_PINCONFIG, FUNC4 );
  GPIO_SetDir(0x7, 1<<25, IO_OUTPUT);
  GPIO_SetValue(0x7, 25);
    
  //SSP0 Pin-setup 
  scu_pinmux(0xF, 0, (PUP_ENABLE|SLEWRATE_FAST|INBUF_ENABLE|FILTER_DISABLE), FUNC0);   // PF.0 connected to SCL/SCLK 
  scu_pinmux(0xF, 1, (PUP_ENABLE|SLEWRATE_FAST|INBUF_ENABLE|FILTER_DISABLE), FUNC2);   // PF.1 connected to nCS 
  scu_pinmux(0xF, 2, (PUP_ENABLE|SLEWRATE_FAST|INBUF_ENABLE|FILTER_DISABLE), FUNC2);   // PF.2 connected to SO 
  scu_pinmux(0xF, 3, (PUP_ENABLE|SLEWRATE_FAST|INBUF_ENABLE|FILTER_DISABLE), FUNC2);   // PF.3 connected to nSI 
  
  //Synchronous Serial Port configuration
  SSP_CFG_Type ssp_config;
  SSP_ConfigStructInit(&ssp_config);
  
  ssp_config.FrameFormat = SSP_FRAME_SPI;
  ssp_config.Databit = SSP_DATABIT_8;
  ssp_config.CPHA = SSP_CR0_CPHA_SECOND;
  ssp_config.CPOL = SSP_CR0_CPOL_HI;

  SSP_Init(LPC_SSP0, &ssp_config);
  SSP_Cmd(LPC_SSP0, ENABLE);
  
  //wait a while
  delay(200);
  
  LCD_ReadStatus();
   
  LCD_WriteReg(0x02, 0x0200);
  LCD_WriteReg(0x03, 0x6364);
  LCD_WriteReg(0x04, 0x0440);
  
  LCD_WriteReg(0x05, 0);
  LCD_WriteReg(0x0D, 0x3221);
  LCD_WriteReg(0x0E, 0x3100);

  //wait a while
  delay( 100 );

  // Gamma curve adjustment
  LCD_WriteReg(0x30, 0x0000);
  LCD_WriteReg(0x31, 0x0407);
  LCD_WriteReg(0x32, 0x0202);
  LCD_WriteReg(0x33, 0x0000);
  LCD_WriteReg(0x34, 0x0505);
  LCD_WriteReg(0x36, 0x0707);
  LCD_WriteReg(0x37, 0x0000);
  LCD_WriteReg(0x3A, 0x0904);
  LCD_WriteReg(0x3B, 0x0904);
    
  LCD_ReadStatus();
  
  SSP_Cmd(LPC_SSP0, DISABLE);
  SSP_DeInit(LPC_SSP0);
    
  //Configure Backlight for 4.3"
  scu_pinmux(BACKLIGHT_4_3_IO,  LCD_PINCONFIG, FUNC0);
  GPIO_SetDir(3, 0x1000, IO_OUTPUT);

  //Configure Backlight for 5.7"
  scu_pinmux(BACKLIGHT_5_7_IO,  LCD_PINCONFIG, FUNC0);
  GPIO_SetDir(3, 0x2000, IO_OUTPUT);
  
  // Configure Backlight for 7.0"
  scu_pinmux(10, 4,  LCD_PINCONFIG, FUNC4);
  GPIO_SetDir(5, 1<<19, IO_OUTPUT);  
  GPIO_ClearValue(5, 1<<19);

  //Configure L/R for 5.7"
  scu_pinmux(0xD, 0,  LCD_PINCONFIG, FUNC4);
  GPIO_SetDir(6, (1<<14), IO_OUTPUT);
  GPIO_ClearValue(6, (1<<14));
  
  //Apply LCD controller configuration and turn on dislay. Manually bypass pixelclock for TFT displays
  LCD_Init(LPC_LCD, (LCD_CFG_Type *) &LCD_Cfg);
  uint32_t lcd_pol = LPC_LCD->POL; 
  LPC_LCD->POL = lcd_pol | (1<<26); 
  LCD_SetFrameBuffer(LPC_LCD, (void *) 0x28000000); //SDRAM_BASE_ADDRESS
  LCD_Power(LPC_LCD, ENABLE);
   
  //Turn on backlight 
  GPIO_SetValue(0x5, 1<<19);  //7"
  GPIO_SetValue(0x3, 1<<12);  //4.3"
  GPIO_SetValue(0x7, 1<<5);   //3.5"
  GPIO_SetValue(0x3, 1<<13); //5.7
}

/*******************************
    Main board bringup 
********************************/
extern uint32_t SystemCoreClock;
void Board_Init(void)
{
  //Set SystemCoreClock
  SystemCoreClock = 204000000;

  //Setup TFT controller
  LCD_Initialize();
  
  //Touch controller pin configuration
  scu_pinmux(TOUCH_INT_PIN_IO, (MD_PLN|INBUF_ENABLE), FUNC4);
  GPIO_SetDir(7,0x2, IO_INPUT);
  scu_pinmux(0xE, 0 , MD_PUP, FUNC4);
  GPIO_SetDir(7, 0, 1);
  GPIO_SetValue(7, 1);
  delay(100);
  GPIO_ClearValue(7, 1);
  delay(100);
  GPIO_SetValue(7, 1);

#ifdef __GNUC__
  // Explicitly enable FPU 
  SCB->CPACR = (0x3 << (10*2)) | (0x3 << (11*2));
#endif

}
