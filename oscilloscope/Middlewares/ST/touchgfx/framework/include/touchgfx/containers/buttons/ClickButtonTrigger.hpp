/******************************************************************************
* Copyright (c) 2018(-2023) STMicroelectronics.
* All rights reserved.
*
* This file is part of the TouchGFX 4.22.1 distribution.
*
* This software is licensed under terms that can be found in the LICENSE file in
* the root directory of this software component.
* If no LICENSE file comes with this software, it is provided AS-IS.
*
*******************************************************************************/

/**
 * @file touchgfx/containers/buttons/ClickButtonTrigger.hpp
 *
 * Declares the touchgfx::ClickButtonTrigger class.
 */
#ifndef TOUCHGFX_CLICKBUTTONTRIGGER_HPP
#define TOUCHGFX_CLICKBUTTONTRIGGER_HPP

#include <touchgfx/containers/buttons/AbstractButtonContainer.hpp>
#include <touchgfx/events/ClickEvent.hpp>
#include <touchgfx/hal/Types.hpp>

namespace touchgfx
{
/**
 * A click button trigger. This trigger will create a button that reacts on clicks. This means
 * it will call the set action when it gets a touch released event. The
 * ClickButtonTrigger can be combined with one or more of the ButtonStyle classes to
 * create a fully functional button.
 *
 * @see TouchButtonTrigger
 */
class ClickButtonTrigger : public AbstractButtonContainer
{
public:
    /**
     * Handles a ClickAvent. The action callback is called when the ClickButtonTrigger
     * receives a ClickEvent::RELEASED event in PRESSED state. Function setPressed() will
     * be called with the new button state.
     *
     * @param  event The click event.
     *
     * @see setAction, setPressed, getPressed
     */
    virtual void handleClickEvent(const ClickEvent& event)
    {
        bool wasPressed = getPressed();
        bool newPressedValue = (event.getType() == ClickEvent::PRESSED);
        if ((newPressedValue && !wasPressed) || (!newPressedValue && wasPressed))
        {
            setPressed(newPressedValue);
            invalidate();
        }
        if (wasPressed && (event.getType() == ClickEvent::RELEASED))
        {
            executeAction();
        }
    }
};

} // namespace touchgfx

#endif // TOUCHGFX_CLICKBUTTONTRIGGER_HPP
