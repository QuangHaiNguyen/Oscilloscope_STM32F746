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
#include <touchgfx/containers/progress_indicators/BoxProgress.hpp>

namespace touchgfx
{

BoxProgress::BoxProgress()
    : AbstractDirectionProgress(), box()
{
    progressIndicatorContainer.add(box);
}

BoxProgress::~BoxProgress()
{
}

void BoxProgress::setProgressIndicatorPosition(int16_t x, int16_t y, int16_t width, int16_t height)
{
    box.setPosition(0, 0, width, height);

    AbstractProgressIndicator::setProgressIndicatorPosition(x, y, width, height);
}

void BoxProgress::setColor(colortype color)
{
    box.setColor(color);
}

touchgfx::colortype BoxProgress::getColor() const
{
    return box.getColor();
}

void BoxProgress::setAlpha(uint8_t alpha)
{
    box.setAlpha(alpha);
}

uint8_t BoxProgress::getAlpha() const
{
    return box.getAlpha();
}

void BoxProgress::setValue(int value)
{
    AbstractProgressIndicator::setValue(value);
    uint16_t progress = AbstractProgressIndicator::getProgress();
    box.invalidate();
    int16_t w = (progressIndicatorContainer.getWidth() * progress) / 100;
    int16_t h = (progressIndicatorContainer.getHeight() * progress) / 100;
    switch (progressDirection)
    {
    case AbstractDirectionProgress::RIGHT:
        box.setPosition(0, 0, w, progressIndicatorContainer.getHeight());
        break;
    case AbstractDirectionProgress::LEFT:
        box.setPosition(getWidth() - w, 0, w, progressIndicatorContainer.getHeight());
        break;
    case AbstractDirectionProgress::DOWN:
        box.setPosition(0, 0, progressIndicatorContainer.getWidth(), h);
        break;
    case AbstractDirectionProgress::UP:
        box.setPosition(0, progressIndicatorContainer.getHeight() - h, progressIndicatorContainer.getWidth(), h);
        break;
    }
    box.invalidate();
}

}
