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
#ifndef DISPLAYTRANSFORMATION_HPP
#define DISPLAYTRANSFORMATION_HPP

#include <touchgfx/hal/Types.hpp>

namespace touchgfx
{
/**
 * @class DisplayTransformation DisplayTransformation.hpp touchgfx/transforms/DisplayTransformation.hpp
 *
 * @brief Defines transformations from display space to frame buffer space.
 *
 *        Defines transformations from display space to frame buffer space. The display might
 *        be (considered) in portrait mode from 0,0 to 272,480, while the actual frame buffer
 *        is from 0,0 to 480,272. This class handles the transformations.
 */
class DisplayTransformation
{
public:

    /**
     * @fn static void DisplayTransformation::transformDisplayToFrameBuffer(int16_t& x, int16_t& y);
     *
     * @brief Transform x,y from display to frame buffer coordinates.
     *
     *        Transform x,y from display to frame buffer coordinates.
     *
     * @param [in,out] x the x part to translate.
     * @param [in,out] y the y part to translate.
     */
    static void transformDisplayToFrameBuffer(int16_t& x, int16_t& y);

    /**
     * @fn static void DisplayTransformation::transformDisplayToFrameBuffer(float& x, float& y);
     *
     * @brief Transform x,y from display to frame buffer coordinates.
     *
     *        Transform x,y from display to frame buffer coordinates.
     *
     * @param [in,out] x the x part to translate.
     * @param [in,out] y the y part to translate.
     */
    static void transformDisplayToFrameBuffer(float& x, float& y);

    /**
     * @fn static void DisplayTransformation::transformFrameBufferToDisplay(int16_t& x, int16_t& y);
     *
     * @brief Transform x,y from frame buffer to display coordinates.
     *
     *        Transform x,y from frame buffer to display coordinates.
     *
     * @param [in,out] x the x part to translate.
     * @param [in,out] y the y part to translate.
     */
    static void transformFrameBufferToDisplay(int16_t& x, int16_t& y);

    /**
     * @fn static void DisplayTransformation::transformDisplayToFrameBuffer(int16_t& x, int16_t& y, const Rect& in);
     *
     * @brief Transform x,y from coordinates relative to the in rect to frame buffer coordinates.
     *
     *        Transform x,y from coordinates relative to the in rect to frame buffer
     *        coordinates.
     *
     * @param [in,out] x the x part to translate.
     * @param [in,out] y the y part to translate.
     * @param in         the rectangle defining the coordinate space.
     */
    static void transformDisplayToFrameBuffer(int16_t& x, int16_t& y, const Rect& in);

    /**
     * @fn static void DisplayTransformation::transformDisplayToFrameBuffer(float& x, float& y, const Rect& in);
     *
     * @brief Transform x,y from coordinates relative to the in rect to frame buffer coordinates.
     *
     *        Transform x,y from coordinates relative to the in rect to frame buffer
     *        coordinates.
     *
     * @param [in,out] x the x part to translate.
     * @param [in,out] y the y part to translate.
     * @param in         the rectangle defining the coordinate space.
     */
    static void transformDisplayToFrameBuffer(float& x, float& y, const Rect& in);

    /**
     * @fn static void DisplayTransformation::transformDisplayToFrameBuffer(Rect& r);
     *
     * @brief Transform rectangle from display to frame buffer coordinates.
     *
     *        Transform rectangle from display to frame buffer coordinates.
     *
     * @param [in,out] r the rectangle to translate.
     */
    static void transformDisplayToFrameBuffer(Rect& r);

    /**
     * @fn static void DisplayTransformation::transformDisplayToFrameBuffer(Rect& r, const Rect& in);
     *
     * @brief Transform rectangle r from coordinates relative to the in rect to frame buffer
     *        coordinates.
     *
     *        Transform rectangle r from coordinates relative to the in rect to frame buffer
     *        coordinates.
     *
     * @param [in,out] r the rectangle to translate.
     * @param in         the rectangle defining the coordinate space.
     */
    static void transformDisplayToFrameBuffer(Rect& r, const Rect& in);

};

} // namespace touchgfx

#endif // DISPLAYTRANSFORMATION_HPP
