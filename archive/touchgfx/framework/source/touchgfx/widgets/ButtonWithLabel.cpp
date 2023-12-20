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
#include <touchgfx/widgets/ButtonWithLabel.hpp>
#include <touchgfx/FontManager.hpp>

namespace touchgfx
{

ButtonWithLabel::ButtonWithLabel()
    : Button(), color(0), colorPressed(0), rotation(TEXT_ROTATE_0), textHeightIncludingSpacing(0)
{
}

void ButtonWithLabel::draw(const Rect& area) const
{
    Button::draw(area);

    const Font* fontToDraw = typedText.getFont();
    if ((fontToDraw != 0) && typedText.hasValidId())
    {
        uint8_t height = textHeightIncludingSpacing;
        int16_t offset;
        Rect labelRect;
        switch (rotation)
        {
        default:
        case TEXT_ROTATE_0:
        case TEXT_ROTATE_180:
            offset = (this->getHeight() - height) / 2;
            labelRect = Rect(0, offset, this->getWidth(), height);
            break;
        case TEXT_ROTATE_90:
        case TEXT_ROTATE_270:
            offset = (this->getWidth() - height) / 2;
            labelRect = Rect(offset, 0, height, this->getHeight());
            break;
        }
        Rect dirty = labelRect & area;

        if (!dirty.isEmpty())
        {
            dirty.x -= labelRect.x;
            dirty.y -= labelRect.y;
            translateRectToAbsolute(labelRect);
            LCD::StringVisuals visuals(fontToDraw, pressed ? colorPressed : color, alpha, typedText.getAlignment(), 0, rotation, typedText.getTextDirection(), 0, WIDE_TEXT_NONE);
            HAL::lcd().drawString(labelRect, dirty, visuals, typedText.getText());
        }
    }
}

} // namespace touchgfx
