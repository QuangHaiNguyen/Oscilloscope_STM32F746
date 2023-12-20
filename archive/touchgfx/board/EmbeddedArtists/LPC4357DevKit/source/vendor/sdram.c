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

#include "lpc43xx.h"
#include "lpc_types.h"
#include "lpc43xx_scu.h"
#include "lpc43xx_timer.h"
#include "lpc43xx_cgu.h"
#include "sdram.h"
#include <string.h>

/******************************************************************************
 * Defines and typedefs
 *****************************************************************************/

/* SDRAM refresh time to 16 clock num */
#define EMC_SDRAM_REFRESH(freq,time)  \
  (((uint64_t)((uint64_t)time * freq)/16000000000ull)+1)

#define CLK0_DELAY 7

/******************************************************************************
 * External global variables
 *****************************************************************************/

/******************************************************************************
 * Local variables
 *****************************************************************************/

/******************************************************************************
 * Local Functions
 *****************************************************************************/


/*-------------------------PRIVATE FUNCTIONS------------------------------*/
/*********************************************************************
 * @brief       Calculate EMC Clock from nano second
 * @param[in]   freq - frequency of EMC Clk
 * @param[in]   time - nano second
 * @return      None
 **********************************************************************/
uint32_t NS2CLK(uint32_t freq, uint32_t time){
  uint32_t tmp = freq/1000000;
  return (tmp * time) / 1000;
}

