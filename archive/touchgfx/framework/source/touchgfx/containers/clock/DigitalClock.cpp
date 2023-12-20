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
#include <touchgfx/containers/clock/DigitalClock.hpp>

namespace touchgfx
{

DigitalClock::DigitalClock() :
    AbstractClock(),
    displayMode(DISPLAY_24_HOUR),
    useLeadingZeroForHourIndicator(false)
{
    buffer[0] = '\0';
    text.setXY(0, 0);
    text.setWildcard(buffer);
    Container::add(text);
}

DigitalClock::~DigitalClock()
{
}

void DigitalClock::setWidth(int16_t width)
{
    Container::setWidth(width);
    text.setWidth(width);
}

void DigitalClock::setHeight(int16_t height)
{
    Container::setHeight(height);
    text.setHeight(height);
}

void DigitalClock::setBaselineY(int16_t baselineY)
{
    moveTo(getX(), baselineY - text.getTypedText().getFont()->getFontHeight());
}

void DigitalClock::displayLeadingZeroForHourIndicator(bool displayLeadingZero)
{
    useLeadingZeroForHourIndicator = displayLeadingZero;
}

void DigitalClock::setAlpha(uint8_t alpha)
{
    text.setAlpha(alpha);
}

uint8_t DigitalClock::getAlpha() const
{
    return text.getAlpha();
}

void DigitalClock::setTypedText(TypedText typedText)
{
    text.setTypedText(typedText);
    text.invalidate();
}

void DigitalClock::setColor(colortype color)
{
    text.setColor(color);
    text.invalidate();
}

void DigitalClock::updateClock()
{
    if (displayMode == DISPLAY_12_HOUR_NO_SECONDS)
    {
        const char* format = useLeadingZeroForHourIndicator ? "%02d:%02d %cM" : "%d:%02d %cM";
        Unicode::snprintf(buffer, BUFFER_SIZE, format, ((currentHour + 11) % 12) + 1, currentMinute, currentHour < 12 ? 'A' : 'P');
    }
    else if (displayMode == DISPLAY_24_HOUR_NO_SECONDS)
    {
        const char* format = useLeadingZeroForHourIndicator ? "%02d:%02d" : "%d:%02d";
        Unicode::snprintf(buffer, BUFFER_SIZE, format, currentHour, currentMinute);
    }
    else if (displayMode == DISPLAY_12_HOUR)
    {
        const char* format = useLeadingZeroForHourIndicator ? "%02d:%02d:%02d %cM" : "%d:%02d:%02d %cM";
        Unicode::snprintf(buffer, BUFFER_SIZE, format, ((currentHour + 11) % 12) + 1, currentMinute, currentSecond, currentHour < 12 ? 'A' : 'P');
    }
    else if (displayMode == DISPLAY_24_HOUR)
    {
        const char* format = useLeadingZeroForHourIndicator ? "%02d:%02d:%02d" : "%d:%02d:%02d";
        Unicode::snprintf(buffer, BUFFER_SIZE, format, currentHour, currentMinute, currentSecond);

    }
    text.invalidate();
}

}
