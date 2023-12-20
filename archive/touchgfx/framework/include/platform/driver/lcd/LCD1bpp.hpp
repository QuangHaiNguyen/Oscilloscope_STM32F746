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
#ifndef LCD1BPP_HPP
#define LCD1BPP_HPP

#include <touchgfx/hal/Types.hpp>
#include <touchgfx/hal/HAL.hpp>
#include <touchgfx/lcd/LCD.hpp>
#include <touchgfx/Font.hpp>
#include <touchgfx/Bitmap.hpp>
#include <touchgfx/Unicode.hpp>
#include <touchgfx/TextProvider.hpp>
#include <stdarg.h>

namespace touchgfx
{
#undef LCD

/**
 * @class LCD1bpp LCD1bpp.hpp platform/driver/lcd/LCD1bpp.hpp
 *
 * @brief This class contains the various low-level drawing routines for drawing bitmaps, texts and
 *        rectangles.
 *
 *        This class contains the various low-level drawing routines for drawing bitmaps, texts
 *        and rectangles on 1 bits per pixel displays.
 *
 * @note All coordinates are expected to be in absolute coordinates!
 *
 * @see LCD
 */
class LCD1bpp : public LCD
{
public:

    virtual ~LCD1bpp() {}

    /**
     * @fn virtual void LCD1bpp::drawPartialBitmap(const Bitmap& bitmap, int16_t x, int16_t y, const Rect& rect, uint8_t alpha = 255, bool useOptimized = true);
     *
     * @brief Draws a portion of a bitmap.
     *
     *        Draws a portion of a bitmap.
     *
     * @param bitmap       The bitmap to draw.
     * @param x            The absolute x coordinate to place pixel (0, 0) on the screen.
     * @param y            The absolute y coordinate to place pixel (0, 0) on the screen.
     * @param rect         A rectangle describing what region of the bitmap is to be drawn.
     * @param alpha        Optional alpha value (0 = invisible, otherwise solid). Default is 255
     *                     (solid).
     * @param useOptimized if false, do not attempt to substitute (parts of) this bitmap with
     *                     faster fillrects.
     */
    virtual void drawPartialBitmap(const Bitmap& bitmap, int16_t x, int16_t y, const Rect& rect, uint8_t alpha = 255, bool useOptimized = true);

    /**
     * @fn virtual void LCD1bpp::blitCopy(const uint16_t* sourceData, const Rect& source, const Rect& blitRect, uint8_t alpha, bool hasTransparentPixels);
     *
     * @brief Blits a 2D source-array to the frame buffer.
     *
     *        Blits a 2D source-array to the frame buffer unless alpha is zero.
     *
     * @param sourceData           The source-array pointer (points to the beginning of the
     *                             data).  The sourceData must be stored as 16-bits RGB565
     *                             values.
     * @param source               The location and dimension of the source.
     * @param blitRect             A rectangle describing what region is to be drawn.
     * @param alpha                The alpha value to use for blending (0 = invisible, otherwise
     *                             solid).
     * @param hasTransparentPixels If true, this data copy contains transparent pixels and
     *                             require hardware support for that to be enabled.
     */
    virtual void blitCopy(const uint16_t* sourceData, const Rect& source, const Rect& blitRect, uint8_t alpha, bool hasTransparentPixels);

    /**
     * @fn virtual void LCD1bpp::blitCopy(const uint8_t* sourceData, Bitmap::BitmapFormat sourceFormat, const Rect& source, const Rect& blitRect, uint8_t alpha, bool hasTransparentPixels);
     *
     * @brief Blits a 2D source-array to the framebuffer while converting the format.
     *
     *        Blits a 2D source-array to the framebuffer perfoming alpha-blending (and
     *        tranparency keying) as specified. Performs a software blend if HAL does not
     *        support BLIT_COPY_WITH_ALPHA and alpha != 255.
     *
     * @param sourceData           The source-array pointer (points to the beginning of the
     *                             data). The sourceData must be stored in a format suitable for
     *                             the selected display.
     * @param sourceFormat         The bitmap format used in the source data.
     * @param source               The location and dimension of the source.
     * @param blitRect             A rectangle describing what region is to be drawn.
     * @param alpha                The alpha value to use for blending (255 = solid, no blending)
     * @param hasTransparentPixels Ignored
     */
    virtual void blitCopy(const uint8_t* sourceData, Bitmap::BitmapFormat sourceFormat, const Rect& source, const Rect& blitRect, uint8_t alpha, bool hasTransparentPixels);

