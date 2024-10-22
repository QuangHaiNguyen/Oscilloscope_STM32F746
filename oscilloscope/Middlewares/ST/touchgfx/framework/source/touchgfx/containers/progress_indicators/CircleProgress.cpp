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

#include <touchgfx/containers/progress_indicators/CircleProgress.hpp>
#include <touchgfx/widgets/canvas/CWRUtil.hpp>

namespace touchgfx
{
CircleProgress::CircleProgress()
    : AbstractProgressIndicator(), circle(), circleEndAngle(360)
{
    progressIndicatorContainer.add(circle);
    circle.setPosition(0, 0, getWidth(), getHeight());
    CircleProgress::setStartEndAngle(0, 360);
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
    CircleProgress::setValue(CircleProgress::getValue());
}

int CircleProgress::getStartAngle() const
{
    return circle.getArcStart();
}

int CircleProgress::getEndAngle() const
{
    return circleEndAngle;
}

void CircleProgress::setAlpha(uint8_t newAlpha)
{
    AbstractProgressIndicator::setAlpha(newAlpha);
    circle.setAlpha(newAlpha);
}

void CircleProgress::setValue(int value)
{
    AbstractProgressIndicator::setValue(value);
    CWRUtil::Q5 startAngle;
    CWRUtil::Q5 const endAngle = CWRUtil::toQ5(circleEndAngle);
    circle.getArcStart<CWRUtil::Q5>(startAngle);
    const uint16_t rangeAngleSteps = endAngle < startAngle ? (int)(startAngle - endAngle) : (int)(endAngle - startAngle);
    CWRUtil::Q5 const progress = CWRUtil::Q5(AbstractProgressIndicator::getProgress(rangeAngleSteps));
    if (endAngle < startAngle)
    {
        circle.updateArcEnd<CWRUtil::Q5>(startAngle - progress);
    }
    else
    {
        circle.updateArcEnd<CWRUtil::Q5>(startAngle + progress);
    }
}
} // namespace touchgfx
