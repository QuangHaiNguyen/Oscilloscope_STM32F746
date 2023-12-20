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
#ifndef SHAPE_HPP
#define SHAPE_HPP

#include <touchgfx/hal/Types.hpp>
#include <touchgfx/widgets/Widget.hpp>

#include <touchgfx/widgets/canvas/Canvas.hpp>
#include <touchgfx/widgets/canvas/CanvasWidget.hpp>
#include <touchgfx/widgets/canvas/AbstractShape.hpp>

namespace touchgfx
{
/**
 * @class Shape Shape.hpp touchgfx/widgets/canvas/Shape.hpp
 *
 * @brief Simple widget capable of drawing a shape.
 *
 *        Simple widget capable of drawing a shape. The shape can be scaled and rotated around
 *        0,0. Note that the  y axis goes down, so a shape that goes up must be given negative
 *        coordinates.
 *
 *        The Shape class allows the user to draw any shape and allows the defined shape to be
 *        scaled, rotated and moved freely. Example uses could be the hands of a clock (see the
 *        touchgfx_demo2014 for an actual implementation).
 *
 * @tparam POINTS The number of points in the given shape.
 * @tparam T      The type of the points used for the shape. Must be int or float.
 *
 * @see CanvasWidget
 */
template <uint16_t POINTS>
class Shape : public AbstractShape
{
public:

    /**
     * @fn virtual Shape::~Shape()
     *
     * @brief Virtual Destructor.
     *
     *        Virtual Destructor.
     */
    virtual ~Shape()
    {
    }

    /**
     * @fn virtual int Shape::getNumPoints() const
     *
     * @brief Gets number points used to make up the shape.
     *
     *        Gets number points used to make up the shape.
     *
     * @return The number points.
     */
    virtual int getNumPoints() const
    {
        return POINTS;
    }

    /**
     * @fn virtual void Shape::setCorner(int i, CWRUtil::Q5 x, CWRUtil::Q5 y)
     *
     * @brief Sets a corner of the shape in Q5 format.
     *
     *        Sets a corner of the shape in Q5 format.
     *
     * @param i Zero-based index of the corner.
     * @param x The x coordinate in Q5 format.
     * @param y The y coordinate in Q5 format.
     */
    virtual void setCorner(int i, CWRUtil::Q5 x, CWRUtil::Q5 y)
    {
        if (i >= 0 && i < POINTS)
        {
            xCorner[i] = x, yCorner[i] = y;
        }
    }

    /**
     * @fn virtual CWRUtil::Q5 Shape::getCornerX(int i) const
     *
     * @brief Gets the x coordinate of a corner.
     *
     *        Gets the x coordinate of a corner.
     *
     * @param i Zero-based index of the corner.
     *
     * @return The corner x coordinate.
     */
    virtual CWRUtil::Q5 getCornerX(int i) const
    {
        if (i >= 0 && i < POINTS)
        {
            return xCorner[i];
        }
        return CWRUtil::toQ5(0);
    }

    /**
     * @fn virtual CWRUtil::Q5 Shape::getCornerY(int i) const
     *
     * @brief Gets the y coordinate of a corner.
     *
     *        Gets the y coordinate of a corner.
     *
     * @param i Zero-based index of the corner.
     *
     * @return The corner y coordinate.
     */
    virtual CWRUtil::Q5 getCornerY(int i) const
    {
        if (i >= 0 && i < POINTS)
        {
            return yCorner[i];
        }
        return CWRUtil::toQ5(0);
    }

    /**
     * @fn virtual void Shape::setCache(int i, CWRUtil::Q5 x, CWRUtil::Q5 y)
     *
     * @brief Sets the cached coordinates of a given corner.
     *
     *        Sets the cached coordinates of a given corner. The coordinates in the cache are
     *        the coordinates from the corners after rotating and scaling the coordinate.
     *
     * @param i Zero-based index of the corner.
     * @param x The x coordinate.
     * @param y The y coordinate.
     */
    virtual void setCache(int i, CWRUtil::Q5 x, CWRUtil::Q5 y)
    {
        if (i >= 0 && i < POINTS)
        {
            xCache[i] = x, yCache[i] = y;
        }
    }

    /**
     * @fn virtual CWRUtil::Q5 Shape::getCacheX(int i) const
     *
     * @brief Gets cached x coordinate of a corner.
     *
     *        Gets cached x coordinate of a corner.
     *
     * @param i Zero-based index of the corner.
     *
     * @return The cached x coordinate.
     */
    virtual CWRUtil::Q5 getCacheX(int i) const
    {
        if (i >= 0 && i < POINTS)
        {
            return xCache[i];
        }
        return CWRUtil::toQ5(0);
    }

    /**
     * @fn virtual CWRUtil::Q5 Shape::getCacheY(int i) const
     *
     * @brief Gets cached y coordinate of a corner.
     *
     *        Gets cached y coordinate of a corner.
     *
     * @param i Zero-based index of the corner.
     *
     * @return The cached y coordinate.
     */
    virtual CWRUtil::Q5 getCacheY(int i) const
    {
        if (i >= 0 && i < POINTS)
        {
            return yCache[i];
        }
        return CWRUtil::toQ5(0);
    }

private:
    CWRUtil::Q5 xCorner[POINTS];
    CWRUtil::Q5 yCorner[POINTS];
    CWRUtil::Q5 xCache[POINTS];
    CWRUtil::Q5 yCache[POINTS];
};

} // namespace touchgfx

#endif // SHAPE_HPP