    /**
     * @fn virtual uint16_t* LCD1bpp::copyFrameBufferRegionToMemory(const Rect& region);
     *
     * @brief Copies a part of the frame buffer.
     *
     *        Copies a part of the frame buffer.
     *
     * @param region The part to copy.
     *
     * @return A pointer to the copy.
     */
    virtual uint16_t* copyFrameBufferRegionToMemory(const Rect& region);

    /**
     * @fn virtual void LCD1bpp::fillRect(const Rect& rect, colortype color, uint8_t alpha = 255);
     *
     * @brief Draws a filled rectangle in the specified color.
     *
     *        Draws a filled rectangle in the specified color.
     *
     * @param rect  The rectangle to draw in absolute coordinates.
     * @param color The rectangle color (values other than 0 or 1 are treated as being 1).
     * @param alpha The rectangle opacity (0 = invisible, otherwise solid). Default is 255
     *              (solid).
     */
    virtual void fillRect(const Rect& rect, colortype color, uint8_t alpha = 255);

    /**
     * @fn virtual uint8_t LCD1bpp::bitDepth() const
     *
     * @brief Number of bits per pixel used by the display.
     *
     *        Number of bits per pixel used by the display.
     *
     * @return 1.
     */
    virtual uint8_t bitDepth() const
    {
        return 1;
    }
protected:

    /**
     * @fn virtual void LCD1bpp::drawTextureMapScanLine(const DrawingSurface& dest, const Gradients& gradients, const Edge* leftEdge, const Edge* rightEdge, const TextureSurface& texture, const Rect& absoluteRect, const Rect& dirtyAreaAbsolute, RenderingVariant renderVariant, uint8_t alpha, uint16_t subDivisionSize)
     *
     * @brief Draw scan line. Not supported for 1bpp.
     *
     * @param dest              The description of where the texture is drawn - can be used to
     *                          issue a draw off screen.
     * @param gradients         The gradients using in interpolation across the scan line.
     * @param leftEdge          The left edge of the scan line.
     * @param rightEdge         The right edge of the scan line.
     * @param texture           The texture.
     * @param absoluteRect      The containing rectangle in absolute coordinates.
     * @param dirtyAreaAbsolute The dirty area in absolute coordinates.
     * @param renderVariant     The render variant - includes the algorithm and the pixel format.
     * @param alpha             The alpha.
     * @param subDivisionSize   The size of the subdivisions of the scan line. A value of 1 will
     *                          give a completely perspective correct texture mapped scan line. A
     *                          large value will give an affine texture mapped scan line.
     */
    virtual void drawTextureMapScanLine(const DrawingSurface& dest, const Gradients& gradients, const Edge* leftEdge, const Edge* rightEdge, const TextureSurface& texture, const Rect& absoluteRect, const Rect& dirtyAreaAbsolute, RenderingVariant renderVariant, uint8_t alpha, uint16_t subDivisionSize)
    {
        assert(0 && "Texture mapping not supported for 1bpp");
    }

    /**
     * @fn static int LCD1bpp::nextPixel(bool portrait, TextRotation rotation);
     *
     * @brief Find out how much to advance in the display buffer to get to the next pixel.
     *
     *        Find out how much to advance in the display buffer to get to the next pixel.
     *
     * @param portrait Is the display running in portrait mode?
     * @param rotation Rotation to perform.
     *
     * @return How much to advance to get to the next pixel.
     */
    static int nextPixel(bool portrait, TextRotation rotation);

    /**
     * @fn static int LCD1bpp::nextLine(bool portrait, TextRotation rotation);
     *
     * @brief Find out how much to advance in the display buffer to get to the next line.
     *
     *        Find out how much to advance in the display buffer to get to the next line.
     *
     * @param portrait Is the display running in portrait mode?
     * @param rotation Rotation to perform.
     *
     * @return How much to advance to get to the next line.
     */
    static int nextLine(bool portrait, TextRotation rotation);

