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
#ifndef IO_CONFIG_H
#define IO_CONFIG_H

#define IO_OUTPUT 1
#define IO_INPUT  0

typedef enum
{
    PORT_1 = 1,
    PORT_2 = 2,
    PORT_3 = 3,
    PORT_4 = 4,
    PORT_5 = 5,
    PORT_6 = 6,
    PORT_7 = 7,
    PORT_8 = 8,
    PORT_9 = 9,
    PORT_A = 10,
    PORT_B = 11,
    PORT_C = 12,
    PORT_D = 13,
    PORT_E = 14,
    PORT_F = 15,
}LPC_PORT_t;

#define P7_4  7,4
#define P7_5  7,5

#define P9_0  9,0
#define P9_1  9,1
#define P9_2  9,2
#define P9_3  9,3
#define P9_4  9,4

#define PD_13 13,13
#define PD_14 13,14
#define PD_15 13,15
#define PD_16 13,16

#define PE_1  14,1
#define PE_5  14,5
#define PE_6  14,6
#define PE_7  14,7
#define PE_8  14,8
#define PE_10 14,10
#define PE_11 14,11
#define PE_12 14,12
#define PE_14 14,14
#define PE_15 14,15

#define TOUCH_INT_PIN_IO PE_1

#define BACKLIGHT_3_5_IO PE_5
#define BACKLIGHT_4_3_IO P7_4
#define BACKLIGHT_5_7_IO P7_5
#define BACKLIGHT_7_IO   PE_8

#define MLED_RED_IO     PE_10
#define MLED_YELLOW_IO  PE_11
#define MLED_GREEN_IO   PE_12
#define SLED_RED_IO     PE_14
#define SLED_GREEN_IO   PE_15

#define SWIN_1_IO       P9_0
#define SWIN_2_IO       P9_1
#define SWIN_3_IO       P9_2
#define SWIN_4_IO       P9_3
#define SWIN_5_IO       P9_4

#define LCD_RESB_IO     PORT_D,3
#define LCD_TB_IO       PORT_D,4
#define LCD_REV_IO      PORT_D,5
#define LCD_RL_IO       PORT_D,6
#define LCD_BGR_IO      PORT_D,7
#define LCD_SEL0_IO     PORT_D,8
#define LCD_SEL1_IO     PORT_D,9
#define LCD_SEL2_IO     PORT_D,10
#define LCD_CPE_IO      PORT_D,11
#define LCD_PINV_IO     PORT_D,12

#define LCD_RESB        6,17
#define LCD_TB          6,18
#define LCD_REV         6,19
#define LCD_RL          6,20
#define LCD_BGR         6,21
#define LCD_SEL0        6,22
#define LCD_SEL1        6,23
#define LCD_SEL2        6,24
#define LCD_CPE         6,25
#define LCD_PINV        6,26

#define ID_1_IO         PD_13
#define ID_2_IO         PD_14
#define ID_3_IO         PD_15
#define ID_4_IO         PD_16

#define TOUCH_INT_PIN    7,1

#define BACKLIGHT_3_5   7,5
#define BACKLIGHT_4_3   3,12
#define BACKLIGHT_5_7   3,13
#define BACKLIGHT_7     5,19

#define MLED_RED        7,10
#define MLED_YELLOW     7,11
#define MLED_GREEN      7,12
#define SLED_RED        7,14
#define SLED_GREEN      7,15

#define MLED_RED_CONF           7,0x400
#define MLED_YELLOW_CONF        7,0x800
#define MLED_GREEN_CONF         7,0x1000
#define SLED_RED_CONF           7,0x4000
#define SLED_GREEN_CONF         7,0x8000

#define SWIN_1          4,12
#define SWIN_2          4,13
#define SWIN_3          4,14
#define SWIN_4          4,15
#define SWIN_5          5,17

#define ID_1            6,27
#define ID_2            6,28
#define ID_3            6,29
#define ID_4            6,30

#endif // IO_CONFIG_H
