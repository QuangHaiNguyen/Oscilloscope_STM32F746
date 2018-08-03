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
#include <bsp/TSC2046.hpp>
#include <touchgfx/transforms/TouchCalibration.hpp>
using namespace touchgfx;

#ifndef PERFORM_TOUCH_CALIBRATION
//calibration pixel points
static  Point  lcdCalibPoints[3] =  /* Display size is 480x272 */
{
    {  48,  27 },     /* 10%, 10% */
    { 432, 136 },     /* 90%, 50% */
    { 240, 245 },     /* 50%, 90% */
};

//calibration ADC 12 bit values
static  Point  touchCalibPoints[3] =
{
    { 543, 3377 },         /* 10%, 10% */
    { 3669, 1903 },       /* 90%, 50% */
    { 2122, 480 },       /* 50%, 90% */
};
#endif


void TSC2046::init()
{
    // Touch controller initialize
    touch_init();

    //Perform calibration
#ifndef PERFORM_TOUCH_CALIBRATION
    TouchCalibration::setCalibrationMatrix(lcdCalibPoints, touchCalibPoints);
#endif
}

bool TSC2046::sampleTouch(int32_t& x, int32_t& y)
{
    int32_t z = 0;
    touch_xyz(&x, &y, &z);
    return ((z > 0) && (z < 4000));
}
