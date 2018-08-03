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
#ifndef GPIO_HPP
#define GPIO_HPP

namespace touchgfx
{
/**
 * @class GPIO GPIO.hpp touchgfx/hal/GPIO.hpp
 *
 * @brief Interface class for manipulating GPIOs in order to do performance measurements on target.
 *
 *        Interface class for manipulating GPIOs in order to do performance measurements on
 *        target. Not used on the PC simulator.
 */
class GPIO
{
public:

    /**
     * @typedef enum GPIO_ID
     *
     * @brief Enum for the GPIOs used.
     *
     *        Enum for the GPIOs used.
     */
    typedef enum
    {
        VSYNC_FREQ,  /// Pin is toggled at each VSYNC
        RENDER_TIME, /// Pin is high when frame rendering begins, low when finished
        FRAME_RATE,  /// Pin is toggled when the frame buffers are swapped.
        MCU_ACTIVE   /// Pin is high when the MCU is doing work (i.e. not in idle task).
    } GPIO_ID;

    /**
     * @fn static void GPIO::init();
     *
     * @brief Perform configuration of IO pins.
     *
     *        Perform configuration of IO pins.
     */
    static void init();

    /**
     * @fn static void GPIO::set(GPIO_ID id);
     *
     * @brief Sets a pin high.
     *
     *        Sets a pin high.
     *
     * @param id the pin to set.
     */
    static void set(GPIO_ID id);

    /**
     * @fn static void GPIO::clear(GPIO_ID id);
     *
     * @brief Sets a pin low.
     *
     *        Sets a pin low.
     *
     * @param id the pin to set.
     */
    static void clear(GPIO_ID id);

    /**
     * @fn static void GPIO::toggle(GPIO_ID id);
     *
     * @brief Toggles a pin.
     *
     *        Toggles a pin.
     *
     * @param id the pin to toggle.
     */
    static void toggle(GPIO_ID id);

    /**
     * @fn static bool GPIO::get(GPIO_ID id);
     *
     * @brief Gets the state of a pin.
     *
     *        Gets the state of a pin.
     *
     * @param id the pin to get.
     *
     * @return true if the pin is high, false otherwise.
     */
    static bool get(GPIO_ID id);
}; // class GPIO

} // namespace touchgfx

#endif // GPIO_HPP
