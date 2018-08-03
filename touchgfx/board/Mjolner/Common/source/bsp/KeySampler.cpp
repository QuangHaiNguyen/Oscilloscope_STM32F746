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
#include "KeySampler.hpp"
#include "touchgfx/hal/Buttons.hpp"

using namespace touchgfx;

static int btnmax = 3;
static uint8_t btnstatus[5];

void KeySampler::init()
{
    Buttons::init();

    btnstatus[0] = 0;
    btnstatus[1] = 0;
    btnstatus[2] = 0;
    btnstatus[3] = 0;
    btnstatus[4] = 0;
}

bool KeySampler::sample(uint8_t& key)
{
    unsigned int buttons = Buttons::sample();

    //Test for button 1
    if (buttons & 0x1)
    {
        btnstatus[0]++;
        if (btnstatus[0] == btnmax)
        {
            key = 1;
            return true;
        }
    }
    else
    {
        btnstatus[0] = 0;
    }

    //Test for button 2
    if (buttons & 0x2)
    {
        btnstatus[1]++;
        if (btnstatus[1] == btnmax)
        {
            key = 2;
            return true;
        }
    }
    else
    {
        btnstatus[1] = 0;
    }

    //Test for button 3
    if (buttons & 0x4)
    {
        btnstatus[2]++;
        if (btnstatus[2] == btnmax)
        {
            key = 3;
            return true;
        }
    }
    else
    {
        btnstatus[2] = 0;
    }

    //Test for button 4
    if (buttons & 0x8)
    {
        btnstatus[3]++;
        if (btnstatus[3] == btnmax)
        {
            key = 4;
            return true;
        }
    }
    else
    {
        btnstatus[3] = 0;
    }

    //Test for button 5
    if (buttons & 0x10)
    {
        btnstatus[4]++;
        if (btnstatus[4] == btnmax)
        {
            key = 5;
            return true;
        }
    }
    else
    {
        btnstatus[4] = 0;
    }

    return false;
}
