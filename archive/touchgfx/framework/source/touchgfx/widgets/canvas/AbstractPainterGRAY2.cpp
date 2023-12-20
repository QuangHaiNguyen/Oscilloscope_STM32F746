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
#include <touchgfx/widgets/canvas/AbstractPainterGRAY2.hpp>
#include <touchgfx/Color.hpp>
#include <platform/driver/lcd/LCD2bpp.hpp>

namespace touchgfx
{

void AbstractPainterGRAY2::render(uint8_t* ptr,
                                  int x,
                                  int xAdjust,
                                  int y,
                                  unsigned count,
                                  const uint8_t* covers)
{
    currentX = x + areaOffsetX;
    currentY = y + areaOffsetY;
    x += xAdjust;
    if (renderInit())
    {
        do
        {
            uint8_t gray, alpha;
            if (renderNext(gray, alpha))
            {
                if (widgetAlpha < 255)
                {
                    alpha = static_cast<uint8_t>((alpha * widgetAlpha) / 255);
                }
                uint16_t combinedAlpha = (*covers) * alpha / 255;
                covers++;

                if (combinedAlpha == 255) // max alpha=255 on "*covers" and max alpha=255 on "widgetAlpha"
                {
                    // Render a solid pixel
                    renderPixel(ptr, x, gray);
                }
                else
                {
                    uint8_t p_gray = LCD2getPixel(ptr, x);
                    uint16_t ialpha = 0x100 - combinedAlpha;
                    renderPixel(ptr, x, static_cast<uint8_t>((p_gray * ialpha + gray * combinedAlpha) >> 8));
                }
            }
            currentX++;
            x++;
        }
        while (--count != 0);
    }
}

void AbstractPainterGRAY2::renderPixel(uint8_t* p, uint16_t offset, uint8_t gray)
{
    LCD2setPixel(p, offset, gray);
}

} // namespace touchgfx
