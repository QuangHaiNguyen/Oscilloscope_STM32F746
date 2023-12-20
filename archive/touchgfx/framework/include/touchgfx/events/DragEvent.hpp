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
#ifndef DRAGEVENT_HPP
#define DRAGEVENT_HPP

#include <touchgfx/Event.hpp>
#include <touchgfx/hal/Types.hpp>

namespace touchgfx
{
/**
 * @class DragEvent DragEvent.hpp touchgfx/events/DragEvent.hpp
 *
 * @brief A drag event.
 *
 *        A drag event. The only drag event currently supported is DRAGGED, which will be
 *        issued every time the input system detects a drag.
 *
 * @see Event
 */
class DragEvent : public Event
{
public:

    /**
     * @typedef enum DragEventType
     *
     * @brief The drag event types.
     */
    typedef enum
    {
        DRAGGED
    } DragEventType;

    /**
     * @fn DragEvent::DragEvent(DragEventType type, int16_t oldX, int16_t oldY, int16_t newX, int16_t newY)
     *
     * @brief Constructor.
     *
     *        Constructor. Create a drag event of the specified type with the specified
     *        coordinates.
     *
     * @param type The type of the drag event.
     * @param oldX The x coordinate of the drag start position (dragged from)
     * @param oldY The y coordinate of the drag start position (dragged from)
     * @param newX The x coordinate of the new position (dragged to)
     * @param newY The y coordinate of the new position (dragged to)
     */
    DragEvent(DragEventType type, int16_t oldX, int16_t oldY, int16_t newX, int16_t newY) :
        _type(type), _oldX(oldX), _oldY(oldY), _newX(newX), _newY(newY) { }

    /**
     * @fn virtual DragEvent::~DragEvent()
     *
     * @brief Destructor.
     *
     *        Destructor.
     */
    virtual ~DragEvent() { }

    /**
     * @fn int16_t DragEvent::getOldX() const
     *
     * @brief Gets the x coordinate where the drag operation was started (dragged from).
     *
     *        Gets the x coordinate where the drag operation was started (dragged from).
     *
     * @return The x coordinate where the drag operation was started (dragged from).
     */
    int16_t getOldX() const
    {
        return _oldX;
    }

    /**
     * @fn int16_t DragEvent::getOldY() const
     *
     * @brief Gets the y coordinate where the drag operation was started (dragged from).
     *
     *        Gets the y coordinate where the drag operation was started (dragged from).
     *
     * @return The y coordinate where the drag operation was started (dragged from).
     */
    int16_t getOldY() const
    {
        return _oldY;
    }

    /**
     * @fn int16_t DragEvent::getNewX() const
     *
     * @brief Gets the new x coordinate (dragged to).
     *
     *        Gets the new x coordinate (dragged to).
     *
     * @return The new x coordinate (dragged to).
     */
    int16_t getNewX() const
    {
        return _newX;
    }

    /**
     * @fn int16_t DragEvent::getNewY() const
     *
     * @brief Gets the new x coordinate (dragged to).
     *
     *        Gets the new x coordinate (dragged to).
     *
     * @return The new y coordinate (dragged to).
     */
    int16_t getNewY() const
    {
        return _newY;
    }

    /**
     * @fn DragEventType DragEvent::getType() const
     *
     * @brief Gets the type of this drag event.
     *
     *        Gets the type of this drag event.
     *
     * @return The type of this drag event.
     */
    DragEventType getType() const
    {
        return _type;
    }

    /**
     * @fn int16_t DragEvent::getDeltaX() const
     *
     * @brief Gets the distance in x coordinates (how long was the drag).
     *
     *        Gets the distance in x coordinates (how long was the drag).
     *
     * @return The distance of this drag event.
     */
    int16_t getDeltaX() const
    {
        return _newX - _oldX;
    }

    /**
     * @fn int16_t DragEvent::getDeltaY() const
     *
     * @brief Gets the distance in y coordinates (how long was the drag).
     *
     *        Gets the distance in y coordinates (how long was the drag).
     *
     * @return The distance of this drag event.
     */
    int16_t getDeltaY() const
    {
        return _newY - _oldY;
    }

    /**
     * @fn virtual Event::EventType DragEvent::getEventType()
     *
     * @brief Gets event type.
     *
     *        Gets event type.
     *
     * @return The type of this event.
     */
    virtual Event::EventType getEventType()
    {
        return Event::EVENT_DRAG;
    }

private:
    DragEventType _type;
    int16_t _oldX;
    int16_t _oldY;
    int16_t _newX;
    int16_t _newY;
};

} // namespace touchgfx
#endif // DRAGEVENT_HPP
