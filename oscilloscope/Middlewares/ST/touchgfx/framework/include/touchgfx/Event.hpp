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
 * @file touchgfx/Event.hpp
 *
 * Declares the touchgfx::Event class.
 */
#ifndef TOUCHGFX_EVENT_HPP
#define TOUCHGFX_EVENT_HPP

namespace touchgfx
{
/** Simple base class for events. */
class Event
{
public:
    /** The event types. */
    enum EventType
    {
        EVENT_CLICK,  ///< A click
        EVENT_DRAG,   ///< A drag
        EVENT_GESTURE ///< A gesture
    };

    /** Default constructor. */
    Event()
    {
    }

    /** Destructor. */
    virtual ~Event()
    {
    }

    /**
     * Gets event type.
     *
     * @return The type of this event.
     */
    virtual EventType getEventType() const = 0;
};

} // namespace touchgfx

#endif // TOUCHGFX_EVENT_HPP
