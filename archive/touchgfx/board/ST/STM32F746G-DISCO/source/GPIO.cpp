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
#include <touchgfx/hal/GPIO.hpp>

#include "stm32746g_discovery.h"
using namespace touchgfx;

static bool ledState;

// The STM32746G-Discovery board only supports a single LED.
// This GPIO utility links that LED to GPIO::RENDER_TIME.

static bool isRenderTime(GPIO::GPIO_ID id)
{
    return id == GPIO::RENDER_TIME;
}

void GPIO::init()
{
    BSP_LED_Init(LED_GREEN);
    clear(GPIO::RENDER_TIME);
}

void GPIO::set(GPIO::GPIO_ID id)
{
    if (isRenderTime(id))
    {
        ledState = 1;
        BSP_LED_On(LED_GREEN);
    }
}

void GPIO::clear(GPIO::GPIO_ID id)
{
    if (isRenderTime(id))
    {
        ledState = 0;
        BSP_LED_Off(LED_GREEN);
    }
}

void GPIO::toggle(GPIO::GPIO_ID id)
{
    if (isRenderTime(id))
    {
        BSP_LED_Toggle(LED_GREEN);
    }
}

bool GPIO::get(GPIO::GPIO_ID id)
{
    if (isRenderTime(id))
    {
        return ledState;
    }
    return 0;
}
