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

/**
 * @file touchgfx/widgets/canvas/Line.hpp
 *
 * Declares the touchgfx::Line class.
 */
#ifndef TOUCHGFX_LINE_HPP
#define TOUCHGFX_LINE_HPP

#include <touchgfx/hal/Types.hpp>
#include <touchgfx/widgets/canvas/CWRUtil.hpp>
#include <touchgfx/widgets/canvas/CanvasWidget.hpp>

namespace touchgfx
{
/**
 * Simple CanvasWidget capable of drawing a line from one point to another point. The end points
 * can be moved to new locations and the line width can be set and changed. A 10 pixel
 * long line along the top of the screen with a width on 1 pixel has endpoints in (0,
 * 0.5) and (10, 0.5) and line width 1. The Line class calculates the corners of the
 * shape, which in this case would be (0, 0), (10, 0), (10, 1) and (0, 1)
 * and tells CanvasWidgetRenderer to moveTo() the first coordinate and then lineTo() the
 * next coordinates in order. Finally it tells CWR to render the inside of the shape
 * using the set Painter object.
 *
 * The Line class caches the four corners of the shape to speed up redrawing. In general,
 * drawing lines involve some extra mathematics for calculating the normal vector of the
 * line and this computation would slow down re-draws if not cached.
 *
 * @note All coordinates are internally handled as CWRUtil::Q5 which means that floating point
 *       values are rounded down to a fixed number of binary digits, for example:
 *       @code
 *            Line line;
 *            line.setStart(1.1f, 1.1f); // Will use (35/32, 35/32) = (1.09375f, 1.09375f)
 *            int x, y;
 *            line.getStart(&x, &y); // Will return (1, 1)
 *       @endcode.
 */
class Line : public CanvasWidget
{
public:
    /** Values that represent line ending styles. */
    enum LINE_ENDING_STYLE
    {
        BUTT_CAP_ENDING,  ///< The line ending is cut 90 degrees at the end of the line
        ROUND_CAP_ENDING, ///< The line ending is rounded as a circle with center at the end of the line
        SQUARE_CAP_ENDING ///< The line ending is cut 90 degrees, but extends half the width of the line
    };

    Line();

    /**
     * Sets the starting point and ending point of the line.
     *
     * @tparam T Generic type parameter, either int or float.
     * @param  startX The x coordinate of the start point.
     * @param  startY The y coordinate of the start point.
     * @param  endX   The x coordinate of the end point.
     * @param  endY   The y coordinate of the end point.
     *
     * @see setStart, setEnd
     *
     * @note The area containing the Line is not invalidated.
     */
    template <typename T>
    void setLine(T startX, T startY, T endX, T endY)
    {
        setStart(startX, startY);
        setEnd(endX, endY);
    }

    /**
     * Sets the starting point of the line.
     *
     * @tparam T Generic type parameter, either int or float.
     * @param  x The x coordinate of the start point.
     * @param  y The y coordinate of the start point.
     *
     * @see updateStart, getStart, setLine, setEnd
     *
     * @note The area containing the Line is not invalidated.
     */
    template <typename T>
    void setStart(T x, T y)
    {
        setStart(CWRUtil::toQ5<T>(x), CWRUtil::toQ5<T>(y));
    }

    /**
     * Sets the starting point of the line.
     *
     * @param  xQ5 The x coordinate of the start point in Q5 format.
     * @param  yQ5 The y coordinate of the start point in Q5 format.
     *
     * @see updateStart, getStart, setLine, setEnd
     *
     * @note The area containing the Line is not invalidated.
     */
    void setStart(CWRUtil::Q5 xQ5, CWRUtil::Q5 yQ5);

    /**
     * Update the start point for this Line. The rectangle that surrounds the line before
     * and after will be invalidated.
     *
     * @tparam T Generic type parameter, either int or float.
     * @param  x The x coordinate of the start point.
     * @param  y The y coordinate of the start point.
     *
     * @see setStart, updateEnd
     *
     * @note The area containing the Line is invalidated before and after the change.
     */
    template <typename T>
    void updateStart(T x, T y)
    {
        updateStart(CWRUtil::toQ5<T>(x), CWRUtil::toQ5<T>(y));
    }

