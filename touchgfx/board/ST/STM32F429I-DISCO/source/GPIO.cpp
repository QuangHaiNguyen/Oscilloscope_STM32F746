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

#include "stm32f429i_discovery.h"
using namespace touchgfx;

static bool gpioState[4];
const uint16_t GPIO_PIN[4] = { GPIO_Pin_2, GPIO_Pin_9, GPIO_Pin_13, GPIO_Pin_14 };

//    VSYNC_FREQ, /// Pin is toggled at each VSYNC
//    RENDER_TIME, /// Pin is high when frame rendering begins, low when finished
//    FRAME_RATE, /// Pin is toggled when the frame buffers are swapped.
//    MCU_ACTIVE /// Pin is high when the MCU is doing work (ie. not in idle task).


void GPIO::init()
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    /* Enable the GPIO_LED Clock */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);

    /* Configure the GPIO_LED pin */
    GPIO_InitStructure.GPIO_Pin = GPIO_PIN[VSYNC_FREQ] | GPIO_PIN[RENDER_TIME] | GPIO_PIN[FRAME_RATE] | GPIO_PIN[MCU_ACTIVE];
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOG, &GPIO_InitStructure);

    for (int i = 0; i < 4; i++)
    {
        gpioState[i] = 0;
    }
}

void GPIO::set(GPIO_ID id)
{
    GPIOG->BSRRL = GPIO_PIN[id];
    gpioState[id] = 1;
}

void GPIO::clear(GPIO_ID id)
{
    GPIOG->BSRRH = GPIO_PIN[id];
    gpioState[id] = 0;
}

void GPIO::toggle(GPIO_ID id)
{
    if (get(id))
    {
        clear(id);
    }
    else
    {
        set(id);
    }
}

bool GPIO::get(GPIO_ID id)
{
    return gpioState[id];
}
