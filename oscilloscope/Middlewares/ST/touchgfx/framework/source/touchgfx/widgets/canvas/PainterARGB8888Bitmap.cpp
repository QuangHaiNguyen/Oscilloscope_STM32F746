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

#include <touchgfx/Color.hpp>
#include <touchgfx/lcd/LCD.hpp>
#include <touchgfx/transforms/DisplayTransformation.hpp>
#include <touchgfx/widgets/canvas/PainterARGB8888Bitmap.hpp>

namespace touchgfx
{
void PainterARGB8888Bitmap::setBitmap(const Bitmap& bmp)
{
    AbstractPainterBitmap::setBitmap(bmp);
    assert((bitmap.getId() == BITMAP_INVALID || bitmapFormat == Bitmap::RGB565 || bitmapFormat == Bitmap::RGB888 || bitmapFormat == Bitmap::ARGB8888) && "PainterARGB8888Bitmap only works with RGB565, RGB888 and ARGB8888 bitmaps");
    assert(bitmap.getId() == BITMAP_INVALID || bitmapData);
    bitmapExtraData = bitmap.getExtraData();
}

void PainterARGB8888Bitmap::paint(uint8_t* destination, int16_t offset, int16_t widgetX, int16_t widgetY, int16_t count, uint8_t alpha) const
{
    if (!adjustBitmapXY(widgetX, widgetY, offset, count))
    {
        return;
    }

    uint8_t* RESTRICT framebuffer = destination + offset * 4;
    const uint8_t* const lineEnd = framebuffer + count * 4;
    const int32_t rowSkip = widgetY * bitmapRect.width;
    int16_t bitmapAvailable = bitmapRect.width - widgetX;
    if (bitmapFormat == Bitmap::ARGB8888)
    {
        const uint32_t* const bitmapLineStart = reinterpret_cast<const uint32_t*>(bitmapData) + rowSkip;
        const uint32_t* bitmapPointer = bitmapLineStart + widgetX;
        do
        {
            const int16_t length = MIN(bitmapAvailable, count);
            const uint8_t* const chunkend = framebuffer + length * 4;
            count -= length;
            do
            {
                const uint8_t srcAlpha = (*bitmapPointer) >> 24;
                const uint8_t alphaFg = LCD::div255(alpha * srcAlpha);
                const uint8_t alphaBg = framebuffer[3];
                if (alphaFg == 255 || alphaBg == 0)
                {
                    const uint8_t blueFg = *bitmapPointer;
                    *framebuffer++ = blueFg;
                    const uint8_t greenFg = (*bitmapPointer) >> 8;
                    *framebuffer++ = greenFg;
                    const uint8_t redFg = (*bitmapPointer) >> 16;
                    *framebuffer++ = redFg;
                    *framebuffer++ = alphaFg;
                }
                else if (alphaFg)
                {
                    const uint8_t alphaMult = LCD::div255(alphaFg * alphaBg);
                    const uint8_t alphaOut = alphaFg + alphaBg - alphaMult;

                    const uint8_t blueBg = *framebuffer;
                    const uint8_t blueFg = *bitmapPointer;
                    *framebuffer++ = (blueFg * alphaFg + blueBg * (alphaBg - alphaMult)) / alphaOut;
                    const uint8_t greenBg = *framebuffer;
                    const uint8_t greenFg = (*bitmapPointer) >> 8;
                    *framebuffer++ = (greenFg * alphaFg + greenBg * (alphaBg - alphaMult)) / alphaOut;
                    const uint8_t redBg = *framebuffer;
                    const uint8_t redFg = (*bitmapPointer) >> 16;
                    *framebuffer++ = (redFg * alphaFg + redBg * (alphaBg - alphaMult)) / alphaOut;
                    *framebuffer++ = alphaOut;
                }
                else
                {
                    framebuffer += 4;
                }
                bitmapPointer++;
            } while (framebuffer < chunkend);
            bitmapPointer = bitmapLineStart;
            bitmapAvailable = bitmapRect.width;
        } while (framebuffer < lineEnd);
    }
    else if (bitmapFormat == Bitmap::RGB888)
    {
        const uint8_t* const bitmapLineStart = reinterpret_cast<const uint8_t*>(bitmapData) + rowSkip * 3;
        const uint8_t* bitmapPointer = bitmapLineStart + widgetX * 3;
        do
        {
            const int16_t length = MIN(bitmapAvailable, count);
            const uint8_t* const chunkend = framebuffer + length * 4;
            count -= length;
            do
            {
                const uint8_t alphaBg = framebuffer[3];
                const uint8_t alphaMult = LCD::div255(alpha * alphaBg);
                const uint8_t alphaOut = alpha + alphaBg - alphaMult;

                const uint8_t blueBg = *framebuffer;
                const uint8_t blueFg = *bitmapPointer++;
                *framebuffer++ = (blueFg * alpha + blueBg * alphaBg - blueBg * alphaMult) / alphaOut;
                const uint8_t greenBg = *framebuffer;
                const uint8_t greenFg = *bitmapPointer++;
                *framebuffer++ = (greenFg * alpha + greenBg * alphaBg - greenBg * alphaMult) / alphaOut;
                const uint8_t redBg = *framebuffer;
                const uint8_t redFg = *bitmapPointer++;
                *framebuffer++ = (redFg * alpha + redBg * alphaBg - redBg * alphaMult) / alphaOut;
                *framebuffer++ = alphaOut;
            } while (framebuffer < chunkend);
            bitmapPointer = bitmapLineStart;
            bitmapAvailable = bitmapRect.width;
        } while (framebuffer < lineEnd);
    }
    else
    {
        const uint16_t* const bitmapLineStart = reinterpret_cast<const uint16_t*>(bitmapData) + rowSkip;
        const uint16_t* bitmapPointer = bitmapLineStart + widgetX;
        if (bitmapExtraData == 0)
        {
            do
            {
                const int16_t length = MIN(bitmapAvailable, count);
                const uint8_t* const chunkend = framebuffer + length * 4;
                count -= length;
                do
                {
                    const uint8_t alphaBg = framebuffer[3];
                    const uint8_t alphaMult = LCD::div255(alpha * alphaBg);
                    const uint8_t alphaOut = alpha + alphaBg - alphaMult;

                    const uint8_t blueBg = *framebuffer;
                    const uint8_t blueFg = Color::getBlueFromRGB565(*bitmapPointer);
                    *framebuffer++ = (blueFg * alpha + blueBg * alphaBg - blueBg * alphaMult) / alphaOut;
                    const uint8_t greenBg = *framebuffer;
                    const uint8_t greenFg = Color::getGreenFromRGB565(*bitmapPointer);
                    *framebuffer++ = (greenFg * alpha + greenBg * alphaBg - greenBg * alphaMult) / alphaOut;
                    const uint8_t redBg = *framebuffer;
                    const uint8_t redFg = Color::getRedFromRGB565(*bitmapPointer);
                    *framebuffer++ = (redFg * alpha + redBg * alphaBg - redBg * alphaMult) / alphaOut;
                    *framebuffer++ = alphaOut;
                    bitmapPointer++;
                } while (framebuffer < chunkend);
                bitmapPointer = bitmapLineStart;
                bitmapAvailable = bitmapRect.width;
            } while (framebuffer < lineEnd);
        }
        else
        {
            const uint8_t* const alpha_linestart = bitmapExtraData + rowSkip;
            const uint8_t* alphaPointer = alpha_linestart + widgetX;
            do
            {
                const int16_t length = MIN(bitmapAvailable, count);
                const uint8_t* const chunkend = framebuffer + length * 4;
                count -= length;
                do
                {
                    const uint8_t srcAlpha = *alphaPointer++;
                    const uint8_t alphaFg = LCD::div255(alpha * srcAlpha);
                    if (alphaFg)
                    {
                        const uint8_t alphaBg = framebuffer[3];
                        const uint8_t alphaMult = LCD::div255(alphaFg * alphaBg);
                        const uint8_t alphaOut = alphaFg + alphaBg - alphaMult;

                        const uint8_t blueBg = *framebuffer;
                        const uint8_t blueFg = Color::getBlueFromRGB565(*bitmapPointer);
                        *framebuffer++ = (blueFg * alphaFg + blueBg * alphaBg - blueBg * alphaMult) / alphaOut;
                        const uint8_t greenBg = *framebuffer;
                        const uint8_t greenFg = Color::getGreenFromRGB565(*bitmapPointer);
                        *framebuffer++ = (greenFg * alphaFg + greenBg * alphaBg - greenBg * alphaMult) / alphaOut;
                        const uint8_t redBg = *framebuffer;
                        const uint8_t redFg = Color::getRedFromRGB565(*bitmapPointer);
                        *framebuffer++ = (redFg * alphaFg + redBg * alphaBg - redBg * alphaMult) / alphaOut;
                        *framebuffer++ = alphaOut;
                    }
                    else
                    {
                        framebuffer += 4;
                    }
                    bitmapPointer++;
                } while (framebuffer < chunkend);
                bitmapPointer = bitmapLineStart;
                alphaPointer = alpha_linestart;
                bitmapAvailable = bitmapRect.width;
            } while (framebuffer < lineEnd);
        }
    }
}
} // namespace touchgfx
