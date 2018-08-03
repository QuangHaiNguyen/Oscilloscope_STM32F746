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
#ifndef LED_HPP
#define LED_HPP

#include "lpc_types.h"
namespace touchgfx
{
/**
 * @class LED LED.hpp touchgfx\hal\LED.hpp
 *
 * @brief A led.
 */
class LED
{
public:

    /**
     * @fn static void LED::init();
     *
     * @brief Perform configuration of IO pins.
     *
     *        Perform configuration of IO pins.
     */
    static void init();

    /**
     * @fn static void LED::on(uint8_t nr);
     *
     * @brief Turn on a LED.
     *
     *        Turn on a LED.
     *
     * @param nr of the LED to switch on.
     */
    static void on(uint8_t nr);

    /**
     * @fn static void LED::off(uint8_t nr);
     *
     * @brief Turn off a LED.
     *
     *        Turn off a LED.
     *
     * @param nr of the LED to switch off.
     */
    static void off(uint8_t nr);

    /**
     * @fn static void LED::toggle(uint8_t nr);
     *
     * @brief Toggles a LED.
     *
     *        Toggles a LED.
     *
     * @param nr of the LED to toggle.
     */
    static void toggle(uint8_t nr);

    /**
     * @fn static bool LED::get(uint8_t nr);
     *
     * @brief Get state of a LED.
     *
     *        Get state of a LED.
     *
     * @param nr of the LED to toggle.
     *
     * @return the state of the LED.
     */
    static bool get(uint8_t nr);

};
}
#endif // LED_HPP
