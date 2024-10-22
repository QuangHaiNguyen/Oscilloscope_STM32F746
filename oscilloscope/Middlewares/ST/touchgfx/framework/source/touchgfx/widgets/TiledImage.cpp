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

#include <touchgfx/Drawable.hpp>
#include <touchgfx/hal/HAL.hpp>
#include <touchgfx/lcd/LCD.hpp>
#include <touchgfx/widgets/TiledImage.hpp>

namespace touchgfx
{
void TiledImage::setBitmap(const Bitmap& bmp)
{
    Image::setBitmap(bmp);
    // Make sure the xOffset and yOffset are correct (in range)
    setOffset(xOffset, yOffset);
}

void TiledImage::setOffset(int16_t x, int16_t y)
{
    setXOffset(x);
    setYOffset(y);
}

void TiledImage::setXOffset(int16_t x)
{
    xOffset = x;
    if (bitmap.getWidth() != 0)
    {
        xOffset %= bitmap.getWidth();
        if (xOffset < 0)
        {
            xOffset += bitmap.getWidth();
        }
    }
}

void TiledImage::setYOffset(int16_t y)
{
    yOffset = y;
    if (bitmap.getHeight() != 0)
    {
        yOffset %= bitmap.getHeight();
        if (yOffset < 0)
        {
            yOffset += bitmap.getHeight();
        }
    }
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
    const int16_t bitmapWidth = bitmap.getWidth();
    const int16_t bitmapHeight = bitmap.getHeight();

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
        return Rect();
    }

    Rect solidRect = bitmap.getSolidRect();
    if (solidRect.width == bitmap.getWidth())
    {
        solidRect.width = getWidth();
    }
    else
    {
        solidRect.x -= xOffset;
        Rect solidRect2 = solidRect;
        solidRect2.x += bitmap.getWidth();
        if (solidRect.x < 0)
        {
            const int16_t right = solidRect.right();
            solidRect.width = MAX(right, 0);
            solidRect.x = 0;
        }
        if (solidRect2.right() > getWidth())
        {
            solidRect2.width = solidRect2.right() - getWidth();
        }
        if (solidRect2.width > solidRect.width)
        {
            solidRect = solidRect2;
        }
    }
    if (solidRect.height == bitmap.getHeight())
    {
        solidRect.height = getHeight();
    }
    else
    {
        solidRect.y -= yOffset;
        Rect solidRect2 = solidRect;
        solidRect2.y += bitmap.getHeight();
        if (solidRect.y < 0)
        {
            const int16_t bottom = solidRect.bottom();
            solidRect.height = MAX(bottom, 0);
            solidRect.y = 0;
        }
        if (solidRect2.bottom() > getHeight())
        {
            solidRect2.height = solidRect2.bottom() - getHeight();
        }
        if (solidRect2.height > solidRect.height)
        {
            solidRect = solidRect2;
        }
    }
    return solidRect;
}
} // namespace touchgfx
