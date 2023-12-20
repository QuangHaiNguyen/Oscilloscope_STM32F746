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
#ifndef LINE_HPP
#define LINE_HPP

#include <touchgfx/hal/Types.hpp>
#include <touchgfx/widgets/Widget.hpp>

#include <touchgfx/widgets/canvas/Canvas.hpp>
#include <touchgfx/widgets/canvas/CanvasWidget.hpp>

namespace touchgfx
{
/**
 * @class Line Line.hpp touchgfx/widgets/canvas/Line.hpp
 *
 * @brief Simple CanvasWidget capable of drawing a line.
 *
 *        Simple CanvasWidget capable of drawing a line from one point to another point. The
 *        end points can be moved to new locations and the line width can easily be set and
 *        changed. A 10 pixel long line along the top of the screen with a width on 1 pixel has
 *        endpoints in (0, 0.5) and (10, 0.5) and line width 1. The Line class calculates the
 *        corners of the shape, which in this case would be (0, 0), (10, 0), (10, 1) and (0, 1)
 *        and tells CWR to moveTo the first coordinate and then lineTo the next coordinates in
 *        order. Finally it tells CWR to render the inside of the shape using a Painter object.
 *
 *        The Line class caches the four corners of the shape to speed up redrawing. In general,
 *        drawing lines involve some extra mathematics for calculating the normal vector of the
 *        line and this computation would slow down re-draws if not cached.
 *
 * @see CanvasWidget
 */
class Line : public CanvasWidget
{
public:

    /**
     * @enum LINE_ENDING_STYLE
     *
     * @brief Values that represent line ending styles.
     */
    enum LINE_ENDING_STYLE
    {
        BUTT_CAP_ENDING,  ///< The line ending is cut 90 degrees at the end of the line
        ROUND_CAP_ENDING, ///< The line ending is rounded as a circle with center at the end of the line
        SQUARE_CAP_ENDING ///< The line ending is cut 90 degrees, but extends half the width of the line
    } ;

    /**
     * @fn Line::Line();
     *
     * @brief Construct a new Line.
     *
     *        Construct a new line.
     */
    Line();

    /**
     * @fn template <class T> void Line::setLine(T x1, T y1, T x2, T y2)
     *
     * @brief Sets the endpoints of the line.
     *
     *        Sets the endpoints of the line.
     *
     * @note The area containing the Line is not invalidated.
     *
     * @tparam T Generic type parameter, either int or float.
     * @param x1 The x coordinate of the start point.
     * @param y1 The y coordinate of the start point.
     * @param x2 The x coordinate of the end point.
     * @param y2 The y coordinate of the end point.
     *
     * @see setStart
     * @see setEnd
     */
    template <class T>
    void setLine(T x1, T y1, T x2, T y2)
    {
        setStart(x1, y1);
        setEnd(x2, y2);
    }

    /**
     * @fn template <class T> void Line::setStart(T x, T y)
     *
     * @brief Sets the start point for this Line.
     *
     *        Sets the start point for this Line.
     *
     * @note The area containing the Line is not invalidated.
     *
     * @tparam T Generic type parameter, either int or float.
     * @param x The x coordinate of the start point.
     * @param y The y coordinate of the start point.
     *
     * @see updateStart
     * @see getStart
     * @see setLine
     * @see setEnd
     */
    template <class T>
    void setStart(T x, T y)
    {
        setStart(CWRUtil::toQ5<T>(x), CWRUtil::toQ5<T>(y));
    }

    /**
     * @fn void setStart(CWRUtil::Q5 xQ5, CWRUtil::Q5 yQ5);
     *
     * @brief Sets the start point for this Line.
     *
     *        Sets the start point for this Line.
     *
     * @param xQ5 The x coordinate of the start point in Q5 format.
     * @param yQ5 The y coordinate of the start point in Q5 format.
     *
     * @note The area containing the Line is not invalidated.
     *
     * @see updateStart
     * @see getStart
     * @see setLine
     * @see setEnd
     */
    void setStart(CWRUtil::Q5 xQ5, CWRUtil::Q5 yQ5);

    /**
     * @fn template <class T> void Line::updateStart(T x, T y)
     *
     * @brief Update the start point for this Line.
     *
     *        Update the start point for this Line. The rectangle that surrounds the line
     *        before and after will be invalidated.
     *
     * @note The area containing the Line is invalidated before and after the change.
     *
     * @tparam T Generic type parameter, either int or float.
     * @param x The x coordinate of the start point.
     * @param y The y coordinate of the start point.
     *
     * @see setStart
     * @see updateEnd
     */
    template <class T>
    void updateStart(T x, T y)
    {
        updateStart(CWRUtil::toQ5<T>(x), CWRUtil::toQ5<T>(y));
    }

