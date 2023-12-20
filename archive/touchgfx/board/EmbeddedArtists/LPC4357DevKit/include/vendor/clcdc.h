/*****************************************************************************
 *   clcdc.h:  Header file for NXP LPC177x_8x Family Microprocessors
 *
 *   Copyright(C) 2009, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2009.05.26  ver 1.00    Prelimnary version, first Release
 *
******************************************************************************/
#ifndef __CLCDC_H
#define __CLCDC_H

/***********************************************************************
 * _BIT(p) sets the bit at position "p"
 **********************************************************************/
#undef _BIT
#define _BIT(p) (((uint32_t)(1)) << (p))

/***********************************************************************
 * _BITMASK constructs a symbol with 'field_width' least significant bits set.
 **********************************************************************/
#undef _BITMASK
#define _BITMASK(field_width) (_BIT(field_width) - 1)

/***********************************************************************
 * _SBF(p,v) sets the bit field starting at position "p" to value "v". 
 **********************************************************************/
#undef _SBF
#define _SBF(p,v) (((uint32_t)(v)) << (p))

/***********************************************************************
 * Color LCD controller horizontal axis plane control register definitions
 **********************************************************************/

/* LCD controller horizontal axis plane control register pixels per line */
#define CLCDC_LCDTIMING0_PPL_WIDTH 6
#define CLCDC_LCDTIMING0_PPL(n) _SBF(2, (((n) / 16) - 1) & _BITMASK(CLCDC_LCDTIMING0_PPL_WIDTH))
/* LCD controller horizontal axis plane control register HSYNC pulse width */
#define CLCDC_LCDTIMING0_HSW_WIDTH 8
#define CLCDC_LCDTIMING0_HSW(n) _SBF(8, ((n) - 1) & _BITMASK(CLCDC_LCDTIMING0_HSW_WIDTH))
/* LCD controller horizontal axis plane control register horizontal front porch */
#define CLCDC_LCDTIMING0_HFP_WIDTH 8
#define CLCDC_LCDTIMING0_HFP(n) _SBF(16, ((n) - 1) & _BITMASK(CLCDC_LCDTIMING0_HFP_WIDTH))
/* LCD controller horizontal axis plane control register horizontal back porch */
#define CLCDC_LCDTIMING0_HBP_WIDTH 8
#define CLCDC_LCDTIMING0_HBP(n) _SBF(24, ((n) - 1) & _BITMASK(CLCDC_LCDTIMING0_HBP_WIDTH))

/***********************************************************************
 * Color LCD controller vertical axis plane control register definitions
 **********************************************************************/

/* LCD controller vertical axis plane control register lines per panel */
#define CLCDC_LCDTIMING1_LPP_WIDTH 10
#define CLCDC_LCDTIMING1_LPP(n) _SBF(0, ((n) - 1) & _BITMASK(CLCDC_LCDTIMING1_LPP_WIDTH))
/* LCD controller vertical axis plane control register VSYNC pulse width */
#define CLCDC_LCDTIMING1_VSW_WIDTH 6
#define CLCDC_LCDTIMING1_VSW(n) _SBF(10, ((n) - 1) & _BITMASK(CLCDC_LCDTIMING1_VSW_WIDTH))
/* LCD controller vertical axis plane control register vertical front porch */
#define CLCDC_LCDTIMING1_VFP_WIDTH 8 
#define CLCDC_LCDTIMING1_VFP(n) _SBF(16, (n) & _BITMASK(CLCDC_LCDTIMING1_VFP_WIDTH))
/* LCD controller vertical axis plane control register vertical back porch */
#define CLCDC_LCDTIMING1_VBP_WIDTH 8
#define CLCDC_LCDTIMING1_VBP(n) _SBF(24, (n) & _BITMASK(CLCDC_LCDTIMING1_VBP_WIDTH))

/***********************************************************************
 * Color LCD controller clock and signal polarity control register definitions
 **********************************************************************/

/* LCD controller clock and signal polarity control register panel clock divisor low*/
#define CLCDC_LCDTIMING2_PCD_LO_WIDTH 5
#define CLCDC_LCDTIMING2_PCD_LO(n) _SBF(0, ((n) - 2) & _BITMASK(CLCDC_LCDTIMING2_PCD_LO_WIDTH))
/* LCD controller clock and signal polarity control register clock select */
#define CLCDC_LCDTIMING2_CLKSEL _BIT(5)
/* LCD controller clock and signal polarity control register AC bias pin frequency */
#define CLCDC_LCDTIMING2_ACB_WIDTH 5
#define CLCDC_LCDTIMING2_ACB(n) _SBF(6, ((n) - 1) & _BITMASK(CLCDC_LCDTIMING2_ACB_WIDTH))
/* LCD controller clock and signal polarity control register invert VSYNC */
#define CLCDC_LCDTIMING2_IVS    _BIT(11)
/* LCD controller clock and signal polarity control register invert HSYNC */
#define CLCDC_LCDTIMING2_IHS    _BIT(12)
/* LCD controller clock and signal polarity control register invert plane clock */
#define CLCDC_LCDTIMING2_IPC    _BIT(13)
/* LCD controller clock and signal polarity control register invert output enable */
#define CLCDC_LCDTIMING2_IOE    _BIT(14)
/* LCD controller clock and signal polarity control register clocks per line */
#define CLCDC_LCDTIMING2_CPL_WIDTH 10
#define CLCDC_LCDTIMING2_CPL(n) _SBF(16, (n) & _BITMASK(CLCDC_LCDTIMING2_CPL_WIDTH))
/* LCD controller clock and signal polarity control register bypass pixel clock divider */
#define CLCDC_LCDTIMING2_BCD    _BIT(26)
/* LCD controller clock and signal polarity control register panel clock divisor high*/
#define CLCDC_LCDTIMING2_PCD_HI_WIDTH 5
#define CLCDC_LCDTIMING2_PCD_HI(n) _SBF((27 - CLCDC_LCDTIMING2_PCD_LO_WIDTH), ((n) - 2) & _SBF(CLCDC_LCDTIMING2_PCD_LO_WIDTH, _BITMASK(CLCDC_LCDTIMING2_PCD_HI_WIDTH)))

