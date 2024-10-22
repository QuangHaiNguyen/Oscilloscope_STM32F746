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
 * @file touchgfx/widgets/canvas/AbstractPainterRGB565.hpp
 *
 * Declares the touchgfx::AbstractPainterRGB565 class.
 */
#ifndef TOUCHGFX_ABSTRACTPAINTERRGB565_HPP
#define TOUCHGFX_ABSTRACTPAINTERRGB565_HPP

#include <touchgfx/Bitmap.hpp>
#include <touchgfx/hal/Types.hpp>
#include <touchgfx/lcd/LCD.hpp>
#include <touchgfx/widgets/canvas/AbstractPainter.hpp>

namespace touchgfx
{
/**
 * The AbstractPainterRGB565 class is an abstract class for creating a painter to draw on a
 * RGB565 display using CanvasWidgetRenderer.
 *
 * @see AbstractPainter
 */
class AbstractPainterRGB565 : public AbstractPainter
{
public:
    static const uint16_t RMASK = 0xF800; ///< Mask for red   (1111100000000000)
    static const uint16_t GMASK = 0x07E0; ///< Mask for green (0000011111100000)
    static const uint16_t BMASK = 0x001F; ///< Mask for blue  (0000000000011111)

    AbstractPainterRGB565()
        : AbstractPainter()
    {
        assert(compatibleFramebuffer(Bitmap::RGB565) && "The chosen painter only works with RGB565 displays");
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
    FORCE_INLINE_FUNCTION uint16_t alphaBlend(uint16_t newpix, uint16_t bufpix, uint8_t alpha) const
    {
        return alphaBlend(newpix & RMASK, newpix & GMASK, newpix & BMASK, bufpix, alpha);
    }

    /**
     * Mix colors from a new pixel and a buffer pixel with the given alpha applied to the
     * new pixel, and the inverse alpha applied to the buffer pixel.
     *
     * @param  R      The red color (0-31 shifted into #RMASK).
     * @param  G      The green color (0-63 shifted into #GMASK).
     * @param  B      The blue color (0-31 shifted into #BMASK).
     * @param  bufpix The buffer pixel value.
     * @param  alpha  The alpha of the R,G,B.
     *
     * @return The result of blending the two colors into a new color.
     */
    FORCE_INLINE_FUNCTION uint16_t alphaBlend(uint16_t R, uint16_t G, uint16_t B, uint16_t bufpix, uint8_t alpha) const
    {
        const uint8_t ialpha = 0xFF - alpha;
        return (((R * alpha + (bufpix & RMASK) * ialpha) / 255) & RMASK) |
               (((G * alpha + (bufpix & GMASK) * ialpha) / 255) & GMASK) |
               (((B * alpha + (bufpix & BMASK) * ialpha) / 255) & BMASK);
    }
};

} // namespace touchgfx

#endif // TOUCHGFX_ABSTRACTPAINTERRGB565_HPP
