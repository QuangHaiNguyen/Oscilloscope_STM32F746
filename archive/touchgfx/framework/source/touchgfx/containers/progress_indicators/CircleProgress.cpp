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
#include <touchgfx/containers/progress_indicators/CircleProgress.hpp>

namespace touchgfx
{

CircleProgress::CircleProgress()
    : AbstractProgressIndicator(), circle()
{
    progressIndicatorContainer.add(circle);
    circle.setPosition(0, 0, getWidth(), getHeight());
    CircleProgress::setStartEndAngle(0, 360);
}

CircleProgress::~CircleProgress()
{
}

void CircleProgress::setProgressIndicatorPosition(int16_t x, int16_t y, int16_t width, int16_t height)
{
    circle.setPosition(0, 0, width, height);

    AbstractProgressIndicator::setProgressIndicatorPosition(x, y, width, height);
}

void CircleProgress::setPainter(AbstractPainter& painter)
{
    circle.setPainter(painter);
}

void CircleProgress::setCenter(int x, int y)
{
    circle.setCenter(x, y);
}

void CircleProgress::getCenter(int& x, int& y) const
{
    circle.getCenter(x, y);
}

void CircleProgress::setRadius(int r)
{
    circle.setRadius(r);
}

int CircleProgress::getRadius() const
{
    int radius;
    circle.getRadius(radius);
    return radius;
}

void CircleProgress::setLineWidth(int width)
{
    circle.setLineWidth(width);
}

int CircleProgress::getLineWidth() const
{
    int width;
    circle.getLineWidth(width);
    return width;
}

void CircleProgress::setCapPrecision(int precision)
{
    circle.setCapPrecision(precision);
}

void CircleProgress::setStartEndAngle(int startAngle, int endAngle)
{
    assert(startAngle != endAngle);
    circle.setArc(startAngle, endAngle);
    circleEndAngle = endAngle;
}

int CircleProgress::getStartAngle() const
{
    return circle.getArcStart();
}

int CircleProgress::getEndAngle() const
{
    return circleEndAngle;
}

void CircleProgress::setAlpha(uint8_t alpha)
{
    circle.setAlpha(alpha);
}

uint8_t CircleProgress::getAlpha() const
{
    return circle.getAlpha();
}

void CircleProgress::setValue(int value)
{
    int startAngle = circle.getArcStart();
    AbstractProgressIndicator::setValue(value);
    uint16_t rangeAngleSteps = circleEndAngle < startAngle ? startAngle - circleEndAngle : circleEndAngle - startAngle;
    uint16_t progress = AbstractProgressIndicator::getProgress(rangeAngleSteps);
    if (circleEndAngle < startAngle)
    {
        circle.updateArcEnd(startAngle - progress);
    }
    else
    {
        circle.updateArcEnd(startAngle + progress);
    }
}

}
