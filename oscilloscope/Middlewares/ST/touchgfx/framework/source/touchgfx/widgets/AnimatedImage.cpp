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

#include <touchgfx/Application.hpp>
#include <touchgfx/widgets/AnimatedImage.hpp>

namespace touchgfx
{
void AnimatedImage::handleTickEvent()
{
    if (!running)
    {
        return;
    }
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

void AnimatedImage::startAnimation(const bool rev, const bool reset /*= false*/, const bool loop /*= false*/)
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
    if (running)
    {
        Application::getInstance()->unregisterTimerWidget(this);
        running = false;
    }
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

void AnimatedImage::setBitmap(const Bitmap& bmp)
{
    startId = bmp.getId();
    Image::setBitmap(bmp);
}

void AnimatedImage::setBitmapEnd(const Bitmap& bmp)
{
    endId = bmp.getId();
}

void AnimatedImage::setBitmaps(BitmapId start, BitmapId end)
{
    setBitmap(start);
    setBitmapEnd(end);
}

void AnimatedImage::setUpdateTicksInterval(uint8_t updateInterval)
{
    updateTicksInterval = updateInterval;
    ticksSinceUpdate = 0;
}
} // namespace touchgfx
