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
#include <touchgfx/widgets/canvas/Canvas.hpp>

namespace touchgfx
{
Canvas::Canvas(const CanvasWidget* _widget, const Rect& invalidatedArea) : widget(_widget),
    enoughMemory(false), penUp(true), penHasBeenDown(false), previousOutside(0), penDownOutside(0)
{
    assert(CanvasWidgetRenderer::hasBuffer() && "No buffer allocated for CanvasWidgetRenderer drawing");
    assert(Rasterizer::POLY_BASE_SHIFT == 5 && "CanvasWidget assumes Q5 but Rasterizer uses a different setting");

    // Area to redraw (relative coordinates)
    Rect dirtyArea = Rect(0, 0, widget->getWidth(), widget->getHeight()) & invalidatedArea;

    // Absolute position of the scalableImage.
    Rect dirtyAreaAbsolute = dirtyArea;
    widget->translateRectToAbsolute(dirtyAreaAbsolute);

    // Transform rects to match frame buffer coordinates
    // This is needed if the display is rotated compared to the frame buffer
    DisplayTransformation::transformDisplayToFrameBuffer(dirtyArea, widget->getRect());
    DisplayTransformation::transformDisplayToFrameBuffer(dirtyAreaAbsolute);

    // Re-size buffers for optimum memory buffer layout.
    enoughMemory = CanvasWidgetRenderer::setScanlineWidth(dirtyArea.width);
    ras.reset();

    offsetX = dirtyArea.x;
    offsetY = dirtyArea.y;
    invalidatedAreaX = CWRUtil::toQ5(dirtyArea.x);
    invalidatedAreaY = CWRUtil::toQ5(dirtyArea.y);
    invalidatedAreaWidth = CWRUtil::toQ5(dirtyArea.width);
    invalidatedAreaHeight = CWRUtil::toQ5(dirtyArea.height);

    // Create the rendering buffer
    uint8_t* RESTRICT buf = reinterpret_cast<uint8_t*>(HAL::getInstance()->lockFrameBuffer());
    int stride = 0;
    uint8_t offset = 0;
    uint8_t bitDepth = HAL::lcd().bitDepth();
    switch (bitDepth)
    {
    case 1: // BW
    case 2: // GRAY2
    case 4: // GRAY4
        {
            uint8_t bitsPerByte = 8 / bitDepth;
            stride = (HAL::FRAME_BUFFER_WIDTH + (bitsPerByte - 1)) / bitsPerByte;
            buf += (dirtyAreaAbsolute.x / bitsPerByte) + dirtyAreaAbsolute.y * stride;
            offset = dirtyAreaAbsolute.x % bitsPerByte;
        }
        break;
    case 16: // RGB565
    case 24: // RGB888
    case 32: // ARGB8888
        stride = HAL::FRAME_BUFFER_WIDTH * (bitDepth / 8);
        buf += dirtyAreaAbsolute.x * (bitDepth / 8) + dirtyAreaAbsolute.y * stride;
        break;
    default:
        assert(0 && "Unsupported bit depth");
        break;
    }
    ras.setMaxRenderY(dirtyAreaAbsolute.height);
    rbuf.attach(buf, offset, dirtyAreaAbsolute.width, dirtyAreaAbsolute.height, stride);
}

Canvas::~Canvas()
{
    HAL::getInstance()->unlockFrameBuffer(); //lint !e1551
}

void Canvas::moveTo(CWRUtil::Q5 x, CWRUtil::Q5 y)
{
    if (!enoughMemory)
    {
        return;
    }

    if (!penUp)
    {
        close();
    }

    transformFrameBufferToDisplay(x, y);
    x = x - invalidatedAreaX;
    y = y - invalidatedAreaY;

    uint8_t outside = isOutside(y, invalidatedAreaHeight);

    if (outside)
    {
        penUp = true;
    }
    else
    {
        penDownOutside = outside;
        ras.moveTo(x, y);
        penUp = false;
        penHasBeenDown = true;
    }

    initialX = x;
    initialY = y;

    previousX = x;
    previousY = y;
    previousOutside = outside;
}

void Canvas::lineTo(CWRUtil::Q5 x, CWRUtil::Q5 y)
{
    if (!enoughMemory)
    {
        return;
    }

    transformFrameBufferToDisplay(x, y);
    x = x - invalidatedAreaX;
    y = y - invalidatedAreaY;

    uint8_t outside = isOutside(y, invalidatedAreaHeight);

    if (!previousOutside)
    {
        ras.lineTo(x, y);
    }
    else
    {
        if (!outside || !(previousOutside & outside))
        {
            // x,y is inside, or on other side compared to previous
            if (penUp)
            {
                penDownOutside = previousOutside;
                ras.moveTo(previousX, previousY);
                penUp = false;
                penHasBeenDown = true;
            }
            else
            {
                ras.lineTo(previousX, previousY);
            }
            ras.lineTo(x, y);
        }
    }
    previousX = x;
    previousY = y;
    previousOutside = outside;
}

bool Canvas::render()
{
    // If the invalidated rect is too wide compared to the allocated buffer for CWR,
    // redrawing will not help. The CanvasWidget needs to know about this situation
    // and maybe try to divide the area vertically instead, but this has not been
    // implemented. And probably should not.
    if (!enoughMemory)
    {
        return true; // Redrawing a rect with fewer scanlines will not help, fake "ok" to move on
    }

    if (!penHasBeenDown)
    {
        return true; // Nothing drawn. Done
    }

    if (widget->getAlpha() == 0)
    {
        return true; // Invisible. Done
    }

    close();

    widget->getPainter().setOffset(offsetX/*+widget->getX()*/, offsetY/*+widget->getY()*/);
    widget->getPainter().setWidgetAlpha(widget->getAlpha());
    Renderer renderer(rbuf, widget->getPainter());
    return ras.render(renderer);
}

uint8_t Canvas::isOutside(const CWRUtil::Q5& y, const CWRUtil::Q5& height) const
{
    if (y < 0)
    {
        return POINT_IS_ABOVE;
    }
    if (y >= height)
    {
        return POINT_IS_BELOW;
    }
    return 0;
}

void Canvas::transformFrameBufferToDisplay(CWRUtil::Q5& x, CWRUtil::Q5& y) const
{
    switch (HAL::DISPLAY_ROTATION)
    {
    case rotate0:
        break;
    case rotate90:
        CWRUtil::Q5 tmpY = y;
        y = CWRUtil::toQ5(widget->getWidth()) - x;
        x = tmpY;
        break;
    }
}

void Canvas::close()
{
    if (!penUp)
    {
        if (previousOutside & penDownOutside)
        {
            // We are outside on the same side as we started. No need
            //  to close the path, CWR will do this for us.
            //lineTo(penDownX, penDownY);
        }
        else
        {
            if (previousOutside)
            {
                ras.lineTo(previousX, previousY);
            }
            ras.lineTo(initialX, initialY);
        }
    }
    penUp = false;
}
}
