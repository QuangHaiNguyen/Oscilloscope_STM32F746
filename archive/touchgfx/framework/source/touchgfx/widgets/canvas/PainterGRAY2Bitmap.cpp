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
#include <touchgfx/widgets/canvas/PainterGRAY2Bitmap.hpp>
#include <platform/driver/lcd/LCD2bpp.hpp>

namespace touchgfx
{

PainterGRAY2Bitmap::PainterGRAY2Bitmap(const Bitmap& bmp, uint8_t alpha) :
    AbstractPainterGRAY2(), bitmapGRAY2Pointer(0), bitmapAlphaPointer(0)
{
    setBitmap(bmp);
    setAlpha(alpha);
}

void PainterGRAY2Bitmap::setBitmap(const Bitmap& bmp)
{
    bitmap = bmp;
    bitmapRectToFrameBuffer = bitmap.getRect();
    DisplayTransformation::transformDisplayToFrameBuffer(bitmapRectToFrameBuffer);
}

void PainterGRAY2Bitmap::setAlpha(uint8_t alpha)
{
    painterAlpha = alpha;
}

uint8_t PainterGRAY2Bitmap::getAlpha() const
{
    return painterAlpha;
}

void PainterGRAY2Bitmap::render(uint8_t* ptr, int x, int xAdjust, int y, unsigned count, const uint8_t* covers)
{
    currentX = x + areaOffsetX;
    currentY = y + areaOffsetY;
    x += xAdjust;

    if (!renderInit())
    {
        return;
    }

    uint8_t totalAlpha = (widgetAlpha * painterAlpha) / 255;
    if (bitmapAlphaPointer)
    {
        if (totalAlpha == 255)
        {
            do
            {
                uint8_t gray = LCD2getPixel(bitmapGRAY2Pointer, currentX);
                uint16_t alpha = (*covers) * LCD2getPixel(bitmapAlphaPointer, currentX);
                covers++;

                if (alpha == 255u * 3u)
                {
                    // Render a solid pixel
                    LCD2setPixel(ptr, x, gray);
                }
                else
                {
                    uint8_t p_gray = LCD2getPixel(ptr, x);
                    LCD2setPixel(ptr, x, static_cast<uint8_t>((((gray - p_gray) * alpha) >> 10) + p_gray));
                }
                currentX++;
                x++;
            }
            while (--count != 0);
        }
        else
        {
            do
            {
                uint8_t gray = LCD2getPixel(bitmapGRAY2Pointer, currentX);
                uint32_t alpha = (*covers) * totalAlpha * LCD2getPixel(bitmapAlphaPointer, currentX);
                covers++;

                uint8_t p_gray = LCD2getPixel(ptr, x);
                LCD2setPixel(ptr, x, static_cast<uint8_t>((((gray - p_gray) * alpha) >> 18) + p_gray));
                currentX++;
                x++;
            }
            while (--count != 0);
        }
    }
    else
    {
        if (totalAlpha == 255)
        {
            do
            {
                uint8_t gray = LCD2getPixel(bitmapGRAY2Pointer, currentX);
                uint8_t alpha = (*covers);
                covers++;

                if (alpha == 255)
                {
                    // Render a solid pixel
                    LCD2setPixel(ptr, x, gray);
                }
                else
                {
                    uint8_t p_gray = LCD2getPixel(ptr, x);
                    LCD2setPixel(ptr, x, static_cast<uint8_t>((((gray - p_gray) * alpha) >> 8) + p_gray));
                }
                currentX++;
                x++;
            }
            while (--count != 0);
        }
        else
        {
            do
            {
                uint8_t gray = LCD2getPixel(bitmapGRAY2Pointer, currentX);
                uint16_t alpha = (*covers) * totalAlpha;
                covers++;

                uint8_t p_gray = LCD2getPixel(ptr, x);
                LCD2setPixel(ptr, x, static_cast<uint8_t>((((gray - p_gray) * alpha) >> 16) + p_gray));
                currentX++;
                x++;
            }
            while (--count != 0);
        }
    }
}

bool PainterGRAY2Bitmap::renderInit()
{
    bitmapGRAY2Pointer = 0;
    bitmapAlphaPointer = 0;

    if ((currentX >= bitmapRectToFrameBuffer.width) ||
            (currentY >= bitmapRectToFrameBuffer.height))
    {
        // Outside bitmap area, do not draw anything
        return false;
    }

    bitmapGRAY2Pointer = (const uint8_t*)bitmap.getData();
    if (!bitmapGRAY2Pointer)
    {
        return false;
    }
    bitmapGRAY2Pointer += currentY * ((bitmapRectToFrameBuffer.width + 3) / 4);
    // Get alpha data (GRAY2 format)
    bitmapAlphaPointer = (const uint8_t*)bitmap.getAlphaData();
    if (bitmapAlphaPointer)
    {
        bitmapAlphaPointer += currentY * ((bitmapRectToFrameBuffer.width + 3) / 4);
    }

    return true;
}

bool PainterGRAY2Bitmap::renderNext(uint8_t& gray, uint8_t& alpha)
{
    if (currentX >= bitmapRectToFrameBuffer.width)
    {
        return false;
    }

    if (bitmapGRAY2Pointer != 0)
    {
        gray = LCD2getPixel(bitmapGRAY2Pointer, currentX);
        if (bitmapAlphaPointer)
        {
            alpha = LCD2getPixel(bitmapAlphaPointer, currentX);
            alpha *= 0x55; // Upscale from 00-03 to 00-FF
        }
        else
        {
            alpha = 255; // No alpha per pixel in the image, it is solid
        }
    }
    if (painterAlpha < 255)
    {
        // Apply given alpha from setAlpha()
        alpha = (((uint16_t)alpha) * ((uint16_t)painterAlpha)) / 255;
    }
    return true;
}

} // namespace touchgfx
