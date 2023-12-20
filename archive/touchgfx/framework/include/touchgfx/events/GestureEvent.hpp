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
#ifndef GESTUREEVENT_HPP
#define GESTUREEVENT_HPP

#include <touchgfx/Event.hpp>
#include <touchgfx/hal/Types.hpp>

namespace touchgfx
{
/**
 * @class GestureEvent GestureEvent.hpp touchgfx/events/GestureEvent.hpp
 *
 * @brief A gesture event.
 *
 *        A gesture event. The only gesture events currently supported is SWIPE_HORIZONTAL and
 *        SWIPE_VERTICAL, which will be issued every time the input system detects a swipe.
 *
 * @see Event
 */
class GestureEvent : public Event
{
public:

    /**
     * @typedef enum GestureType
     *
     * @brief The gesture event types.
     */
    typedef enum
    {
        SWIPE_HORIZONTAL, ///< An enum constant representing a horizontal swipe
        SWIPE_VERTICAL    ///< An enum constant representing a vertical swipe
    } GestureType;

    /**
     * @fn GestureEvent::GestureEvent(GestureType t, int16_t v, int16_t x_coord, int16_t y_coord)
     *
     * @brief Constructor.
     *
     *        Constructor. Create a gesture event of the specified type with the specified
     *        coordinates.
     *
     * @param t       The type of the gesture event.
     * @param v       The velocity of this gesture (swipe)
     * @param x_coord The x coordinate of the gesture.
     * @param y_coord The y coordinate of the gesture.
     */
    GestureEvent(GestureType t, int16_t v, int16_t x_coord, int16_t y_coord) :
        type(t),
        velocity(v),
        x(x_coord),
        y(y_coord)
    {
    }

    /**
     * @fn int16_t GestureEvent::getVelocity() const
     *
     * @brief Gets the velocity of this gesture event.
     *
     *        Gets the velocity of this gesture event.
     *
     * @return The velocity of this gesture event.
     */
    int16_t getVelocity() const
    {
        return velocity;
    }

    /**
     * @fn GestureType GestureEvent::getType() const
     *
     * @brief Gets the type of this gesture event.
     *
     *        Gets the type of this gesture event.
     *
     * @return The type of this gesture event.
     */
    GestureType getType() const
    {
        return type;
    }

    /**
     * @fn int16_t GestureEvent::getX() const
     *
     * @brief Gets the x coordinate of this gesture event.
     *
     *        Gets the x coordinate of this gesture event.
     *
     * @return The x coordinate of this gesture event.
     */
    int16_t getX() const
    {
        return x;
    }

    /**
     * @fn int16_t GestureEvent::getY() const
     *
     * @brief Gets the y coordinate of this gesture event.
     *
     *        Gets the y coordinate of this gesture event.
     *
     * @return The y coordinate of this gesture event.
     */
    int16_t getY() const
    {
        return y;
    }

    /**
     * @fn virtual Event::EventType GestureEvent::getEventType()
     *
     * @brief Gets event type.
     *
     * @return The type of this event.
     */
    virtual Event::EventType getEventType()
    {
        return Event::EVENT_GESTURE;
    }

private:
    GestureEvent() { }
    GestureType type;
    int16_t velocity;
    int16_t x;
    int16_t y;
};

} // namespace touchgfx
#endif // GESTUREEVENT_HPP
