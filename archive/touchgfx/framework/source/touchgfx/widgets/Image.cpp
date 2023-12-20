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
#include <touchgfx/widgets/Image.hpp>

namespace touchgfx
{
void Image::setBitmap(const Bitmap& bmp)
{
    bitmap = bmp;
    // When setting bitmap, adjust size of this widget to match.
    setWidth(bitmap.getWidth());
    setHeight(bitmap.getHeight());

    // This bool is no longer used, but maintained for backwards compat.
    hasTransparentPixels = bitmap.hasTransparentPixels();
}

void Image::draw(const Rect& invalidatedArea) const
{
    Rect meAbs;
    translateRectToAbsolute(meAbs); //To find our x and y coords in absolute.

    // Calculate intersection between bitmap rect and invalidated area.
    Rect dirtyBitmapArea = bitmap.getRect() & invalidatedArea;

    if (!dirtyBitmapArea.isEmpty())
    {
        HAL::lcd().drawPartialBitmap(bitmap, meAbs.x, meAbs.y, dirtyBitmapArea, alpha);
    }
}

Rect Image::getSolidRect() const
{
    // If alpha is less than solid, we have an empty solid rect.
    if (alpha < 255)
    {
        return Rect(0, 0, 0, 0);
    }
    // Return solid rect from bitmap (precalculated).
    return bitmap.getSolidRect();
}
} // namespace touchgfx
