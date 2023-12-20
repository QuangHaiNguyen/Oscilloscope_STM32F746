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
#include <lpc43xx.h>
#include "spifilib_api.h"

void SetupMux(void);
void SetupMemory(void); 
void SetupPixelClock(void);

typedef struct
{
  volatile uint32_t CTRL; //Ctrl register at address 0x4000 3000)
  volatile uint32_t CMD;
  volatile uint32_t ADDR;
  volatile uint32_t DATINTM;
  volatile uint32_t CACHELIMIT;
  volatile uint32_t DAT;
  volatile uint32_t MEMCMD;
  volatile uint32_t STAT;
} LPC_SPIFI_TypeDef;

void SystemInit_RAM()
{
  //SPIFI control register address 0x4000 3000)
  LPC_SPIFI_TypeDef * LPC_SPIFI = (LPC_SPIFI_TypeDef*)0x40003000;
  uint32_t pll1;
  __disable_irq();
  SCnSCB->ACTLR &= ~2; // disable Cortex write buffer to avoid exceptions when switching back to SPIFI for execution
      
  LPC_SCU->SFSP3_3 = 0xF3; /* high drive for SCLK */
  
  // SPIFI IO pins
  LPC_SCU->SFSP3_4=LPC_SCU->SFSP3_5=LPC_SCU->SFSP3_6=LPC_SCU->SFSP3_7 = 0xF3;
  LPC_SCU->SFSP3_8 = 0x33; /* CS doesn't need feedback */
    
  LPC_CGU->BASE_SPIFI_CLK = (0xD<<24)|(1<<11);
  LPC_CGU->IDIVB_CTRL = (0x9<<24)|(1<<11)|(1<<2);
  
  LPC_CGU->BASE_M4_CLK = (0x1<<24)|(1<<11);
  LPC_CGU->XTAL_OSC_CTRL = 0;
  {
  volatile int i=0;
  while (i++<1000000);
  }

   pll1 = (16<<16) | (6<<24);
  
  LPC_CGU->PLL1_CTRL = pll1;
  while ((LPC_CGU->PLL1_STAT & 1)==0);

  pll1 = LPC_CGU->PLL1_CTRL;
  pll1 &= ~(3<<8);
  LPC_CGU->PLL1_CTRL = pll1;
  
  // Switch M4 clock to PLL1
  LPC_CGU->BASE_M4_CLK = (0x9<<24)|(1<<11);

#if 0
  //shadow area map
  if (!(LPC_RGU->RESET_ACTIVE_STATUS1 & (1<<24))) // CM0 is in reset  
  { 
    LPC_CCU1->CLK_M4_M0APP_CFG = 1;
    LPC_CREG->M0APPMEMMAP = (unsigned)/*&__SPIFI_CM0_segment_start__*/0x14000000;
    LPC_RGU->RESET_CTRL1 = 0; // Release CM0 from reset
  }
#endif
  
  {
  volatile int i=0;
  while (i++<1000000);
  }
  
  pll1 = LPC_CGU->PLL1_CTRL;
  pll1 |= (1<<7);
  LPC_CGU->PLL1_CTRL = pll1;
  
  if (LPC_SPIFI->STAT & 0x01) { // In memory mode?
    LPC_SPIFI->STAT = 0x10; // Go to command mode
    while(LPC_SPIFI->STAT & 0x10); // wait for reset to complete
  }
    
  // SPIFI highspeed mode - Verify that timeout and clock parameters match the S25FL129P Spansion flash  
  LPC_SPIFI->ADDR = 0xffffffff;
  LPC_SPIFI->DATINTM = 0xffffffff;
  LPC_SPIFI->CMD = // send all ones for a while to hopefully reset SPI Flash
    (0xfful << 24) | // opcode 0xff
    (0x5 << 21) | // frame form indicating opcode and 4 address bytes
    (0x0 << 19) | // field form indicating all serial fields
    (0x4 << 16); // 3 intermediate data bytes
  while(LPC_SPIFI->STAT & 2); // wait for command to complete
  
  LPC_SPIFI->CTRL = // set up new CTRL register with high speed options
    (0x100 << 0) | // timeout
    (0x1 << 16) | // cs high, this parameter is dependent on the SPI Flash part and is in SPIFI_CLK cycles. A lower cs high performs faster
    (1 << 29) | // receive full clock (rfclk) - allows for higher speeds
    (1 << 30); // feedback clock (fbclk) - allows for higher speeds
  
  // put part in high speed mode (skipping opcodes)
  LPC_SPIFI->DATINTM = 0xa5; // 0xAx will cause part to use high performace reads (skips opcode on subsequent accesses)
  LPC_SPIFI->CMD =
    (0xebul << 24) | // opcode 0xeb Quad IO High Performance Read for Spansion
    (0x4 << 21) | // frame form indicating opcode and 3 address bytes
    (0x2 << 19) | // field form indicating serial opcode and dual/quad other fields
    (0x3 << 16); // 3 intermediate data bytes
  while(LPC_SPIFI->STAT & 2); // wait for command to complete
  
  LPC_SPIFI->MEMCMD =
    (0xebul << 24) | // opcode 0xeb Quad IO High Performance Read for Spansion
    (0x6 << 21) | // frame form indicating no opcode and 3 address bytes
    (0x2 << 19) | // field form indicating serial opcode and dual/quad other fields
    (0x3 << 16); // 3 intermediate data bytes
  
  // CMSIS - enable interrupts
  __enable_irq();  
  SCnSCB->ACTLR |= 2; // reenable Cortex write buffer
}

