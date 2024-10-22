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

/**
 * @file touchgfx/widgets/canvas/AbstractPainterARGB2222.hpp
 *
 * Declares the touchgfx::AbstractPainterARGB2222 class.
 */
#ifndef TOUCHGFX_ABSTRACTPAINTERARGB2222_HPP
#define TOUCHGFX_ABSTRACTPAINTERARGB2222_HPP

#include <platform/driver/lcd/LCD8bpp_ARGB2222.hpp>
#include <touchgfx/Bitmap.hpp>
#include <touchgfx/hal/Types.hpp>
#include <touchgfx/lcd/LCD.hpp>
#include <touchgfx/widgets/canvas/AbstractPainter.hpp>

namespace touchgfx
{
/**
 * The AbstractPainterARGB2222 class is an abstract class for creating a painter to draw on a
 * ARGB2222 display using CanvasWidgetRenderer.
 *
 * @see AbstractPainter
 */
class AbstractPainterARGB2222 : public AbstractPainter
{
public:
    AbstractPainterARGB2222()
        : AbstractPainter()
    {
        assert(compatibleFramebuffer(Bitmap::ARGB2222) && "The chosen painter only works with ARGB2222 displays");
    }

    /**
     * Mix colors from a new pixel and a buffer pixel with the given alpha applied to the
     * new pixel, and the inverse alpha applied to the buffer pixel.
     *
     * @param  newpix The new pixel value.
     * @param  bufpix The buffer pixel value.
     * @param  alpha  The alpha to apply to the new pixel.
     *
     * @return The result of blending the two colors into a new color.
     */
    FORCE_INLINE_FUNCTION uint8_t alphaBlend(uint8_t newpix, uint8_t bufpix, uint8_t alpha) const
    {
        return alphaBlend(LCD8bpp_ARGB2222::getRedFromNativeColor(newpix),
                          LCD8bpp_ARGB2222::getGreenFromNativeColor(newpix),
                          LCD8bpp_ARGB2222::getBlueFromNativeColor(newpix), bufpix, alpha);
    }

    /**
     * Mix colors from a new pixel and a buffer pixel with the given alpha applied to the new pixel,
     * and the inverse alpha applied to the buffer pixel.
     *
     * @param  R      The red color.
     * @param  G      The green color.
     * @param  B      The blue color.
     * @param  bufpix The buffer pixel value.
     * @param  alpha  The alpha of the R,G,B.
     *
     * @return The result of blending the two colors into a new color.
     */
    FORCE_INLINE_FUNCTION uint8_t alphaBlend(uint8_t R, uint8_t G, uint8_t B, uint8_t bufpix, uint8_t alpha) const
    {
        const uint8_t ialpha = 0xFF - alpha;
        const uint8_t p_red = LCD8bpp_ARGB2222::getRedFromNativeColor(bufpix);
        const uint8_t p_green = LCD8bpp_ARGB2222::getGreenFromNativeColor(bufpix);
        const uint8_t p_blue = LCD8bpp_ARGB2222::getBlueFromNativeColor(bufpix);
        const uint8_t red = LCD::div255(R * alpha + p_red * ialpha);
        const uint8_t green = LCD::div255(G * alpha + p_green * ialpha);
        const uint8_t blue = LCD::div255(B * alpha + p_blue * ialpha);
        return LCD8bpp_ARGB2222::getNativeColorFromRGB(red, green, blue);
    }
};

} // namespace touchgfx

#endif // TOUCHGFX_ABSTRACTPAINTERARGB2222_HPP
