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
#include <touchgfx/containers/clock/AnalogClock.hpp>

namespace touchgfx
{

AnalogClock::AnalogClock() :
    AbstractClock(),
    animationEquation(EasingEquations::linearEaseNone),
    animationDuration(0),
    clockRotationCenterX(0),
    clockRotationCenterY(0),
    lastHour(0),
    lastMinute(0),
    lastSecond(0),
    hourHandMinuteCorrectionActive(false),
    minuteHandSecondCorrectionActive(false)
{
    Container::add(background);

    hourHand.updateZAngle(0.f);
    minuteHand.updateZAngle(0.f);
    secondHand.updateZAngle(0.f);

    hourHand.setVisible(false);
    minuteHand.setVisible(false);
    secondHand.setVisible(false);
}

AnalogClock::~AnalogClock()
{
}

void AnalogClock::setBackground(const BitmapId backgroundBitmapId)
{
    setBackground(backgroundBitmapId, Bitmap(backgroundBitmapId).getWidth() / 2, Bitmap(backgroundBitmapId).getHeight() / 2);
}

void AnalogClock::setBackground(const BitmapId backgroundBitmapId, const int16_t rotationCenterX, const int16_t rotationCenterY)
{
    background.setBitmap(Bitmap(backgroundBitmapId));

    clockRotationCenterX = rotationCenterX;
    clockRotationCenterY = rotationCenterY;

    setWidth(background.getWidth());
    setHeight(background.getHeight());
}

void AnalogClock::setupHourHand(const BitmapId hourHandBitmapId, int16_t rotationCenterX, int16_t rotationCenterY)
{
    setupHand(hourHand, hourHandBitmapId, rotationCenterX, rotationCenterY);
}

void AnalogClock::setupMinuteHand(const BitmapId minuteHandBitmapId, int16_t rotationCenterX, int16_t rotationCenterY)
{
    setupHand(minuteHand, minuteHandBitmapId, rotationCenterX, rotationCenterY);
}

void AnalogClock::setupSecondHand(const BitmapId secondHandBitmapId, int16_t rotationCenterX, int16_t rotationCenterY)
{
    setupHand(secondHand, secondHandBitmapId, rotationCenterX, rotationCenterY);
}

void AnalogClock::setupHand(TextureMapper& hand, const BitmapId bitmapId, int16_t rotationCenterX, int16_t rotationCenterY)
{
    remove(hand);

    hand.setBitmap(Bitmap(bitmapId));
    hand.setWidth(background.getWidth());
    hand.setHeight(background.getHeight());
    hand.setXY(0, 0);
    hand.setBitmapPosition(clockRotationCenterX - rotationCenterX, clockRotationCenterY - rotationCenterY);
    hand.setCameraDistance(300.0f);
    hand.setOrigo((float) clockRotationCenterX, (float) clockRotationCenterY, hand.getCameraDistance());
    hand.setCamera(hand.getOrigoX(), hand.getOrigoY());
    hand.setRenderingAlgorithm(TextureMapper::BILINEAR_INTERPOLATION);

    add(hand);
    hand.setVisible(true);
}

void AnalogClock::initializeTime24Hour(uint8_t hour, uint8_t minute, uint8_t second)
{
    lastHour = 255;
    lastMinute = 255;
    lastSecond = 255;

    // Disable animation and set time
    uint16_t tempAnimationDuration = animationDuration;
    animationDuration = 1;
    setTime24Hour(hour, minute, second);

    animationDuration = tempAnimationDuration;
}

void AnalogClock::initializeTime12Hour(uint8_t hour, uint8_t minute, uint8_t second, bool am)
{
    initializeTime24Hour((hour % 12) + (am ? 0 : 12), minute, second);
}

void AnalogClock::updateClock()
{
    // Make sure that animating to 0 will move from left to right
    if (lastHour != 0 && currentHour == 0)
    {
        hourHand.updateZAngle(hourHand.getZAngle() - (2 * PI));
    }
    if (lastMinute != 0 && currentMinute == 0)
    {
        minuteHand.updateZAngle(minuteHand.getZAngle() - (2 * PI));
    }
    if (lastSecond != 0 && currentSecond == 0)
    {
        secondHand.updateZAngle(secondHand.getZAngle() - (2 * PI));
    }

    float newHandAngle;

    // Move hour hand
    if (hourHand.isVisible() && ((currentHour != lastHour) || (hourHandMinuteCorrectionActive && (currentMinute != lastMinute))))
    {
        newHandAngle = convertHandValueToAngle(12, currentHour, hourHandMinuteCorrectionActive ? currentMinute : 0);
        if (animationEnabled())
        {
            hourHand.setupAnimation(AnimationTextureMapper::Z_ROTATION, newHandAngle, animationDuration, 0, animationEquation);
            hourHand.startAnimation();
        }
        else
        {
            hourHand.updateZAngle(newHandAngle);
        }
    }

    // Move minute hand
    if (minuteHand.isVisible() && ((currentMinute != lastMinute) || (minuteHandSecondCorrectionActive && (currentSecond != lastSecond))))
    {
        newHandAngle = convertHandValueToAngle(60, currentMinute, minuteHandSecondCorrectionActive ? currentSecond : 0);
        if (animationEnabled())
        {
            minuteHand.setupAnimation(AnimationTextureMapper::Z_ROTATION, newHandAngle, animationDuration, 0, animationEquation);
            minuteHand.startAnimation();
        }
        else
        {
            minuteHand.updateZAngle(newHandAngle);
        }
    }

    // Move second hand
    if (secondHand.isVisible() && (currentSecond != lastSecond))
    {
        newHandAngle = convertHandValueToAngle(60, currentSecond);
        if (animationEnabled())
        {
            secondHand.setupAnimation(AnimationTextureMapper::Z_ROTATION, newHandAngle, animationDuration, 0, animationEquation);
            secondHand.startAnimation();
        }
        else
        {
            secondHand.updateZAngle(newHandAngle);
        }
    }

    lastHour = currentHour;
    lastMinute = currentMinute;
    lastSecond = currentSecond;
}

float AnalogClock::convertHandValueToAngle(uint8_t steps, uint8_t handValue, uint8_t secondHandValue /*= 0*/) const
{
    return ((handValue / (float)steps) + (secondHandValue / (steps * 60.f))) * 2 * PI;
}

void AnalogClock::setHourHandMinuteCorrection(bool active)
{
    hourHandMinuteCorrectionActive = active;
    setTime24Hour(getCurrentHour(), getCurrentMinute(), getCurrentSecond());
}

bool AnalogClock::getHourHandMinuteCorrection() const
{
    return hourHandMinuteCorrectionActive;
}

void AnalogClock::setMinuteHandSecondCorrection(bool active)
{
    minuteHandSecondCorrectionActive = active;
    setTime24Hour(getCurrentHour(), getCurrentMinute(), getCurrentSecond());
}

bool AnalogClock::getMinuteHandSecondCorrection() const
{
    return minuteHandSecondCorrectionActive;
}

bool AnalogClock::animationEnabled() const
{
    return animationDuration > 1;
}

void AnalogClock::setAnimation(uint16_t duration, EasingEquation animationProgressionEquation)
{
    animationDuration = duration;
    animationEquation = animationProgressionEquation;
}

}
