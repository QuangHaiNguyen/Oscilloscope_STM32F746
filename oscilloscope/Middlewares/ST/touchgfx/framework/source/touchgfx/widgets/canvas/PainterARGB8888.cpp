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

#include <touchgfx/lcd/LCD.hpp>
#include <touchgfx/widgets/canvas/PainterARGB8888.hpp>

namespace touchgfx
{
void PainterARGB8888::paint(uint8_t* destination, int16_t offset, int16_t /*widgetX*/, int16_t /*widgetY*/, int16_t count, uint8_t alpha) const
{
    uint32_t* RESTRICT framebuffer = reinterpret_cast<uint32_t*>(destination) + offset;
    const uint32_t* const lineEnd = framebuffer + count;
    const uint32_t color = (painterColor & 0x00FFFFFF) | (alpha << 24);

    if (alpha == 0xFF)
    {
        do
        {
            *framebuffer = color;
        } while (++framebuffer < lineEnd);
    }
    else
    {
        do
        {
            const uint32_t argbBg = *framebuffer;
            const uint8_t alphaBg = argbBg >> 24;
            if (alphaBg == 0)
            {
                *framebuffer = color;
            }
            else if (alpha > 0)
            {
                const uint8_t alphaMult = LCD::div255(alpha * alphaBg);
                const uint8_t alphaOut = alpha + alphaBg - alphaMult;
                const uint8_t blueBg = (argbBg & 0xFF);
                const uint8_t greenBg = ((argbBg >> 8) & 0xFF);
                const uint8_t redBg = ((argbBg >> 16) & 0xFF);
                *framebuffer = ((painterBlue * alpha + blueBg * alphaBg - blueBg * alphaMult) / alphaOut) |
                               (((painterGreen * alpha + greenBg * alphaBg - greenBg * alphaMult) / alphaOut) << 8) |
                               (((painterRed * alpha + redBg * alphaBg - redBg * alphaMult) / alphaOut) << 16) |
                               (alphaOut << 24);
            }
        } while (++framebuffer < lineEnd);
    }
}
} // namespace touchgfx
