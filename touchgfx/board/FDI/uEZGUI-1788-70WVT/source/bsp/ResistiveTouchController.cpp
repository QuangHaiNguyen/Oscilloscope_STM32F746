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
#include <bsp/ResistiveTouchController.hpp>
#include <touchgfx/transforms/TouchCalibration.hpp>

using namespace touchgfx;

#include "vendor/FDI_Touch.hpp"

static  Point  lcdCalibPoints[3] =  /* Display size is 800x480 */
{
    {  80,  48 },     /* 10%, 10% */
    { 720, 240 },     /* 90%, 50% */
    { 400, 432 },     /* 50%, 90% */
};

//calibration ADC 12 bit values
static  Point  touchCalibPoints[3] =
{
    { 581, 4096 - 3128 },       /* 10%, 10% */
    { 3475, 4096 - 2025 },     /* 90%, 50% */
    { 2001, 4096 - 978 },     /* 50%, 90% */
};

void ResistiveTouchController::init()
{
    TouchCalibration::setCalibrationMatrix(lcdCalibPoints, touchCalibPoints);
}

static FDI_TOUCH_STATE touchState = TOUCH_RELEASED;

bool ResistiveTouchController::sampleTouch(int32_t& x, int32_t& y)
{
    if (FDI_Touch_HasTouch((touchState == TOUCH_PRESSED)))
    {
        touchState = TOUCH_PRESSED;
        Point coords;
        FDI_Touch_ReadXY(coords.x, coords.y);
        coords.y = 4096 - coords.y;

        x = coords.x;
        y = coords.y;
        return true;
    }
    else
    {
        touchState = TOUCH_RELEASED;
        return false;
    }
}
