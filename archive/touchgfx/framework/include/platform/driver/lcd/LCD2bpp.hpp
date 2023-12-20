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
#ifndef LCD2BPP_HPP
#define LCD2BPP_HPP

#include <touchgfx/hal/Types.hpp>
#include <touchgfx/hal/HAL.hpp>
#include <touchgfx/lcd/LCD.hpp>
#include <touchgfx/Font.hpp>
#include <touchgfx/Bitmap.hpp>
#include <touchgfx/Unicode.hpp>
#include <touchgfx/TextProvider.hpp>
#include <touchgfx/TextureMapTypes.hpp>
#include <stdarg.h>

namespace touchgfx
{
#undef LCD
#define USE_LSB

/**
 * @class LCD2bpp LCD2bpp.hpp platform/driver/lcd/LCD2bpp.hpp
 *
 * @brief This class contains the various low-level drawing routines for drawing bitmaps.
 *
 *        This class contains the various low-level drawing routines for drawing bitmaps, texts
 *        and rectangles on 2 bits per pixel grayscale displays.
 *
 * @note All coordinates are expected to be in absolute coordinates!
 *
 * @see LCD
 */
class LCD2bpp : public LCD
{
public:

    virtual ~LCD2bpp() {}

    /**
     * @fn virtual void LCD2bpp::init();
     *
     * @brief Performs initialization.
     *
     *        Performs initialization.
     */
    virtual void init();

    /**
     * @fn virtual void LCD2bpp::drawPartialBitmap(const Bitmap& bitmap, int16_t x, int16_t y, const Rect& rect, uint8_t alpha = 255, bool useOptimized = true);
     *
     * @brief Draws a portion of a bitmap.
     *
     * @param bitmap       The bitmap to draw.
     * @param x            The absolute x coordinate to place pixel (0, 0) on the screen.
     * @param y            The absolute y coordinate to place pixel (0, 0) on the screen.
     * @param rect         A rectangle describing what region of the bitmap is to be drawn.
     * @param alpha        Optional alpha value. Default is 255 (solid).
     * @param useOptimized if false, do not attempt to substitute (parts of) this bitmap with
     *                     faster fillrects.
     */
    virtual void drawPartialBitmap(const Bitmap& bitmap, int16_t x, int16_t y, const Rect& rect, uint8_t alpha = 255, bool useOptimized = true);

    /**
     * @fn virtual void LCD2bpp::blitCopy(const uint16_t* sourceData, const Rect& source, const Rect& blitRect, uint8_t alpha, bool hasTransparentPixels);
     *
     * @brief Blits a 2D source-array to the framebuffer.
     *
     *        Blits a 2D source-array to the framebuffer perfoming alpha-blending (and
     *        tranparency keying) as specified Performs a software blend if HAL does not
     *        support BLIT_COPY_WITH_ALPHA and alpha != 255.
     *
     * @param sourceData           The source-array pointer (points to the beginning of the
     *                             data).  The sourceData must be stored as 16-bits RGB565
     *                             values.
     * @param source               The location and dimension of the source.
     * @param blitRect             A rectangle describing what region is to be drawn.
     * @param alpha                The alpha value to use for blending (255 = solid, no blending)
     * @param hasTransparentPixels If true, this data copy contains transparent pixels and
     *                             require hardware support for that to be enabled.
     */
    virtual void blitCopy(const uint16_t* sourceData, const Rect& source, const Rect& blitRect, uint8_t alpha, bool hasTransparentPixels);

