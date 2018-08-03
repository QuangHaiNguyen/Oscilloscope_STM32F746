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
#include <touchgfx/widgets/canvas/AbstractShape.hpp>
#include <touchgfx/widgets/canvas/Canvas.hpp>

namespace touchgfx
{

AbstractShape::AbstractShape() : CanvasWidget(),
    dx(0), dy(0), shapeAngle(0),
    xScale(CWRUtil::toQ5<int>(1)), yScale(CWRUtil::toQ5<int>(1))
{
    Drawable::setWidth(0);
    Drawable::setHeight(0);
}

AbstractShape::~AbstractShape()
{
}

void AbstractShape::setAngle(int angle)
{
    if (shapeAngle != angle)
    {
        shapeAngle = angle;
        updateAbstractShapeCache();
    }
}

void AbstractShape::updateAngle(int angle)
{
    if (shapeAngle != angle)
    {
        Rect rectBefore = getMinimalRect();

        shapeAngle = angle;
        updateAbstractShapeCache();

        Rect rectAfter = getMinimalRect();
        rectBefore.expandToFit(rectAfter);
        invalidateRect(rectBefore);
    }
}

int AbstractShape::getAngle() const
{
    return shapeAngle;
}

bool AbstractShape::drawCanvasWidget(const Rect& invalidatedArea) const
{
    Canvas canvas(this, invalidatedArea);
    int numPoints = getNumPoints();
    if (numPoints == 0)
    {
        return true;
    }

    canvas.moveTo(getCacheX(0), getCacheY(0));
    for (int i = 1; i < numPoints; i++)
    {
        canvas.lineTo(getCacheX(i), getCacheY(i));
    }
    return canvas.render();
}

void AbstractShape::updateAbstractShapeCache()
{
    int numPoints = getNumPoints();

    for (int i = 0; i < numPoints; i++)
    {
        CWRUtil::Q5 xCorner = getCornerX(i);
        CWRUtil::Q5 yCorner = getCornerY(i);

        CWRUtil::Q5 xCache = dx + ((CWRUtil::Q5(xCorner * xScale) * CWRUtil::cosine(shapeAngle))) - ((CWRUtil::Q5(yCorner * yScale) * CWRUtil::sine(shapeAngle)));
        if (i == 0 || xCache > xMax)
        {
            xMax = xCache;
        }
        if (i == 0 || xCache < xMin)
        {
            xMin = xCache;
        }
        CWRUtil::Q5 yCache = dy + ((CWRUtil::Q5(yCorner * yScale) * CWRUtil::cosine(shapeAngle))) + ((CWRUtil::Q5(xCorner * xScale) * CWRUtil::sine(shapeAngle)));
        if (i == 0 || yCache > yMax)
        {
            yMax = yCache;
        }
        if (i == 0 || yCache < yMin)
        {
            yMin = yCache;
        }
        setCache(i, xCache, yCache);
    }
}

Rect AbstractShape::getMinimalRect() const
{
    int minX = xMin.to<int>();
    int minY = yMin.to<int>();
    int maxX = xMax.to<int>();
    int maxY = yMax.to<int>();

    return Rect(minX, minY, maxX - minX + 1, maxY - minY + 1);
}

} // namespace touchgfx