    /**
     * Update the start point for this Line. The rectangle that surrounds the line before
     * and after will be invalidated.
     *
     * @param  xQ5 The x coordinate of the start point in CWRUtil::Q5 format.
     * @param  yQ5 The y coordinate of the start point in CWRUtil::Q5 format.
     *
     * @see setStart, updateEnd
     *
     * @note The area containing the Line is invalidated before and after the change.
     */
    void updateStart(CWRUtil::Q5 xQ5, CWRUtil::Q5 yQ5);

    /**
     * Gets the starting point of the line as either integers or floats.
     *
     * @tparam T Generic type parameter, either int or float.
     * @param [out] x The x coordinate rounded down to the precision of T.
     * @param [out] y The y coordinate rounded down to the precision of T.
     *
     * @see setStart, setLine
     */
    template <typename T>
    void getStart(T& x, T& y) const
    {
        x = startXQ5.to<T>();
        y = startYQ5.to<T>();
    }

    /**
     * Sets the endpoint coordinates of the line.
     *
     * @tparam T Generic type parameter, either int or float.
     * @param  x The x coordinate of the end point.
     * @param  y The y coordinate of the end point.
     *
     * @see updateEnd, getEnd
     *
     * @note The area containing the Line is not invalidated.
     */
    template <typename T>
    void setEnd(T x, T y)
    {
        setEnd(CWRUtil::toQ5<T>(x), CWRUtil::toQ5<T>(y));
    }

    /**
     * Sets the endpoint coordinates of the line.
     *
     * @param  xQ5 The x coordinate of the end point in Q5 format.
     * @param  yQ5 The y coordinate of the end point in Q5 format.
     *
     * @see updateEnd, getEnd
     *
     * @note The area containing the Line is not invalidated.
     */
    void setEnd(CWRUtil::Q5 xQ5, CWRUtil::Q5 yQ5);

    /**
     * Update the endpoint for this Line. The rectangle that surrounds the line before and
     * after will be invalidated.
     *
     * @tparam T Generic type parameter, either int or float.
     * @param  x The x coordinate of the end point.
     * @param  y The y coordinate of the end point.
     *
     * @see setEnd, updateStart
     *
     * @note The area containing the Line is invalidated before and after the change.
     */
    template <typename T>
    void updateEnd(T x, T y)
    {
        CWRUtil::Q5 xQ5 = CWRUtil::toQ5<T>(x);
        CWRUtil::Q5 yQ5 = CWRUtil::toQ5<T>(y);
        updateEnd(xQ5, yQ5);
    }

    /**
     * Update the endpoint for this Line. The rectangle that surrounds the line before and
     * after will be invalidated.
     *
     * @param  xQ5 The x coordinate of the end point in Q5 format.
     * @param  yQ5 The y coordinate of the end point in Q5 format.
     *
     * @see setEnd, updateStart
     *
     * @note The area containing the Line is invalidated before and after the change.
     */
    void updateEnd(CWRUtil::Q5 xQ5, CWRUtil::Q5 yQ5);

    /**
     * Gets the endpoint coordinates for the line.
     *
     * @tparam T Generic type parameter, either int or float.
     * @param [out] x The x coordinate rounded down to the precision of T.
     * @param [out] y The y coordinate rounded down to the precision of T.
     *
     * @see setEnd, updateEnd
     */
    template <typename T>
    void getEnd(T& x, T& y) const
    {
        x = endXQ5.to<T>();
        y = endYQ5.to<T>();
    }

    /**
     * Sets the width for this Line.
     *
     * @tparam T Generic type parameter, either int or float.
     * @param  width The width of the line measured in pixels.
     *
     * @see updateLineWidth
     *
     * @note The area containing the Line is not invalidated.
     */
    template <typename T>
    void setLineWidth(T width)
    {
        setLineWidth(CWRUtil::toQ5<T>(width));
    }

    /**
     * Sets the width for this Line.
     *
     * @param  widthQ5 The width of the line measured in pixels in Q5 format.
     *
     * @see updateLineWidth
     *
     * @note The area containing the Line is not invalidated.
     */
    void setLineWidth(CWRUtil::Q5 widthQ5)
    {
        if (lineWidthQ5 == widthQ5)
        {
            return;
        }

        lineWidthQ5 = widthQ5;

        updateCachedShape();
    }

