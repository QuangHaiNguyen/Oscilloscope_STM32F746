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
#ifndef IMAGEPROGRESS_HPP
#define IMAGEPROGRESS_HPP

#include <touchgfx/containers/progress_indicators/AbstractDirectionProgress.hpp>
#include <touchgfx/widgets/Box.hpp>
#include <touchgfx/widgets/TiledImage.hpp>

namespace touchgfx
{
/**
 * @class ImageProgress ImageProgress.hpp ImageProgress.hpp touchgfx/containers/progress_indicators/ImageProgress.hpp
 *
 * @brief An image progress.
 *
 *        An image progress will show parts of an image as a progress indicator. The image can
 *        progress from the left, the right, the bottom or the top of the given area, and can
 *        visually be fixed with a larger and larger portion of the image showing, or it can be
 *        moved into view.
 */
class ImageProgress : public AbstractDirectionProgress
{
public:

    /**
     * @fn ImageProgress::ImageProgress();
     *
     * @brief Default constructor.
     *
     *        Default constructor.
     */
    ImageProgress();

    /**
     * @fn virtual ImageProgress::~ImageProgress();
     *
     * @brief Destructor.
     *
     *        Destructor.
     */
    virtual ~ImageProgress();

    /**
     * @fn virtual void ImageProgress::setProgressIndicatorPosition(int16_t x, int16_t y, int16_t width, int16_t height);
     *
     * @brief Sets the position and dimension of the image progress indicator.
     *
     *        Sets the position and dimension of the image progress indicator relative to the
     *        background image.
     *
     * @param x      The x coordinate.
     * @param y      The y coordinate.
     * @param width  The width of the image progress indicator.
     * @param height The height of the image progress indicator.
     */
    virtual void setProgressIndicatorPosition(int16_t x, int16_t y, int16_t width, int16_t height);

    /**
     * @fn virtual void ImageProgress::setAnchorAtZero(const bool anchorAtZero);
     *
     * @brief Sets anchor at zero.
     *
     *        Sets anchor at zero will control whether the image will be placed so that it is
     *        not moving during progress, only more and more of the image will become visible,
     *        or if the image is anchored at the current progress and will appear to slide into
     *        view.
     *
     * @param anchorAtZero true to anchor at zero, false to anchor at current progress.
     *
     * @see getAnchoredAtZero
     */
    virtual void setAnchorAtZero(bool anchorAtZero);

    /**
     * @fn virtual bool ImageProgress::getAnchorAtZero() const;
     *
     * @brief Gets anchor at zero.
     *
     *        Gets anchor at zero.
     *
     * @return true if the image is anchored at zero, false if it is anchored at current
     *         progress.
     *
     * @see setAnchorAtZero
     */
    virtual bool getAnchorAtZero() const;

    /**
     * @fn virtual void ImageProgress::setBitmap(touchgfx::BitmapId bitmapId);
     *
     * @brief Sets the bitmap id.
     *
     *        Sets the bitmap id to use for progress. Please note that the bitmap is tiled
     *        which will allow smaller bitmaps to repeat and save memory.
     *
     * @param bitmapId The bitmap id.
     *
     * @see getBitmap
     * @see TiledImage
     */
    virtual void setBitmap(touchgfx::BitmapId bitmapId);

    /**
     * @fn virtual touchgfx::BitmapId ImageProgress::getBitmap() const;
     *
     * @brief Gets the image.
     *
     *        Gets the image.
     *
     * @return The image.
     *
     * @see setBitmap
     */
    virtual touchgfx::BitmapId getBitmap() const;

    /**
     * @fn virtual void ImageProgress::setAlpha(uint8_t alpha);
     *
     * @brief Sets the alpha.
     *
     *        Sets the alpha of the image.
     *
     * @param alpha The alpha.
     *
     * @see getAlpha
     * @see Image::setAlpha
     */
    virtual void setAlpha(uint8_t alpha);

    /**
     * @fn virtual uint8_t ImageProgress::getAlpha() const;
     *
     * @brief Gets the alpha.
     *
     * @return The the alpha of the image.
     *
     * @see setAlpha
     * @see Image::getAlpha
     */
    virtual uint8_t getAlpha() const;

    virtual void setValue(int value);

protected:
    Container container; ///< The container
    TiledImage image;    ///< The image
    bool fixedPosition;  ///< true if the image should not move during progress
};

}

#endif // IMAGEPROGRESS_HPP