    /**
     * @fn virtual void LCD1bpp::drawGlyph(uint16_t* wbuf, Rect widgetArea, int16_t x, int16_t y, uint16_t offsetX, uint16_t offsetY, const Rect& invalidatedArea, const GlyphNode* glyph, const uint8_t* glyphData, colortype color, uint8_t bitsPerPixel, uint8_t alpha, TextRotation rotation = TEXT_ROTATE_0);
     *
     * @brief Private version of draw-glyph with explicit destination buffer pointer argument.
     *
     *        Private version of draw-glyph with explicit destination buffer pointer argument.
     *        For all parameters (except the buffer pointer) see the public version of
     *        drawGlyph()
     *
     * @param [in] wbuf       The destination (frame) buffer to draw to.
     * @param widgetArea      The canvas to draw the glyph inside.
     * @param x               Horizontal offset to start drawing the glyph.
     * @param y               Vertical offset to start drawing the glyph.
     * @param offsetX         Horizontal offset in the glyph to start drawing from.
     * @param offsetY         Vertical offset in the glyph to start drawing from.
     * @param invalidatedArea The area to draw within.
     * @param glyph           Specifications of the glyph to draw.
     * @param glyphData       Data containing the actual glyph (dense format)
     * @param color           The color of the glyph.
     * @param bitsPerPixel    Bit depth of the glyph.
     * @param alpha           The transparency of the glyph.
     * @param rotation        Rotation to do before drawing the glyph.
     */
    virtual void drawGlyph(uint16_t* wbuf, Rect widgetArea, int16_t x, int16_t y, uint16_t offsetX, uint16_t offsetY, const Rect& invalidatedArea, const GlyphNode* glyph, const uint8_t* glyphData, colortype color, uint8_t bitsPerPixel, uint8_t alpha, TextRotation rotation = TEXT_ROTATE_0);

    /**
     * @fn static void LCD1bpp::fillMemory(void* RESTRICT dst, colortype color, uint16_t bytesToFill);
     *
     * @brief Fill memory efficiently.
     *
     *        Fill memory efficiently. Try to get 32bit aligned or 16bit aligned and then copy
     *        as quickly as possible.
     *
     * @param [out] dst   Pointer to memory to fill.
     * @param color       Color to write to memory, either 0 => 0x00000000 or 1 => 0xFFFFFFFF.
     * @param bytesToFill Number of bytes to fill.
     */
    static void fillMemory(void* RESTRICT dst, colortype color, uint16_t bytesToFill);

    /**
     * @fn virtual void LCD1bpp::blitCopyRLE(const uint16_t* _sourceData, const Rect& source, const Rect& blitRect, uint8_t alpha);
     *
     * @brief Blits a run-length encoded 2D source-array to the frame buffer.
     *
     *        Blits a run-length encoded2D source-array to the frame buffer unless alpha is
     *        zero.
     *
     * @param _sourceData The source-array pointer (points to the beginning of the data). Data
     *                    stored in RLE format, where each byte indicates number of pixels with
     *                    certain color, alternating between black and white. First byte
     *                    represents black.
     * @param source      The location and dimension of the source.
     * @param blitRect    A rectangle describing what region is to be drawn.
     * @param alpha       The alpha value to use for blending (0 = invisible, otherwise solid).
     */
    virtual void blitCopyRLE(const uint16_t* _sourceData, const Rect& source, const Rect& blitRect, uint8_t alpha);

private:
    class bwRLEdata
    {
    public:
        bwRLEdata(const uint8_t* src) : data(src), rleByte(0), firstHalfByte(true)
        {
            // Read two half-bytes ahead
            thisHalfByte = getNextHalfByte();
            nextHalfByte = getNextHalfByte();
        }
        uint32_t getNextLength()
        {
            uint32_t length = thisHalfByte; // Length is the next byte
            // update read ahead buffer
            thisHalfByte = nextHalfByte;
            nextHalfByte = getNextHalfByte();
            // If number after 'length' is 0
            while (thisHalfByte == 0)
            {
                length <<= 4; // Multiply length by 16 and
                length += nextHalfByte; // add the number after 0
                // We have used the next two half bytes, read two new ones
                thisHalfByte = getNextHalfByte();
                nextHalfByte = getNextHalfByte();
            }
            return length;
        }
        void skipNext(uint32_t skip, uint32_t& length, uint8_t& color)
        {
            while (skip > 0) // Are there more pixels to skip?
            {
                if (length > skip) // is the current length enough?
                {
                    length -= skip; // Reduce the length
                    skip = 0; // No more to skip
                }
                else
                {
                    skip -= length; // Skip the entire run
                    length = getNextLength(); // Read length of next run
                    color = ~color; // Update the color of next run
                }
            }
        }
    private:
        uint8_t getNextHalfByte()
        {
            if (firstHalfByte) // Start of new byte, read data from BW_RLE stream
            {
                rleByte = *data++;
            }
            uint8_t length = rleByte & 0xF; // Read lower half
            rleByte >>= 4; // Shift upper half down to make it ready
            firstHalfByte = !firstHalfByte; // Toggle 'start of byte'
            return length;
        }
        const uint8_t* data; // Pointer to compressed data (BW_RLE)
        uint8_t thisHalfByte; // The next half byte from the input
        uint8_t nextHalfByte; // The next half byte after 'thisHalfByte'
        uint8_t rleByte; // Byte read from compressed data
        bool firstHalfByte; // Are we about to process first half byte of rleByte?
    };
};

} // namespace touchgfx
#endif // LCD1BPP_HPP
