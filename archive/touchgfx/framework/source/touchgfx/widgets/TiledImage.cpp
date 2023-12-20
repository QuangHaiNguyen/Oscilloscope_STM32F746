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
#include <touchgfx/widgets/TiledImage.hpp>

namespace touchgfx
{
void TiledImage::setBitmap(const Bitmap& bmp)
{
    Image::setBitmap(bmp);
    // Make sure the xOffset and yOffset are correct
    setOffset(xOffset, yOffset);
}

void TiledImage::setOffset(int16_t x, int16_t y)
{
    setXOffset(x);
    setYOffset(y);
}

void TiledImage::setXOffset(int16_t x)
{
    xOffset = ((x % bitmap.getWidth()) + bitmap.getWidth()) % bitmap.getWidth();
}

void TiledImage::setYOffset(int16_t y)
{
    yOffset = ((y % bitmap.getHeight()) + bitmap.getHeight()) % bitmap.getHeight();
}

void TiledImage::getOffset(int16_t& x, int16_t& y)
{
    x = getXOffset();
    y = getYOffset();
}

int16_t TiledImage::getXOffset()
{
    return xOffset;
}

int16_t TiledImage::getYOffset()
{
    return yOffset;
}

void TiledImage::draw(const Rect& invalidatedArea) const
{
    uint16_t bitmapWidth = bitmap.getWidth();
    uint16_t bitmapHeight = bitmap.getHeight();

    if (bitmapWidth == 0 || bitmapHeight == 0)
    {
        return;
    }

    Rect meAbs;
    translateRectToAbsolute(meAbs);

    const int16_t minX = ((invalidatedArea.x + xOffset) / bitmapWidth) * bitmapWidth - xOffset;
    const int16_t maxX = (((invalidatedArea.right() + xOffset) - 1) / bitmapWidth) * bitmapWidth;
    const int16_t minY = ((invalidatedArea.y + yOffset) / bitmapHeight) * bitmapHeight - yOffset;
    const int16_t maxY = (((invalidatedArea.bottom() + yOffset) - 1) / bitmapHeight) * bitmapHeight;
    for (int16_t x = minX; x <= maxX; x += bitmapWidth)
    {
        for (int16_t y = minY; y <= maxY; y += bitmapHeight)
        {
            Rect dirty = Rect(x, y, bitmapWidth, bitmapHeight) & invalidatedArea;
            dirty.x -= x;
            dirty.y -= y;
            HAL::lcd().drawPartialBitmap(bitmap, meAbs.x + x, meAbs.y + y, dirty, alpha);
        }
    }
}

Rect TiledImage::getSolidRect() const
{
    if (alpha < 255)
    {
        return Rect(0, 0, 0, 0);
    }

    Rect solidRect = bitmap.getSolidRect();
    if (solidRect.width == bitmap.getWidth())
    {
        solidRect.width = getWidth();
    }
    if (solidRect.height == bitmap.getHeight())
    {
        solidRect.height = getHeight();
    }
    return solidRect;
}
} // namespace touchgfx