static void pinConfig(void)
{
  /* Set up EMC pin */
  scu_pinmux(   2  ,   9  ,  MD_PLN_FAST  ,  3  );//A0
  scu_pinmux(   2  ,  10  ,  MD_PLN_FAST  ,  3  );//A1
  scu_pinmux(   2  ,  11  ,  MD_PLN_FAST  ,  3  );//A2
  scu_pinmux(   2  ,  12  ,  MD_PLN_FAST  ,  3  );//A3
  scu_pinmux(   2  ,  13  ,  MD_PLN_FAST  ,  3  );//A4
  scu_pinmux(   1  ,   0  ,  MD_PLN_FAST  ,  2  );//A5
  scu_pinmux(   1  ,   1  ,  MD_PLN_FAST  ,  2  );//A6
  scu_pinmux(   1  ,   2  ,  MD_PLN_FAST  ,  2  );//A7
  scu_pinmux(   2  ,   8  ,  MD_PLN_FAST  ,  3  );//A8
  scu_pinmux(   2  ,   7  ,  MD_PLN_FAST  ,  3  );//A9
  scu_pinmux(   2  ,   6  ,  MD_PLN_FAST  ,  2  );//A10
  scu_pinmux(   2  ,   2  ,  MD_PLN_FAST  ,  2  );//A11
  scu_pinmux(   2  ,   1  ,  MD_PLN_FAST  ,  2  );//A12
  scu_pinmux(   2  ,   0  ,  MD_PLN_FAST  ,  2  );//A13
  scu_pinmux(   6  ,   8  ,  MD_PLN_FAST  ,  1  );//A14
  scu_pinmux(   6  ,   7  ,  MD_PLN_FAST  ,  1  );//A15
  scu_pinmux(  13  ,  16  ,  MD_PLN_FAST  ,  2  );//A16
  scu_pinmux(  13  ,  15  ,  MD_PLN_FAST  ,  2  );//A17
  scu_pinmux(  14  ,   0  ,  MD_PLN_FAST  ,  3  );//A18
  scu_pinmux(  14  ,   1  ,  MD_PLN_FAST  ,  3  );//A19
  scu_pinmux(  14  ,   2  ,  MD_PLN_FAST  ,  3  );//A20
  scu_pinmux(  14  ,   3  ,  MD_PLN_FAST  ,  3  );//A21
  scu_pinmux(  14  ,   4  ,  MD_PLN_FAST  ,  3  );//A22
  scu_pinmux(  10  ,   4  ,  MD_PLN_FAST  ,  3  );//A23

  scu_pinmux(   1  ,   7  ,  MD_PLN_FAST  ,  3  );//D0
  scu_pinmux(   1  ,   8  ,  MD_PLN_FAST  ,  3  );//D1
  scu_pinmux(   1  ,   9  ,  MD_PLN_FAST  ,  3  );//D2
  scu_pinmux(   1  ,  10  ,  MD_PLN_FAST  ,  3  );//D3
  scu_pinmux(   1  ,  11  ,  MD_PLN_FAST  ,  3  );//D4
  scu_pinmux(   1  ,  12  ,  MD_PLN_FAST  ,  3  );//D5
  scu_pinmux(   1  ,  13  ,  MD_PLN_FAST  ,  3  );//D6
  scu_pinmux(   1  ,  14  ,  MD_PLN_FAST  ,  3  );//D7
  scu_pinmux(   5  ,   4  ,  MD_PLN_FAST  ,  2  );//D8
  scu_pinmux(   5  ,   5  ,  MD_PLN_FAST  ,  2  );//D9
  scu_pinmux(   5  ,   6  ,  MD_PLN_FAST  ,  2  );//D10
  scu_pinmux(   5  ,   7  ,  MD_PLN_FAST  ,  2  );//D11
  scu_pinmux(   5  ,   0  ,  MD_PLN_FAST  ,  2  );//D12
  scu_pinmux(   5  ,   1  ,  MD_PLN_FAST  ,  2  );//D13
  scu_pinmux(   5  ,   2  ,  MD_PLN_FAST  ,  2  );//D14
  scu_pinmux(   5  ,   3  ,  MD_PLN_FAST  ,  2  );//D15
  scu_pinmux(  13  ,   2  ,  MD_PLN_FAST  ,  2  );//D16
  scu_pinmux(  13  ,   3  ,  MD_PLN_FAST  ,  2  );//D17
  scu_pinmux(  13  ,   4  ,  MD_PLN_FAST  ,  2  );//D18
  scu_pinmux(  13  ,   5  ,  MD_PLN_FAST  ,  2  );//D19
  scu_pinmux(  13  ,   6  ,  MD_PLN_FAST  ,  2  );//D20
  scu_pinmux(  13  ,   7  ,  MD_PLN_FAST  ,  2  );//D21
  scu_pinmux(  13  ,   8  ,  MD_PLN_FAST  ,  2  );//D22
  scu_pinmux(  13  ,   9  ,  MD_PLN_FAST  ,  2  );//D23
  scu_pinmux(  14  ,   5  ,  MD_PLN_FAST  ,  3  );//D24
  scu_pinmux(  14  ,   6  ,  MD_PLN_FAST  ,  3  );//D25
  scu_pinmux(  14  ,   7  ,  MD_PLN_FAST  ,  3  );//D26
  scu_pinmux(  14  ,   8  ,  MD_PLN_FAST  ,  3  );//D27
  scu_pinmux(  14  ,   9  ,  MD_PLN_FAST  ,  3  );//D28
  scu_pinmux(  14  ,  10  ,  MD_PLN_FAST  ,  3  );//D29
  scu_pinmux(  14  ,  11  ,  MD_PLN_FAST  ,  3  );//D30
  scu_pinmux(  14  ,  12  ,  MD_PLN_FAST  ,  3  );//D31

  scu_pinmux(   1  ,   3  ,  MD_PLN_FAST  ,  3  );//OE
  scu_pinmux(   1  ,   6  ,  MD_PLN_FAST  ,  3  );//WE

  scu_pinmux(   1  ,   4  ,  MD_PLN_FAST  ,  3  );//BLS0
  scu_pinmux(   6  ,   6  ,  MD_PLN_FAST  ,  1  );//BLS1
  scu_pinmux(  13  ,  13  ,  MD_PLN_FAST  ,  2  );//BLS2
  scu_pinmux(  13  ,  10  ,  MD_PLN_FAST  ,  2  );//BLS3

  scu_pinmux(   1  ,   5  ,  MD_PLN_FAST  ,  3  );//CS0
  scu_pinmux(   6  ,   3  ,  MD_PLN_FAST  ,  3  );//CS1
  scu_pinmux(  13  ,  12  ,  MD_PLN_FAST  ,  2  );//CS2
  scu_pinmux(  13  ,  11  ,  MD_PLN_FAST  ,  2  );//CS3

  scu_pinmux(   6  ,   4  ,  MD_PLN_FAST  ,  3  );//CAS
  scu_pinmux(   6  ,   5  ,  MD_PLN_FAST  ,  3  );//RAS

  scu_pinmux(   6  ,   9  ,  MD_PLN_FAST  ,  3  );//DYCS0
  scu_pinmux(   6  ,   1  ,  MD_PLN_FAST  ,  1  );//DYCS1
  scu_pinmux(  13  ,  14  ,  MD_PLN_FAST  ,  2  );//DYCS2
  scu_pinmux(  15  ,  14  ,  MD_PLN_FAST  ,  3  );//DYCS3

  scu_pinmux(   6  ,  11  ,  MD_PLN_FAST  ,  3  );//CKEOUT0
  scu_pinmux(   6  ,   2  ,  MD_PLN_FAST  ,  1  );//CKEOUT1
  scu_pinmux(  13  ,   1  ,  MD_PLN_FAST  ,  2  );//CKEOUT2
  scu_pinmux(  14  ,  15  ,  MD_PLN_FAST  ,  3  );//CKEOUT3

  scu_pinmux(   6  ,  12  ,  MD_PLN_FAST  ,  3  );//DQMOUT0
  scu_pinmux(   6  ,  10  ,  MD_PLN_FAST  ,  3  );//DQMOUT1
  scu_pinmux(  13  ,   0  ,  MD_PLN_FAST  ,  2  );//DQMOUT2
  scu_pinmux(  14  ,  13  ,  MD_PLN_FAST  ,  3  );//DQMOUT3
}



/******************************************************************************
 * Public Functions
 *****************************************************************************/


/******************************************************************************
 *
 * Description:
 *    Initialize the SDRAM
 *
 *****************************************************************************/
