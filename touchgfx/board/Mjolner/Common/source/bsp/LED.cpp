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
#include <touchgfx/hal/LED.hpp>
#include "io_config.h"
#include "led_config.h"

#include "lpc43xx_gpio.h"
#include "lpc43xx_scu.h"

using namespace touchgfx;

volatile static bool ledState[5] = {false, false, false, false, false};

void LED::init()
{
    scu_pinmux(MLED_RED_IO,       MD_BUK, FUNC4);
    scu_pinmux(MLED_YELLOW_IO,    MD_BUK, FUNC4);
    scu_pinmux(MLED_GREEN_IO,     MD_BUK, FUNC4);
    scu_pinmux(SLED_RED_IO,       MD_BUK, FUNC4);
    scu_pinmux(SLED_GREEN_IO,     MD_BUK, FUNC4);

    //Set direction for all LED GPIO
    GPIO_SetDir(7, 0xDC00, IO_OUTPUT);

    //Ensure all leds off
    LED::off(M4_RED);
    LED::off(M4_YELLOW);
    LED::off(M4_GREEN);
    LED::off(M0_RED);
    LED::off(M0_GREEN);
}

void LED::on(uint8_t id)
{
    if (id <= NR_OF_LEDS)
    {
        switch (id)
        {
        case M4_RED:
            GPIO_ClearValue(7, 0x400);
            break;
        case M4_GREEN:
            GPIO_ClearValue(7, 0x800);
            break;
        case M4_YELLOW:
            GPIO_ClearValue(7, 0x1000);
            break;
        case M0_RED:
            GPIO_ClearValue(7, 0x4000);
            break;
        case M0_GREEN:
            GPIO_ClearValue(7, 0x8000);
            break;
        }

        ledState[id] = true;
    }
}

void LED::toggle(uint8_t id)
{
    if (get(id))
    {
        off(id);
    }
    else
    {
        on(id);
    }
}

bool LED::get(uint8_t id)
{
    return ledState[id];
}

void LED::off(uint8_t id)
{
    if (id <= NR_OF_LEDS)
    {
        switch (id)
        {
        case M4_RED:
            GPIO_SetValue(7, 0x400);
            break;
        case M4_GREEN:
            GPIO_SetValue(7, 0x800);
            break;
        case M4_YELLOW:
            GPIO_SetValue(7, 0x1000);
            break;
        case M0_RED:
            GPIO_SetValue(7, 0x4000);
            break;
        case M0_GREEN:
            GPIO_SetValue(7, 0x8000);
            break;
        }

        ledState[id] = false;
    }
}
