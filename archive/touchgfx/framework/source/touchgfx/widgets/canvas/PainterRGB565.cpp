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
#include <touchgfx/widgets/canvas/PainterRGB565.hpp>
#include <touchgfx/Color.hpp>

namespace touchgfx
{

PainterRGB565::PainterRGB565(colortype color, uint8_t alpha) :
    AbstractPainterRGB565()
{
    setColor(color, alpha);
}


void PainterRGB565::setColor(colortype color, uint8_t alpha)
{
    painterColor = (uint16_t)color;
    painterRed   = painterColor & RMASK;
    painterGreen = painterColor & GMASK;
    painterBlue  = painterColor & BMASK;
    painterAlpha = alpha;
}

touchgfx::colortype PainterRGB565::getColor() const
{
    return painterColor;
}

void PainterRGB565::setAlpha(uint8_t alpha)
{
    painterAlpha = alpha;
}

uint8_t PainterRGB565::getAlpha() const
{
    return painterAlpha;
}

void PainterRGB565::render(uint8_t* ptr, int x, int xAdjust, int y, unsigned count, const uint8_t* covers)
{
    uint16_t* p = reinterpret_cast<uint16_t*>(ptr) + (x + xAdjust);
    uint8_t totalAlpha = (widgetAlpha * painterAlpha) / 255u;
    if (totalAlpha == 255)
    {
        do
        {
            uint32_t alpha = *covers;
            covers++;
            if (alpha == 255u)
            {
                *p = painterColor;
            }
            else
            {
                *p = mixColors(painterRed, painterGreen, painterBlue, *p, alpha, 8);
            }
            p++;
        }
        while (--count != 0);
    }
    else
    {
        do
        {
            uint32_t alpha = (*covers) * totalAlpha;
            covers++;
            if (alpha == 255u * 255u)
            {
                *p = painterColor;
            }
            else
            {
                *p = mixColors(painterRed, painterGreen, painterBlue, *p, alpha, 16);
            }
            p++;
        }
        while (--count != 0);
    }
}

bool PainterRGB565::renderNext(uint8_t& red, uint8_t& green, uint8_t& blue, uint8_t& alpha)
{
    red   = painterRed >> 8;
    green = painterGreen >> 3;
    blue  = painterBlue << 3;
    alpha = painterAlpha;
    return true;
}

} // namespace touchgfx
