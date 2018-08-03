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
#include <touchgfx/hal/Buttons.hpp>
#include <touchgfx/hal/LED.hpp>
#include "io_config.h"

#include "lpc43xx_gpio.h"
#include "lpc43xx_scu.h"

using namespace touchgfx;

void Buttons::init()
{
    scu_pinmux(SWIN_1_IO,   INBUF_ENABLE, FUNC0);
    scu_pinmux(SWIN_2_IO,   INBUF_ENABLE, FUNC0);
    scu_pinmux(SWIN_3_IO,   INBUF_ENABLE, FUNC0);
    scu_pinmux(SWIN_4_IO,   INBUF_ENABLE, FUNC0);
    scu_pinmux(SWIN_5_IO,   INBUF_ENABLE, FUNC4);

    GPIO_SetDir(4, 0xF000,  IO_INPUT);
    GPIO_SetDir(5, 0x10000, IO_INPUT);
}

unsigned int Buttons::sample()
{
    unsigned int result = 0;

    uint32_t buttonsPressed_p4 = GPIO_ReadValue(4);
    uint32_t buttonsPressed_p5 = GPIO_ReadValue(5);

    if (buttonsPressed_p4 & (1 << 12))
    {
        result |= 0x1;
    }
    else if (buttonsPressed_p4 & (1 << 13))
    {
        result |= 0x2;
    }
    else if (buttonsPressed_p4 & (1 << 14))
    {
        result |= 0x4;
    }
    else if (buttonsPressed_p4 & (1 << 15))
    {
        result |= 0x8;
    }
    else if (buttonsPressed_p5 & (1 << 17))
    {
        result |= 0x10;
    }

    return result;
}


