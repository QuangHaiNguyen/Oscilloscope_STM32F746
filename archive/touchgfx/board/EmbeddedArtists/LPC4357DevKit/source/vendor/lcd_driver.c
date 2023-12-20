/*****************************************************************************
 *   lcd_driver.c:  LCD module file for NXP LPC43xx Family Microprocessors
 *
 *   Copyright(C) 2009, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2009.05.26  ver 1.00    Prelimnary version, first Release
 *
******************************************************************************/
#include "lpc43xx.h"
#include "lpc_types.h"
#include "lpc43xx_cgu.h"
#include "lpc43xx_timer.h"
#include "lcd_params.h"
#include "lcd_driver.h"


/***********************************************************************
 * LCD driver private data and types
 **********************************************************************/

/* LCD device configuration structure type */
typedef struct
{
  uint8_t init;          /* Device initialized flag */
  LCD_PARAM_T *dptr;     /* Pointer to LCD controller settings */
} CLCDC_CFG_T;

/***********************************************************************
 * Forward declration
 **********************************************************************/
void lcd_display_init(void);


/* LCD device configuration structure */
static CLCDC_CFG_T lcdcfg;

static uint32_t getClockDivisor(uint32_t desired_clock)
{
  uint32_t pixel_div, tmp = 0;
  uint32_t clk;

  clk = CGU_GetPCLKFrequency(CGU_PERIPHERAL_M4CORE);//CLKPWR_CLKTYPE_CPU);

  /* Find closest clock divider to get clock rate */
  pixel_div = 1;
  while (((clk / pixel_div) > desired_clock) && (pixel_div <= 0x3F))
  {
    pixel_div++;
  }

  if (pixel_div <= 1)
  {
    /* Pixel clock divider is 1, skip divider logic */
    tmp = CLCDC_LCDTIMING2_BCD;
  }
  else
  {
    /* Add in new divider */
    pixel_div -= 2;

    tmp |= (((pixel_div >> 0) & 0x1F) 
           | (((pixel_div >> 5) & 0x1F) << 27));
  }

  return tmp;
}

/***********************************************************************
 *
 * Function: lcd_initialize
 *
 * Purpose: Initialize the LCD controller
 *
 * Processing:
 *     Prior to resetting LCD values, disable the LCD controller.
 *     Configurate the LCD registers with the values in the passed 
 *     LCD_PARAM_T structure.
 *
 * Parameters:
 *     lcdcfgptr : A pointer to an LCD configuration data structure
 *
 * Outputs: None
 *
 * Returns: (0) if the LCD was initialized, otherwise (-1)
 *
 **********************************************************************/
