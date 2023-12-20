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
#ifndef REPEATBUTTON_HPP
#define REPEATBUTTON_HPP

#include <touchgfx/widgets/Button.hpp>

namespace touchgfx
{
/**
* @class RepeatButton RepeatButton.hpp touchgfx/widgets/RepeatButton.hpp
 *
 * @brief A button with two states.
 *
 *        A button consists of two images, one for its normal state and one when it is pressed
 *        down. The button activates its pressed action immediately, the after a given delay
 *        and then repeatedly after an interval.
 *
 * @see Button
 */
class RepeatButton : public touchgfx::Button
{
public:

    /**
     * @fn RepeatButton::RepeatButton();
     *
     * @brief Default constructor.
     *
     *        Default constructor. Sets delay to 10 ticks and interval to 5 ticks.
     *
     * @see setDelay
     * @see setInterval
     */
    RepeatButton();

    /**
     * @fn virtual void RepeatButton::setDelay(int delay);
     *
     * @brief Sets the delay.
     *
     *        Sets the number of ticks from the first button activation until the next time it gets activated.
     *
     * @param delay The delay.
     *
     @see setInterval
     @see getDelay
     */
    virtual void setDelay(int delay);

    /**
     * @fn virtual int RepeatButton::getDelay();
     *
     * @brief Gets the delay.
     *
     Gets the delay in ticks.
     *
     * @return The delay.
     *
     * @see setDelay
     */
    virtual int getDelay();

    /**
     * @fn virtual void RepeatButton::setInterval(int interval);
     *
     * @brief Sets the interval.
     *
     *        Sets the interval in number of ticks between each each activation of the pressed
     *        button.
     *
     * @param interval The interval.
     *
     * @see setDelay
     * @see getInterval
     */
    virtual void setInterval(int interval);

    /**
     * @fn virtual int RepeatButton::getInterval();
     *
     * @brief Gets the interval.
     *
     *        Gets the interval in ticks.
     *
     * @return The interval.
     */
    virtual int getInterval();

    /**
     * @fn virtual void RepeatButton::handleClickEvent(const touchgfx::ClickEvent& event);
     *
     * @brief Handles the click event.
     *
     *        Handles the click event by immediately activating the button and then setting up
     *        a timer to repeatedly activate the button.
     *
     * @param event The event.
     */
    virtual void handleClickEvent(const touchgfx::ClickEvent& event);

    /**
     * @fn virtual void RepeatButton::handleTickEvent();
     *
     * @brief Handles the tick event.
     *
     *        Handles the tick event that takes care of counting down until the next time the
     *        buttons should be activated.
     */
    virtual void handleTickEvent();

private:
    int16_t ticksDelay;
    int16_t ticksInterval;

    int16_t ticks;
    int16_t ticksBeforeContinuous;
};

} // namespace touchgfx

#endif // REPEATBUTTON_HPP