    /**
     * @fn virtual void LCD2bpp::blitCopy(const uint8_t* sourceData, Bitmap::BitmapFormat sourceFormat, const Rect& source, const Rect& blitRect, uint8_t alpha, bool hasTransparentPixels);
     *
     * @brief Blits a 2D source-array to the framebuffer while converting the format.
     *
     *        Blits a 2D source-array to the framebuffer perfoming alpha-blending (and
     *        tranparency keying) as specified. Performs a software blend if HAL does not
     *        support BLIT_COPY_WITH_ALPHA and alpha != 255. LCD2 supports source data
     *        formats: RGB565 and ARGB8888.
     *
     * @param sourceData           The source-array pointer (points to the beginning of the
     *                             data). The sourceData must be stored in a format suitable for
     *                             the selected display.
     * @param sourceFormat         The bitmap format used in the source data.
     * @param source               The location and dimension of the source.
     * @param blitRect             A rectangle describing what region is to be drawn.
     * @param alpha                The alpha value to use for blending (255 = solid, no blending)
     * @param hasTransparentPixels If true, this data copy contains transparent pixels and
     *                             require hardware support for that to be enabled.
     */
    virtual void blitCopy(const uint8_t* sourceData, Bitmap::BitmapFormat sourceFormat, const Rect& source, const Rect& blitRect, uint8_t alpha, bool hasTransparentPixels);

    /**
     * @fn virtual uint16_t* LCD2bpp::copyFrameBufferRegionToMemory(const Rect& region);
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
     * @fn virtual void LCD2bpp::fillRect(const Rect& rect, colortype color, uint8_t alpha = 255);
     *
     * @brief Draws a filled rectangle in the specified color.
     *
     *        Draws a filled rectangle in the specified color.
     *
     * @param rect  The rectangle to draw in absolute coordinates.
     * @param color The rectangle color.
     * @param alpha The rectangle opacity (255=solid)
     */
    virtual void fillRect(const Rect& rect, colortype color, uint8_t alpha = 255);

    /**
     * @fn virtual uint8_t LCD2bpp::bitDepth() const
     *
     * @brief Number of bits per pixel used by the display.
     *
     *        Number of bits per pixel used by the display.
     *
     * @return The number of bits per pixel.
     */
    virtual uint8_t bitDepth() const
    {
        return 2;
    }

protected:
    static const uint16_t TRANSPARENT_COL = 0xABCD; ///< Transparency color. Deprecated, do not use.
    static const uint8_t alphaTable2bpp[64];        ///< The alpha lookup table to avoid arithmetics when alpha blending