    /**
     * @fn void updateStart(CWRUtil::Q5 xQ5, CWRUtil::Q5 yQ5);
     *
     * @brief Update the start point for this Line.
     *
     *        Update the start point for this Line. The rectangle that surrounds the line
     *        before and after will be invalidated.
     *
     * @param xQ5 The x coordinate of the start point in Q5 format.
     * @param yQ5 The y coordinate of the start point in Q5 format.
     *
     * @note The area containing the Line is invalidated before and after the change.
     *
     * @see setStart
     * @see updateEnd
     */
    void updateStart(CWRUtil::Q5 xQ5, CWRUtil::Q5 yQ5);

    /**
     * @fn template <class T> void Line::getStart(T& x, T& y) const
     *
     * @brief Gets the start coordinates for the line.
     *
     *        Gets the start coordinates for the line.
     *
     * @tparam T Generic type parameter, either int or float.
     * @param [out] x The x coordinate.
     * @param [out] y The y coordinate.
     *
     * @see setStart
     * @see setLine
     */
    template <class T>
    void getStart(T& x, T& y) const
    {
        x = int(x1) / T(Rasterizer::POLY_BASE_SIZE);
        y = int(y1) / T(Rasterizer::POLY_BASE_SIZE);
    }

    /**
     * @fn template <class T> void Line::setEnd(T x, T y)
     *
     * @brief Sets the end point for this Line.
     *
     *        Sets the end point for this Line.
     *
     * @note The area containing the Line is not invalidated.
     *
     * @tparam T Generic type parameter, either int or float.
     * @param x The x coordinate of the end point.
     * @param y The y coordinate of the end point.
     *
     * @see updateEnd
     * @see getEnd
     */
    template <class T>
    void setEnd(T x, T y)
    {
        setEnd(CWRUtil::toQ5<T>(x), CWRUtil::toQ5<T>(y));
    }

    /**
     * @fn void setEnd(CWRUtil::Q5 xQ5, CWRUtil::Q5 yQ5);
     *
     * @brief Sets the end point for this Line.
     *
     *        Sets the end point for this Line.
     *
     * @param xQ5 The x coordinate of the end point in Q5 format.
     * @param yQ5 The y coordinate of the end point in Q5 format.
     *
     * @note The area containing the Line is not invalidated.
     *
     * @see updateEnd
     * @see getEnd
     */
    void setEnd(CWRUtil::Q5 xQ5, CWRUtil::Q5 yQ5);

    /**
     * @fn template <class T> void Line::updateEnd(T x, T y)
     *
     * @brief Update the end point for this Line.
     *
     *        Update the end point for this Line. The rectangle that surrounds the line before
     *        and after will be invalidated.
     *
     * @note The area containing the Line is invalidated before and after the change.
     *
     * @tparam T Generic type parameter, either int or float.
     * @param x The x coordinate of the end point.
     * @param y The y coordinate of the end point.
     */
    template <class T>
    void updateEnd(T x, T y)
    {
        CWRUtil::Q5 xQ5 = CWRUtil::toQ5<T>(x);
        CWRUtil::Q5 yQ5 = CWRUtil::toQ5<T>(y);
        updateEnd(xQ5, yQ5);
    }

    /**
     * @fn void updateEnd(CWRUtil::Q5 xQ5, CWRUtil::Q5 yQ5);
     *
     * @brief Update the end point for this Line.
     *
     *        Update the end point for this Line. The rectangle that surrounds the line before
     *        and after will be invalidated.
     *
     * @param xQ5 The x coordinate of the end point in Q5 format.
     * @param yQ5 The y coordinate of the end point in Q5 format.
     *
     * @note The area containing the Line is invalidated before and after the change.
     */
    void updateEnd(CWRUtil::Q5 xQ5, CWRUtil::Q5 yQ5);

    /**
     * @fn template <class T> void Line::getEnd(T& x, T& y) const
     *
     * @brief Gets the end coordinates for the line.
     *
     *        Gets the end coordinates for the line.
     *
     * @tparam T Generic type parameter, either int or float.
     * @param [out] x The x coordinate.
     * @param [out] y The y coordinate.
     *
     * @see setEnd
     * @see setLine
     */
    template <class T>
    void getEnd(T& x, T& y) const
    {
        x = (int)x2 / T(Rasterizer::POLY_BASE_SIZE);
        y = (int)y2 / T(Rasterizer::POLY_BASE_SIZE);
    }

    /**
     * @fn template <class T> void Line::setLineWidth(T width)
     *
     * @brief Sets the width for this Line.
     *
     *        Sets the width for this Line.
     *
     * @note The area containing the Line is not invalidated.
     *
     * @tparam T Generic type parameter, either int or float.
     * @param width The width of the line measured in pixels.
     *
     * @see updateLineWidth
     */
    template <class T>
    void setLineWidth(T width)
    {
        setLineWidth(CWRUtil::toQ5<T>(width));
    }

    /**
     * @fn void setLineWidth(CWRUtil::Q5 widthQ5)
     *
     * @brief Sets the width for this Line.
     *
     *        Sets the width for this Line.
     *
     * @param widthQ5 The width of the line measured in pixels in Q5 format.
     *
     * @note The area containing the Line is not invalidated.
     *
     * @see updateLineWidth
     */
    void setLineWidth(CWRUtil::Q5 widthQ5)
    {
        if (lineWidth == widthQ5)
        {
            return;
        }

        lineWidth = widthQ5;

        updateCachedShape();
    }

