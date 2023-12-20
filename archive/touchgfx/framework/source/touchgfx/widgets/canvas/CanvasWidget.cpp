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
#include <touchgfx/widgets/canvas/CanvasWidget.hpp>

namespace touchgfx
{
CanvasWidget::CanvasWidget() :
    Widget(),
    canvasPainter(0),
    maxRenderLines(0x7FFF),
    alpha(255)
{
}

CanvasWidget::~CanvasWidget()
{
}

void CanvasWidget::setPainter(AbstractPainter& painter)
{
    canvasPainter = &painter;
}

AbstractPainter& CanvasWidget::getPainter() const
{
    assert(canvasPainter != 0 && "No painter set");
    return *canvasPainter; //lint !e613
} //lint !e1763

void CanvasWidget::draw(const Rect& invalidatedArea) const
{
    Rect area = invalidatedArea;

    int16_t* offset;
    int16_t* lines;
    int16_t* width;
    int16_t* height;

    int16_t wantedRenderLines;

    switch (HAL::DISPLAY_ROTATION)
    {
    default:
    case rotate0:
        offset = &area.y;
        lines = &area.height;
        width = &area.width;
        height = &wantedRenderLines;
        break;
    case rotate90:
        offset = &area.x;
        lines = &area.width;
        width = &wantedRenderLines;
        height = &area.height;
        break;
    }

    Rect minimalRect = getMinimalRect();

    bool failedAtLeastOnce = false;
    while (*lines)
    {
        wantedRenderLines = MIN(maxRenderLines, *lines);

        while (wantedRenderLines > 0)
        {
            Rect smallArea(area.x, area.y, *width, *height);
            if (!smallArea.intersect(minimalRect))
            {
                break;
            }
            if (drawCanvasWidget(smallArea))
            {
                break;
            }
#ifdef SIMULATOR
            if (CanvasWidgetRenderer::getWriteMemoryUsageReport())
            {
                if (wantedRenderLines > 1)
                {
                    printf("CWR will split draw into multiple draws due to limited memory.\n");
                }
                else
                {
                    printf("CWR was unable to complete a draw operation due to limited memory.\n");
                }
            }
#endif
            wantedRenderLines >>= 1;
            failedAtLeastOnce = true;
        }
        if (wantedRenderLines == 0)
        {
            // We did not manage to draw anything. Set wantedHeight to
            // one to skip a single raster line and try to render the
            // rest of the CanvasWidget.
            wantedRenderLines = 1;
        }
        else
        {
            if (failedAtLeastOnce)
            {
                maxRenderLines = wantedRenderLines;
            }
        }
        *offset += wantedRenderLines;
        *lines -= wantedRenderLines;
    }
}

void CanvasWidget::invalidate() const
{
    Rect minimalRect = getMinimalRect();
    invalidateRect(minimalRect);
}

Rect CanvasWidget::getMinimalRect() const
{
    return Rect(0, 0, getWidth(), getHeight());
}

Rect CanvasWidget::getSolidRect() const
{
    return Rect(0, 0, 0, 0);
}
}
