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
#ifndef BUTTON_HPP
#define BUTTON_HPP

#include <touchgfx/widgets/AbstractButton.hpp>
#include <touchgfx/Bitmap.hpp>

namespace touchgfx
{
/**
 * @class Button Button.hpp touchgfx/widgets/Button.hpp
 *
 * @brief A button with two states.
 *
 *        A button consists of two images, one for its normal state and one when it is pressed
 *        down.
 *
 * @see AbstractButton
 */
class Button : public AbstractButton
{
public:

    /**
     * @fn Button::Button()
     *
     * @brief Default constructor.
     *
     *        Default constructor.
     */
    Button() : AbstractButton(), up(), down(), alpha(255) { }

    /**
     * @fn virtual Button::~Button()
     *
     * @brief Destructor.
     *
     *        Destructor.
     */
    virtual ~Button() { }

    /**
     * @fn virtual void Button::draw(const Rect& invalidatedArea) const;
     *
     * @brief Draws the given invalidated area.
     *
     * @param invalidatedArea The rectangle to draw, with coordinates relative to this drawable.
     *
     * @see Drawable::draw()
     */
    virtual void draw(const Rect& invalidatedArea) const;

    /**
     * @fn virtual void Button::setBitmaps(const Bitmap& bmpReleased, const Bitmap& bmpPressed);
     *
     * @brief Sets the bitmaps used by this button.
     *
     *        Sets the bitmaps used by this button.
     *
     * @param bmpReleased Bitmap to use when button is released.
     * @param bmpPressed  Bitmap to use when button is pressed.
     */
    virtual void setBitmaps(const Bitmap& bmpReleased, const Bitmap& bmpPressed);

    /**
     * @fn virtual Rect Button::getSolidRect() const;
     *
     * @brief Gets solid rectangle.
     *
     *        Gets solid rectangle.
     *
     * @return largest possible solid rect. Delegated to the largest solid rect of the button
     *         bitmap(s).
     */
    virtual Rect getSolidRect() const;

    /**
     * @fn void Button::setAlpha(uint8_t alpha)
     *
     * @brief Sets the alpha value for the image.
     *
     *        Sets the alpha value for the image.
     *
     * @param alpha The alpha value. 255 = completely solid.
     */
    void setAlpha(uint8_t alpha)
    {
        this->alpha = alpha;
    }

    /**
     * @fn uint8_t Button::getAlpha() const
     *
     * @brief Gets the current alpha value.
     *
     *        Gets the current alpha value.
     *
     * @return The current alpha value.
     */
    uint8_t getAlpha() const
    {
        return alpha;
    }

    /**
     * @fn Bitmap Button::getCurrentlyDisplayedBitmap() const
     *
     * @brief Gets currently displayed bitmap.
     *
     *        Function to obtain the currently displayed bitmap, which depends on the button's
     *        pressed state.
     *
     * @return The bitmap currently displayed.
     */
    Bitmap getCurrentlyDisplayedBitmap() const
    {
        return (AbstractButton::pressed ? down : up);
    }

    /**
     * @fn virtual uint16_t Button::getType() const
     *
     * @brief For GUI testing only.
     *
     *        For GUI testing only. Returns type of this drawable.
     *
     * @return TYPE_BUTTON.
     */
    virtual uint16_t getType() const
    {
        return (uint16_t)TYPE_BUTTON;
    }

protected:
    Bitmap  up;    ///< The image to display when button is released.
    Bitmap  down;  ///< The image to display when button is pressed.
    uint8_t alpha; ///< The current alpha value. 255 denotes solid, 0 denotes completely transparent.
};

} // namespace touchgfx

#endif // BUTTON_HPP