    /**
     * Update the width for this Line and invalidates the minimal rectangle surrounding the
     * line on screen.
     *
     * @tparam T Generic type parameter, either int or float.
     * @param  width The width of the line measured in pixels.
     *
     * @see setLineWidth
     *
     * @note The area containing the Line is invalidated before and after the change.
     */
    template <typename T>
    void updateLineWidth(T width)
    {
        updateLineWidth(CWRUtil::toQ5<T>(width));
    }

    /**
     * Update the width for this Line.
     *
     * Update the width for this Line and invalidates the minimal rectangle surrounding the
     * line on screen.
     *
     * @param  widthQ5 The width of the line measured in pixels in Q5 format.
     *
     * @see setLineWidth
     *
     * @note The area containing the Line is invalidated before and after the change.
     */
    void updateLineWidth(CWRUtil::Q5 widthQ5)
    {
        if (lineWidthQ5 == widthQ5)
        {
            return;
        }

        Rect rectBefore = getMinimalRect();
        invalidateRect(rectBefore);

        lineWidthQ5 = widthQ5;

        updateCachedShape();

        Rect rectAfter = getMinimalRect();
        invalidateRect(rectAfter);
    }

    /**
     * Gets line width.
     *
     * @tparam T Generic type parameter, either int or float.
     * @param [out] width The line width rounded down to the precision of T.
     *
     * @see setLineWidth
     */
    template <typename T>
    void getLineWidth(T& width) const
    {
        width = lineWidthQ5.to<T>();
    }

    /**
     * Gets line width.
     *
     * @tparam T Generic type parameter, either int or float.
     *
     * @return The line width rounded down to the precision of T.
     *
     * @see setLineWidth
     */
    template <typename T>
    T getLineWidth() const
    {
        return lineWidthQ5.to<T>();
    }

    /**
     * Sets line ending style. The same style is applied to both ends of the line.
     *
     * @param  lineEnding The line ending style.
     *
     * @see LINE_ENDING_STYLE, getLineEndingStyle
     *
     * @note The area containing the Line is not invalidated.
     */
    void setLineEndingStyle(LINE_ENDING_STYLE lineEnding);

    /**
     * Gets line ending style.
     *
     * @return The line ending style.
     *
     * @see LINE_ENDING_STYLE, setLineEndingStyle
     */
    LINE_ENDING_STYLE getLineEndingStyle() const;

    /**
     * Sets a precision of the arc at the ends of the Line. This only works for
     * ROUND_CAP_ENDING. The precision is given in degrees where 18 is the default which
     * results in a nice half circle with 10 line segments. 90 will draw "an arrow head",
     * 180 will look exactly like a BUTT_CAP_ENDING.
     *
     * @param  precision The new ROUND_CAP_ENDING precision.
     *
     * @note The line is not invalidated.
     * @note This is only used if line ending is set to ROUND_CAP_ENDING.
     */
    void setCapPrecision(int precision);

    virtual bool drawCanvasWidget(const Rect& invalidatedArea) const;

    virtual Rect getMinimalRect() const;

    /**
     * Update the end point for this Line given the new length and angle in degrees. The rectangle
     * that surrounds the line before and after will be invalidated. The starting coordinates will
     * be fixed but the ending point will be updated. This is simply a different way to update the
     * ending point.
     *
     * @param   length  The new length of the line in Q5 format.
     * @param   angle   The new angle of the line in Q5 format.
     *
     * @see updateEnd
     *
     * @note    The area containing the Line is invalidated before and after the change.
     * @note    Angles are given in degrees, so a full circle is 360.
     */
    void updateLengthAndAngle(CWRUtil::Q5 length, CWRUtil::Q5 angle);

    virtual void invalidateContent() const;

private:
    CWRUtil::Q5 startXQ5;
    CWRUtil::Q5 startYQ5;
    CWRUtil::Q5 endXQ5;
    CWRUtil::Q5 endYQ5;
    CWRUtil::Q5 lineWidthQ5;
    LINE_ENDING_STYLE lineEnding;
    CWRUtil::Q5 cornerXQ5[4];
    CWRUtil::Q5 cornerYQ5[4];
    Rect minimalRect;
    int lineCapArcIncrement;

    void updateCachedShape();

    Rect rectContainingPoints(const Rect& fullRect, CWRUtil::Q5 x0, CWRUtil::Q5 y0, CWRUtil::Q5 x1, CWRUtil::Q5 y1, CWRUtil::Q5 x2, CWRUtil::Q5 y2) const;
};

} // namespace touchgfx

#endif // TOUCHGFX_LINE_HPP