static uint8_t lcd_initialize(CLCDC_CFG_T *lcdcfgptr)
{
  uint32_t tmp, i;
  int32_t status = (int32_t)(0);
  LCD_PARAM_T *cgdatptr = lcdcfgptr->dptr;

  /* Disable the display in case it is on */
  LPC_LCD->CTRL &= ~CLCDC_LCDCTRL_ENABLE;

  /* Generate the horizontal axis plane control word */  
  tmp = (CLCDC_LCDTIMING0_PPL(cgdatptr->pixels_per_line) |
         CLCDC_LCDTIMING0_HSW(cgdatptr->h_sync_pulse_width) |
         CLCDC_LCDTIMING0_HFP(cgdatptr->h_front_porch) |
         CLCDC_LCDTIMING0_HBP(cgdatptr->h_back_porch));
  LPC_LCD->TIMH = tmp;

  /* Generate the vertical axis plane control word */
  tmp = (CLCDC_LCDTIMING1_LPP(cgdatptr->lines_per_panel) |
         CLCDC_LCDTIMING1_VSW(cgdatptr->v_sync_pulse_width) |
         CLCDC_LCDTIMING1_VFP(cgdatptr->v_front_porch) |
         CLCDC_LCDTIMING1_VBP(cgdatptr->v_back_porch));
  LPC_LCD->TIMV = tmp;

  /* Generate the clock and signal polarity control word */
  if(cgdatptr->ac_bias_frequency != 0)
  {
    /* STN panel has AC bias value */
    tmp = CLCDC_LCDTIMING2_ACB(cgdatptr->ac_bias_frequency);
  }
  else
  {
    tmp = 0;
  }

  if (cgdatptr->invert_output_enable != 0)
  {
    tmp |= CLCDC_LCDTIMING2_IOE;
  }
  
  if (cgdatptr->invert_panel_clock != 0)
  {
    tmp |= CLCDC_LCDTIMING2_IPC;
  }
  
  if (cgdatptr->invert_hsync != 0)
  {
    tmp |= CLCDC_LCDTIMING2_IHS;
  }
  
  if (cgdatptr->invert_vsync != 0)
  {
    tmp |= CLCDC_LCDTIMING2_IVS;
  }

  /* Compute clocks per line based on panel type */
  switch (cgdatptr->lcd_panel_type)
  {
    case MONO_4BIT:
      /* Clocks per line is a quarter of pixels per line */
      tmp = tmp | CLCDC_LCDTIMING2_CPL((cgdatptr->pixels_per_line / 4) - 1);
      break;

    case MONO_8BIT:
      /* Clocks per line is an eighth of pixels per line */
      tmp = tmp | CLCDC_LCDTIMING2_CPL((cgdatptr->pixels_per_line / 8) - 1);
      break;

    case CSTN:
      /* CSTN Clocks per line (* 3 / 8) */
      tmp = tmp | CLCDC_LCDTIMING2_CPL(((cgdatptr->pixels_per_line * 3) / 8) - 1);
      break;
            
    case TFT:
    case ADTFT:
    case HRTFT:
    default:
      /* Clocks per line and pixels per line are the same */
      tmp = tmp | CLCDC_LCDTIMING2_CPL(cgdatptr->pixels_per_line - 1);
      break;
  }

  /* clock */
  if (cgdatptr->optimal_clock == 0)
  {
    /* Setup the LCD clock to use the external clock */
    CGU_EntityConnect(CGU_CLKSRC_GP_CLKIN, CGU_BASE_LCD);    
    
    /* Use external clock in and bypass the pixel clock divider */
    tmp = tmp | CLCDC_LCDTIMING2_CLKSEL | CLCDC_LCDTIMING2_BCD;    
  }
  else
  {
    /* Setup the LCD clock to use the internal clock and dividers */
    CGU_EntityConnect(CGU_CLKSRC_PLL1, CGU_BASE_LCD);    
    
    tmp = tmp | getClockDivisor(cgdatptr->optimal_clock);    
  }

  LPC_LCD->POL = tmp;

  /* Skip line end control word - just set to 0x0 */
  LPC_LCD->LE = 0x00000000;

  /* Default with all interrupts of */
  LPC_LCD->INTMSK = 0x00000000;

  /* Default configuration is 16 bits per pixel 565 */
  tmp = CLCDC_LCDCTRL_BPP16_565_MODE;

  /* blue and blue swapped */
  tmp |= CLCDC_LCDCTRL_BGR;

  switch (cgdatptr->lcd_panel_type)
  {
    case ADTFT:
    case HRTFT:
    case TFT:
      tmp |= CLCDC_LCDCTRL_TFT;
      break;

    case MONO_4BIT:
      tmp |= CLCDC_LCDCTRL_BW_MONO;
      break;

    case MONO_8BIT:
      tmp |= (CLCDC_LCDCTRL_MON8 | CLCDC_LCDCTRL_BW_MONO);
      break;

    case CSTN:
      ;
      break;

    default:
      /* Unsupported panel type */
      status = (int32_t)(-1);
      break;
  }

  /* Dual panel operation */
  if (cgdatptr->dual_panel == 1)
  {
    tmp |= CLCDC_LCDCTRL_DUAL;
  }

  LPC_LCD->CTRL = tmp;

  /* clear the palette (color is black )*/
  for (i = 0; i < sizeof(LPC_LCD->PAL)/sizeof(LPC_LCD->PAL[0]); i++)
  {
    LPC_LCD->PAL[i] = 0;
  }

//   LPC_SC->LCD_CFG = 0x0;

  return status;
}

