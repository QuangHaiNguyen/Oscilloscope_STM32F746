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
#ifndef __TRULY_DISPLAY_H
#define __TRULY_DISPLAY_H

#define DISPLAY_WIDTH  240
#define DISPLAY_HEIGHT 320

int32_t truly_lcd_open (int32_t framebuf);
int32_t truly_lcd_close (int32_t devid);
void truly_lcd_setBacklight (uint8_t percent);



#endif /* end __TRULY_DISPLAY_H */
/****************************************************************************
**                            End Of File
*****************************************************************************/
