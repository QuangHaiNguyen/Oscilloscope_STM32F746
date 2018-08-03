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
#include <touchgfx/widgets/PixelDataWidget.hpp>

namespace touchgfx
{

PixelDataWidget::PixelDataWidget() :
    Widget(),
    buffer(0),
    format(Bitmap::RGB888),
    alpha(255)
{
}

void PixelDataWidget::setPixelData(uint8_t* const data)
{
    buffer = data;
}

void PixelDataWidget::setBitmapFormat(Bitmap::BitmapFormat f)
{
    format = f;
}

void PixelDataWidget::setAlpha(uint8_t a)
{
    this->alpha = a;
}

uint8_t PixelDataWidget::getAlpha() const
{
    return alpha;
}

void PixelDataWidget::draw(const touchgfx::Rect& invalidatedArea) const
{
    if (buffer != 0)
    {
        //convert to lcd coordinates
        const touchgfx::Rect absolute = getAbsoluteRect();
        //copy to LCD
        HAL::lcd().blitCopy(buffer, format, absolute, invalidatedArea, alpha, false);
    }
}

touchgfx::Rect PixelDataWidget::getSolidRect() const
{
    Rect solidRect(0, 0, 0, 0);
    if (alpha == 255)
    {
        // There are at least some solid pixels
        switch (format)
        {
        case Bitmap::BW:       ///< 1-bit, black / white, no alpha channel
        case Bitmap::BW_RLE:   ///< 1-bit, black / white, no alpha channel compressed with horizontal RLE
        case Bitmap::GRAY2:    ///< 2-bit, gray scale, no alpha channel
        case Bitmap::GRAY4:    ///< 4-bit, gray scale, no alpha channel
        case Bitmap::RGB565:   ///< 16-bit, 5 bits for red, 6 bits for green, 5 bits for blue, no alpha channel
        case Bitmap::RGB888:   ///< 16-bit, 5 bits for red, 6 bits for green, 5 bits for blue, no alpha channel
            //All solid pixels
            solidRect.width = getWidth();
            solidRect.height = getHeight();
            break;
        case Bitmap::ARGB8888: ///< 32-bit, 8 bits for each of red, green, blue and alpha channel
        default:
            //No knowledge about solid pixels
            break;
        }
    }
    return solidRect;
}

} // namespace touchgfx
