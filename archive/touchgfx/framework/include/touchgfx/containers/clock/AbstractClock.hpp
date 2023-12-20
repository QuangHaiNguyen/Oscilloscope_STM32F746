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
#ifndef ABSTRACTCLOCK_HPP
#define ABSTRACTCLOCK_HPP

#include <touchgfx/containers/Container.hpp>
#include <touchgfx/widgets/Image.hpp>
#include <touchgfx/widgets/TextureMapper.hpp>

namespace touchgfx
{
/**
 * @class AbstractClock AbstractClock.hpp touchgfx/containers/clock/AbstractClock.hpp
 *
 * @brief Superclass of clock widgets.
 */
class AbstractClock : public Container
{
public:

    /**
     * @fn AbstractClock::AbstractClock();
     *
     * @brief Default constructor.
     *
     *        Default constructor.
     */
    AbstractClock();

    /**
     * @fn virtual AbstractClock::~AbstractClock()
     *
     * @brief Destructor.
     *
     *        Destructor.
     */
    virtual ~AbstractClock() {}

    /**
     * @fn virtual void AbstractClock::setTime24Hour(uint8_t hour, uint8_t minute, uint8_t second)
     *
     * @brief Sets the time with input format as 24H.
     *
     *        Sets the time with input format as 24H. Note that this does not affect any selected
     *        presentation formats.
     *
     * @param hour   The hours (in 24H format).
     * @param minute The minutes (in 24H format).
     * @param second The seconds (in 24H format).
     */
    virtual void setTime24Hour(uint8_t hour, uint8_t minute, uint8_t second);

    /**
     * @fn virtual void AbstractClock::setTime12Hour(uint8_t hour, uint8_t minute, uint8_t second, bool am)
     *
     * @brief Sets the time with input format as 12H.
     *
     *        Sets the time with input format as 12H. Note that this does not affect any selected
     *        presentation formats.
     *
     * @param hour   The hours (in 12H format).
     * @param minute The minutes (in 12H format).
     * @param second The seconds (in 12H format).
     * @param am     AM/PM setting. True = AM, false = PM.
     */
    virtual void setTime12Hour(uint8_t hour, uint8_t minute, uint8_t second, bool am);

    /**
     * @fn uint8_t AbstractClock::getCurrentHour() const;
     *
     * @brief Gets the current hour.
     *
     *        Gets the current hour.
     *
     * @return The current hour.
     */
    uint8_t getCurrentHour() const;

    /**
     * @fn uint8_t AbstractClock::getCurrentMinute() const;
     *
     * @brief Gets the current minute.
     *
     *        Gets the current minute.
     *
     * @return The current minute.
     */
    uint8_t getCurrentMinute() const;

    /**
     * @fn uint8_t AbstractClock::getCurrentSecond() const;
     *
     * @brief Gets the current second.
     *
     *        Gets the current second.
     *
     * @return The current second.
     */
    uint8_t getCurrentSecond() const;

protected:
    uint8_t currentHour;    ///< Local copy of the current hour
    uint8_t currentMinute;  ///< Local copy of the current minute
    uint8_t currentSecond;  ///< Local copy of the current second

    /**
     * @fn virtual void AbstractClock::updateClock() = 0;
     *
     * @brief Updates the visual representation of the clock.
     *
     *        Updates the visual representation of the clock.
     */
    virtual void updateClock() = 0;
};

}

#endif // ABSTRACTCLOCK_HPP
