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

extern void SystemInit_RAM(void);

void SystemInit()
{ 
#if defined(__ARMCC_VERSION)  
  //Enable FPU in CPACR register
  *((volatile unsigned long*)0xE000ED88) = (0x3 << (10*2)) | (0x3 << (11*2));
#endif
  
  typedef void (*voidfnptr)(void);
  volatile unsigned *SPIFIInitPtr = (unsigned *)((unsigned)&SystemInit_RAM & ~1);
  volatile unsigned *SRAMPtr = (unsigned *) 0x1008A000;
  unsigned i;
  //Copy method to internal RAM
  for (i=0;i<100;i++) 
    *SRAMPtr++ = *SPIFIInitPtr++;
  //Call RAM method
  ((voidfnptr)(0x1008A001))();

  //Setup pin muxing and system memory
  SetupMux();  
  SetupMemory();
}

void delay(int count)
{
  volatile int i=0;
  while (i++<count);
}
