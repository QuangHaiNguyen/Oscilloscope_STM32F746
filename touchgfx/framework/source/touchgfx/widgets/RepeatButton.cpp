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
#include <touchgfx/widgets/RepeatButton.hpp>

namespace touchgfx
{

RepeatButton::RepeatButton() : Button(), ticksDelay(30), ticksInterval(15), ticks(0), ticksBeforeContinuous(0)
{
}

void RepeatButton::setDelay(int delay)
{
    ticksDelay = delay;
}

int RepeatButton::getDelay()
{
    return ticksDelay;
}

void RepeatButton::setInterval(int interval)
{
    ticksInterval = interval;
}

int RepeatButton::getInterval()
{
    return ticksInterval;
}

void RepeatButton::handleClickEvent(const touchgfx::ClickEvent& event)
{
    pressed = false; // To prevent AbstractButton from calling action->execute().
    invalidate(); // Force redraw after forced state change
    Button::handleClickEvent(event);
    if (event.getType() == touchgfx::ClickEvent::PRESSED)
    {
        if (action && action->isValid())
        {
            action->execute(*this);
        }
        ticks = 0;
        ticksBeforeContinuous = ticksDelay;
        touchgfx::Application::getInstance()->registerTimerWidget(this);
    }
    else
    {
        touchgfx::Application::getInstance()->unregisterTimerWidget(this);
    }
}

void RepeatButton::handleTickEvent()
{
    Button::handleTickEvent();

    if (pressed)
    {
        if (ticks == ticksBeforeContinuous)
        {
            if (action && action->isValid())
            {
                action->execute(*this);
            }

            ticks = 0;
            ticksBeforeContinuous = ticksInterval;
        }
        else
        {
            ticks++;
        }
    }
}

} // namespace touchgfx
