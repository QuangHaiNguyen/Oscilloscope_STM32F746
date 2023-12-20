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
#ifndef CANVAS_HPP
#define CANVAS_HPP

#include <touchgfx/transforms/DisplayTransformation.hpp>
#include <touchgfx/Bitmap.hpp>
#include <touchgfx/widgets/Widget.hpp>
#include <touchgfx/widgets/canvas/CWRUtil.hpp>
#include <touchgfx/widgets/canvas/AbstractPainter.hpp>
#include <touchgfx/widgets/canvas/CanvasWidget.hpp>
#include <touchgfx/canvas_widget_renderer/Rasterizer.hpp>
#include <touchgfx/hal/HAL.hpp>

namespace touchgfx
{
/**
 * @class Canvas Canvas.hpp touchgfx/widgets/canvas/Canvas.hpp
 *
 * @brief Class for easy rendering using CanvasWidgetRenderer.
 *
 *        The Canvas class will make implementation of a new CanvasWidget very easy. The few
 *        simple primitives allows moving a "pen" and drawing the outline of a shape which can
 *        then be rendered.
 *
 *        The Canvas class has been optimized to eliminate drawing unnecessary lines above and
 *        below the currently invalidated rectangle. This was chosen because
 *        CanvasWidgetRenderer works with horizontal scan lines, and eliminating unnecessary
 *        lines on the left and right does result in as good optimizations, and in some cases
 *        (as e.g. Circle) work against the desired (and expected) optimization.
 */
class Canvas
{
public:

    /**
     * @fn Canvas::Canvas(const CanvasWidget* _widget, const Rect& invalidatedArea);
     *
     * @brief Canvas Constructor.
     *
     *        Canvas Constructor. Locks the frame buffer and prepares for drawing only in the
     *        allowed area which has been invalidated. The color depth of the LCD is taken into
     *        account.
     *
     * @param _widget         a pointer to the CanvasWidget using this Canvas. Used for getting the
     *                        canvas dimensions.
     * @param invalidatedArea the are which should be updated.
     */
    Canvas(const CanvasWidget* _widget, const Rect& invalidatedArea);

    /**
     * @fn virtual Canvas::~Canvas();
     *
     * @brief Destructor.
     *
     *        Destructor. Takes care of unlocking the frame buffer.
     */
    virtual ~Canvas();

    /**
     * @fn void Canvas::moveTo(CWRUtil::Q5 x, CWRUtil::Q5 y);
     *
     * @brief Move the current pen position.
     *
     *        Move the current pen position to (x, y). If the pen is outside (above or below)
     *        the drawing area, nothing is done, but the coordinates are saved in case the next
     *        operation is lineTo a coordinate which is inside (or on the opposite side of) the
     *        drawing area.
     *
     * @param x The x coordinate for the pen position in Q5 format.
     * @param y The y coordinate for the pen position in Q5 format.
     *
     * @see CWRUtil::Q5
     * @see lineTo
     */
    void moveTo(CWRUtil::Q5 x, CWRUtil::Q5 y);

    /**
     * @fn void Canvas::lineTo(CWRUtil::Q5 x, CWRUtil::Q5 y);
     *
     * @brief Draw line from current pen position.
     *
     *        Mark the line from the current (x, y) to the new (x, y) as part of the shape
     *        being drawn. As for moveTo, moveTo and lineTo commands completely outside the
     *        drawing are are discarded.
     *
     * @param x The x coordinate for the pen position in Q5 format.
     * @param y The y coordinate for the pen position in Q5 format.
     *
     * @see CWRUtil::Q5
     * @see moveTo
     */
    void lineTo(CWRUtil::Q5 x, CWRUtil::Q5 y);

    /**
     * @fn template <class T> void Canvas::moveTo(T x, T y)
     *
     * @brief Move the current pen position.
     *
     *        Move the current pen position to (x, y). If the pen is outside (above or below)
     *        the drawing area, nothing is done, but the coordinates are saved in case the next
     *        operation is lineTo a coordinate which is inside (or on the opposite side of) the
     *        drawing area.
     *
     * @tparam T Either int or float.
     * @param x The x coordinate for the pen position.
     * @param y The y coordinate for the pen position.
     */
    template <class T>
    void moveTo(T x, T y)
    {
        moveTo(CWRUtil::toQ5<T>(x), CWRUtil::toQ5<T>(y));
    }

    /**
     * @fn template <class T> void Canvas::lineTo(T x, T y)
     *
     * @brief Draw line from current pen position.
     *
     *        Mark the line from the current (x, y) to the new (x, y) as part of the shape
     *        being drawn. As for moveTo, moveTo and lineTo commands completely outside the
     *        drawing are are discarded.
     *
     * @tparam T either int or float.
     * @param x The x coordinate for the pen position.
     * @param y The y coordinate for the pen position.
     */
    template <class T>
    void lineTo(T x, T y)
    {
        lineTo(CWRUtil::toQ5<T>(x), CWRUtil::toQ5<T>(y));
    }

    /**
     * @fn bool Canvas::render();
     *
     * @brief Render the drawn shape.
     *
     *        Render the graphical shape drawn (using moveTo() and lineTo()) using the widgets
     *        Painter. The shape is automatically closed,
     *        i.e. a lineTo() is automatically inserted connecting the current pen position
     *        with the initial pen position given in the previous moveTo() command.
     *
     * @return true if the widget was rendered, false if insufficient memory was available to
     *         render the widget.
     */
    bool render();

private:
    // Pointer to the widget using the Canvas
    const CanvasWidget* widget;

    // Invalidate area in Q5 coordinates
    CWRUtil::Q5 invalidatedAreaX;
    CWRUtil::Q5 invalidatedAreaY;
    CWRUtil::Q5 invalidatedAreaWidth;
    CWRUtil::Q5 invalidatedAreaHeight;

    // For drawing
    RenderingBuffer rbuf;
    Rasterizer ras;

    int16_t offsetX, offsetY;

    // Used for optimization of drawing algorithm
    bool enoughMemory;
    bool penUp, penHasBeenDown;
    CWRUtil::Q5 previousX, previousY;
    uint8_t previousOutside;
    uint8_t penDownOutside;
    CWRUtil::Q5 initialX, initialY;

    enum
    {
        POINT_IS_ABOVE = 1 << 0,
        POINT_IS_BELOW = 1 << 1
    };

    uint8_t isOutside(const CWRUtil::Q5& y, const CWRUtil::Q5& height) const;

    /**
     * @fn void Canvas::transformFrameBufferToDisplay(CWRUtil::Q5& x, CWRUtil::Q5& y) const;
     *
     * @brief Transform frame buffer coordinates to display coordinates.
     *
     *        Transform frame buffer coordinates to display coordinates for Q5 coordinates.
     *
     * @param [in,out] x The x coordinate.
     * @param [in,out] y The y coordinate.
     *
     * @see DisplayTransformation::transformFrameBufferToDisplay()
     */
    void transformFrameBufferToDisplay(CWRUtil::Q5& x, CWRUtil::Q5& y) const;

    /**
        * @fn void Canvas::close()
        *
        * @brief Closes the path being drawn.
        *
        *        Closes the path being drawn using moveTo and lineTo.
        */
    void close();

};

} // namespace touchgfx

#endif // CANVAS_HPP
