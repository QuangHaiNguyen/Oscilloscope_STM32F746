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

uint32_t spifiHandleMemory[20];
uint32_t SystemCoreClock;

void SystemInit()
{ 
  LPC_CGU->BASE_SPIFI_CLK = (0xD<<24)|(1<<11);
  LPC_CGU->IDIVB_CTRL = (0x9<<24)|(1<<11)|(1<<2);
  
  /*
    1. Select the IRC as BASE_M4_CLK source.
    2. Enable the crystal oscillator (see Table 123).
    3. Wait 250 ?s.
    4. Reconfigure PLL1 as follows (see Table 134):
    - Select the M and N divider values to produce the final desired PLL1 output
    frequency foutPLL.
    - Select the crystal oscillator as clock source for PLL1.
    5. Wait for the PLL1 to lock.
    6. Set the PLL1 P-divider to divide by 2 (DIRECT = 0, PSEL=0).
    7. Select PLL1 as BASE_M4_CLK source. The BASE_M4_CLK now operates in the
    mid-frequency range.
    8. Wait 50 ?s.
    9. Set the PLL1 P-divider to direct output mode (DIRECT = 1).
    The BASE_M4_CLK now operates in the high-frequency range.
  */

  LPC_CGU->BASE_M4_CLK = (0x1<<24)|(1<<11);
  LPC_CGU->XTAL_OSC_CTRL = 0;
  {
  volatile int i=0;
  while (i++<1000000);
  }

  uint32_t pll1 = (16<<16) | (6<<24);
  
  LPC_CGU->PLL1_CTRL = pll1;
  while ((LPC_CGU->PLL1_STAT & 1)==0);

  pll1 = LPC_CGU->PLL1_CTRL;
  pll1 &= ~(3<<8);
  LPC_CGU->PLL1_CTRL = pll1;
  
  // Switch M4 clock to PLL1
  LPC_CGU->BASE_M4_CLK = (0x9<<24)|(1<<11);

  {
  volatile int i=0;
  while (i++<1000000);
  }
  
  pll1 = LPC_CGU->PLL1_CTRL;
  pll1 |= (1<<7);
  LPC_CGU->PLL1_CTRL = pll1;

  LPC_SCU->SFSP3_3 = 0xF3; /* high drive for SCLK */

  // IO pins
  LPC_SCU->SFSP3_4=LPC_SCU->SFSP3_5=LPC_SCU->SFSP3_6=LPC_SCU->SFSP3_7 = 0xF3;
  LPC_SCU->SFSP3_8 = 0x33; /* CS doesn't need feedback */
  
  // Intitialize spifi
  spifiInit(0x40003000, 1);
  SPIFI_DEV_FAMILY_T* pFamilyHandle = spifiRegisterFamily(SPIFI_REG_FAMILY_SpansionS25FLP);
  
  uint32_t memSize = spifiGetHandleMemSize(0x40003000);
  SPIFI_HANDLE_T* pSpifi = spifiInitDevice(&spifiHandleMemory, sizeof(spifiHandleMemory), 0x40003000, 0x14000000);

  // Enable quad-spi
  spifiDevSetOpts(pSpifi, SPIFI_CAP_QUAD, 1);

  uint32_t maxSpifiClock = spifiDevGetInfo(pSpifi, SPIFI_INFO_MAXCLOCK);

  spifiDevSetMemMode(pSpifi, 1);
  
  // CMSIS - enable interrupts
  __enable_irq();  
  
  //Setup pin muxing and system memory
  SetupMux();  
  SetupMemory();

// 7.0" 34Mhz pixelclock configuration
#if defined(DISPLAY_UMSH_8596MD_T)

  // Divider A runs PLL1/3
  LPC_CGU->IDIVA_CTRL = (9<<24)|(1<<11)|(2<<2);
  
  // Divider C runs IDIVA/2
  LPC_CGU->IDIVC_CTRL = (0xC<<24)|(1<<11)|(1<<2);
  
  // Set LCD base clock to DIVC = 34 Mhz PxClk
  LPC_CGU->BASE_LCD_CLK = (0xE<<24)|(1<<11);
  
// 5.7" pixelclock configuration
#elif defined(DISPLAY_UMSH_8247MD_T)

  // Divider A runs PLL1/4
  LPC_CGU->IDIVA_CTRL = (9<<24)|(1<<11)|(3<<2);
  
  // Divider C runs IDIVA/2
  LPC_CGU->IDIVC_CTRL = (0xC<<24)|(1<<11)|(1<<2);
  
  // Set LCD base clock to DIVC = 25 Mhz PxClk
  LPC_CGU->BASE_LCD_CLK = (0xE<<24)|(1<<11);

#endif
  SCB->CPACR = (0x3 << (10*2)) | (0x3 << (11*2));
}

void delay(int count)
{
  volatile int i=0;
  while (i++<count);
}
