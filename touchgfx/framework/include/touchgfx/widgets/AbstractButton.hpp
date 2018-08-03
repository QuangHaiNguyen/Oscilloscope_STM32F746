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
#ifndef ABSTRACTBUTTON_HPP
#define ABSTRACTBUTTON_HPP

#include <touchgfx/widgets/Widget.hpp>
#include <touchgfx/Callback.hpp>

namespace touchgfx
{
/**
 * @class AbstractButton AbstractButton.hpp touchgfx/widgets/AbstractButton.hpp
 *
 * @brief This class defines an abstract interface for button-like elements.
 *
 *        This class defines an abstract interface for button-like elements. A button is a
 *        clickable element that has two states - pressed or released - and executes an action
 *        when the pressed->released transition is made.
 *
 * @see Widget
 */
class AbstractButton : public Widget
{
public:

    /**
     * @fn AbstractButton::AbstractButton()
     *
     * @brief Constructor.
     *
     *        Constructs an AbstractButton instance in released state without an associated
     *        action.
     */
    AbstractButton() : Widget(), action(), pressed(false)
    {
        setTouchable(true);
    }

    /**
     * @fn virtual void AbstractButton::handleClickEvent(const ClickEvent& event);
     *
     * @brief Updates the current state of the button.
     *
     *        Updates the current state of the button - pressed or released - and invalidates
     *        it.
     *
     *        If a transition from the pressed to the released state was made, the associated
     *        action is executed and then the Widget is invalidated.
     *
     * @param event Information about the click.
     *
     * @see Drawable::handleClickEvent()
     */
    virtual void handleClickEvent(const ClickEvent& event);

    /**
     * @fn void AbstractButton::setAction(GenericCallback< const AbstractButton& >& callback)
     *
     * @brief Associates an action to be performed when the AbstractButton is clicked.
     *
     *        Associates an action to be performed when the AbstractButton is clicked.
     *
     * @param callback The callback to be executed. The callback will be given a reference to
     *                 the AbstractButton.
     *
     * @see GenericCallback
     */
    void setAction(GenericCallback< const AbstractButton& >& callback)
    {
        action = &callback;
    }

    /**
     * @fn virtual bool AbstractButton::getPressedState() const
     *
     * @brief Function to determine whether this AbstractButton is currently pressed.
     *
     *        Function to determine whether this AbstractButton is currently pressed.
     *
     * @return true if button is pressed, false otherwise.
     */
    virtual bool getPressedState() const
    {
        return pressed;
    }

    /**
     * @fn virtual uint16_t AbstractButton::getType() const
     *
     * @brief For GUI testing only.
     *
     *        For GUI testing only. Returns type of this drawable.
     *
     * @return TYPE_ABSTRACTBUTTON.
     */
    virtual uint16_t getType() const
    {
        return (uint16_t)TYPE_ABSTRACTBUTTON;
    }

protected:
    GenericCallback< const AbstractButton& >* action; ///< The callback to be executed when this AbstractButton is clicked

    bool pressed; ///< Is the button pressed or released? True if pressed.
};

} // namespace touchgfx

#endif // ABSTRACTBUTTON_HPP
