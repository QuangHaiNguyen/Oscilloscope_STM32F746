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
#ifndef BOXPROGRESS_HPP
#define BOXPROGRESS_HPP

#include <touchgfx/containers/progress_indicators/AbstractDirectionProgress.hpp>
#include <touchgfx/widgets/Box.hpp>

namespace touchgfx
{
/**
 * @class BoxProgress BoxProgress.hpp touchgfx/containers/progress_indicators/BoxProgress.hpp
 *
 * @brief A box progress.
 *
 *        A Box progress which shows the current progress using a simple Box. It is possible to
 *        set the color and the alpha of the box. It is also possible to control in what
 *        direction the box will progress (up, down, to the left or to the right).
 *
 * @see Box
 */
class BoxProgress : public AbstractDirectionProgress
{
public:

    /**
     * @fn BoxProgress::BoxProgress();
     *
     * @brief Default constructor.
     *
     *        Default constructor.
     */
    BoxProgress();

    /**
     * @fn virtual BoxProgress::~BoxProgress();
     *
     * @brief Destructor.
     *
     *        Destructor.
     */
    virtual ~BoxProgress();

    /**
     * @fn virtual void BoxProgress::setProgressIndicatorPosition(int16_t x, int16_t y, int16_t width, int16_t height);
     *
     * @brief Sets the position and dimension of the box progress indicator.
     *
     *        Sets the position and dimension of the box progress indicator relative to the
     *        background image.
     *
     * @param x      The x coordinate.
     * @param y      The y coordinate.
     * @param width  The width of the box progress indicator.
     * @param height The height of the box progress indicator.
     */
    virtual void setProgressIndicatorPosition(int16_t x, int16_t y, int16_t width, int16_t height);

    /**
     * @fn virtual void BoxProgress::setColor(colortype color);
     *
     * @brief Sets the color.
     *
     *        Sets the color of the Box.
     *
     * @param color The color.
     *
     * @see Box
     */
    virtual void setColor(colortype color);

    /**
     * @fn virtual colortype BoxProgress::getColor() const;
     *
     * @brief Gets the color.
     *
     *        Gets the color of the Box.
     *
     * @return The color.
     *
     * @see Box
     */
    virtual colortype getColor() const;

    /**
     * @fn virtual void BoxProgress::setAlpha(uint8_t alpha);
     *
     * @brief Sets the alpha.
     *
     *        Sets the alpha of the Box.
     *
     * @param alpha The alpha.
     *
     * @see Box
     */
    virtual void setAlpha(uint8_t alpha);

    /**
     * @fn virtual uint8_t BoxProgress::getAlpha() const;
     *
     * @brief Gets the alpha.
     *
     *        Gets the alpha of the Box.
     *
     * @return The alpha.
     *
     * @see Box
     */
    virtual uint8_t getAlpha() const;

    virtual void setValue(int value);

protected:
    Box box;    ///< The box
};

}

#endif // BOXPROGRESS_HPP
