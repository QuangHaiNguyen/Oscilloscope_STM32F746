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
#include <touchgfx/containers/progress_indicators/ImageProgress.hpp>

namespace touchgfx
{

ImageProgress::ImageProgress()
    : AbstractDirectionProgress(), image(), fixedPosition(true)
{
    container.add(image);
    progressIndicatorContainer.add(container);
}

ImageProgress::~ImageProgress()
{
}

void ImageProgress::setProgressIndicatorPosition(int16_t x, int16_t y, int16_t width, int16_t height)
{
    container.setPosition(0, 0, width, height);

    AbstractProgressIndicator::setProgressIndicatorPosition(x, y, width, height);
}

void ImageProgress::setAnchorAtZero(bool anchorAtZero)
{
    fixedPosition = anchorAtZero;
}

bool ImageProgress::getAnchorAtZero() const
{
    return fixedPosition;
}

void ImageProgress::setBitmap(touchgfx::BitmapId bitmapId)
{
    image.setBitmap(Bitmap(bitmapId));
}

touchgfx::BitmapId ImageProgress::getBitmap() const
{
    return image.getBitmap();
}

void ImageProgress::setAlpha(uint8_t alpha)
{
    image.setAlpha(alpha);
}

uint8_t ImageProgress::getAlpha() const
{
    return image.getAlpha();
}

void ImageProgress::setValue(int value)
{
    AbstractProgressIndicator::setValue(value);
    container.invalidate();
    uint16_t maxProgress = 0;
    if (progressDirection == AbstractDirectionProgress::RIGHT || progressDirection == AbstractDirectionProgress::LEFT)
    {
        maxProgress = progressIndicatorContainer.getWidth();
    }
    else
    {
        maxProgress = progressIndicatorContainer.getHeight();
    }
    int16_t progress = AbstractProgressIndicator::getProgress(maxProgress);
    if (fixedPosition)
    {
        switch (progressDirection)
        {
        case AbstractDirectionProgress::RIGHT:
            container.setPosition(0, 0, progress, progressIndicatorContainer.getHeight());
            image.setPosition(0, 0, progress, progressIndicatorContainer.getHeight());
            break;
        case AbstractDirectionProgress::LEFT:
            container.setPosition(getWidth() - progress, 0, progress, progressIndicatorContainer.getHeight());
            image.setPosition(-container.getX(), 0, progressIndicatorContainer.getWidth(), progressIndicatorContainer.getHeight());
            break;
        case AbstractDirectionProgress::DOWN:
            container.setPosition(0, 0, progressIndicatorContainer.getWidth(), progress);
            image.setPosition(0, 0, progressIndicatorContainer.getWidth(), progress);
            break;
        case AbstractDirectionProgress::UP:
            container.setPosition(0, progressIndicatorContainer.getHeight() - progress, progressIndicatorContainer.getWidth(), progress);
            image.setPosition(0, -container.getY(), progressIndicatorContainer.getWidth(), progressIndicatorContainer.getHeight());
            break;
        }
    }
    else
    {
        switch (progressDirection)
        {
        case AbstractDirectionProgress::RIGHT:
            container.setPosition(0, 0, progress, getHeight());
            image.setPosition(progress - progressIndicatorContainer.getWidth(), 0, progressIndicatorContainer.getWidth(), getHeight());
            break;
        case AbstractDirectionProgress::LEFT:
            container.setPosition(progressIndicatorContainer.getWidth() - progress, 0, progress, progressIndicatorContainer.getHeight());
            image.setPosition(0, 0, progress, progressIndicatorContainer.getHeight());
            break;
        case AbstractDirectionProgress::DOWN:
            container.setPosition(0, 0, progressIndicatorContainer.getWidth(), progress);
            image.setPosition(0, progress - progressIndicatorContainer.getHeight(), progressIndicatorContainer.getWidth(), progressIndicatorContainer.getHeight());
            break;
        case AbstractDirectionProgress::UP:
            container.setPosition(0, progressIndicatorContainer.getHeight() - progress, progressIndicatorContainer.getWidth(), progress);
            image.setPosition(0, 0, progressIndicatorContainer.getWidth(), progress);
            break;
        }
    }
    container.invalidate();
}

}
