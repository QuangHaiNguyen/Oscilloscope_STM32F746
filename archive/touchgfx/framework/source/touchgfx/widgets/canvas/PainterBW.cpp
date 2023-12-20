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
#include <touchgfx/widgets/canvas/PainterBW.hpp>

namespace touchgfx
{

unsigned PainterBW::bw(unsigned red, unsigned green, unsigned blue)
{
    return (red * 77 + green * 150 + blue * 29) >> 15;
}

void PainterBW::setColor(colortype color)
{
    painterColor = color ? 1 : 0;
}

touchgfx::colortype PainterBW::getColor() const
{
    return static_cast<colortype>(painterColor);
}

void PainterBW::render(uint8_t* ptr, int x, int xAdjust, int y, unsigned count, const uint8_t* covers)
{
    (void)covers; // Unused

    currentX = x + areaOffsetX;
    currentY = y + areaOffsetY;
    x += xAdjust;
    unsigned char* p = ptr + (x / 8);

    do
    {
        unsigned pixel = painterColor << (7 - (x % 8));
        if (!painterColor)
        {
            *p &= ~pixel;
        }
        else
        {
            *p |= pixel;
        }
        if (((++x) % 8) == 0)
        {
            p++;
        }
        currentX++;
    }
    while (--count);
}

bool PainterBW::renderNext(uint8_t& color)
{
    color = painterColor;
    return true;
}

} // namespace touchgfx
