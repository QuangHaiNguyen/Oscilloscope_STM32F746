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

#include "lpc_types.h"
#include "lpc43xx.h"
#include "lpc43xx_cgu.h"
#include "bsp/LEDs_init.hpp"

static uint16_t ledsState = 0;

uint32_t LEDs_init (void)
{
// Has been set up by the NOR flash  
//  LPC_EMC->CONTROL   = 0x00000001;
//  LPC_EMC->CONFIG    = 0x00000000;
//
//  pinConfig();

  // Setup for 16-bit access
  LPC_EMC->STATICCONFIG2   = 0x00000001;
  setLEDs(0);
  return TRUE;
}

void setLEDs(uint16_t val)
{
  uint16_t* memreg = (uint16_t*)MEMREG_BASE;
  *memreg = val;
  ledsState = val;
}

uint16_t getLEDs(void)
{
  return ledsState;

}