/***********************************************************************
 *
 * Function: lcd_open
 *
 * Purpose: Open the LCD controller
 *
 * Processing:
 *     If init is not FALSE, return 0x00000000.
 *     Otherwise, return a pointer to the LCD configuration data structure.
 *
 * Parameters:
 *     arg   : LCD type - LCD_PARAM_T
 *
 * Outputs: None
 *
 * Returns: The pointer to a LCD config structure or 0
 *
 **********************************************************************/
int32_t lcd_open(int32_t arg)
{
  int32_t status = 0;

  if (lcdcfg.init == FALSE)
  {
    /* Power Control for CLCDC */ 
//     LPC_SC->PCONP |= 0x00000001;  

    /* Device is valid and not previously initialized */
    lcdcfg.init = TRUE;

    /* Save and return address of peripheral block */
    lcdcfg.dptr = (LCD_PARAM_T *) arg;

    /* Disable LCD */
    LPC_LCD->CTRL &= ~CLCDC_LCDCTRL_ENABLE;

    /* If the passed argument is not NULL, then it is a pointer
       to a LCD panel parameters structure that needs to be configured */
    if (arg != 0)
    {
      lcd_initialize(&lcdcfg);
    }

    /* Return pointer to LCD configuration data structure */
    status = (int32_t) &lcdcfg;
  }
  return status;
}

/***********************************************************************
 *
 * Function: lcd_close
 *
 * Purpose: Close the LCD controller
 *
 * Processing:
 *     If init is not TRUE, then return _ERROR to the caller as the
 *     device was not previously opened. Otherwise, disable the LCD,
 *     set init to FALSE, and return _NO_ERROR to the caller.
 *
 * Parameters:
 *     devid: Pointer to LCD config structure
 *
 * Outputs: None
 *
 * Returns: The status of the close operation
 *
 **********************************************************************/
int32_t lcd_close(int32_t devid)
{
  CLCDC_CFG_T *lcdcfgptr = (CLCDC_CFG_T *) devid;
  int32_t status = (int32_t)(-1);

  if (lcdcfgptr->init == TRUE)
  {
    /* 'Uninitialize' device */
    lcdcfgptr->init = FALSE;
    status = (int32_t)(0);
  }
  return status;
}

/***********************************************************************
 *
 * Function: lcd_ioctl
 *
 * Purpose: LCD configuration block
 *
 * Processing:
 *     This function is a large case block. Based on the passed function
 *     and option values, set or get the appropriate LCD
 *     parameter.
 *
 * Parameters:
 *     devid: Pointer to LCD config structure
 *     cmd:   ioctl command
 *     arg:   ioctl argument
 *
 * Outputs: None
 *
 * Returns: The status of the ioctl operation
 *
 **********************************************************************/