    /**
     * @fn template <class T> void Line::updateLineWidth(T width)
     *
     * @brief Update the width for this Line.
     *
     *        Update the width for this Line and invalidates the minimal rectangle surrounding
     *        the line on screen.
     *
     * @note The area containing the Line is invalidated before and after the change.
     *
     * @tparam T Generic type parameter, either int or float.
     * @param width The width of the line measured in pixels.
     *
     * @see setLineWidth
     */
    template <class T>
    void updateLineWidth(T width)
    {
        updateLineWidth(CWRUtil::toQ5<T>(width));
    }

    /**
     * @fn void updateLineWidth(CWRUtil::Q5 widthQ5)
     *
     * @brief Update the width for this Line.
     *
     *        Update the width for this Line and invalidates the minimal rectangle surrounding
     *        the line on screen.
     *
     * @param widthQ5 The width of the line measured in pixels in Q5 format.
     *
     * @note The area containing the Line is invalidated before and after the change.
     *
     * @see setLineWidth
     */
    void updateLineWidth(CWRUtil::Q5 widthQ5)
    {
        if (lineWidth == widthQ5)
        {
            return;
        }

        Rect rectBefore = getMinimalRect();

        lineWidth = widthQ5;

        updateCachedShape();

        Rect rectAfter = getMinimalRect();
        rectBefore.expandToFit(rectAfter);
        invalidateRect(rectBefore);
    }

    /**
     * @fn template <class T> void Line::getLineWidth(T& width) const
     *
     * @brief Gets line width.
     *
     *        Gets line width.
     *
     * @tparam T Generic type parameter, either int or float.
     * @param [out] width The line width.
     *
     * @see setLineWidth
     */
    template <class T>
    void getLineWidth(T& width) const
    {
        width = int(lineWidth) / T(Rasterizer::POLY_BASE_SIZE);
    }

    /**
     * @fn void Line::setLineEndingStyle(LINE_ENDING_STYLE lineEnding);
     *
     * @brief Sets line ending style.
     *
     *        Sets line ending style. See LINE_ENDING_STYLE for possible styles.
     *
     * @note The area containing the Line is not invalidated.
     *
     * @param lineEnding The line ending style.
     *
     * @see LINE_ENDING_STYLE
     * @see getLineEndingStyle
     */
    void setLineEndingStyle(LINE_ENDING_STYLE lineEnding);

    /**
     * @fn LINE_ENDING_STYLE Line::getLineEndingStyle() const;
     *
     * @brief Gets line ending style.
     *
     *        Gets line ending style. See LINE_ENDING_STYLE for possible styles.
     *
     * @return The line ending style.
     *
     * @see LINE_ENDING_STYLE
     * @see setLineEndingStyle
     */
    LINE_ENDING_STYLE getLineEndingStyle() const;

    /**
     * @fn void Line::setCapPrecision(int precision);
     *
     * @brief Sets a precision of the ends of the Line.
     *
     *        Sets a precision of the ends of the Line arc. The precision is given in degrees
     *        where 18 is the default which results in a nice half circle with 10 line
     *        segments. 90 will draw "an arrow head".
     *
     * @note The line is not invalidated.
     * @note This is only used if line ending is set to ROUND_CAP_ENDING.
     *
     * @param precision The new cap precision.
     */
    void setCapPrecision(int precision);

    /**
     * @fn virtual bool Line::drawCanvasWidget(const Rect& invalidatedArea) const;
     *
     * @brief Draws the Line.
     *
     *        Draws the Line. This class supports partial drawing, so only the area described
     *        by the rectangle will be drawn. As the corners of the shape are cached, the line
     *        can quickly be redrawn when required.
     *
     * @param invalidatedArea The rectangle to draw, with coordinates relative to this drawable.
     *
     * @return true if it succeeds, false if it fails.
     */
    virtual bool drawCanvasWidget(const Rect& invalidatedArea) const;

    /**
     * @fn virtual Rect Line::getMinimalRect() const;
     *
     * @brief Gets minimal rectangle containing the shape drawn by this widget.
     *
     *        Gets minimal rectangle containing the shape drawn by this widget. Default
     *        implementation returns the size of the entire widget, but this function should be
     *        overwritten in subclasses and return the minimal rectangle containing the shape.
     *        See classes such as Circle for example implementations.
     *
     * @return The minimal rectangle containing the shape drawn by this widget.
     */
    virtual Rect getMinimalRect() const;

private:
    CWRUtil::Q5 x1, y1, x2, y2;
    CWRUtil::Q5 lineWidth;
    LINE_ENDING_STYLE lineEnding;
    CWRUtil::Q5 xCorner[4], yCorner[4];
    CWRUtil::Q5 xMin, yMin, xMax, yMax;
    int lineCapArcIncrement;

    void updateCachedShape();
};

} // namespace touchgfx

#endif // LINE_HPP
