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
#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <touchgfx/hal/Types.hpp>
#include <touchgfx/widgets/Widget.hpp>
#include <touchgfx/Bitmap.hpp>
#include <touchgfx/lcd/LCD.hpp>

namespace touchgfx
{
/**
 * @class Image Image.hpp touchgfx/widgets/Image.hpp
 *
 * @brief Simple widget capable of showing a bitmap.
 *
 *        Simple widget capable of showing a bitmap. The bitmap can be alpha-blended with the
 *        background and have areas of transparency.
 *
 * @see Widget
 */
class Image : public Widget
{
public:

    /**
     * @fn Image::Image(const Bitmap& bmp = Bitmap())
     *
     * @brief Default Constructor.
     *
     *        Constructs a new Image with a default alpha value of 255 (solid) and a default
     *        Bitmap if none is specified.
     *
     * @param bmp The bitmap to display.
     */
    Image(const Bitmap& bmp = Bitmap()) : Widget(), alpha(255)
    {
        setBitmap(bmp);
    }

    /**
     * @fn virtual void Image::setBitmap(const Bitmap& bmp);
     *
     * @brief Sets the bitmap ID for this Image.
     *
     *        Sets the bitmap ID for this Image. Updates the width and height of this widget to
     *        match that of the bitmap.
     *
     * @param bmp The bitmap instance.
     *
     * @see Bitmap
     */
    virtual void setBitmap(const Bitmap& bmp);

    /**
     * @fn void Image::setAlpha(uint8_t alpha)
     *
     * @brief Sets the alpha channel for the image.
     *
     *        Sets the alpha channel for the image.
     *
     * @param alpha The alpha value. 255 = completely solid.
     */
    void setAlpha(uint8_t alpha)
    {
        this->alpha = alpha;
    }

    /**
     * @fn virtual void Image::draw(const Rect& invalidatedArea) const;
     *
     * @brief Draws the image.
     *
     *        Draws the image. This class supports partial drawing, so only the area described
     *        by the rectangle will be drawn.
     *
     * @param invalidatedArea The rectangle to draw, with coordinates relative to this drawable.
     */
    virtual void draw(const Rect& invalidatedArea) const;

    /**
     * @fn BitmapId Image::getBitmap() const
     *
     * @brief Gets the BitmapId currently contained by the widget.
     *
     *        Gets the BitmapId currently contained by the widget.
     *
     * @return The current BitmapId of the widget.
     */
    BitmapId getBitmap() const
    {
        return bitmap.getId();
    }

    /**
     * @fn uint8_t Image::getAlpha() const
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
     * @fn virtual Rect Image::getSolidRect() const;
     *
     * @brief Gets the largest solid (non-transparent) rectangle.
     *
     *        Gets the largest solid (non-transparent) rectangle. This value is pre-calculated
     *        by the imageconverter tool.
     *
     * @return The largest solid (non-transparent) rectangle.
     */
    virtual Rect getSolidRect() const;

    /**
     * @fn virtual uint16_t Image::getType() const
     *
     * @brief For GUI testing only.
     *
     *        For GUI testing only. Returns type of this drawable.
     *
     * @return TYPE_IMAGE.
     */
    virtual uint16_t getType() const
    {
        return (uint16_t)TYPE_IMAGE;
    }
protected:
    Bitmap  bitmap;               ///< The Bitmap to display.
    uint8_t alpha;                ///< The Alpha for this image.
    bool    hasTransparentPixels; ///< true if this object has transparent pixels
};

} // namespace touchgfx

#endif // IMAGE_HPP