    /**
     * @fn virtual void LCD2bpp::drawTextureMapScanLine(const DrawingSurface& dest, const Gradients& gradients, const Edge* leftEdge, const Edge* rightEdge, const TextureSurface& texture, const Rect& absoluteRect, const Rect& dirtyAreaAbsolute, RenderingVariant renderVariant, uint8_t alpha, uint16_t subDivisionSize);
     *
     * @brief Draw scan line. Draw one horizontal line of the texture map on screen. The scan line
     *        will be drawn using perspective correct texture mapping. The appearance of the
     *        line is determined by the left and right edge and the gradients structure. The
     *        edges contain the information about the x,y,z coordinates of the left and right
     *        side respectively and also information about the u,v coordinates of the texture
     *        map used. The gradients structure contains information about how to interpolate
     *        all the values across the scan line. The data drawn should be present in the
     *        texture argument.
     *
     *        The scan line will be drawn using the additional arguments. The scan line will be
     *        placed and clipped using the absolute and dirty rectangles The alpha will
     *        determine how the scan line should be alpha blended. The subDivisionSize will
     *        determine the size of the piecewise affine texture mapped lines.
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
    virtual void drawTextureMapScanLine(const DrawingSurface& dest, const Gradients& gradients, const Edge* leftEdge, const Edge* rightEdge, const TextureSurface& texture, const Rect& absoluteRect, const Rect& dirtyAreaAbsolute, RenderingVariant renderVariant, uint8_t alpha, uint16_t subDivisionSize);

    /**
     * @fn static int LCD2bpp::nextPixel(bool portrait, TextRotation rotation);
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
     * @fn static int LCD2bpp::nextLine(bool portrait, TextRotation rotation);
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
     * @fn virtual void LCD2bpp::drawGlyph(uint16_t* wbuf, Rect widgetArea, int16_t x, int16_t y, uint16_t offsetX, uint16_t offsetY, const Rect& invalidatedArea, const GlyphNode* glyph, const uint8_t* glyphData, colortype color, uint8_t bitsPerPixel, uint8_t alpha, TextRotation rotation = TEXT_ROTATE_0);
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
     * @fn static void LCD2bpp::blitCopyAlphaPerPixel(const uint16_t* sourceData16, const uint8_t* sourceAlphaData, const Rect& source, const Rect& blitRect, uint8_t alpha);
     *
     * @brief Blits a 2D source-array to the framebuffer.
     *
     *        Blits a 2D source-array to the framebuffer performing alpha-blending per pixel as
     *        specified Performs always a software blend.
     *
     * @param sourceData16    The source-array pointer (points to the beginning of the data). The
     *                        sourceData must be stored as 2bpp GREY2 values.
     * @param sourceAlphaData The alpha channel array pointer (points to the beginning of the data)
     * @param source          The location and dimension of the source.
     * @param blitRect        A rectangle describing what region is to be drawn.
     * @param alpha           The alpha value to use for blending applied to the whole image (255 =
     *                        solid, no blending)
     */
    static void blitCopyAlphaPerPixel(const uint16_t* sourceData16, const uint8_t* sourceAlphaData, const Rect& source, const Rect& blitRect, uint8_t alpha);
};

/**
 * @fn FORCE_INLINE_FUNCTION int LCD2shiftVal(int offset)
 *
 * @brief Shift value to get the right pixel in a byte.
 *
 * @param offset The offset.
 *
 * @return The shift value.
 */
FORCE_INLINE_FUNCTION int LCD2shiftVal(int offset)
{
#ifdef USE_LSB
    return (offset & 3) << 1;
#else
    return (3 - (offset & 3)) << 1;
#endif
}

/**
 * @fn FORCE_INLINE_FUNCTION uint8_t LCD2getPixel(const uint8_t* addr, int offset)
 *
 * @brief Get pixel from buffer/image.
 *
 * @param addr   The address.
 * @param offset The offset.
 *
 * @return The pixel value.
 */
FORCE_INLINE_FUNCTION uint8_t LCD2getPixel(const uint8_t* addr, int offset)
{
    return (addr[offset >> 2] >> LCD2shiftVal(offset)) & 3;
}

/**
 * @fn FORCE_INLINE_FUNCTION uint8_t LCD2getPixel(const uint16_t* addr, int offset)
 *
 * @brief Get pixel from buffer/image.
 *
 * @param addr   The address.
 * @param offset The offset.
 *
 * @return The pixel value.
 */
FORCE_INLINE_FUNCTION uint8_t LCD2getPixel(const uint16_t* addr, int offset)
{
    return LCD2getPixel(reinterpret_cast<const uint8_t*>(addr), offset);
}

/**
 * @fn FORCE_INLINE_FUNCTION void LCD2setPixel(uint8_t* addr, int offset, uint8_t value)
 *
 * @brief Set pixel in buffer.
 *
 * @param [in] addr The address.
 * @param offset    The offset.
 * @param value     The value.
 */
FORCE_INLINE_FUNCTION void LCD2setPixel(uint8_t* addr, int offset, uint8_t value)
{
    int shift = LCD2shiftVal(offset);
    addr[offset >> 2] = (addr[offset >> 2] & ~(3 << shift)) | ((value & 3) << shift);
}

/**
 * @fn FORCE_INLINE_FUNCTION void LCD2setPixel(uint16_t* addr, int offset, uint8_t value)
 *
 * @brief Set pixel in buffer.
 *
 * @param [in] addr The address.
 * @param offset    The offset.
 * @param value     The value.
 */
FORCE_INLINE_FUNCTION void LCD2setPixel(uint16_t* addr, int offset, uint8_t value)
{
    LCD2setPixel(reinterpret_cast<uint8_t*>(addr), offset, value);
}

} // namespace touchgfx
#endif // LCD2BPP_HPP