int32_t lcd_ioctl(int32_t devid,
               int32_t cmd,
               int32_t arg)
{
  uint32_t i;
  CLCDC_CFG_T *lcdcfgptr = (CLCDC_CFG_T *) devid;
  int32_t status = (int32_t)(-1);
    
  PALETTE_ENTRY_T pal_entry, *ptr_pal_entry;
  uint8_t *pal_ptr;
  uint32_t *crsr_ptr;
    
  if (lcdcfgptr->init == TRUE)
  {
    status = (int32_t)(0);

    switch (cmd)
    {
      case LCD_SET_BUFFER:
        /* Set LCD display frame buffer, arg = memory pointer for
           image to be displayed */
        LPC_LCD->UPBASE = (uint32_t)arg;
        break;

      case LCD_CRSR_SET_BUFFER:
        /* Set LCD cursor image, arg = memory pointer for
           image to be displayed */
        crsr_ptr = (uint32_t *)arg;
        for(i = 0; i < 256; i++)
        {
          LPC_LCD->CRSR_IMG[i] = *(uint32_t *)crsr_ptr;
          crsr_ptr++;
        }
        break;

      case LCD_PWR_ON:
        /* Turn on the LCD controller power,
           arg = 1, turn on, arg = 0, turn off */
        if (arg == 1)
        {
          //LPC_GPIO0->SET = 0x18000000; /* P0.27, P0.28 output HIGH */
          LPC_LCD->CTRL |= CLCDC_LCDCTRL_ENABLE;
          LPC_LCD->CTRL |= CLCDC_LCDCTRL_PWR;
        }
        else if (arg == 0)
        {
          LPC_LCD->CTRL &= ~CLCDC_LCDCTRL_PWR;
          LPC_LCD->CTRL &= ~CLCDC_LCDCTRL_ENABLE;
          //LPC_GPIO0->CLR = 0x18000000; /* P0.27, P0.28 output LOW */
        }
        else
        {
          status = (int32_t)(-1);
        }
        break;

      case LCD_ENABLE:
        /* Enable the LCD controller,
           arg = 1, enable, arg = 0, disable */
        if (arg == 1)
        {
          LPC_LCD->CTRL |= CLCDC_LCDCTRL_ENABLE;
        }
        else if (arg == 0)
        {
          LPC_LCD->CTRL &= ~CLCDC_LCDCTRL_ENABLE;
        }
        else
        {
          status = (int32_t)(-1);
        }
        break;

      case LCD_CRSR_ENABLE:
        /* Enable the LCD cursor,
           arg = 1, enable, arg = 0, disable */
        if (arg == 1)
        {
          LPC_LCD->CRSR_CTRL |= 1;
        }
        else if (arg == 0)
        {
          LPC_LCD->CRSR_CTRL &= ~1;
        }
        else
        {
          status = (int32_t)(-1);
        }
        break;

      case LCD_CRSR_NUM:
        /* Select the LCD cursor number,
           arg = 0, 64x64, arg = 0, 1, 2, 3, 32x32 */
        LPC_LCD->CRSR_CTRL &= ~30;
        LPC_LCD->CRSR_CTRL |= (uint32_t)arg << 3;
        break;
                
      case LCD_CRSR_SIZE:
        /* Set the LCD cursor size,
           arg = 1, 64x64, arg = 0, 32x32 */
        if (arg == 1)
        {
          LPC_LCD->CRSR_CFG |= 1;
        }
        else if (arg == 0)
        {
          LPC_LCD->CRSR_CFG &= ~1;
        }
        else
        {
          status = (int32_t)(-1);
        }
        break;

      case LCD_CRSR_SYNC:
        /* Set the LCD cursor frame sync,
           arg = 1, sync, arg = 0, async */
        if (arg == 1)
        {
          LPC_LCD->CRSR_CFG |= 2;
        }
        else if (arg == 0)
        {
          LPC_LCD->CRSR_CFG &= ~2;
        }
        else
        {
          status = (int32_t)(-1);
        }
        break;
                
      case LCD_LOAD_PALETTE:
        /* This function supports loading of the color palette from
           the C file generated by the bmp2c utility. It expects the
           palette to be passed as an array of 32-bit BGR entries having
           the following format:
           7:3 - Blue
           2:0 - Not used
           15:11 - Green
           10:8 - Not used
           23:19 - Red
           18:16 - Not used
           31:24 - Not used
           arg = pointer to input palette table address */
        ptr_pal_entry = &pal_entry;
        pal_ptr = (uint8_t *) arg;

        /* 256 entry in the palette table */
        for(i = 0; i < 256/2; i++)
        {
          pal_entry.Bl = (*pal_ptr++) >> 3;  /* blue first */
          pal_entry.Gl = (*pal_ptr++) >> 3;  /* get green */
          pal_entry.Rl = (*pal_ptr++) >> 3;  /* get red */
          pal_ptr++;      /* skip over the unused byte */
          /* do the most significant halfword of the palette */
          pal_entry.Bu = (*pal_ptr++) >> 3;  /* blue first */
          pal_entry.Gu = (*pal_ptr++) >> 3;  /* get green */
          pal_entry.Ru = (*pal_ptr++) >> 3;  /* get red */
          pal_ptr++;      /* skip over the unused byte */

          LPC_LCD->PAL[i] = *(uint32_t *)ptr_pal_entry;
        }
        break;

      case LCD_CRSR_LOAD_PALETTE0:
        /* 7:0 - Red
           15:8 - Green
           23:16 - Blue
           31:24 - Not used*/
        LPC_LCD->CRSR_PAL0 = (uint32_t)arg;
        break;

      case LCD_CRSR_LOAD_PALETTE1:
        /* 7:0 - Red
           15:8 - Green
           23:16 - Blue
           31:24 - Not used*/
        LPC_LCD->CRSR_PAL1 = (uint32_t)arg;
        break;

      case LCD_SET_BPP:
        /* Set current display bits per pixel, arg = bits per pixel */
        switch (arg)
        {
          case BPP_IS_1:
            LPC_LCD->CTRL &= ~_SBF(1, _BITMASK(3));
            LPC_LCD->CTRL |= CLCDC_LCDCTRL_BPP1;
            break;
          
          case BPP_IS_2:
            LPC_LCD->CTRL &= ~_SBF(1, _BITMASK(3));
            LPC_LCD->CTRL |= CLCDC_LCDCTRL_BPP2;
            break;
          
          case BPP_IS_4:
            LPC_LCD->CTRL &= ~_SBF(1, _BITMASK(3));
            LPC_LCD->CTRL |= CLCDC_LCDCTRL_BPP4;
            break;
          
          case BPP_IS_8:
            LPC_LCD->CTRL &= ~_SBF(1, _BITMASK(3));
            LPC_LCD->CTRL |= CLCDC_LCDCTRL_BPP8;
            break;
          
          case BPP_IS_16:
            LPC_LCD->CTRL &= ~_SBF(1, _BITMASK(3));
            LPC_LCD->CTRL |= CLCDC_LCDCTRL_BPP16;
            break;
          
          case BPP_IS_24:
            LPC_LCD->CTRL &= ~_SBF(1, _BITMASK(3));
            LPC_LCD->CTRL |= CLCDC_LCDCTRL_BPP24;
            break;
          
          case BPP_IS_16_565_MODE:
            LPC_LCD->CTRL &= ~_SBF(1, _BITMASK(3));
            LPC_LCD->CTRL |= CLCDC_LCDCTRL_BPP16_565_MODE;
            break;
          
          case BPP_IS_12_444_MODE:
            LPC_LCD->CTRL &= ~_SBF(1, _BITMASK(3));
            LPC_LCD->CTRL |= CLCDC_LCDCTRL_BPP12_444_MODE;
            break;
          
          default:
            status = (int32_t)(-1);
            break;
        }
        break;

      case LCD_SET_BGR:
        /* LCD invert the bgr bit in the LCD controller. arg = 1, invert
           BGR for reverse color, arg = 0, BGR for normal color */
        if (arg == 1)
        {
          /*  
           *   Palette and direct color storage is 
           *   blue in bits 4-0,
           *   green in bits 9-5,
           *   red in bits 14-10,
           *   intensity in bit 15.
           */
          LPC_LCD->CTRL |= CLCDC_LCDCTRL_BGR;
        }
        else if (arg == 0)
        {
          /*
           *   Palette and direct color storage is 
           *   red in bits 4-0,
           *   green in bits 9-5,
           *   blue in bits 14-10,
           *   intensity in bit 15.
           */
          LPC_LCD->CTRL &= ~CLCDC_LCDCTRL_BGR;
        }
        else
        {
          status = (int32_t)(-1);
        }
        break;

      case LCD_SET_INTERRUPT:
        /* Set interrupt mask set/clear register */
        if (arg == CLCDC_LCD_INTERRUPT_FUF)
        {
          LPC_LCD->INTMSK |= CLCDC_LCD_INTERRUPT_FUF;
        }
        else if (arg == CLCDC_LCD_INTERRUPT_LNBU)
        {
          LPC_LCD->INTMSK |= CLCDC_LCD_INTERRUPT_LNBU;
        }
        else if (arg == CLCDC_LCD_INTERRUPT_VCOMP)
        {
          LPC_LCD->INTMSK |= CLCDC_LCD_INTERRUPT_VCOMP;
        }
        else if (arg == CLCDC_LCD_INTERRUPT_MBERROR)
        {
          LPC_LCD->INTMSK |= CLCDC_LCD_INTERRUPT_MBERROR;
        }
        else
        {
          status = (int32_t)(-1);
        }
        break;

      case LCD_CLR_INTERRUPT:
        /* Set interrupt clear register*/
        if (arg == CLCDC_LCD_INTERRUPT_FUF)
        {
          LPC_LCD->INTCLR |= CLCDC_LCD_INTERRUPT_FUF;
        }
        else if (arg == CLCDC_LCD_INTERRUPT_LNBU)
        {
          LPC_LCD->INTCLR |= CLCDC_LCD_INTERRUPT_LNBU;
        }
        else if (arg == CLCDC_LCD_INTERRUPT_VCOMP)
        {
          LPC_LCD->INTCLR |= CLCDC_LCD_INTERRUPT_VCOMP;
        }
        else if (arg == CLCDC_LCD_INTERRUPT_MBERROR)
        {
          LPC_LCD->INTCLR |= CLCDC_LCD_INTERRUPT_MBERROR;
        }
        else
        {
          status = (int32_t)(-1);
        }
        break;

      case LCD_CRSR_SET_INTERRUPT:
        /* Set cursor interrupt mask set/clear register */
        LPC_LCD->CRSR_INTMSK |= 1;
        break;

      case LCD_CRSR_CLR_INTERRUPT:
        /* Set cursor interrupt clear register*/
        LPC_LCD->CRSR_INTCLR |= 1;
        break;

      case LCD_CRSR_XY:
        /* Set lcd cursor xy position register*/
        LPC_LCD->CRSR_XY = (uint32_t)arg;
        break;

      case LCD_CRSR_CLIP:
        /* Set lcd cursor clip position register*/
        LPC_LCD->CRSR_CLIP = (uint32_t)arg;
        break;

      case LCD_GET_STATUS:
        switch (arg)
        {
          case LCD_GET_BUFFER:
            /* Returns LCD fram buffer address */
            status = (int32_t)LPC_LCD->UPBASE;
            break;
                        
          case LCD_GET_BPP:
            /* Returns current LCD panel bit per pixel */
            i = (LPC_LCD->CTRL) & _SBF(1,_BITMASK(3) );
            switch (i)
            {
              case CLCDC_LCDCTRL_BPP1:
                status = BPP_IS_1;
                break;
              
              case CLCDC_LCDCTRL_BPP2:
                status = BPP_IS_2;
                break;
              
              case CLCDC_LCDCTRL_BPP4:
                status = BPP_IS_4;
                break;
              
              case CLCDC_LCDCTRL_BPP8:
                status = BPP_IS_8;
                break;
              
              case CLCDC_LCDCTRL_BPP16:
                status = BPP_IS_16;
                break;
              
              case CLCDC_LCDCTRL_BPP24:
                status = BPP_IS_24;
                break;
              
              case CLCDC_LCDCTRL_BPP16_565_MODE:
                status = BPP_IS_16_565_MODE;
                break;
              
              case CLCDC_LCDCTRL_BPP12_444_MODE:
                status = BPP_IS_12_444_MODE;
                break;
              
              default:
                status = (int32_t)(-1);
                break;
            }
            break;
                        
          case LCD_GET_PALETTE:
            /* Returns a pointer to palette table */
            status = (int32_t)LPC_LCD->PAL;
            break;

          case LCD_CRSR_GET_PALETTE0:
            /* Returns a pointer to cursor palette table */
            status = (int32_t)LPC_LCD->CRSR_PAL0;
            break;

          case LCD_CRSR_GET_PALETTE1:
            /* Returns a pointer to cursor palette table */
            status = (int32_t)LPC_LCD->CRSR_PAL1;
            break;

          case LCD_GET_INTERRUPT:
            /* Get interrupt mask sstatus register */
            status = (int32_t)LPC_LCD->INTSTAT;
            break;

          case LCD_CRSR_GET_INTERRUPT:
            /* Get cursor interrupt mask sstatus register */
            status = (int32_t)LPC_LCD->CRSR_INTSTAT;
            break;

          default:
            /* Unsupported parameter */
            status = (int32_t)(-1);
            break;
        }
        break;

      default:
        /* Unsupported parameter */
        status = (int32_t)(-1);
        break;
    }
  }
  return status;
}
