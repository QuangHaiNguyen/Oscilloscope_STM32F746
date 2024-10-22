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
 * @file touchgfx/containers/progress_indicators/CircleProgress.hpp
 *
 * Declares the touchgfx::CircleProgress class.
 */
#ifndef TOUCHGFX_CIRCLEPROGRESS_HPP
#define TOUCHGFX_CIRCLEPROGRESS_HPP

#include <touchgfx/containers/progress_indicators/AbstractProgressIndicator.hpp>
#include <touchgfx/hal/Types.hpp>
#include <touchgfx/widgets/canvas/AbstractPainter.hpp>
#include <touchgfx/widgets/canvas/Circle.hpp>

namespace touchgfx
{
/**
 * A circle progress indicator uses CanvasWidgetRenderer for drawing the arc of a Circle to show
 * progress. This means that the user must create a painter for painting the circle. The
 * circle progress is defined by setting the minimum and maximum angle of the arc.
 *
 * @note As CircleProgress uses CanvasWidgetRenderer, it is important that a buffer is set up
 *       by calling CanvasWidgetRendere::setBuffer().
 */
class CircleProgress : public AbstractProgressIndicator
{
public:
    CircleProgress();

    virtual void setProgressIndicatorPosition(int16_t x, int16_t y, int16_t width, int16_t height);

    /**
     * Sets the painter to use for drawing the circle progress.
     *
     * @param [in] painter The painter.
     *
     * @see Circle::setPainter, AbstractPainter
     */
    virtual void setPainter(AbstractPainter& painter);

    /**
     * Sets the center of the circle / arc.
     *
     * @param  x The x coordinate of the center of the circle.
     * @param  y The y coordinate of the center of the circle.
     */
    virtual void setCenter(int x, int y);

    /**
     * Gets the circle center coordinates.
     *
     * @param [out] x The x coordinate of the center of the circle.
     * @param [out] y The y coordinate of the center of the circle.
     */
    virtual void getCenter(int& x, int& y) const;

    /**
     * Sets the radius of the circle.
     *
     * @param  r The radius.
     *
     * @see Circle::setRadius
     */
    virtual void setRadius(int r);

    /**
     * Gets the radius of the circle.
     *
     * @return The radius.
     */
    virtual int getRadius() const;

    /**
     * Sets line width of the circle. If a line width of zero is specified, it has a special
     * meaning of drawing a filled circle (with the set radius) instead of just the circle arc.
     *
     * @param  width The width of the line (0 produces a filled circle with the given radius).
     *
     * @see Circle::setLineWidth, setRadius
     */
    virtual void setLineWidth(int width);

    /**
     * Gets line width.
     *
     * @return The line width.
     *
     * @see setLineWidth
     */
    virtual int getLineWidth() const;

    /**
     * Sets the cap precision of end of the circle arc. This is not used if line width is
     * zero.
     *
     * @param  precision The cap precision.
     *
     * @see Circle::setCapPrecision, getCapPrecision
     */
    virtual void setCapPrecision(int precision);

    /**
     * Gets the cap precision.
     *
     * @return The cap precision.
     *
     * @see setCapPrecision
     */
    virtual int getCapPrecision() const
    {
        return circle.getCapPrecision();
    }

    /**
     * Sets start angle and end angle in degrees. By swapping end and start angles, circles can
     * progress backwards.
     *
     * @param   startAngle  The start angle.
     * @param   endAngle    The end angle.
     *
     * @note    Angles are given in degrees, so a full circle is 360.
     */
    virtual void setStartEndAngle(int startAngle, int endAngle);

    /**
     * Gets start angle in degrees.
     *
     * @return  The start angle.
     *
     * @see setStartEndAngle, getEndAngle
     *
     * @note    Angles are given in degrees, so a full circle is 360.
     */
    virtual int getStartAngle() const;

    /**
     * Gets end angle in degrees. Beware that the value returned is not related to the current
     * progress of the circle but rather the end point of the circle when it is at 100%.
     *
     * @return  The end angle.
     *
     * @see setStartEndAngle
     *
     * @note    Angles are given in degrees, so a full circle is 360.
     */
    virtual int getEndAngle() const;

    /**
     * @copydoc Image::setAlpha
     *
     * @note The alpha can also be set on the Painter, but this can be controlled directly from
     *       the user app, setting alpha for the CircleProgress will set the alpha of the
     *       actual circle.
     */
    virtual void setAlpha(uint8_t newAlpha);

    virtual void setValue(int value);

protected:
    Circle circle;      ///< The circle
    int circleEndAngle; ///< The end angle
};

} // namespace touchgfx

#endif // TOUCHGFX_CIRCLEPROGRESS_HPP
