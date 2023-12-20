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
#include <touchgfx/widgets/canvas/Line.hpp>

namespace touchgfx
{
Line::Line() : CanvasWidget(),
    x1(0), y1(0), x2(0), y2(0),
    lineWidth(CWRUtil::toQ5<int>(1)),
    lineEnding(BUTT_CAP_ENDING),
    lineCapArcIncrement(18)
{
    Drawable::setWidth(0);
    Drawable::setHeight(0);
}

void Line::setStart(CWRUtil::Q5 xQ5, CWRUtil::Q5 yQ5)
{
    if (x1 == xQ5 && y1 == yQ5)
    {
        return;
    }

    x1 = xQ5;
    y1 = yQ5;

    updateCachedShape();
}

void Line::updateStart(CWRUtil::Q5 xQ5, CWRUtil::Q5 yQ5)
{
    if (x1 == xQ5 && y1 == yQ5)
    {
        return;
    }

    Rect rectBefore = getMinimalRect();

    x1 = xQ5;
    y1 = yQ5;

    updateCachedShape();

    Rect rectAfter = getMinimalRect();
    rectBefore.expandToFit(rectAfter);
    invalidateRect(rectBefore);
}

void Line::setEnd(CWRUtil::Q5 xQ5, CWRUtil::Q5 yQ5)
{
    if (x2 == xQ5 && y2 == yQ5)
    {
        return;
    }

    x2 = xQ5;
    y2 = yQ5;

    updateCachedShape();
}

void Line::updateEnd(CWRUtil::Q5 xQ5, CWRUtil::Q5 yQ5)
{
    if (x2 == xQ5 && y2 == yQ5)
    {
        return;
    }

    Rect rectBefore = getMinimalRect();

    x2 = xQ5;
    y2 = yQ5;

    updateCachedShape();

    Rect rectAfter = getMinimalRect();
    rectBefore.expandToFit(rectAfter);
    invalidateRect(rectBefore);
}

void Line::setLineEndingStyle(Line::LINE_ENDING_STYLE lineEndingStyle)
{
    lineEnding = lineEndingStyle;
    updateCachedShape();
}

Line::LINE_ENDING_STYLE Line::getLineEndingStyle() const
{
    return lineEnding;
}

void Line::setCapPrecision(int precision)
{
    if (precision < 1)
    {
        precision = 1;
    }
    if (precision > 180)
    {
        precision = 180;
    }
    lineCapArcIncrement = precision;
}

bool Line::drawCanvasWidget(const Rect& invalidatedArea) const
{
    Canvas canvas(this, invalidatedArea);

    CWRUtil::Q5 radius;
    int angleInDegrees = CWRUtil::angle(xCorner[0] - x1, yCorner[0] - y1 , radius);

    canvas.moveTo(xCorner[0], yCorner[0]);
    canvas.lineTo(xCorner[1], yCorner[1]);
    if (lineEnding == ROUND_CAP_ENDING)
    {
        // Fixed 10 steps (steps 0 and 9 are at Corner[1] and [2])
        for (int i = lineCapArcIncrement; i < 180; i += lineCapArcIncrement)
        {
            canvas.lineTo(x2 + radius * CWRUtil::sine(angleInDegrees - i), y2 - radius * CWRUtil::cosine(angleInDegrees - i));
        }
    }
    canvas.lineTo(xCorner[2], yCorner[2]);
    canvas.lineTo(xCorner[3], yCorner[3]);
    if (lineEnding == ROUND_CAP_ENDING)
    {
        // Fixed 10 steps (steps 0 and 9 are at Corner[3] and [0])
        for (int i = 180 - lineCapArcIncrement; i > 0; i -= lineCapArcIncrement)
        {
            canvas.lineTo(x1 + radius * CWRUtil::sine(angleInDegrees + i), y1 - radius * CWRUtil::cosine(angleInDegrees + i));
        }
    }

    return canvas.render();
}

void Line::updateCachedShape()
{
    CWRUtil::Q5 dx = x2 - x1;
    CWRUtil::Q5 dy = y2 - y1;
    CWRUtil::Q5 d = CWRUtil::sqrtQ10(dy * dy + dx * dx);

    if (d == 0)
    {
        xCorner[0] = xCorner[1] = xCorner[2] = xCorner[3] = x1;
        yCorner[0] = yCorner[1] = yCorner[2] = yCorner[3] = y1;
        return;
    }

    dy = (lineWidth * dy / d) / 2;
    dx = (lineWidth * dx / d) / 2;

    switch (lineEnding)
    {
    case BUTT_CAP_ENDING:
        xCorner[0] = x1 - dy;
        yCorner[0] = y1 + dx;
        xCorner[1] = x2 - dy;
        yCorner[1] = y2 + dx;
        xCorner[2] = x2 + dy;
        yCorner[2] = y2 - dx;
        xCorner[3] = x1 + dy;
        yCorner[3] = y1 - dx;
        break;
    case ROUND_CAP_ENDING:
    // Nothing cached, calculated on each draw, but extremes are same as SQUARE_CAP_ENDING, so
    // Fall Through (for calculations)
    default:
    case SQUARE_CAP_ENDING:
        xCorner[0] = x1 - dy - dx;
        yCorner[0] = y1 + dx - dy;
        xCorner[1] = x2 - dy + dx;
        yCorner[1] = y2 + dx + dy;
        xCorner[2] = x2 + dy + dx;
        yCorner[2] = y2 - dx + dy;
        xCorner[3] = x1 + dy - dx;
        yCorner[3] = y1 - dx - dy;
        break;
    }

    xMin = xCorner[0];
    xMax = xCorner[0];
    yMin = yCorner[0];
    yMax = yCorner[0];
    for (int i = 1 ; i < 4; i++)
    {
        if (xCorner[i] < xMin)
        {
            xMin = xCorner[i];
        }
        if (xCorner[i] > xMax)
        {
            xMax = xCorner[i];
        }
        if (yCorner[i] < yMin)
        {
            yMin = yCorner[i];
        }
        if (yCorner[i] > yMax)
        {
            yMax = yCorner[i];
        }
    }
    if (lineEnding == ROUND_CAP_ENDING)
    {
        xCorner[0] = x1 - dy;
        yCorner[0] = y1 + dx;
        xCorner[1] = x2 - dy;
        yCorner[1] = y2 + dx;
        xCorner[2] = x2 + dy;
        yCorner[2] = y2 - dx;
        xCorner[3] = x1 + dy;
        yCorner[3] = y1 - dx;
    }
}

Rect Line::getMinimalRect() const
{
    int minX = xMin.to<int>();
    int minY = yMin.to<int>();
    int maxX = xMax.to<int>();
    int maxY = yMax.to<int>();

    return Rect(minX, minY, maxX - minX + 1, maxY - minY + 1);
}

} // namespace touchgfx
