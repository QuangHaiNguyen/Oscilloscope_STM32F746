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
#ifndef PAINTERBW_HPP
#define PAINTERBW_HPP

#include <touchgfx/widgets/canvas/AbstractPainterBW.hpp>
#include <touchgfx/hal/Types.hpp>

namespace touchgfx
{
/**
 * @class PainterBW PainterBW.hpp touchgfx/widgets/canvas/PainterBW.hpp
 *
 * @brief A Painter that will paint using a color on a LCD1bpp display.
 *
 *        PainterBW is used for drawing one 1bpp displays. The color is either on or off No
 *        transparency is supported.
 *
 * @see AbstractPainter
 */
class PainterBW : public AbstractPainterBW
{
public:

    /**
     * @fn static unsigned PainterBW::bw(unsigned red, unsigned green, unsigned blue);
     *
     * @brief Convert color to black/white.
     *
     *        Converts the selected color to either white (1) or black (0) depending on the
     *        converted gray value.
     *
     * @param red   The red color.
     * @param green The green color.
     * @param blue  The blue color.
     *
     * @return 1 (white) if the brightness of the RGB color is above 50% and 0 (black) otherwise.
     */
    static unsigned bw(unsigned red, unsigned green, unsigned blue);

    /**
     * @fn void PainterBW::setColor(colortype color);
     *
     * @brief Sets color to use when drawing the CanvasWidget.
     *
     *        Sets color to use when drawing the CanvasWidget.
     *
     * @param color The color, 0=black, otherwise white.
     */
    void setColor(colortype color);


    /**
     * @fn colortype PainterBW::getColor() const;
     *
     * @brief Gets the current color.
     *
     *        Gets the current color.
     *
     * @return The color.
     */
    colortype getColor() const;

    virtual void render(uint8_t* ptr, int x, int xAdjust, int y, unsigned count, const uint8_t* covers);

protected:
    virtual bool renderNext(uint8_t& color);

    uint8_t painterColor; ///< The color to use when painting

}; // class PainterBW

} // namespace touchgfx

#endif // PAINTERBW_HPP
