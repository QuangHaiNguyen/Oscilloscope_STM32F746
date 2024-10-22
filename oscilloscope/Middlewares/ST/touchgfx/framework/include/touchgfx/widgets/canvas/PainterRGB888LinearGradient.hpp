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
 * @file touchgfx/widgets/canvas/PainterRGB888LinearGradient.hpp
 *
 * Declares the touchgfx::PainterRGB888LinearGradient class.
 */
#ifndef TOUCHGFX_PAINTERRGB888LINEARGRADIENT_HPP
#define TOUCHGFX_PAINTERRGB888LINEARGRADIENT_HPP

#include <touchgfx/hal/Types.hpp>
#include <touchgfx/widgets/canvas/AbstractPainterLinearGradient.hpp>

namespace touchgfx
{
/**
 * An abstract class for creating painter classes for drawing canvas widgets. All canvas widgets
 * need a painter to fill the shape drawn with a CanvasWidgetRenderer. The painter must provide
 * the color of a pixel on a given coordinate, which will the be blended into the framebuffer
 * depending on the position of the canvas widget and the transparency of the given pixel.
 */
class PainterRGB888LinearGradient : public AbstractPainterLinearGradient
{
public:
    /** Destructor. */
    virtual ~PainterRGB888LinearGradient()
    {
    }

    virtual void paint(uint8_t* destination, int16_t offset, int16_t widgetX, int16_t widgetY, int16_t count, uint8_t alpha) const;

    virtual void tearDown() const;
};

} // namespace touchgfx

#endif // TOUCHGFX_PAINTERRGB888LINEARGRADIENT_HPP