uint32_t sdram_init (void)
{
  uint32_t temp;
  uint32_t emcclk;

  pinConfig(); //Full 32-bit Data bus, 24-bit Address

  /* Select EMC clock-out */
  LPC_SCU->SFSCLK_0 = MD_PLN_FAST;
  LPC_SCU->SFSCLK_1 = MD_PLN_FAST;
  LPC_SCU->SFSCLK_2 = MD_PLN_FAST;
  LPC_SCU->SFSCLK_3 = MD_PLN_FAST;

  /* Setup EMC delays */
  LPC_SCU->EMCDELAYCLK = ((CLK0_DELAY) | (CLK0_DELAY << 4) | (CLK0_DELAY << 8) | (CLK0_DELAY << 12));

  /* EMC clock is half of CGU_BASE_M4 if CGU_BASE_M4 is >104 MHz */
  if (SystemCoreClock > 104000000)
  {
    LPC_CCU1->CLK_M4_EMCDIV_CFG = ( (1<<0) | (1<<1) | (1<<2) | (1<<5) );
    LPC_CREG->CREG6 |= (1<<16);
    emcclk = SystemCoreClock / 2;
  }
  else
  {
    LPC_CCU1->CLK_M4_EMCDIV_CFG = ( (1<<0) | (1<<1) | (1<<2) | (0<<5) );
    LPC_CREG->CREG6 &= ~(1<<16);
    emcclk = SystemCoreClock;
  }

  LPC_EMC->CONTROL   = 0x00000001;
  LPC_EMC->CONFIG    = 0x00000000;
  LPC_EMC->DYNAMICCONFIG0    = 1<<14 | 0<<12 | 2<<9 | 1<<7; /* 256Mb, 8Mx32, 4 banks, row=12, column=9 */

  LPC_EMC->DYNAMICRASCAS0    = 0x00000303; /* 3 RAS, 3 CAS latency */
  LPC_EMC->DYNAMICREADCONFIG = 0x00000001; /* Command delayed strategy, using EMCCLKDELAY */

  LPC_EMC->DYNAMICRP         = NS2CLK(emcclk, 20);
  LPC_EMC->DYNAMICRAS        = NS2CLK(emcclk, 45);
  LPC_EMC->DYNAMICSREX       = NS2CLK(emcclk, 70);
  LPC_EMC->DYNAMICAPR        = 0x00000005;
  LPC_EMC->DYNAMICDAL        = 0x00000005;
  LPC_EMC->DYNAMICWR         = NS2CLK(emcclk, 14);
  LPC_EMC->DYNAMICRC         = NS2CLK(emcclk, 68);
  LPC_EMC->DYNAMICRFC        = NS2CLK(emcclk, 68);
  LPC_EMC->DYNAMICXSR        = NS2CLK(emcclk, 70);
  LPC_EMC->DYNAMICRRD        = NS2CLK(emcclk, 14);
  LPC_EMC->DYNAMICMRD        = 0x00000002;

  TIM_Waitus(100);               /* wait 100us */
  LPC_EMC->DYNAMICCONTROL    = 0x00000183; /* Issue NOP command */

  TIM_Waitus(200);               /* wait 200us */
  LPC_EMC->DYNAMICCONTROL    = 0x00000103; /* Issue PALL command */

  LPC_EMC->DYNAMICREFRESH    = 2; /* ( n * 16 ) -> 32 clock cycles */

  TIM_Waitus(200);               /* wait 200us */

  /* (64ms / 4096 row) */
  LPC_EMC->DYNAMICREFRESH    = NS2CLK(emcclk, 64000000 / 4096)/16; /* ( n * 16 ) */

  LPC_EMC->DYNAMICCONTROL    = 0x00000083; /* Issue MODE command */

  temp = *((volatile uint32_t *)(SDRAM_BASE | (3<<4| 2)<<(9+2+2))); /* 4 burst, 3 CAS latency */
  temp = temp;
  LPC_EMC->DYNAMICCONTROL    = 0x00000000; /* Issue NORMAL command */

  //[re]enable buffers
  LPC_EMC->DYNAMICCONFIG0    |= 1<<19;

  return TRUE;
}

uint32_t sdram_test( void )
{
  volatile uint32_t *wr_ptr;
  volatile uint16_t *short_wr_ptr;
  uint32_t data;
  uint32_t i, j;

  wr_ptr = (uint32_t *)SDRAM_BASE;
  short_wr_ptr = (uint16_t *)wr_ptr;

  /* 16 bit write */
  for (i = 0; i < SDRAM_SIZE/0x40000; i++)
  {
    for (j = 0; j < 0x100; j++)
    {
      *short_wr_ptr++ = (i + j) & 0xFFFF;
      *short_wr_ptr++ = ((i + j) + 1) & 0xFFFF;
    }
  }

  /* Verifying */
  wr_ptr = (uint32_t *)SDRAM_BASE;
  for (i = 0; i < SDRAM_SIZE/0x40000; i++)
  {
    for (j = 0; j < 0x100; j++)
    {
      data = *wr_ptr;
      if (data != (((((i + j) + 1) & 0xFFFF) << 16) | ((i + j) & 0xFFFF)))
      {
        return 0x0;
      }
      wr_ptr++;
    }
  }
  return 0x1;
}
