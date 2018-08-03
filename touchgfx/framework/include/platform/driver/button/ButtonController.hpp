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
#ifndef BUTTONCONTROLLER_HPP
#define BUTTONCONTROLLER_HPP

#include <touchgfx/hal/Types.hpp>

namespace touchgfx
{
/**
 * @class ButtonController ButtonController.hpp platform/driver/button/ButtonController.hpp
 *
 * @brief Interface for sampling external key events.
 *
 *        Interface for sampling external key events.
 */
class ButtonController
{
public:
    /**
     * @fn virtual ButtonController::~ButtonController()
     *
     * @brief Destructor.
     *
     *        Destructor.
     */
    virtual ~ButtonController()
    {
    }

    /**
     * @fn virtual void ButtonController::init() = 0;
     *
     * @brief Initializes button controller.
     *
     *        Initializes button controller.
     */
    virtual void init() = 0;

    /**
     * @fn virtual bool ButtonController::sample(uint8_t& key) = 0;
     *
     * @brief Sample external key events.
     *
     *        Sample external key events.
     *
     * @param [in,out] key Output parameter that will be set to the key value if a keypress was
     *                     detected.
     *
     * @return True if a keypress was detected and the "key" parameter is set to a value.
     */
    virtual bool sample(uint8_t& key) = 0;

    /**
     * @fn virtual void reset()
     *
     * @brief Resets button controller.
     *
     *        Resets button controller. Does nothing in the default implementation.
     *
     */
    virtual void reset() {}
};

} // namespace touchgfx

#endif // BUTTONCONTROLLER_HPP
