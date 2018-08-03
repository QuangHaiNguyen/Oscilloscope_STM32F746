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
#include <touchgfx/widgets/canvas/Circle.hpp>

namespace touchgfx
{
Circle::Circle() : CanvasWidget(),
    circleCenterX(0), circleCenterY(0), circleRadius(0),
    circleArcAngleStart(0), circleArcAngleEnd(360),
    circleLineWidth(0), circleArcIncrement(5),
    circleCapArcIncrement(180)
{
    Drawable::setWidth(0);
    Drawable::setHeight(0);
}

void Circle::setArc(int16_t startAngle, int16_t endAngle)
{
    circleArcAngleStart = startAngle;
    circleArcAngleEnd = endAngle;
}

void Circle::getArc(int16_t& startAngle, int16_t& endAngle) const
{
    startAngle = circleArcAngleStart;
    endAngle = circleArcAngleEnd;
}

int16_t Circle::getArcStart() const
{
    return circleArcAngleStart;
}

int16_t Circle::getArcEnd() const
{
    return circleArcAngleEnd;
}

void Circle::updateArcStart(int16_t startAngle)
{
    if (circleArcAngleStart == startAngle)
    {
        return;
    }

    Rect minimalRect = getMinimalRect(circleArcAngleStart, startAngle);

    circleArcAngleStart = startAngle;

    invalidateRect(minimalRect);
}

void Circle::updateArcEnd(int16_t endAngle)
{
    if (circleArcAngleEnd == endAngle)
    {
        return;
    }

    Rect minimalRect = getMinimalRect(circleArcAngleEnd, endAngle);

    circleArcAngleEnd = endAngle;

    invalidateRect(minimalRect);
}

void Circle::setPrecision(int precision)
{
    if (precision < 1)
    {
        precision = 1;
    }
    if (precision > 120)
    {
        precision = 120;
    }
    circleArcIncrement = precision;
}

void Circle::setCapPrecision(int precision)
{
    if (precision < 1)
    {
        precision = 1;
    }
    if (precision > 180)
    {
        precision = 180;
    }
    circleCapArcIncrement = precision;
}

int Circle::getCapPrecision() const
{
    return circleCapArcIncrement;
}

bool Circle::drawCanvasWidget(const Rect& invalidatedArea) const
{
    int16_t arcStart = circleArcAngleStart;
    int16_t arcEnd = circleArcAngleEnd;

    // Put start before end by swapping
    if (arcStart > arcEnd)
    {
        int16_t tmp = arcStart;
        arcStart = arcEnd;
        arcEnd = tmp;
    }

    if ((arcEnd - arcStart) >= 360)
    {
        // The entire circle has to be drawn
        arcStart = 0;
        arcEnd = 360;
    }

    if (circleLineWidth != 0)
    {
        // Check if invalidated area is completely inside the circle
        CWRUtil::Q5 rMin = circleRadius - (circleLineWidth / 2);
        CWRUtil::Q10 d1_sqr = distanceSquared(circleCenterX, circleCenterY, CWRUtil::toQ5(invalidatedArea.x),       CWRUtil::toQ5(invalidatedArea.y));
        CWRUtil::Q10 d2_sqr = distanceSquared(circleCenterX, circleCenterY, CWRUtil::toQ5(invalidatedArea.right()), CWRUtil::toQ5(invalidatedArea.y));
        CWRUtil::Q10 d3_sqr = distanceSquared(circleCenterX, circleCenterY, CWRUtil::toQ5(invalidatedArea.right()), CWRUtil::toQ5(invalidatedArea.bottom()));
        CWRUtil::Q10 d4_sqr = distanceSquared(circleCenterX, circleCenterY, CWRUtil::toQ5(invalidatedArea.x),       CWRUtil::toQ5(invalidatedArea.bottom()));

        CWRUtil::Q10 dMax_sqr = MAX(MAX(d1_sqr, d2_sqr), MAX(d3_sqr, d4_sqr));

        // Check if invalidatedArea is completely inside circle
        if (dMax_sqr < rMin * rMin)
        {
            return true;
        }
    }

    Canvas canvas(this, invalidatedArea);

    int i;
    moveToAR2(canvas, arcStart, (circleRadius * 2) + circleLineWidth);
    for (i = ROUNDUP(arcStart + 1, circleArcIncrement); i <= arcEnd; i += circleArcIncrement)
    {
        lineToAR2(canvas, i, (circleRadius * 2) + circleLineWidth);
    }
    if (i - circleArcIncrement < arcEnd)
    {
        lineToAR2(canvas, arcEnd, (circleRadius * 2) + circleLineWidth);
    }

    if (circleLineWidth == 0)
    {
        // Draw a filled circle / pie / pacman
        if (arcEnd - arcStart < 360)
        {
            // Not a complete circle, line to center
            canvas.lineTo(circleCenterX, circleCenterY);
        }
    }
    else
    {
        if (arcEnd - arcStart < 360)
        {
            // Draw the circle cap
            CWRUtil::Q5 capX = circleCenterX + (circleRadius * CWRUtil::sine(arcEnd));
            CWRUtil::Q5 capY = circleCenterY - (circleRadius * CWRUtil::cosine(arcEnd));
            for (int capAngle = arcEnd + circleCapArcIncrement; capAngle < arcEnd + 180; capAngle += circleCapArcIncrement)
            {
                lineToXYAR2(canvas, capX, capY, capAngle, circleLineWidth);
            }
        }

        // Not a filled circle, draw the path on the inside of the circle
        if (i - circleArcIncrement < arcEnd)
        {
            lineToAR2(canvas, arcEnd, (circleRadius * 2) - circleLineWidth);
        }

        // Here we have a fixed number of approximation steps, namely 360
        // while in reality it's supposed to be smarter.
        for (i -= circleArcIncrement; i >= arcStart; i -= circleArcIncrement)
        {
            lineToAR2(canvas, i, (circleRadius * 2) - circleLineWidth);
        }
        if (i + circleArcIncrement > arcStart)
        {
            lineToAR2(canvas, arcStart, (circleRadius * 2) - circleLineWidth);
        }

        if (arcEnd - arcStart < 360)
        {
            // Draw the circle cap
            CWRUtil::Q5 capX = circleCenterX + (circleRadius * CWRUtil::sine(arcStart));
            CWRUtil::Q5 capY = circleCenterY - (circleRadius * CWRUtil::cosine(arcStart));
            for (int capAngle = arcStart - 180 + circleCapArcIncrement; capAngle < arcStart; capAngle += circleCapArcIncrement)
            {
                lineToXYAR2(canvas, capX, capY, capAngle, circleLineWidth);
            }
        }
    }

    return canvas.render();
}

Rect Circle::getMinimalRect() const
{
    return getMinimalRect(circleArcAngleStart, circleArcAngleEnd);
}

Rect Circle::getMinimalRect(int16_t arcStart, int16_t arcEnd) const
{
    CWRUtil::Q5 xMin = CWRUtil::toQ5(getWidth());
    CWRUtil::Q5 xMax = CWRUtil::toQ5(0);
    CWRUtil::Q5 yMin = CWRUtil::toQ5(getHeight());
    CWRUtil::Q5 yMax = CWRUtil::toQ5(0);
    calculateMinimalRect(arcStart, arcEnd, xMin, xMax, yMin, yMax);
    return Rect(xMin.to<int>(), yMin.to<int>(),
                xMax.to<int>() - xMin.to<int>() + 1, yMax.to<int>() - yMin.to<int>() + 1);
}

CWRUtil::Q10 Circle::distanceSquared(const CWRUtil::Q5& x1, const CWRUtil::Q5& y1, const CWRUtil::Q5& x2, const CWRUtil::Q5& y2) const
{
    return (((x1) - (x2)) * ((x1) - (x2)) + ((y1) - (y2)) * ((y1) - (y2)));
}

void Circle::moveToAR2(Canvas& canvas, int angle, const CWRUtil::Q5& r2) const
{
    canvas.moveTo(circleCenterX + ((r2 * CWRUtil::sine(angle)) / 2), circleCenterY - ((r2 * CWRUtil::cosine(angle)) / 2));
}

void Circle::lineToAR2(Canvas& canvas, int angle, const CWRUtil::Q5& r2) const
{
    lineToXYAR2(canvas, circleCenterX, circleCenterY, angle, r2);
}

void Circle::lineToXYAR2(Canvas& canvas, const CWRUtil::Q5& x, const CWRUtil::Q5& y, int angle, const CWRUtil::Q5& r2) const
{
    canvas.lineTo(x + ((r2 * CWRUtil::sine(angle)) / 2), y - ((r2 * CWRUtil::cosine(angle)) / 2));
}

void Circle::updateMinMax(int a, const CWRUtil::Q5& r2, CWRUtil::Q5& xMin, CWRUtil::Q5& xMax, CWRUtil::Q5& yMin, CWRUtil::Q5& yMax) const
{
    CWRUtil::Q5 xNew = circleCenterX + ((r2 * CWRUtil::sine(a)) / 2);
    CWRUtil::Q5 yNew = circleCenterY - ((r2 * CWRUtil::cosine(a)) / 2);
    updateMinMax(xNew, yNew, xMin, xMax, yMin, yMax);
}

void Circle::updateMinMax(const CWRUtil::Q5& xNew, const CWRUtil::Q5& yNew, CWRUtil::Q5& xMin, CWRUtil::Q5& xMax, CWRUtil::Q5& yMin, CWRUtil::Q5& yMax) const
{
    if (xNew < xMin)
    {
        xMin = xNew;
    }
    if (xNew > xMax)
    {
        xMax = xNew;
    }
    if (yNew < yMin)
    {
        yMin = yNew;
    }
    if (yNew > yMax)
    {
        yMax = yNew;
    }
}

void Circle::calculateMinimalRect(int16_t arcStart, int16_t arcEnd, CWRUtil::Q5& xMin, CWRUtil::Q5& xMax, CWRUtil::Q5& yMin, CWRUtil::Q5& yMax) const
{
    int i;

    // Put start before end by swapping
    if (arcStart > arcEnd)
    {
        int16_t tmp = arcStart;
        arcStart = arcEnd;
        arcEnd = tmp;
    }

    if ((arcEnd - arcStart) >= 360)
    {
        // The entire circle has to be drawn
        arcStart = 0;
        arcEnd = 360;
    }

    // Check start angle
    updateMinMax(arcStart, (circleRadius * 2) + circleLineWidth, xMin, xMax, yMin, yMax);
    // Here we have a up to 4 approximation steps on angles divisible by 90
    for (i = ROUNDUP(arcStart + 1, 90); i <= arcEnd; i += 90)
    {
        updateMinMax(i, (circleRadius * 2) + circleLineWidth, xMin, xMax, yMin, yMax);
    }
    // Check end angle
    if ((i - 90) < arcEnd)
    {
        updateMinMax(arcEnd, (circleRadius * 2) + circleLineWidth, xMin, xMax, yMin, yMax);
    }

    if (circleLineWidth == 0)
    {
        // A filled circle / pie / pacman
        if ((arcEnd - arcStart) < 360)
        {
            // Not a complete circle, check center
            updateMinMax(0, CWRUtil::toQ5(0), xMin, xMax, yMin, yMax);
        }
    }
    else
    {
        // Not a filled circle, check the inside of the circle. Only star and/or end can cause new min/max values

        updateMinMax(arcStart, (circleRadius * 2) - circleLineWidth, xMin, xMax, yMin, yMax);
        updateMinMax(arcEnd, (circleRadius * 2) - circleLineWidth, xMin, xMax, yMin, yMax);
    }

    // Check if circle cap extends the min/max further
    if ((circleCapArcIncrement < 180) && (arcEnd - arcStart < 360))
    {
        // Round caps
        CWRUtil::Q5 capX = circleCenterX + (circleRadius * CWRUtil::sine(arcStart));
        CWRUtil::Q5 capY = circleCenterY - (circleRadius * CWRUtil::cosine(arcStart));
        updateMinMax(capX - (circleLineWidth / 2), capY - (circleLineWidth / 2), xMin, xMax, yMin, yMax);
        updateMinMax(capX + (circleLineWidth / 2), capY + (circleLineWidth / 2), xMin, xMax, yMin, yMax);
        capX = circleCenterX + (circleRadius * CWRUtil::sine(arcEnd));
        capY = circleCenterY - (circleRadius * CWRUtil::cosine(arcEnd));
        updateMinMax(capX - (circleLineWidth / 2), capY - (circleLineWidth / 2), xMin, xMax, yMin, yMax);
        updateMinMax(capX + (circleLineWidth / 2), capY + (circleLineWidth / 2), xMin, xMax, yMin, yMax);
    }
}

} // namespace touchgfx
