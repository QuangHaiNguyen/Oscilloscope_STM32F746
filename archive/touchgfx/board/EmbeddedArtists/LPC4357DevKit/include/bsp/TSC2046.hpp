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
#ifndef TSC2046_HPP
#define TSC2046_HPP

#include <platform/driver/touch/TouchController.hpp>
#include "tsc2046_touch.h"
#include "lpc43xx_i2c.h"
#include "lpc43xx_rgu.h"

typedef struct
{
    int16_t x;
    int16_t y;
    int16_t z;
} Finger;

namespace touchgfx
{
class TSC2046 : public TouchController /* at some point, make this an i2c touchcontroller with a proper i2c driver */
{

public:
    /**
     * Initializes touch controller.
     */
    virtual void init();

    /**
     * Checks whether the touch screen is being touched, and if so, what coordinates.
     * @param x The x position of the touch (output)
     * @param y The y position of the touch (output)
     *
     * @return True if a touch has been detected, otherwise false
     */
    virtual bool sampleTouch(int32_t& x, int32_t& y);

};
}

#endif // TSC2046_HPP
