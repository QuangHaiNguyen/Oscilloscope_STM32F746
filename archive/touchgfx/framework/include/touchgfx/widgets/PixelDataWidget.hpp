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
#ifndef PIXELDATAWIDGET_HPP
#define PIXELDATAWIDGET_HPP

#include <touchgfx/widgets/Widget.hpp>
#include <touchgfx/hal/Types.hpp>
#include <touchgfx/Bitmap.hpp>

namespace touchgfx
{
/**
 * @class PixelDataWidget PixelDataWidget.hpp touchgfx/widgets/PixelDataWidget.hpp
 *
 * @brief A widget for displaying a buffer of pixel data
 *
 *        The buffer must be of size as widget. If the LCD is 16 bit the buffer must hold 2
 *        bytes for each pixel. If the LCD is 24 bit the buffer must hold 3 bytes for each
 *        pixel.
 *
 * @see touchgfx::Widget
 */
class PixelDataWidget : public touchgfx::Widget
{
public:

    /**
     * @fn PixelDataWidget::PixelDataWidget();
     *
     * @brief Default constructor.
     *
     *        Default constructor.
     */
    PixelDataWidget();

    /**
     * @fn virtual void PixelDataWidget::draw(const touchgfx::Rect& invalidatedArea) const;
     *
     * @brief Draw the part of the RAM buffer that is inside the invalidated area.
     *
     *        Draw the part of the RAM buffer that is inside the invalidated area.
     *
     * @param invalidatedArea The region of this drawable that needs to be redrawn.
     *
     * @see touchgfx::Drawable
     */
    virtual void draw(const touchgfx::Rect& invalidatedArea) const;

    /**
     * @fn virtual touchgfx::Rect PixelDataWidget::getSolidRect() const;
     *
     * @brief Report this widget as being completely solid.
     *
     *        Report this widget as being completely solid.
     *
     * @return The solid rect.
     */
    virtual touchgfx::Rect getSolidRect() const;

    /**
     * @fn void PixelDataWidget::setPixelData(uint8_t* const data);
     *
     * @brief Set the pixel data to display.
     *
     *        Set the pixel data to display.
     *
     * @param [in,out] data If non-null, the data.
     */
    void setPixelData(uint8_t* const data);

    /**
     * @fn void PixelDataWidget::setBitmapFormat(Bitmap::BitmapFormat format);
     *
     * @brief Set the format of the pixel data.
     *
     *        Set the format of the pixel data.
     *
     * @param format Describes the format to use.
     */
    void setBitmapFormat(Bitmap::BitmapFormat format);

    /**
     * @fn void PixelDataWidget::setAlpha(uint8_t a);
     *
     * @brief Sets the alpha channel for the image.
     *
     *        Sets the alpha channel for the image.
     *
     * @param a The alpha value. 255 = completely solid.
     */
    void setAlpha(uint8_t a);

    /**
     * @fn uint8_t PixelDataWidget::getAlpha();
     *
     * @brief Gets the current alpha value.
     *
     *        Gets the current alpha value.
     *
     * @return The current alpha value.
     *
     * @see setAlpha
     */
    uint8_t getAlpha() const;


protected:
    uint8_t* buffer;             ///< The buffer where the pixels are copied from
    Bitmap::BitmapFormat format; ///< The pixel format for the data.
    uint8_t alpha;               ///< The Alpha for this widget.
};

} // namespace touchgfx

#endif // PIXELDATAWIDGET_HPP
