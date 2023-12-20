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
#include <touchgfx/widgets/AnimatedImage.hpp>

namespace touchgfx
{

void AnimatedImage::handleTickEvent()
{
    if (running)
    {
        ++ticksSinceUpdate;
        if (ticksSinceUpdate != updateTicksInterval)
        {
            return;
        }

        ticksSinceUpdate = 0;
        BitmapId currentId = getBitmap();

        if (((currentId == endId) && !reverse) || ((currentId == startId) && reverse))
        {
            if (!loopAnimation)
            {
                Application::getInstance()->unregisterTimerWidget(this);
                running = false;
            }

            if (animationDoneAction && animationDoneAction->isValid())
            {
                animationDoneAction->execute(*this);
            }

            if (running && loopAnimation)
            {
                if (reverse)
                {
                    Image::setBitmap(Bitmap(endId));
                }
                else
                {
                    Image::setBitmap(Bitmap(startId));
                }
                invalidate();
            }
        }
        else
        {
            if (reverse)
            {
                --currentId;
            }
            else
            {
                ++currentId;
            }
            Image::setBitmap(Bitmap(currentId));
            invalidate();
        }
    }
}

void AnimatedImage::startAnimation(const bool& rev, const bool& reset, const bool& loop)
{
    if ((startId != BITMAP_INVALID) && (endId != BITMAP_INVALID))
    {
        reverse = rev;
        loopAnimation = loop;
        if (reverse && reset)
        {
            Image::setBitmap(Bitmap(endId));
            invalidate();
        }
        else if (!reverse && reset)
        {
            Image::setBitmap(Bitmap(startId));
            invalidate();
        }
        Application::getInstance()->registerTimerWidget(this);
        running = true;
    }
}

void AnimatedImage::stopAnimation()
{
    Application::getInstance()->unregisterTimerWidget(this);
    running = false;
    if (reverse)
    {
        Image::setBitmap(Bitmap(endId));
    }
    else
    {
        Image::setBitmap(Bitmap(startId));
    }
    invalidate();
}

void AnimatedImage::pauseAnimation()
{
    if (running)
    {
        Application::getInstance()->unregisterTimerWidget(this);
        running = false;
    }
    else
    {
        Application::getInstance()->registerTimerWidget(this);
        running = true;
    }
}

void AnimatedImage::setBitmaps(BitmapId start, BitmapId end)
{
    startId = start;
    endId = end;
    Image::setBitmap(Bitmap(startId));
}

void AnimatedImage::setUpdateTicksInterval(uint8_t updateInterval)
{
    updateTicksInterval = updateInterval;
    ticksSinceUpdate = 0;
}

} // namespace touchgfx
