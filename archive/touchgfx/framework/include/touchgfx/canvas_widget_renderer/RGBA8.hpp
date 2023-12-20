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
#ifndef RGBA8_HPP
#define RGBA8_HPP

#include <touchgfx/hal/Types.hpp>

namespace touchgfx
{
/**
 * @struct RGBA8 RGBA8.hpp touchgfx/canvas_widget_renderer/RGBA8.hpp
 *
 * @brief Deprecated, use colortype instead.
 *
 * @note Deprecated, use colortype instead.
 *
 * @see colortype
 */
struct RGBA8
{
    uint8_t red;   ///< The red part of the color.
    uint8_t green; ///< The green part of the color.
    uint8_t blue;  ///< The blue part of the color.
    uint8_t alpha; ///< The alpha value of the color, specifying how transparent the color is  (0 = invisible, 255 = solid).

    /**
     * @fn RGBA8()
     *
     * @brief Default constructor. Deprecated, use colortype instead.
     *
     *        Deprecated, use colortype instead.
     *
     * @note Deprecated, use colortype instead.
     *
     * @see colortype
     */
    RGBA8() : red(0), green(0), blue(0), alpha(0) { }

    /**
     * @fn RGBA8(uint16_t rgb565, unsigned alpha_ = 255)
     *
     * @brief Constructor. Deprecated, use colortype instead.
     *
     *        Deprecated, use colortype instead.
     *
     * @note Deprecated, use colortype instead.
     *
     * @param rgb565 RGB565 encoded color.
     * @param alpha_ alpha value, specifying the transparency of the color (0 = invisible, 255 =
     *               solid).
     *
     * @see colortype
     */
    RGBA8(uint16_t rgb565, unsigned alpha_ = 255) :
        red(uint8_t((rgb565 & 0xF800) >> 8)),
        green(uint8_t((rgb565 & 0x07E0) >> 3)),
        blue(uint8_t((rgb565 & 0x001F) << 3)),
        alpha(uint8_t(alpha_))
    {
        red |= red >> 5;
        green |= green >> 6;
        blue |= blue >> 5;
    }

    /**
     * @fn RGBA8(unsigned red_, unsigned green_, unsigned blue_, unsigned alpha_ = 255)
     *
     * @brief Constructor. Deprecated, use colortype instead.
     *
     *        Deprecated, use colortype instead.
     *
     * @note Deprecated, use colortype instead.
     *
     * @param red_   The red part of the color.
     * @param green_ The green part of the color.
     * @param blue_  The blue part of the color.
     * @param alpha_ alpha value, specifying the transparency of the color (0 = invisible, 255 =
     *               solid).
     *
     * @see colortype
     */
    RGBA8(unsigned red_, unsigned green_, unsigned blue_, unsigned alpha_ = 255) :
        red(uint8_t(red_)),
        green(uint8_t(green_)),
        blue(uint8_t(blue_)),
        alpha(uint8_t(alpha_))
    {
    }
};

} // namespace touchgfx

#endif // RGBA8_HPP
