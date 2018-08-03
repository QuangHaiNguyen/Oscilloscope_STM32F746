/*****************************************************************************
 *   lpc_params.h:  Header file for NXP LPC177x_8x Family Microprocessors
 *
 *   Copyright(C) 2009, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2009.05.26  ver 1.00    Prelimnary version, first Release
 *
******************************************************************************/
#ifndef __LCD_PARAM_H
#define __LCD_PARAM_H

#include "lpc_types.h"

/* LCD display types */
typedef enum {
  TFT = 0,   /* standard TFT */
  ADTFT,     /* advanced TFT */
  HRTFT,     /* highly reflective TFT */
  MONO_4BIT, /* 4-bit mono */
  MONO_8BIT, /* 8-bit mono */
  CSTN       /* color STN */
} LCD_PANEL_T;

/* Structure containing the parameters for the LCD panel */
typedef struct {
  uint8_t         h_back_porch;         /* Horizontal back porch in clocks */
  uint8_t         h_front_porch;        /* Horizontal front porch in clocks */
  uint8_t         h_sync_pulse_width;   /* HSYNC pulse width in clocks */
  uint16_t        pixels_per_line;      /* Pixels per line (horizontal resolution) */
  uint8_t         v_back_porch;         /* Vertical back porch in clocks */
  uint8_t         v_front_porch;        /* Vertical front porch in clocks */
  uint8_t         v_sync_pulse_width;   /* VSYNC pulse width in clocks */
  uint16_t        lines_per_panel;      /* Lines per panel (vertical resolution) */
  uint8_t         invert_output_enable; /* Invert output enable, 1 = invert*/
  uint8_t         invert_panel_clock;   /* Invert panel clock, 1 = invert*/
  uint8_t         invert_hsync;         /* Invert HSYNC, 1 = invert */
  uint8_t         invert_vsync;         /* Invert VSYNC, 1 = invert */
  uint8_t         ac_bias_frequency;    /* AC bias frequency in clocks */
  uint8_t         bits_per_pixel;       /* Maximum bits per pixel the display supports */
  uint32_t        optimal_clock;        /* Optimal clock rate (Hz), or 0 to use external clock in */
  LCD_PANEL_T     lcd_panel_type;       /* LCD panel type */
  uint8_t         dual_panel;           /* Dual panel, 1 = dual panel display */
} LCD_PARAM_T;


#endif /* __LCD_PARAM_H */
