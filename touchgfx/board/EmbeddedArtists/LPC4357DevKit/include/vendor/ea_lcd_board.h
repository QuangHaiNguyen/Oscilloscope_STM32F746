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
#ifndef __EA_DISPLAY_BOARD_H
#define __EA_DISPLAY_BOARD_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lcd_params.h"

typedef enum
{
  LCDB_RESULT_OK,              // OK
  LCDB_RESULT_INVALID_CMD_STR, // invalid command string (init or power down)
  LCDB_RESULT_INVALID_ARG,     // invalid argument to a function
  LCD_RESULT_STORAGE,          // invalid data in storage (lcd parameters, ...)
  LCD_RESULT_BUF_SMALL,        // too small buffer was used
  LCD_RESULT_SEQ_VER,          // the sequence string version isn't supported
} lcdb_result_t;

typedef struct {
  uint8_t swap;        // set to 1 if x and y should be swapped
  uint32_t xres;       // x resistance
  uint32_t yres;       // y resistance
} eaLcdbTouchParams_t;

typedef enum
{
  OSC_25_175MHZ,
  OSC_27MHZ,
  OSC_36MHZ,
  OSC_USER_DEFINED,
} osc_e;

lcdb_result_t ea_lcdb_open(LCD_PARAM_T* lcdParams, char* initSeqStr, int32_t* pDev);
lcdb_result_t ea_lcdb_enable(int32_t dev, int32_t framebuf);
lcdb_result_t ea_lcdb_close(int32_t dev);

void ea_lcdb_ctrl_3v3(uint32_t enable);
void ea_lcdb_ctrl_5v(uint32_t enable);
void ea_lcdb_ctrl_lvds_pwr(uint32_t enable);
lcdb_result_t ea_lcdb_ctrl_selectOscillator(osc_e oscillator);
void ea_lcdb_ctrl_display(uint32_t enable);
void ea_lcdb_ctrl_backlightContrast(uint32_t value);

lcdb_result_t ea_lcdb_store_params(
  char* lcdName,
  char* lcdMfg,
  LCD_PARAM_T* lcdParams, 
  char* initSeqStr, 
  char* powerDownSeqStr,
  eaLcdbTouchParams_t* touch);

lcdb_result_t ea_lcdb_getLcdParams(LCD_PARAM_T* lcdParams);
lcdb_result_t ea_lcdb_getInitSeq(char* buf, uint32_t len); 
lcdb_result_t ea_lcdb_getPowerDownSeq(char* buf, uint32_t len);
lcdb_result_t ea_lcdb_getTouchParams(eaLcdbTouchParams_t* touchParams);
lcdb_result_t ea_lcdb_getDisplayName(char* buf, uint32_t len); 
lcdb_result_t ea_lcdb_getDisplayMfg(char* buf, uint32_t len);

#ifdef __cplusplus
}
#endif

#endif /* end __EA_DISPLAY_BOARD_H */
/****************************************************************************
**                            End Of File
*****************************************************************************/
