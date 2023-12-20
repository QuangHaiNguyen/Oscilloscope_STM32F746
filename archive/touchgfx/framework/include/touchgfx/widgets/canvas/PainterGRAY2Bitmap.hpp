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
#ifndef PAINTERGRAY2BITMAP_HPP
#define PAINTERGRAY2BITMAP_HPP

#include <stdint.h>
#include <touchgfx/widgets/canvas/AbstractPainterGRAY2.hpp>
#include <touchgfx/Bitmap.hpp>
#include <touchgfx/transforms/DisplayTransformation.hpp>

namespace touchgfx
{
/**
 * @class PainterGRAY2Bitmap PainterGRAY2Bitmap.hpp touchgfx/widgets/canvas/PainterGRAY2Bitmap.hpp
 *
 * @brief A Painter that will paint using a bitmap.
 *
 *        PainterGRAY2Bitmap will take the color for a given point in the shape from a bitmap.
 *        Please be aware, the the bitmap is used by the CWR (not Shape), so any rotation you
 *        might specify for a Canvas Widget (e.g. Shape) is not applied to the bitmap as CWR is
 *        not aware of this rotation.
 *
 * @see AbstractPainter
 */
class PainterGRAY2Bitmap : public AbstractPainterGRAY2
{
public:

    /**
     * @fn PainterGRAY2Bitmap::PainterGRAY2Bitmap(const Bitmap& bmp = Bitmap(BITMAP_INVALID), uint8_t alpha = 255);
     *
     * @brief Constructor.
     *
     *        Constructor.
     *
     * @param bmp   The bitmap.
     * @param alpha the alpha.
     */
    PainterGRAY2Bitmap(const Bitmap& bmp = Bitmap(BITMAP_INVALID), uint8_t alpha = 255);

    /**
     * @fn void PainterGRAY2Bitmap::setBitmap(const Bitmap& bmp);
     *
     * @brief Sets a bitmap to be used when drawing the CanvasWidget.
     *
     *        Sets a bitmap to be used when drawing the CanvasWidget.
     *
     * @param bmp The bitmap.
     */
    void setBitmap(const Bitmap& bmp);

    /**
     * @fn void PainterGRAY2Bitmap::setAlpha(uint8_t alpha);
     *
     * @brief Sets an alpha value for the bitmap.
     *
     *        Sets an alpha value for the bitmap. If the image contains an alpha channel, this
     *        alpha value is combined with the alpha in the bitmap to produce the final alpha
     *        value.
     *
     * @param alpha The alpha value to use if there is no alpha channel in the bitmap.
     */
    void setAlpha(uint8_t alpha);

    /**
     * @fn uint8_t PainterGRAY2Bitmap::getAlpha() const;
     *
     * @brief Gets the current alpha value.
     *
     *        Gets the current alpha value.
     *
     * @return The current alpha value.
     *
     * @see setAlpha
     */
    uint8_t getAlpha() const;

    virtual void render(uint8_t* ptr, int x, int xAdjust, int y, unsigned count, const uint8_t* covers);

protected:
    virtual bool renderInit();

    virtual bool renderNext(uint8_t& gray, uint8_t& alpha);

    const uint8_t* bitmapGRAY2Pointer; ///< Pointer to the bitmap (GRAY2)
    const uint8_t* bitmapAlphaPointer; ///< Pointer to the bitmap alpha data for GRAY2

    Bitmap bitmap;                  ///< The bitmap to be used when painting
    Rect   bitmapRectToFrameBuffer; ///< Bitmap rectangle translated to frame buffer coordinates

    uint8_t painterAlpha; ///< The alpha to use if no alpha data is present in the given bitmap

}; // class PainterGRAY2Bitmap

} // namespace touchgfx

#endif // PAINTERGRAY2BITMAP_HPP
