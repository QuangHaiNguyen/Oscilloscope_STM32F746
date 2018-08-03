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
#include "chip.h"

// SCR pin definitions for pin muxing
typedef struct {
  uint8_t pingroup;
  uint8_t pinnumber;
  uint16_t modefunc;
} PINMUX_t;

// The following pin muxing configurations covers all configurations of the Mjolner evaluation board.
STATIC const PINMUX_t pinmuxing[] = {
  
  // External data lines
  {0x1, 7,  (SCU_PINIO_FAST | SCU_MODE_FUNC3)}, //D0
  {0x1, 8,  (SCU_PINIO_FAST | SCU_MODE_FUNC3)}, //D1
  {0x1, 9,  (SCU_PINIO_FAST | SCU_MODE_FUNC3)}, //D2
  {0x1, 10, (SCU_PINIO_FAST | SCU_MODE_FUNC3)}, //D3
  {0x1, 11, (SCU_PINIO_FAST | SCU_MODE_FUNC3)}, //D4
  {0x1, 12, (SCU_PINIO_FAST | SCU_MODE_FUNC3)}, //D5
  {0x1, 13, (SCU_PINIO_FAST | SCU_MODE_FUNC3)}, //D6
  {0x1, 14, (SCU_PINIO_FAST | SCU_MODE_FUNC3)}, //D7
  {0x5, 4,  (SCU_PINIO_FAST | SCU_MODE_FUNC2)}, //D8
  {0x5, 5,  (SCU_PINIO_FAST | SCU_MODE_FUNC2)}, //D9
  {0x5, 6,  (SCU_PINIO_FAST | SCU_MODE_FUNC2)}, //D10
  {0x5, 7,  (SCU_PINIO_FAST | SCU_MODE_FUNC2)}, //D11
  {0x5, 0,  (SCU_PINIO_FAST | SCU_MODE_FUNC2)}, //D12
  {0x5, 1,  (SCU_PINIO_FAST | SCU_MODE_FUNC2)}, //D13
  {0x5, 2,  (SCU_PINIO_FAST | SCU_MODE_FUNC2)}, //D14
  {0x5, 3,  (SCU_PINIO_FAST | SCU_MODE_FUNC2)}, //D15
  
  //Address lines
  {0x2, 9,  (SCU_PINIO_FAST | SCU_MODE_FUNC3)}, //A0
  {0x2, 10, (SCU_PINIO_FAST | SCU_MODE_FUNC3)}, //A1
  {0x2, 11, (SCU_PINIO_FAST | SCU_MODE_FUNC3)}, //A2
  {0x2, 12, (SCU_PINIO_FAST | SCU_MODE_FUNC3)}, //A3
  {0x2, 13, (SCU_PINIO_FAST | SCU_MODE_FUNC3)}, //A4
  {0x1, 0,  (SCU_PINIO_FAST | SCU_MODE_FUNC2)}, //A5
  {0x1, 1,  (SCU_PINIO_FAST | SCU_MODE_FUNC2)}, //A6
  {0x1, 2,  (SCU_PINIO_FAST | SCU_MODE_FUNC2)}, //A7
  {0x2, 8,  (SCU_PINIO_FAST | SCU_MODE_FUNC3)}, //A8
  {0x2, 7,  (SCU_PINIO_FAST | SCU_MODE_FUNC3)}, //A9
  {0x2, 6,  (SCU_PINIO_FAST | SCU_MODE_FUNC2)}, //A10
  {0x2, 2,  (SCU_PINIO_FAST | SCU_MODE_FUNC2)}, //A11
  {0x2, 1,  (SCU_PINIO_FAST | SCU_MODE_FUNC2)}, //A12
  {0x2, 0,  (SCU_PINIO_FAST | SCU_MODE_FUNC2)}, //A13
  {0x6, 8,  (SCU_PINIO_FAST | SCU_MODE_FUNC1)}, //A14
  {0x6, 7,  (SCU_PINIO_FAST | SCU_MODE_FUNC1)}, //A15
  {0xD, 16, (SCU_PINIO_FAST | SCU_MODE_FUNC2)}, //A16
  {0xD, 15, (SCU_PINIO_FAST | SCU_MODE_FUNC2)}, //A17
  {0xE, 0,  (SCU_PINIO_FAST | SCU_MODE_FUNC3)}, //A18
  {0xE, 1,  (SCU_PINIO_FAST | SCU_MODE_FUNC3)}, //A19
  {0xE, 2,  (SCU_PINIO_FAST | SCU_MODE_FUNC3)}, //A20
  {0xE, 3,  (SCU_PINIO_FAST | SCU_MODE_FUNC3)}, //A21
  {0xE, 4,  (SCU_PINIO_FAST | SCU_MODE_FUNC3)}, //A22
  {0xA, 4,  (SCU_PINIO_FAST | SCU_MODE_FUNC3)}, //A23
  
  // EMC control signal
  {0x1, 4,  (SCU_PINIO_FAST | SCU_MODE_FUNC3)},
  {0x6, 6,  (SCU_PINIO_FAST | SCU_MODE_FUNC1)},
  {0xD, 13, (SCU_PINIO_FAST | SCU_MODE_FUNC2)},
  {0xD, 10, (SCU_PINIO_FAST | SCU_MODE_FUNC2)},
  {0x6, 9,  (SCU_PINIO_FAST | SCU_MODE_FUNC3)},
  {0x1, 6,  (SCU_PINIO_FAST | SCU_MODE_FUNC3)},
  {0x6, 4,  (SCU_PINIO_FAST | SCU_MODE_FUNC3)},
  {0x6, 5,  (SCU_PINIO_FAST | SCU_MODE_FUNC3)},
  {0x6, 11, (SCU_PINIO_FAST | SCU_MODE_FUNC3)},
  {0x6, 12, (SCU_PINIO_FAST | SCU_MODE_FUNC3)},
  {0x6, 10, (SCU_PINIO_FAST | SCU_MODE_FUNC3)},
  {0xD, 0,  (SCU_PINIO_FAST | SCU_MODE_FUNC2)},
  {0xE, 13, (SCU_PINIO_FAST | SCU_MODE_FUNC3)},
  {0x1, 3,  (SCU_PINIO_FAST | SCU_MODE_FUNC3)}, 
  {0x1, 4,  (SCU_PINIO_FAST | SCU_MODE_FUNC3)},
  {0x6, 6,  (SCU_PINIO_FAST | SCU_MODE_FUNC3)},
  {0x1, 5,  (SCU_PINIO_FAST | SCU_MODE_FUNC3)},
  
  // LCD interface-24bpp (BGR)
  {0x4, 5,  (SCU_MODE_MODE_INACT | SCU_MODE_FUNC2)}, //LCD_VS
  {0x4, 6,  (SCU_MODE_MODE_INACT | SCU_MODE_FUNC2)}, //LCD_DE
  {0x4, 7,  (SCU_MODE_MODE_INACT | SCU_MODE_FUNC0)}, //LCD_CLK
  {0x7, 6,  (SCU_MODE_MODE_INACT | SCU_MODE_FUNC3)}, //LCD_HS
  {0x7, 7,  (SCU_MODE_MODE_INACT | SCU_MODE_FUNC3)}, //LCD_PWR
  {0xB, 0,  (SCU_MODE_MODE_INACT | SCU_MODE_FUNC2)}, //BB0
  {0xB, 1,  (SCU_MODE_MODE_INACT | SCU_MODE_FUNC2)}, //BB1
  {0xB, 2,  (SCU_MODE_MODE_INACT | SCU_MODE_FUNC2)}, //BB2
  {0xB, 3,  (SCU_MODE_MODE_INACT | SCU_MODE_FUNC2)}, //BB3
  {0x7, 1,  (SCU_MODE_MODE_INACT | SCU_MODE_FUNC3)}, //BB4
  {0x7, 2,  (SCU_MODE_MODE_INACT | SCU_MODE_FUNC3)}, //BB5
  {0x7, 3,  (SCU_MODE_MODE_INACT | SCU_MODE_FUNC0)}, //BB6
  {0x7, 4,  (SCU_MODE_MODE_INACT | SCU_MODE_FUNC3)}, //BB7
  {0xB, 4,  (SCU_MODE_MODE_INACT | SCU_MODE_FUNC2)}, //GG0
  {0xB, 5,  (SCU_MODE_MODE_INACT | SCU_MODE_FUNC2)}, //GG1
  {0xB, 6,  (SCU_MODE_MODE_INACT | SCU_MODE_FUNC2)}, //GG2
  {0x8, 3,  (SCU_MODE_MODE_INACT | SCU_MODE_FUNC3)}, //GG3
  {0x4, 9,  (SCU_MODE_MODE_INACT | SCU_MODE_FUNC2)}, //GG4
  {0x4, 10, (SCU_MODE_MODE_INACT | SCU_MODE_FUNC2)}, //GG5
  {0x4, 8,  (SCU_MODE_MODE_INACT | SCU_MODE_FUNC2)}, //GG6
  {0x7, 5,  (SCU_MODE_MODE_INACT | SCU_MODE_FUNC3)}, //GG7
  {0x8, 4,  (SCU_MODE_MODE_INACT | SCU_MODE_FUNC3)}, //RR0
  {0x8, 5,  (SCU_MODE_MODE_INACT | SCU_MODE_FUNC3)}, //RR1
  {0x8, 6,  (SCU_MODE_MODE_INACT | SCU_MODE_FUNC3)}, //RR2
  {0x8, 7,  (SCU_MODE_MODE_INACT | SCU_MODE_FUNC3)}, //RR3
  {0x4, 2,  (SCU_MODE_MODE_INACT | SCU_MODE_FUNC2)}, //RR4
  {0x4, 3,  (SCU_MODE_MODE_INACT | SCU_MODE_FUNC2)}, //RR5
  {0x4, 4,  (SCU_MODE_MODE_INACT | SCU_MODE_FUNC2)}, //RR6
  {0x4, 1,  (SCU_MODE_MODE_INACT | SCU_MODE_FUNC2)}, //RR7
  {0xD, 2,  (SCU_MODE_MODE_INACT | SCU_MODE_FUNC4)}, //POL
  {0xD, 3,  (SCU_MODE_MODE_INACT | SCU_MODE_FUNC4)}, //RESB
  {0xD, 4,  (SCU_MODE_MODE_INACT | SCU_MODE_FUNC4)}, //TB
  {0xD, 5,  (SCU_MODE_MODE_INACT | SCU_MODE_FUNC4)}, //REV
  {0xD, 6,  (SCU_MODE_MODE_INACT | SCU_MODE_FUNC4)}, //RL
  {0xD, 7,  (SCU_MODE_MODE_INACT | SCU_MODE_FUNC4)}, //BGR
  {0xD, 8,  (SCU_MODE_MODE_INACT | SCU_MODE_FUNC4)}, //SEL0
  {0xD, 9,  (SCU_MODE_MODE_INACT | SCU_MODE_FUNC4)}, //SEL1
  {0xD, 10, (SCU_MODE_MODE_INACT | SCU_MODE_FUNC4)}, //SEL2
  {0xD, 11, (SCU_MODE_MODE_INACT | SCU_MODE_FUNC4)}, //CPE
  {0xD, 12, (SCU_MODE_MODE_INACT | SCU_MODE_FUNC4)}, //PINV
};

