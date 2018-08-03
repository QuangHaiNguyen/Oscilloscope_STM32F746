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
#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <touchgfx/canvas_widget_renderer/RenderingBuffer.hpp>
#include <touchgfx/widgets/canvas/AbstractPainter.hpp>
#include <touchgfx/hal/HAL.hpp>
namespace touchgfx
{
/**
 * @class Renderer Renderer.hpp touchgfx/canvas_widget_renderer/Renderer.hpp
 *
 * @brief This class template is used basically for rendering scan lines.
 *
 *        This class template is used basically for rendering scanlines. The 'Span' argument is
 *        one of the span renderers, such as SpanRGB565 and others.
 */
class Renderer
{
public:
    /**
     * @fn Renderer::Renderer()
     *
     * @brief Default constructor.
     *
     *        Default constructor. Function setRenderingBuffer() should be called to specify
     *        where the polygon should be rendered.
     */
    Renderer()
    {
    }

    /**
     * @fn Renderer::Renderer(RenderingBuffer& renderingBuffer, AbstractPainter &painter)
     *
     * @brief Constructor.
     *
     *        Constructor.
     *
     * @param [in] renderingBuffer The screen buffer to render the polygon in.
     * @param [in] painter         The painter to use for drawing individual pixels in a
     *                             scanline.
     */
    Renderer(RenderingBuffer& renderingBuffer, AbstractPainter& painter) : renderingBuffer(&renderingBuffer), painter(&painter)
    {
    }

    /**
     * @fn void Renderer::setRenderingBuffer(RenderingBuffer& renderingBuffer)
     *
     * @brief Sets rendering buffer.
     *
     *        Sets rendering buffer.
     *
     * @param [in] renderingBuffer The screen buffer to render the polygon in.
     */
    void setRenderingBuffer(RenderingBuffer& renderingBuffer)
    {
        this->renderingBuffer = &renderingBuffer;
    }

    /**
     * @fn void Renderer::render(const Scanline& scanline)
     *
     * @brief Render the given Scanline in the given color.
     *
     *        Render the given Scanline in the given color.
     *
     * @param scanline The Scanline.
     */
    void render(const Scanline& scanline)
    {
        if (scanline.getY() < 0 || scanline.getY() >= int(renderingBuffer->getHeight()))
        {
            return;
        }

        unsigned numSpans = scanline.getNumSpans();
        int baseX = 0;
        int y = scanline.getY();
        unsigned char* row = renderingBuffer->row(y);
        Scanline::iterator spanIterator(scanline);

        uint8_t xAdjust = renderingBuffer->getXAdjust();
        do
        {
            int x = spanIterator.next() + baseX;
            const uint8_t* covers = spanIterator.getCovers();
            int numPix = spanIterator.getNumPix();
            if (x < 0)
            {
                numPix += x;
                if (numPix <= 0)
                {
                    continue;
                }
                covers -= x;
                x = 0;
            }
            if (x + numPix >= int(renderingBuffer->getWidth()))
            {
                numPix = renderingBuffer->getWidth() - x;
                if (numPix <= 0)
                {
                    continue;
                }
            }
            painter->render(row, x, xAdjust, y, numPix, covers);
        }
        while (--numSpans);
    }

    /**
     * @fn RenderingBuffer& Renderer::getRenderingBuffer()
     *
     * @brief Gets the getRenderingBuffer.
     *
     *        Gets the getRenderingBuffer.
     *
     * @return A RenderingBuffer&amp;
     */
    RenderingBuffer& getRenderingBuffer()
    {
        return *renderingBuffer;
    }

private:
    RenderingBuffer* renderingBuffer;   ///< Buffer for rendering data
    AbstractPainter*         painter;   ///< The painter
};

} // namespace touchgfx

#endif // RENDERER_HPP
