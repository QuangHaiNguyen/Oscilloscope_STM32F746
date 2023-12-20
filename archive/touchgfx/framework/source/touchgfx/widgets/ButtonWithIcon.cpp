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
#include <touchgfx/widgets/ButtonWithIcon.hpp>

namespace touchgfx
{

ButtonWithIcon::ButtonWithIcon() :
    Button(),
    iconX(0),
    iconY(0)
{
}

void ButtonWithIcon::setBitmaps(const Bitmap& newBackgroundReleased, const Bitmap& newBackgroundPressed,
                                const Bitmap& newIconReleased,       const Bitmap& newIconPressed)
{
    Button::setBitmaps(newBackgroundReleased, newBackgroundPressed);

    iconReleased = newIconReleased;
    iconPressed = newIconPressed;

    iconX = (getWidth() / 2) - (newIconPressed.getWidth() / 2);
    iconY = (getHeight() / 2) - (newIconPressed.getHeight() / 2);
}

void ButtonWithIcon::draw(const Rect& invalidatedArea) const
{
    Button::draw(invalidatedArea);

    Bitmap bmp((pressed ? iconPressed : iconReleased));
    Rect iconRect(iconX, iconY, bmp.getWidth(), bmp.getHeight());
    Rect dirty = invalidatedArea & iconRect;
    if ((bmp.getId() != BITMAP_INVALID) && !dirty.isEmpty())
    {
        Rect r;
        translateRectToAbsolute(r);
        dirty.x -= iconX;
        dirty.y -= iconY;
        HAL::lcd().drawPartialBitmap(bmp, r.x + iconX, r.y + iconY, dirty, alpha);
    }
}

} // namespace touchgfx
