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
#include <touchgfx/containers/progress_indicators/AbstractProgressIndicator.hpp>

namespace touchgfx
{

AbstractProgressIndicator::AbstractProgressIndicator()
    : Container(), rangeMin(0), rangeMax(100), currentValue(0), rangeSteps(100), rangeStepsMin(0)
{
    background.setXY(0, 0);
    Container::add(background);

    Container::add(progressIndicatorContainer);
}

touchgfx::AbstractProgressIndicator::~AbstractProgressIndicator()
{
}

void AbstractProgressIndicator::setBackground(const Bitmap& bmpBackground)
{
    background.setBitmap(bmpBackground);
    Drawable::setWidth(background.getWidth());
    Drawable::setHeight(background.getHeight());
}

void AbstractProgressIndicator::setProgressIndicatorPosition(int16_t x, int16_t y, int16_t width, int16_t height)
{
    progressIndicatorContainer.setPosition(x, y, width, height);

    if (getWidth() < x + width)
    {
        AbstractProgressIndicator::setWidth(x + width);
    }

    if (getHeight() < y + height)
    {
        AbstractProgressIndicator::setHeight(y + height);
    }
}

int16_t AbstractProgressIndicator::getProgressIndicatorX() const
{
    return progressIndicatorContainer.getX();
}

int16_t AbstractProgressIndicator::getProgressIndicatorY() const
{
    return progressIndicatorContainer.getY();
}

int16_t AbstractProgressIndicator::getProgressIndicatorWidth() const
{
    return progressIndicatorContainer.getWidth();
}

int16_t AbstractProgressIndicator::getProgressIndicatorHeight() const
{
    return progressIndicatorContainer.getHeight();
}

void AbstractProgressIndicator::setRange(int16_t min, int16_t max, uint16_t steps /*= 0*/, uint16_t minStep /*= 0*/)
{
    assert(min < max);
    rangeMin = min;
    rangeMax = max;
    setValue(currentValue);
    if (steps == 0)
    {
        rangeSteps = max - min;
    }
    else
    {
        rangeSteps = steps;
    }
    rangeStepsMin = minStep;
    assert(rangeStepsMin < rangeSteps);
}

void AbstractProgressIndicator::getRange(int16_t& min, int16_t& max, uint16_t& steps, uint16_t& minStep) const
{
    min = rangeMin;
    max = rangeMax;
    steps = rangeSteps;
    minStep = rangeStepsMin;
}

void AbstractProgressIndicator::getRange(int16_t& min, int16_t& max, uint16_t& steps) const
{
    min = rangeMin;
    max = rangeMax;
    steps = rangeSteps;
}

void AbstractProgressIndicator::getRange(int16_t& min, int16_t& max) const
{
    min = rangeMin;
    max = rangeMax;
}

void AbstractProgressIndicator::setValue(int value)
{
    int newValue = MAX(value, rangeMin);
    newValue = MIN(newValue, rangeMax);
    currentValue = newValue;
}

int AbstractProgressIndicator::getValue() const
{
    return currentValue;
}

uint16_t AbstractProgressIndicator::getProgress(uint16_t range /*= 100*/) const
{
    if (range == 0)
    {
        return 0;
    }
    uint16_t progress = ((currentValue - rangeMin) * range) / (rangeMax - rangeMin);
    // Calculate range/rangeSteps as "intpart+fracpart/rangeSteps"
    uint16_t intpart = range / rangeSteps;
    uint16_t fracpart = range % rangeSteps;
    // Now "progress/(intpart+fracpart/rangeSteps)" = "(progress*rangeSteps)/(intpart*rangeSteps+fracpart)"
    uint16_t count = rangeStepsMin + (progress * (rangeSteps - rangeStepsMin)) / range;
    return (count * intpart + (count * fracpart) / rangeSteps);
}

}
