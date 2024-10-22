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
 * @file touchgfx/widgets/canvas/PainterARGB2222.hpp
 *
 * Declares the touchgfx::PainterARGB2222 class.
 */
#ifndef TOUCHGFX_PAINTERARGB2222_HPP
#define TOUCHGFX_PAINTERARGB2222_HPP

#include <platform/driver/lcd/LCD8bpp_ARGB2222.hpp>
#include <touchgfx/Color.hpp>
#include <touchgfx/hal/Types.hpp>
#include <touchgfx/widgets/canvas/AbstractPainterARGB2222.hpp>
#include <touchgfx/widgets/canvas/AbstractPainterColor.hpp>

namespace touchgfx
{
/**
 * The PainterARGB2222 class allows a shape to be filled with a given color
 * value. This allows anti-aliased elements to be drawn.
 *
 * @see AbstractPainter
 */
class PainterARGB2222 : public AbstractPainterARGB2222, public AbstractPainterColor
{
public:
    /**
     * Initializes a new instance of the PainterARGB2222 class.
     *
     * @param  color (Optional) the color, default is black.
     */
    PainterARGB2222(colortype color = 0)
        : AbstractPainterARGB2222(), AbstractPainterColor(color)
    {
    }

    virtual void setColor(colortype color)
    {
        AbstractPainterColor::setColor(color);
        painterRed = Color::getRed(color);
        painterGreen = Color::getGreen(color);
        painterBlue = Color::getBlue(color);
        color8 = LCD8bpp_ARGB2222::getNativeColor(painterColor);
    }

    virtual void paint(uint8_t* destination, int16_t offset, int16_t widgetX, int16_t widgetY, int16_t count, uint8_t alpha) const;

protected:
    uint8_t painterRed;   ///< The red part of the color, used when alpha blending
    uint8_t painterGreen; ///< The green part of the color, used when alpha blending
    uint8_t painterBlue;  ///< The blue part of the color, used when alpha blending
    uint8_t color8;       ///< Pre-calculated native color
};

} // namespace touchgfx

#endif // TOUCHGFX_PAINTERARGB2222_HPP
