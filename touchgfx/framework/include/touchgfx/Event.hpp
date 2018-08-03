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
#ifndef EVENT_HPP
#define EVENT_HPP

namespace touchgfx
{
/**
 * @class Event Event.hpp touchgfx/Event.hpp
 *
 * @brief Simple base class for events.
 *
 *        Simple base class for events.
 */
class Event
{
public:

    /**
     * @typedef enum EventType
     *
     * @brief The events types.
     *
     *        The events types.
     */
    typedef enum
    {
        EVENT_CLICK,  ///< A click
        EVENT_DRAG,   ///< A drag
        EVENT_GESTURE ///< A gesture
    } EventType;

    Event() { }

    /**
     * @fn virtual EventType Event::getEventType() = 0;
     *
     * @brief Gets event type.
     *
     *        Gets event type.
     *
     * @return The type of this event.
     */
    virtual EventType getEventType() = 0;

    /**
     * @fn virtual Event::~Event()
     *
     * @brief Destructor.
     *
     *        Destructor.
     */
    virtual ~Event() { }
};

} // namespace touchgfx

#endif // EVENT_HPP
