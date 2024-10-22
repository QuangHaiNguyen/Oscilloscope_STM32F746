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

#include <touchgfx/containers/clock/AbstractClock.hpp>

namespace touchgfx
{
AbstractClock::AbstractClock()
    : Container(),
      currentHour(0),
      currentMinute(0),
      currentSecond(0)
{
}

void AbstractClock::setTime24Hour(uint8_t hour, uint8_t minute, uint8_t second)
{
    currentHour = hour % 24;
    currentMinute = minute % 60;
    currentSecond = second % 60;

    updateClock();
}

void AbstractClock::setTime12Hour(uint8_t hour, uint8_t minute, uint8_t second, bool am)
{
    setTime24Hour((hour % 12) + (am ? 0 : 12), minute, second);
}

uint8_t AbstractClock::getCurrentHour() const
{
    return currentHour;
}

uint8_t AbstractClock::getCurrentHour24() const
{
    return currentHour;
}

uint8_t AbstractClock::getCurrentHour12() const
{
    return ((currentHour + 11) % 12) + 1;
}

bool AbstractClock::getCurrentAM() const
{
    return currentHour < 12;
}

uint8_t AbstractClock::getCurrentMinute() const
{
    return currentMinute;
}

uint8_t AbstractClock::getCurrentSecond() const
{
    return currentSecond;
}
} // namespace touchgfx