STATIC const PINMUX_t pinclockmuxing[] = {
  {0, 0,  (SCU_PINIO_FAST | SCU_MODE_FUNC0)},
  {0, 1,  (SCU_PINIO_FAST | SCU_MODE_FUNC0)},
  {0, 2,  (SCU_PINIO_FAST | SCU_MODE_FUNC0)},
  {0, 3,  (SCU_PINIO_FAST | SCU_MODE_FUNC0)},
};


/***************************************
   Apply pin muxing configurations
    * pinmuxing
    * pinclockmuxing
***************************************/
void SetupMux(void)
{
  int i;
  
  for (i = 0; i < (sizeof(pinmuxing) / sizeof(pinmuxing[0])); i++) {
    Chip_SCU_PinMuxSet(pinmuxing[i].pingroup, pinmuxing[i].pinnumber, pinmuxing[i].modefunc);
  }
  //Clock pins
  for (i = 0; i < (sizeof(pinclockmuxing) / sizeof(pinclockmuxing[0])); i++) {
    Chip_SCU_ClockPinMuxSet(pinclockmuxing[i].pinnumber, pinclockmuxing[i].modefunc);
  }
}

// SDRAM timing and IS42S16400 chip configuration
STATIC const IP_EMC_DYN_CONFIG_T IS42S16400_config = {
        EMC_NANOSECOND(64000000 / 4096),        // Row refresh time
        0x01,                                   //Command Delayed
        EMC_NANOSECOND(20),
        EMC_NANOSECOND(60),
        EMC_NANOSECOND(63),
        EMC_CLOCK(0x05),
        EMC_CLOCK(0x05),
        EMC_CLOCK(0x04),
        EMC_NANOSECOND(63),
        EMC_NANOSECOND(63),
        EMC_NANOSECOND(63),
        EMC_NANOSECOND(14),
        EMC_CLOCK(0x02),
        {
          {
            EMC_ADDRESS_DYCS0,  // DYCS0 bank for SDRAM
            3,                  //Row Address Strobe. The address inputs are captured on the falling edge of /RAS, and select a row to open. The row is held open as long as /RAS is low.            
            EMC_DYN_MODE_WBMODE_PROGRAMMED |
            EMC_DYN_MODE_OPMODE_STANDARD |
            EMC_DYN_MODE_CAS_3 |
            EMC_DYN_MODE_BURST_TYPE_SEQUENTIAL |
            EMC_DYN_MODE_BURST_LEN_8,                  
            EMC_DYN_CONFIG_DATA_BUS_16 |
            EMC_DYN_CONFIG_LPSDRAM |
            EMC_DYN_CONFIG_4Mx16_4BANKS_12ROWS_8COLS |
            EMC_DYN_CONFIG_MD_SDRAM
          },
          {0, 0, 0, 0},
          {0, 0, 0, 0},
          {0, 0, 0, 0}
        }
};

#define CLK0_DELAY 7 // EMC clock delay

/***************************************
   Apply IS42S16400 memory configuration
***************************************/
void SetupMemory(void)
{
  LPC_SCU->EMCDELAYCLK = ((CLK0_DELAY) | (CLK0_DELAY << 4) | (CLK0_DELAY << 8) | (CLK0_DELAY << 12));   //EMC Clock Delay configuration  
  Chip_Clock_EnableOpts(CLK_MX_EMC_DIV, true, true, 2);                                                 //EMC Clock Divider configuration
  LPC_CREG->CREG6 |= (1 << 16);                                                                         //EMC Clock Divider configuration
  Chip_Clock_Enable(CLK_MX_EMC);                                                                        //Enable EMC clock
  Chip_EMC_Init(1, 0);                                                                                  //Init EMC Controller - LE mode
  Chip_EMC_Dynamic_Init((IP_EMC_DYN_CONFIG_T *) &IS42S16400_config);                                    // Init Dynamic Controller
  LPC_EMC->STATICCONFIG0 |= 1 << 19;                                                                    //Enable External Flash Buffer
}