/**********************************************************************
 * Color LCD Controller line end control register definitions
 *********************************************************************/ 

/* Line End Signal Delay */ 
#define CLCDC_LCDTIMING3_LED_WIDTH 7
#define CLCDC_LCDTIMING3_LED(n) _SBF(0, ((n) - 1) & _BITMASK(CLCDC_LCDTIMING3_LED_WIDTH))
/* Line End Enable */ 
#define CLCDC_LCDTIMING3_LEE    _BIT(16)

/***********************************************************************
 * Color LCD controller interrupt mask set/clear register definitions
 * Color LCD controller raw interrupt status register definitions
 * Color LCD controller masked interrupt status register definitions
 * Color LCD controller interrupt clear register definitions
 **********************************************************************/

/* FIFO underflow bit */
#define CLCDC_LCD_INTERRUPT_FUF     _BIT(1)
/* LCD next base address update bit */
#define CLCDC_LCD_INTERRUPT_LNBU        _BIT(2)
/* vertical compare bit */
#define CLCDC_LCD_INTERRUPT_VCOMP       _BIT(3)
/* AHB master error interrupt bit */
#define CLCDC_LCD_INTERRUPT_MBERROR     _BIT(4)

/***********************************************************************
 * Color LCD controller control register definitions
 **********************************************************************/

/* LCD control enable bit */
#define CLCDC_LCDCTRL_ENABLE    _BIT(0)
/* LCD control 1 bit per pixel bit field */ 
#define CLCDC_LCDCTRL_BPP1      _SBF(1, 0)
/* LCD control 2 bits per pixel bit field */ 
#define CLCDC_LCDCTRL_BPP2      _SBF(1, 1)
/* LCD control 4 bits per pixel bit field */ 
#define CLCDC_LCDCTRL_BPP4      _SBF(1, 2)
/* LCD control 8 bits per pixel bit field */ 
#define CLCDC_LCDCTRL_BPP8      _SBF(1, 3)
/* LCD control 16 bits per pixel bit field */ 
#define CLCDC_LCDCTRL_BPP16     _SBF(1, 4)
/* LCD control 24 bits per pixel bit field */ 
#define CLCDC_LCDCTRL_BPP24     _SBF(1, 5)
/* LCD control 16 bits (5:6:5 mode) per pixel bit field */ 
#define CLCDC_LCDCTRL_BPP16_565_MODE _SBF(1, 6)
/* LCD control 12 bits (4:4:4 mode) per pixel bit field */ 
#define CLCDC_LCDCTRL_BPP12_444_MODE _SBF(1, 7)
/* LCD control mono select bit */
#define CLCDC_LCDCTRL_BW_COLOR  _SBF(4, 0)
#define CLCDC_LCDCTRL_BW_MONO   _SBF(4, 1)
/* LCD controler TFT select bit */
#define CLCDC_LCDCTRL_TFT       _BIT(5)
/* LCD control monochrome LCD has 4-bit/8-bit select bit */ 
#define CLCDC_LCDCTRL_MON8      _BIT(6)
/* LCD control dual panel select bit */
#define CLCDC_LCDCTRL_DUAL      _BIT(7)
/* LCD control RGB or BGR format select bit */
#define CLCDC_LCDCTRL_RGB       _SBF(8, 0)
#define CLCDC_LCDCTRL_BGR       _SBF(8, 1)
/* LCD control big-endian byte order select bit */
#define CLCDC_LCDCTRL_BEBO      _BIT(9)
/* LCD control big-endian pixel order within a byte select bit */
#define CLCDC_LCDCTRL_BEPO      _BIT(10)
/* LCD control power enable bit */ 
#define CLCDC_LCDCTRL_PWR       _BIT(11)
/* LCD control VCOMP interrupt is start of VSYNC */
#define CLCDC_LCDCTRL_VCOMP_VS  _SBF(12, 0)
/* LCD control VCOMP interrupt is start of back porch */
#define CLCDC_LCDCTRL_VCOMP_BP  _SBF(12, 1)
/* LCD control VCOMP interrupt is start of active video */
#define CLCDC_LCDCTRL_VCOMP_AV  _SBF(12, 2)
/* LCD control VCOMP interrupt is start of front porch */
#define CLCDC_LCDCTRL_VCOMP_FP  _SBF(12, 3)
/* LCD control watermark level is 8 or more words free bit */
#define CLCDC_LCDCTRL_WATERMARK _BIT(16)

#endif /* __CLCDC_H */ 
