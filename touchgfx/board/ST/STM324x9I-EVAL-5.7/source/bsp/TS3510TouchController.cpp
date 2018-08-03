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
#include <bsp/TS3510TouchController.hpp>
#include <touchgfx/transforms/TouchCalibration.hpp>
#include <touchgfx/Utils.hpp>

using namespace touchgfx;

extern "C"
{
    uint8_t IOE_Config(void);
    uint8_t LCD_GetPosition(uint8_t* ReadBuffer);
}
void TS3510TouchController::init()
{
    IOE_Config();
}

bool TS3510TouchController::sampleTouch(int32_t& x, int32_t& y)
{
    uint8_t tbuf[20];
    static int lastX = 0;
    static int lastY = 0;

    if (!LCD_GetPosition(tbuf))
    {
        if (tbuf[1] == 0xFF && tbuf[2] == 0xFF && tbuf[3] == 0xFF && tbuf[4] == 0xFF)
        {
            return false;
        }
        else
        {
            int tmpX = tbuf[1] << 8 | tbuf[2];
            int tmpY = tbuf[3] << 8 | tbuf[4];

            //Filter
            if ((abs(tmpX - lastX) > 2) || (abs(tmpY - lastY) > 2))
            {
                x = tmpX;
                y = tmpY;
                lastX = x;
                lastY = y;
            }
            else
            {
                x = lastX;
                y = lastY;
            }

            return true;
        }
    }
    return false;
}
