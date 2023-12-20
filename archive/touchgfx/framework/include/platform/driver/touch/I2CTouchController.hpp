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
#ifndef I2CTOUCHCONTROLLER_HPP
#define I2CTOUCHCONTROLLER_HPP

#include <touchgfx/hal/Types.hpp>
#include <platform/driver/touch/TouchController.hpp>
#include <platform/driver/i2c/I2C.hpp>

namespace touchgfx
{
/**
 * @class I2CTouchController I2CTouchController.hpp platform/driver/touch/I2CTouchController.hpp
 *
 * @brief Specific I2C-enabled type of Touch Controller.
 *
 *        Specific I2C-enabled type of Touch Controller.
 *
 * @sa TouchController
 */
class I2CTouchController : public TouchController
{
public:
    /**
     * @fn I2CTouchController::I2CTouchController(I2C& i2c)
     *
     * @brief Constructor.
     *
     *        Constructor. Initializes I2C driver.
     *
     * @param [in,out] i2c I2C driver.
     */
    I2CTouchController(I2C& i2c):
        i2c(i2c)
    {
        i2c.init();
    }

    /**
     * @fn virtual I2CTouchController::~I2CTouchController()
     *
     * @brief Destructor.
     *
     *        Destructor.
     */
    virtual ~I2CTouchController()
    {
    }

    /**
     * @fn virtual void I2CTouchController::init() = 0;
     *
     * @brief Initializes touch controller.
     *
     *        Initializes touch controller.
     */
    virtual void init() = 0;

    /**
     * @fn virtual bool I2CTouchController::sampleTouch(int32_t& x, int32_t& y) = 0;
     *
     * @brief Checks whether the touch screen is being touched.
     *
     *        Checks whether the touch screen is being touched, and if so, what coordinates.
     *
     * @param [out] x The x position of the touch
     * @param [out] y The y position of the touch
     *
     * @return True if a touch has been detected, otherwise false.
     */
    virtual bool sampleTouch(int32_t& x, int32_t& y) = 0;

protected:
    I2C& i2c; ///< I2C driver
};

} // namespace touchgfx

#endif // I2CTOUCHCONTROLLER_HPP
