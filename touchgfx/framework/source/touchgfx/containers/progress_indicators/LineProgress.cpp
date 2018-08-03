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
#include <touchgfx/containers/progress_indicators/LineProgress.hpp>

namespace touchgfx
{

LineProgress::LineProgress()
    : AbstractProgressIndicator(), line(), endX(0), endY(0)
{
    progressIndicatorContainer.add(line);
    line.setPosition(0, 0, getWidth(), getHeight());
}

LineProgress::~LineProgress()
{
}

void LineProgress::setProgressIndicatorPosition(int16_t x, int16_t y, int16_t width, int16_t height)
{
    line.setPosition(0, 0, width, height);

    AbstractProgressIndicator::setProgressIndicatorPosition(x, y, width, height);
}

void LineProgress::setPainter(AbstractPainter& painter)
{
    line.setPainter(painter);
}

void LineProgress::setStart(int x, int y)
{
    startX = CWRUtil::toQ5(x);
    startY = CWRUtil::toQ5(y);
    line.setStart(x, y);
}

void LineProgress::getStart(int& x, int& y) const
{
    x = startX.to<int>();
    y = startY.to<int>();
}

void LineProgress::setEnd(int x, int y)
{
    endX = CWRUtil::toQ5(x);
    endY = CWRUtil::toQ5(y);
}

void LineProgress::getEnd(int& x, int& y) const
{
    x = endX.to<int>();
    y = endY.to<int>();
}

void LineProgress::setLineWidth(int width)
{
    line.setLineWidth(width);
}

int LineProgress::getLineWidth() const
{
    int width;
    line.getLineWidth(width);
    return width;
}

void LineProgress::setLineEndingStyle(Line::LINE_ENDING_STYLE lineEndingStyle)
{
    line.setLineEndingStyle(lineEndingStyle);
}

touchgfx::Line::LINE_ENDING_STYLE LineProgress::getLineEndingStyle() const
{
    return line.getLineEndingStyle();
}

void LineProgress::setAlpha(uint8_t alpha)
{
    line.setAlpha(alpha);
}

uint8_t LineProgress::getAlpha() const
{
    return line.getAlpha();
}

void LineProgress::setValue(int value)
{
    if (rangeSteps > 0)
    {
        AbstractProgressIndicator::setValue(value);
        int progress = (int)AbstractProgressIndicator::getProgress(rangeSteps);
        CWRUtil::Q5 x = startX + (endX - startX) / (int)rangeSteps * progress;
        CWRUtil::Q5 y = startY + (endY - startY) / (int)rangeSteps * progress;
        line.updateEnd(x, y);
    }
}

}
