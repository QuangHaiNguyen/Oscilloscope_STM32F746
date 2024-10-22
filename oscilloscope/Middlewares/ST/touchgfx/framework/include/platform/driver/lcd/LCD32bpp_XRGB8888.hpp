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

/**
 * @file platform/driver/lcd/LCD32bpp_XRGB8888.hpp
 *
 * Declares the touchgfx::LCD32bpp_XRGB8888 and touchgfx::LCD32DebugPrinter classes.
 */
#ifndef TOUCHGFX_LCD32BPP_XRGB8888_HPP
#define TOUCHGFX_LCD32BPP_XRGB8888_HPP

#include <touchgfx/Bitmap.hpp>
#include <touchgfx/Color.hpp>
#include <touchgfx/hal/HAL.hpp>
#include <touchgfx/hal/Types.hpp>
#include <touchgfx/lcd/LCD.hpp>
#include <touchgfx/lcd/LCD32DebugPrinter.hpp>

namespace touchgfx
{
/**
 * This class contains the various low-level drawing routines for drawing bitmaps, texts and
 * rectangles on 16 bits per pixel displays.
 *
 * @see LCD
 *
 * @note All coordinates are expected to be in absolute coordinates!
 */
class LCD32bpp_XRGB8888 : public LCD
{
public:
    LCD32bpp_XRGB8888();

    virtual void drawPartialBitmap(const Bitmap& bitmap, int16_t x, int16_t y, const Rect& rect, uint8_t alpha = 255, bool useOptimized = true);

    virtual void blitCopy(const uint16_t* sourceData, const Rect& source, const Rect& blitRect, uint8_t alpha, bool hasTransparentPixels);

    virtual void blitCopy(const uint8_t* sourceData, Bitmap::BitmapFormat sourceFormat, const Rect& source, const Rect& blitRect, uint8_t alpha, bool hasTransparentPixels);

    virtual uint16_t* copyFrameBufferRegionToMemory(const Rect& visRegion, const Rect& absRegion, const BitmapId bitmapId);

    virtual Rect copyFrameBufferRegionToMemory(const Rect& visRegion, const Rect& absRegion, uint8_t* dst, int16_t dstWidth, int16_t dstHeight);

    virtual void copyAreaFromTFTToClientBuffer(const Rect& region);

    virtual void fillRect(const Rect& rect, colortype color, uint8_t alpha = 255);

    virtual void fillBuffer(uint8_t* const destination, uint16_t pixelStride, const Rect& rect, const colortype color, const uint8_t alpha);

    virtual uint8_t bitDepth() const
    {
        return 32;
    }

    virtual Bitmap::BitmapFormat framebufferFormat() const
    {
        return Bitmap::ARGB8888;
    }

    virtual uint16_t framebufferStride() const
    {
        return getFramebufferStride();
    }

    virtual bool supportDynamicBitmapDrawing(const Bitmap::BitmapFormat /*format*/)
    {
        // DynamicBitmap drawing is not supported by LCD32XRGB
        return false;
    }

    /**
     * Framebuffer stride in bytes. The distance (in bytes) from the start of one
     * framebuffer row, to the next.
     *
     * @return The number of bytes in one framebuffer row.
     */
    FORCE_INLINE_FUNCTION static uint16_t getFramebufferStride()
    {
        assert(HAL::FRAME_BUFFER_WIDTH > 0 && "HAL has not been initialized yet");
        return HAL::FRAME_BUFFER_WIDTH * 4;
    }

    /**
     * Gets blue from color.
     *
     * @param  color The color.
     *
     * @return The blue from color.
     */
    FORCE_INLINE_FUNCTION static uint8_t getBlueFromColor(colortype color)
    {
        return color & 0xFF;
    }

    /**
     * Enables the texture mappers for all image formats. This allows drawing any image
     * using Bilinear Interpolation and Nearest Neighbor algorithms, but might use a lot of
     * memory for the drawing algorithms.
     */
    void enableTextureMapperAll();

    /**
     * Enables the texture mappers for L8_RGB565 image format. This allows drawing L8_RGB565
     * images using Bilinear Interpolation and Nearest Neighbor algorithms.
     *
     * @see enableTextureMapperL8_RGB565_BilinearInterpolation,
     *      enableTextureMapperL8_RGB565_NearestNeighbor
     */
    void enableTextureMapperL8_RGB565();

    /**
     * Enables the texture mappers for L8_RGB565 image format. This allows drawing L8_RGB565
     * images using Bilinear Interpolation algorithm.
     *
     * @see enableTextureMapperL8_RGB565, enableTextureMapperL8_RGB565_NearestNeighbor
     */
    void enableTextureMapperL8_RGB565_BilinearInterpolation();

    /**
     * Enables the texture mappers for L8_RGB565 image format. This allows drawing L8_RGB565
     * images using Nearest Neighbor algorithm.
     *
     * @see enableTextureMapperL8_RGB565, enableTextureMapperL8_RGB565_BilinearInterpolation
     */
    void enableTextureMapperL8_RGB565_NearestNeighbor();

    /**
     * Enables the texture mappers for L8_RGB888 image format. This allows drawing L8_RGB888
     * images using Bilinear Interpolation and Nearest Neighbor algorithms.
     *
     * @see enableTextureMapperL8_RGB888_BilinearInterpolation,
     *      enableTextureMapperL8_RGB888_NearestNeighbor
     */
    void enableTextureMapperL8_RGB888();

    /**
     * Enables the texture mappers for L8_RGB888 image format. This allows drawing L8_RGB888
     * images using Bilinear Interpolation algorithm.
     *
     * @see enableTextureMapperL8_RGB888, enableTextureMapperL8_RGB888_NearestNeighbor
     */
    void enableTextureMapperL8_RGB888_BilinearInterpolation();

    /**
     * Enables the texture mappers for L8_RGB888 image format. This allows drawing L8_RGB888
     * images using Nearest Neighbor algorithm.
     *
     * @see enableTextureMapperL8_RGB888, enableTextureMapperL8_RGB888_BilinearInterpolation
     */
    void enableTextureMapperL8_RGB888_NearestNeighbor();

    /**
     * Enables the texture mappers for L8_ARGB8888 image format. This allows drawing
     * L8_ARGB8888 images using Bilinear Interpolation and Nearest Neighbor algorithms.
     *
     * @see enableTextureMapperL8_ARGB8888_BilinearInterpolation,
     *      enableTextureMapperL8_ARGB8888_NearestNeighbor
     */
    void enableTextureMapperL8_ARGB8888();

    /**
     * Enables the texture mappers for L8_ARGB8888 image format. This allows drawing
     * L8_ARGB8888 images using Bilinear Interpolation algorithm.
     *
     * @see enableTextureMapperL8_ARGB8888, enableTextureMapperL8_ARGB8888_NearestNeighbor
     */
    void enableTextureMapperL8_ARGB8888_BilinearInterpolation();

    /**
     * Enables the texture mappers for L8_ARGB8888 image format. This allows drawing
     * L8_ARGB8888 images using Nearest Neighbor algorithm.
     *
     * @see enableTextureMapperL8_ARGB8888, enableTextureMapperL8_ARGB8888_BilinearInterpolation
     */
    void enableTextureMapperL8_ARGB8888_NearestNeighbor();

    /**
     * Enables the texture mappers for RGB565 image format. This allows drawing RGB565
     * images using Bilinear Interpolation and Nearest Neighbor algorithms.
     */
    void enableTextureMapperRGB565();

    /**
     * Enables the texture mappers for Opaque RGB565 image format. This allows drawing
     * RGB565 images using Bilinear Interpolation algorithm.
     *
     * @see enableTextureMapperRGB565, enableTextureMapperRGB565_NonOpaque_BilinearInterpolation
     */
    void enableTextureMapperRGB565_Opaque_BilinearInterpolation();

    /**
     * Enables the texture mappers for NonOpaque RGB565 image format. This allows drawing
     * RGB565 images using Bilinear Interpolation algorithm.
     *
     * @see enableTextureMapperRGB565, enableTextureMapperRGB565_Opaque_BilinearInterpolation
     */
    void enableTextureMapperRGB565_NonOpaque_BilinearInterpolation();

    /**
     * Enables the texture mappers for Opaque RGB565 image format. This allows drawing
     * RGB565 images using Nearest Neighbor algorithm.
     *
     * @see enableTextureMapperRGB565, enableTextureMapperRGB565_NonOpaque_NearestNeighbor
     */
    void enableTextureMapperRGB565_Opaque_NearestNeighbor();

    /**
     * Enables the texture mappers for NonOpaque RGB565 image format. This allows drawing
     * RGB565 images using Nearest Neighbor algorithm.
     *
     * @see enableTextureMapperRGB565, enableTextureMapperRGB565_Opaque_NearestNeighbor
     */
    void enableTextureMapperRGB565_NonOpaque_NearestNeighbor();

    /**
     * Enables the texture mappers for RGB888 image format. This allows drawing RGB888
     * images using Bilinear Interpolation and Nearest Neighbor algorithms.
     *
     * @see enableTextureMapperRGB888_BilinearInterpolation,
     *      enableTextureMapperRGB888_NearestNeighbor
     */
    void enableTextureMapperRGB888();

    /**
     * Enables the texture mappers for RGB888 image format. This allows drawing RGB888
     * images using Bilinear Interpolation algorithm.
     *
     * @see enableTextureMapperRGB888, enableTextureMapperRGB888_NearestNeighbor
     */
    void enableTextureMapperRGB888_BilinearInterpolation();

    /**
     * Enables the texture mappers for RGB888 image format. This allows drawing RGB888
     * images using Nearest Neighbor algorithm.
     *
     * @see enableTextureMapperRGB888, enableTextureMapperRGB888_BilinearInterpolation
     */
    void enableTextureMapperRGB888_NearestNeighbor();

    /**
     * Enables the texture mappers for ARGB8888 image format. This allows drawing ARGB8888
     * images using Bilinear Interpolation and Nearest Neighbor algorithms.
     *
     * @see enableTextureMapperARGB8888_BilinearInterpolation,
     *      enableTextureMapperARGB8888_NearestNeighbor
     */
    void enableTextureMapperARGB8888();

    /**
     * Enables the texture mappers for ARGB8888 image format. This allows drawing ARGB8888
     * images using Bilinear Interpolation algorithm.
     *
     * @see enableTextureMapperARGB8888, enableTextureMapperARGB8888_NearestNeighbor
     */
    void enableTextureMapperARGB8888_BilinearInterpolation();

    /**
     * Enables the texture mappers for ARGB8888 image format. This allows drawing ARGB8888
     * images using Nearest Neighbor algorithm.
     *
     * @see enableTextureMapperARGB8888, enableTextureMapperARGB8888_BilinearInterpolation
     */
    void enableTextureMapperARGB8888_NearestNeighbor();

    /**
     * Enables the texture mappers for A4 image format. This allows drawing A4 images using
     * Bilinear Interpolation and Nearest Neighbor algorithms.
     *
     * @see enableTextureMapperA4_BilinearInterpolation, enableTextureMapperA4_NearestNeighbor
     */
    void enableTextureMapperA4();

    /**
     * Enables the texture mappers for A4 image format. This allows drawing A4 images using
     * Bilinear Interpolation algorithm.
     *
     * @see enableTextureMapperA4, enableTextureMapperA4_NearestNeighbor
     */
    void enableTextureMapperA4_BilinearInterpolation();

    /**
     * Enables the texture mappers for A4 image format. This allows drawing A4 images using
     * Nearest Neighbor algorithm.
     *
     * @see enableTextureMapperA4, enableTextureMapperA4_BilinearInterpolation
     */
    void enableTextureMapperA4_NearestNeighbor();

    /**
     * Copies the 24bit color (blue, green, red) to the given 32bit destination, setting alpha to
     * zero.
     *
     * @param   rgb888  A pointer to the three bytes.
     *
     * @return  The 24bit RGB888 converted to a 32bit color value with zero alpha.
     */
    FORCE_INLINE_FUNCTION static uint32_t rgb888toXrgb8888(const uint8_t* const rgb888)
    {
        return (rgb888[2] << 16) | (rgb888[1] << 8) | rgb888[0];
    }

    /**
     * Alpha blend R,G,B with the 32bit color given in bgXRGB8888 using the provided fgAlpha (for R,
     * G,B) and bgAlpha (for bgXRGB8888). The resulting color is returned. address.
     *
     * @param   fgR                     The foreground blue.
     * @param   fgG                     The foreground green.
     * @param   fgB                     The foreground blue.
     * @param   bgXRGB8888      The background 32bit color.
     * @param   fgAlpha         The foreground alpha.
     * @param   bgAlpha         The background alpha.
     *
     * @return  The blended value.
     */
    FORCE_INLINE_FUNCTION static uint32_t blendRGBwithXrgb8888(const uint8_t fgR, const uint8_t fgG, const uint8_t fgB, const uint32_t bgXRGB8888, const uint8_t fgAlpha, const uint8_t bgAlpha)
    {
        const uint8_t b = div255(fgB * fgAlpha + (bgXRGB8888 & 0xFF) * bgAlpha);
        const uint8_t g = div255(fgG * fgAlpha + ((bgXRGB8888 >> 8) & 0xFF) * bgAlpha);
        const uint8_t r = div255(fgR * fgAlpha + ((bgXRGB8888 >> 16) & 0xFF) * bgAlpha);
        return (r << 16) | (g << 8) | b;
    }

    /**
     * Alpha blend two 32bit colors using the provided fgAlpha (for fgXRGB8888) and bgAlpha (for
     * bgXRGB8888). The resulting color is returned.
     *
     * @param   fgXRGB8888      The foreground 32bit color.
     * @param   bgXRGB8888      The background 32bit color.
     * @param   fgAlpha         The foreground alpha.
     * @param   bgAlpha         The background alpha.
     *
     * @return  The blended value.
     */
    FORCE_INLINE_FUNCTION static uint32_t blendXrgb888withXrgb8888(const uint32_t fgXRGB8888, const uint32_t bgXRGB8888, const uint8_t fgAlpha, const uint8_t bgAlpha)
    {
        return blendRgb888withXrgb8888(reinterpret_cast<const uint8_t*>(&fgXRGB8888), bgXRGB8888, fgAlpha, bgAlpha);
    }

    /**
     * Alpha blend a 24bit RGB888 with the 32bit color given in bgXRGB8888 using the provided
     * fgAlpha (for fgRGB888) and bgAlpha (for bgXRGB8888). The resulting color is returned. address.
     *
     * @param   fgRGB888        A pointer to the foreground 24bit RGB.
     * @param   bgXRGB8888      The background 32bit color.
     * @param   fgAlpha         The foreground alpha.
     * @param   bgAlpha         The background alpha.
     *
     * @return  The blended value.
     */
    FORCE_INLINE_FUNCTION static uint32_t blendRgb888withXrgb8888(const uint8_t* const fgRGB888, const uint32_t bgXRGB8888, const uint8_t fgAlpha, const uint8_t bgAlpha)
    {
        return blendRGBwithXrgb8888(fgRGB888[2], fgRGB888[1], fgRGB888[0], bgXRGB8888, fgAlpha, bgAlpha);
    }

    /**
     * Alpha blend a 16bit RGB565 color with the 32bit color given in bgXRGB8888 using the provided
     * fgAlpha (for fgRGB565) and bgAlpha (for bgXRGB8888). The resulting color is returned. address.
     *
     * @param   fgRGB565        A pointer to the foreground 24bit RGB.
     * @param   bgXRGB8888      The background 32bit color.
     * @param   fgAlpha         The foreground alpha.
     * @param   bgAlpha         The background alpha.
     *
     * @return  The blended value.
     */
    FORCE_INLINE_FUNCTION static uint32_t blendRgb565withXrgb8888(const uint16_t fgRGB565, const uint32_t bgXRGB8888, const uint8_t fgAlpha, const uint8_t bgAlpha)
    {
        const uint8_t r = (fgRGB565 & 0xF800) >> 8;
        const uint8_t g = (fgRGB565 & 0x07E0) >> 3;
        const uint8_t b = fgRGB565 << 3;
        // return blendRGBwithXrgb8888(r, g, b, bgXRGB8888, fgAlpha, bgAlpha);
        return blendRGBwithXrgb8888(r | (r >> 5), g | (g >> 6), b | (b >> 5), bgXRGB8888, fgAlpha, bgAlpha);
    }

    /**
     * Enables all the decompressors available for L8 images (L4, RLE and LZW9).
     * This allows drawing L4, RLE and LZW9 compressed L8 images.
     *
     * @see enableDecompressorL8_L4, enableDecompressorL8_RLE, enableDecompressorL8_LZW9
     */
    void enableDecompressorL8_All();

    /**
     * Enables the decompressor for L8 images compressed with the L4 algorithm.
     * This allows drawing L4 compressed L8 images.
     */
    void enableDecompressorL8_L4();

    /**
     * Enables the decompressor for L8 images compressed with the RLE algorithm.
     * This allows drawing RLE compressed L8 images.
     */
    void enableDecompressorL8_RLE();

    /**
     * Enables the decompressor for L8 images compressed with the LZW9 algorithm.
     * This allows drawing LZW9 compressed L8 images.
     */
    void enableDecompressorL8_LZW9();

protected:
    virtual DrawTextureMapScanLineBase* getTextureMapperDrawScanLine(const TextureSurface& texture, RenderingVariant renderVariant, uint8_t alpha);

    /**
     * Find out how much to advance in the display buffer to get to the next pixel.
     *
     * @param  rotatedDisplay Is the display running in portrait mode?
     * @param  textRotation   Rotation to perform.
     *
     * @return How much to advance to get to the next pixel.
     */
    static int nextPixel(bool rotatedDisplay, TextRotation textRotation);

    /**
     * Find out how much to advance in the display buffer to get to the next line.
     *
     * @param  rotatedDisplay Is the display running in portrait mode?
     * @param  textRotation   Rotation to perform.
     *
     * @return How much to advance to get to the next line.
     */
    static int nextLine(bool rotatedDisplay, TextRotation textRotation);

    virtual void drawGlyph(uint16_t* wbuf16, Rect widgetArea, int16_t x, int16_t y, uint16_t offsetX, uint16_t offsetY, const Rect& invalidatedArea, const GlyphNode* glyph, const uint8_t* glyphData, uint8_t byteAlignRow, colortype color, uint8_t bitsPerPixel, uint8_t alpha, TextRotation rotation);

    /**
     * Blits a 2D source-array to the framebuffer performing alpha-blending per pixel as
     * specified. If RGB888 is not supported by the DMA a software blend is performed.
     *
     * @param  sourceData16 The source-array pointer (points to the beginning of the data). The
     *                      sourceData must be stored as 24- bits RGB888 values.
     * @param  source       The location and dimensions of the source.
     * @param  blitRect     A rectangle describing what region is to be drawn.
     * @param  alpha        The alpha value to use for blending applied to the whole image (255 =
     *                      solid, no blending)
     */
    static void blitCopyRGB888(const uint16_t* sourceData16, const Rect& source, const Rect& blitRect, uint8_t alpha);

    /**
     * Blits a 2D source-array to the framebuffer performing alpha-blending per pixel as
     * specified. If! RGB565 is not supported by the DMA a software blend is performed.
     *
     * @param  sourceData16 The source-array pointer (points to the beginning of the data). The
     *                      sourceData must be stored as 16- bits RGB565 values.
     * @param  source       The location and dimensions of the source.
     * @param  blitRect     A rectangle describing what region is to be drawn.
     * @param  alpha        The alpha value to use for blending applied to the whole image (255 =
     *                      solid, no blending)
     */
    static void blitCopyRGB565(const uint16_t* sourceData16, const Rect& source, const Rect& blitRect, uint8_t alpha);

    /**
     * Blits a 2D indexed 8-bit source to the framebuffer performing alpha-blending per
     * pixel as specified if indexed format is not supported by the DMA a software blend is
     * performed.
     *
     * @param  sourceData The source-indexes pointer (points to the beginning of the data). The
     *                    sourceData must be stored as 8- bits indexes.
     * @param  clutData   The source-clut pointer (points to the beginning of the CLUT color
     *                    format and size data followed by colors entries.
     * @param  source     The location and dimensions of the source.
     * @param  blitRect   A rectangle describing what region is to be drawn.
     * @param  alpha      The alpha value to use for blending applied to the whole image (255 =
     *                    solid, no blending)
     */
    void blitCopyL8(const uint8_t* sourceData, const uint8_t* clutData, const Rect& source, const Rect& blitRect, uint8_t alpha);

    /**
     * Blits a 2D indexed 8-bit source to the framebuffer performing alpha-blending per
     * pixel as specified if L8_ARGB8888 is not supported by the DMA a software blend is
     * performed.
     *
     * @param  sourceData The source-indexes pointer (points to the beginning of the data). The
     *                    sourceData must be stored as 8- bits indexes.
     * @param  clutData   The source-clut pointer (points to the beginning of the CLUT color
     *                    format and size data followed by colors entries stored as 32-
     *                    bits (ARGB8888) format.
     * @param  source     The location and dimensions of the source.
     * @param  blitRect   A rectangle describing what region is to be drawn.
     * @param  alpha      The alpha value to use for blending applied to the whole image (255 =
     *                    solid, no blending)
     */
    static void blitCopyL8_ARGB8888(const uint8_t* sourceData, const uint8_t* clutData, const Rect& source, const Rect& blitRect, uint8_t alpha);

    /**
     * Blits a 2D indexed 8-bit source to the framebuffer performing alpha-blending per
     * pixel as specified if L8_RGB888 is not supported by the DMA a software blend is
     * performed.
     *
     * @param  sourceData The source-indexes pointer (points to the beginning of the data). The
     *                    sourceData must be stored as 8- bits indexes.
     * @param  clutData   The source-clut pointer (points to the beginning of the CLUT color
     *                    format and size data followed by colors entries stored as 32-
     *                    bits (RGB888) format.
     * @param  source     The location and dimensions of the source.
     * @param  blitRect   A rectangle describing what region is to be drawn.
     * @param  alpha      The alpha value to use for blending applied to the whole image (255 =
     *                    solid, no blending)
     */
    static void blitCopyL8_RGB888(const uint8_t* sourceData, const uint8_t* clutData, const Rect& source, const Rect& blitRect, uint8_t alpha);

    /**
     * Blits a 2D indexed 8-bit source to the framebuffer performing alpha-blending per
     * pixel as specified if L8_RGB565 is not supported by the DMA a software blend is
     * performed.
     *
     * @param  sourceData The source-indexes pointer (points to the beginning of the data). The
     *                    sourceData must be stored as 8- bits indexes.
     * @param  clutData   The source-clut pointer (points to the beginning of the CLUT color
     *                    format and size data followed by colors entries stored as 16-
     *                    bits (RGB565) format.
     * @param  source     The location and dimensions of the source.
     * @param  blitRect   A rectangle describing what region is to be drawn.
     * @param  alpha      The alpha value to use for blending applied to the whole image (255 =
     *                    solid, no blending)
     */
    static void blitCopyL8_RGB565(const uint8_t* sourceData, const uint8_t* clutData, const Rect& source, const Rect& blitRect, uint8_t alpha);

private:
    DrawTextureMapScanLineBase* textureMapper_L8_RGB565_Opaque_NearestNeighbor_GA;
    DrawTextureMapScanLineBase* textureMapper_L8_RGB565_Opaque_NearestNeighbor_NoGA;
    DrawTextureMapScanLineBase* textureMapper_L8_RGB565_Opaque_BilinearInterpolation_GA;
    DrawTextureMapScanLineBase* textureMapper_L8_RGB565_Opaque_BilinearInterpolation_NoGA;
    DrawTextureMapScanLineBase* textureMapper_L8_RGB888_Opaque_NearestNeighbor_GA;
    DrawTextureMapScanLineBase* textureMapper_L8_RGB888_Opaque_NearestNeighbor_NoGA;
    DrawTextureMapScanLineBase* textureMapper_L8_RGB888_Opaque_BilinearInterpolation_GA;
    DrawTextureMapScanLineBase* textureMapper_L8_RGB888_Opaque_BilinearInterpolation_NoGA;
    DrawTextureMapScanLineBase* textureMapper_L8_ARGB8888_NonOpaque_NearestNeighbor_GA;
    DrawTextureMapScanLineBase* textureMapper_L8_ARGB8888_NonOpaque_BilinearInterpolation_GA;
    DrawTextureMapScanLineBase* textureMapper_RGB565_NonOpaque_NearestNeighbor_GA;
    DrawTextureMapScanLineBase* textureMapper_RGB565_NonOpaque_NearestNeighbor_NoGA;
    DrawTextureMapScanLineBase* textureMapper_RGB565_Opaque_NearestNeighbor_GA;
    DrawTextureMapScanLineBase* textureMapper_RGB565_Opaque_NearestNeighbor_NoGA;
    DrawTextureMapScanLineBase* textureMapper_RGB565_NonOpaque_BilinearInterpolation_GA;
    DrawTextureMapScanLineBase* textureMapper_RGB565_NonOpaque_BilinearInterpolation_NoGA;
    DrawTextureMapScanLineBase* textureMapper_RGB565_Opaque_BilinearInterpolation_GA;
    DrawTextureMapScanLineBase* textureMapper_RGB565_Opaque_BilinearInterpolation_NoGA;
    DrawTextureMapScanLineBase* textureMapper_RGB888_Opaque_NearestNeighbor_GA;
    DrawTextureMapScanLineBase* textureMapper_RGB888_Opaque_NearestNeighbor_NoGA;
    DrawTextureMapScanLineBase* textureMapper_RGB888_Opaque_BilinearInterpolation_GA;
    DrawTextureMapScanLineBase* textureMapper_RGB888_Opaque_BilinearInterpolation_NoGA;
    DrawTextureMapScanLineBase* textureMapper_ARGB8888_NonOpaque_NearestNeighbor_GA;
    DrawTextureMapScanLineBase* textureMapper_ARGB8888_NonOpaque_BilinearInterpolation_GA;
    DrawTextureMapScanLineBase* textureMapper_A4_NearestNeighbor_GA;
    DrawTextureMapScanLineBase* textureMapper_A4_NearestNeighbor_NoGA;
    DrawTextureMapScanLineBase* textureMapper_A4_BilinearInterpolation_GA;
    DrawTextureMapScanLineBase* textureMapper_A4_BilinearInterpolation_NoGA;

    FORCE_INLINE_FUNCTION static uint32_t expandRgb565(uint16_t c)
    {
        return ((c & 0x07E0) << 16) | (c & ~0x07E0);
    }

    FORCE_INLINE_FUNCTION static uint16_t compactRgb565(uint32_t c)
    {
        return ((c >> 16) & 0x07E0) | (c & ~0x07E0);
    }

    FORCE_INLINE_FUNCTION static uint16_t bilinearInterpolate565(uint16_t c00, uint16_t c10, uint16_t c01, uint16_t c11, uint8_t x, uint8_t y)
    {
        assert(x < 16 && y < 16);
        uint32_t a00 = expandRgb565(c00);
        uint32_t a10 = expandRgb565(c10);
        uint32_t a01 = expandRgb565(c01);
        uint32_t a11 = expandRgb565(c11);

        uint8_t xy = (x * y) >> 3;
        return compactRgb565((a00 * (32 - 2 * y - 2 * x + xy) + a10 * (2 * x - xy) + a01 * (2 * y - xy) + a11 * xy) >> 5);
    }

    FORCE_INLINE_FUNCTION static uint16_t bilinearInterpolate565(uint16_t c00, uint16_t c10, uint8_t x)
    {
        assert(x < 16);
        uint32_t a00 = expandRgb565(c00);
        uint32_t a10 = expandRgb565(c10);

        return compactRgb565((a00 * (32 - 2 * x) + a10 * (2 * x)) >> 5);
    }

    FORCE_INLINE_FUNCTION static uint8_t bilinearInterpolate8(uint8_t c00, uint8_t c10, uint8_t x)
    {
        assert(x < 16);
        uint16_t xy10 = 16 * x;
        uint16_t xy00 = 256 - xy10;

        return (c00 * xy00 + c10 * xy10) >> 8;
    }

    FORCE_INLINE_FUNCTION static uint8_t bilinearInterpolate8(uint8_t c00, uint8_t c10, uint8_t c01, uint8_t c11, uint8_t x, uint8_t y)
    {
        assert(x < 16 && y < 16);
        uint16_t xy11 = x * y;
        uint16_t xy10 = 16 * x - xy11;
        uint16_t xy01 = 16 * y - xy11;
        uint16_t xy00 = 256 - (xy11 + xy10 + xy01);

        return (c00 * xy00 + c10 * xy10 + c01 * xy01 + c11 * xy11) >> 8;
    }

    FORCE_INLINE_FUNCTION static uint32_t bilinearInterpolate888(uint32_t c00, uint32_t c10, uint8_t x)
    {
        assert(x < 16);
        uint16_t xy10 = 16 * x;
        uint16_t xy00 = 256 - xy10;

        return ((((c00 & 0xFF00FF) * xy00 + (c10 & 0xFF00FF) * xy10) >> 8) & 0xFF00FF) |
               ((((c00 & 0x00FF00) * xy00 + (c10 & 0x00FF00) * xy10) >> 8) & 0x00FF00);
    }

    FORCE_INLINE_FUNCTION static uint32_t bilinearInterpolate888(uint32_t c00, uint32_t c10, uint32_t c01, uint32_t c11, uint8_t x, uint8_t y)
    {
        assert(x < 16 && y < 16);
        uint16_t xy11 = x * y;
        uint16_t xy10 = 16 * x - xy11;
        uint16_t xy01 = 16 * y - xy11;
        uint16_t xy00 = 256 - (xy11 + xy10 + xy01);

        return ((((c00 & 0xFF00FF) * xy00 + (c10 & 0xFF00FF) * xy10 + (c01 & 0xFF00FF) * xy01 + (c11 & 0xFF00FF) * xy11) >> 8) & 0xFF00FF) |
               ((((c00 & 0x00FF00) * xy00 + (c10 & 0x00FF00) * xy10 + (c01 & 0x00FF00) * xy01 + (c11 & 0x00FF00) * xy11) >> 8) & 0x00FF00);
    }

    FORCE_INLINE_FUNCTION static uint32_t div255_888(uint32_t val, uint8_t factor)
    {
        return div255rb((val & 0xFF00FF) * factor) | div255g((val & 0x00FF00) * factor);
    }

    FORCE_INLINE_FUNCTION static uint32_t div255_888_FFcheck(uint32_t val, uint8_t factor)
    {
        return factor < 0xFF ? div255_888(val, factor) : val;
    }

    class DrawTextureMapScanLineBase32 : public DrawTextureMapScanLineBase
    {
    protected:
        FORCE_INLINE_FUNCTION bool overrunCheckNearestNeighbor(uint32_t*& destBits, int& pixelsToDraw, fixed16_16& U, fixed16_16& V, fixed16_16 deltaU, fixed16_16 deltaV, const int16_t maxWidth, const int16_t maxHeight) const;
        FORCE_INLINE_FUNCTION bool overrunCheckBilinearInterpolation(uint32_t*& destBits, int& pixelsToDraw, fixed16_16& U, fixed16_16& V, fixed16_16 deltaU, fixed16_16 deltaV, const int16_t maxWidth, const int16_t maxHeight) const;
    };

    class TextureMapper_L8_RGB565_Opaque_NearestNeighbor_GA : public DrawTextureMapScanLineBase32
    {
    public:
        virtual void drawTextureMapScanLineSubdivisions(int subdivisions, const int widthModLength, int pixelsToDraw, const int affineLength, float oneOverZRight, float UOverZRight, float VOverZRight, fixed16_16 U, fixed16_16 V, fixed16_16 deltaU, fixed16_16 deltaV, float ULeft, float VLeft, float URight, float VRight, float ZRight, const DrawingSurface& dest, const int destX, const int destY, const TextureSurface& texture, uint8_t alpha, const float dOneOverZdXAff, const float dUOverZdXAff, const float dVOverZdXAff);

    private:
        FORCE_INLINE_FUNCTION void writePixel(uint32_t* const destBits, const uint8_t* const textureBits8, const uint16_t* const palette16, const int16_t bitmapWidth, const int UInt, const int VInt, const uint8_t alpha) const;
    };

    class TextureMapper_L8_RGB565_Opaque_NearestNeighbor_NoGA : public DrawTextureMapScanLineBase32
    {
    public:
        virtual void drawTextureMapScanLineSubdivisions(int subdivisions, const int widthModLength, int pixelsToDraw, const int affineLength, float oneOverZRight, float UOverZRight, float VOverZRight, fixed16_16 U, fixed16_16 V, fixed16_16 deltaU, fixed16_16 deltaV, float ULeft, float VLeft, float URight, float VRight, float ZRight, const DrawingSurface& dest, const int destX, const int destY, const TextureSurface& texture, uint8_t alpha, const float dOneOverZdXAff, const float dUOverZdXAff, const float dVOverZdXAff);

    private:
        FORCE_INLINE_FUNCTION void writePixel(uint32_t* const destBits, const uint8_t* const textureBits8, const uint16_t* const palette16, const int16_t bitmapWidth, const int UInt, const int VInt) const;
    };

    class TextureMapper_L8_RGB565_Opaque_BilinearInterpolation_GA : public DrawTextureMapScanLineBase32
    {
    public:
        virtual void drawTextureMapScanLineSubdivisions(int subdivisions, const int widthModLength, int pixelsToDraw, const int affineLength, float oneOverZRight, float UOverZRight, float VOverZRight, fixed16_16 U, fixed16_16 V, fixed16_16 deltaU, fixed16_16 deltaV, float ULeft, float VLeft, float URight, float VRight, float ZRight, const DrawingSurface& dest, const int destX, const int destY, const TextureSurface& texture, uint8_t alpha, const float dOneOverZdXAff, const float dUOverZdXAff, const float dVOverZdXAff);

    private:
        FORCE_INLINE_FUNCTION void writePixel(uint32_t* const destBits, const uint8_t* const textureBits8, const uint16_t* const palette16, const int16_t bitmapWidth, const int UInt, const int VInt, const uint8_t UFrac, const uint8_t VFrac, const uint8_t alpha) const;
        void writePixelOnEdge(uint32_t* const destBits, const uint8_t* const textureBits8, const uint16_t* const palette16, const int16_t bitmapWidth, const int16_t bitmapHeight, const int UInt, const int VInt, const uint8_t UFrac, const uint8_t VFrac, const uint8_t alpha) const;
    };

    class TextureMapper_L8_RGB565_Opaque_BilinearInterpolation_NoGA : public DrawTextureMapScanLineBase32
    {
    public:
        virtual void drawTextureMapScanLineSubdivisions(int subdivisions, const int widthModLength, int pixelsToDraw, const int affineLength, float oneOverZRight, float UOverZRight, float VOverZRight, fixed16_16 U, fixed16_16 V, fixed16_16 deltaU, fixed16_16 deltaV, float ULeft, float VLeft, float URight, float VRight, float ZRight, const DrawingSurface& dest, const int destX, const int destY, const TextureSurface& texture, uint8_t alpha, const float dOneOverZdXAff, const float dUOverZdXAff, const float dVOverZdXAff);

    private:
        FORCE_INLINE_FUNCTION void writePixel(uint32_t* const destBits, const uint8_t* const textureBits8, const uint16_t* const palette16, const int16_t bitmapWidth, const int UInt, const int VInt, const uint8_t UFrac, const uint8_t VFrac) const;
        void writePixelOnEdge(uint32_t* const destBits, const uint8_t* const textureBits8, const uint16_t* const palette16, const int16_t bitmapWidth, const int16_t bitmapHeight, const int UInt, const int VInt, const uint8_t UFrac, const uint8_t VFrac) const;
    };

    class TextureMapper_L8_RGB888_Opaque_NearestNeighbor_GA : public DrawTextureMapScanLineBase32
    {
    public:
        virtual void drawTextureMapScanLineSubdivisions(int subdivisions, const int widthModLength, int pixelsToDraw, const int affineLength, float oneOverZRight, float UOverZRight, float VOverZRight, fixed16_16 U, fixed16_16 V, fixed16_16 deltaU, fixed16_16 deltaV, float ULeft, float VLeft, float URight, float VRight, float ZRight, const DrawingSurface& dest, const int destX, const int destY, const TextureSurface& texture, uint8_t alpha, const float dOneOverZdXAff, const float dUOverZdXAff, const float dVOverZdXAff);

    private:
        FORCE_INLINE_FUNCTION void writePixel(uint32_t* const destBits, const uint8_t* const textureBits8, const uint8_t* const palette8, const int16_t bitmapWidth, const int UInt, const int VInt, const uint8_t alpha) const;
    };

    class TextureMapper_L8_RGB888_Opaque_NearestNeighbor_NoGA : public DrawTextureMapScanLineBase32
    {
    public:
        virtual void drawTextureMapScanLineSubdivisions(int subdivisions, const int widthModLength, int pixelsToDraw, const int affineLength, float oneOverZRight, float UOverZRight, float VOverZRight, fixed16_16 U, fixed16_16 V, fixed16_16 deltaU, fixed16_16 deltaV, float ULeft, float VLeft, float URight, float VRight, float ZRight, const DrawingSurface& dest, const int destX, const int destY, const TextureSurface& texture, uint8_t alpha, const float dOneOverZdXAff, const float dUOverZdXAff, const float dVOverZdXAff);

    private:
        FORCE_INLINE_FUNCTION void writePixel(uint32_t* const destBits, const uint8_t* const textureBits8, const uint8_t* const palette8, const int16_t bitmapWidth, const int UInt, const int VInt) const;
    };

    class TextureMapper_L8_RGB888_Opaque_BilinearInterpolation_GA : public DrawTextureMapScanLineBase32
    {
    public:
        virtual void drawTextureMapScanLineSubdivisions(int subdivisions, const int widthModLength, int pixelsToDraw, const int affineLength, float oneOverZRight, float UOverZRight, float VOverZRight, fixed16_16 U, fixed16_16 V, fixed16_16 deltaU, fixed16_16 deltaV, float ULeft, float VLeft, float URight, float VRight, float ZRight, const DrawingSurface& dest, const int destX, const int destY, const TextureSurface& texture, uint8_t alpha, const float dOneOverZdXAff, const float dUOverZdXAff, const float dVOverZdXAff);

    private:
        FORCE_INLINE_FUNCTION void writePixel(uint32_t* const destBits, const uint8_t* const textureBits8, const uint8_t* const palette8, const int16_t bitmapWidth, const int UInt, const int VInt, const uint8_t UFrac, const uint8_t VFrac, const uint8_t alpha) const;
        void writePixelOnEdge(uint32_t* const destBits, const uint8_t* const textureBits8, const uint8_t* const palette8, const int16_t bitmapWidth, const int16_t bitmapHeight, const int UInt, const int VInt, const uint8_t UFrac, const uint8_t VFrac, const uint8_t alpha) const;
    };

    class TextureMapper_L8_RGB888_Opaque_BilinearInterpolation_NoGA : public DrawTextureMapScanLineBase32
    {
    public:
        virtual void drawTextureMapScanLineSubdivisions(int subdivisions, const int widthModLength, int pixelsToDraw, const int affineLength, float oneOverZRight, float UOverZRight, float VOverZRight, fixed16_16 U, fixed16_16 V, fixed16_16 deltaU, fixed16_16 deltaV, float ULeft, float VLeft, float URight, float VRight, float ZRight, const DrawingSurface& dest, const int destX, const int destY, const TextureSurface& texture, uint8_t alpha, const float dOneOverZdXAff, const float dUOverZdXAff, const float dVOverZdXAff);

    private:
        FORCE_INLINE_FUNCTION void writePixel(uint32_t* const destBits, const uint8_t* const textureBits8, const uint8_t* const palette8, const int16_t bitmapWidth, const int UInt, const int VInt, const uint8_t UFrac, const uint8_t VFrac) const;
        void writePixelOnEdge(uint32_t* const destBits, const uint8_t* const textureBits8, const uint8_t* const palette8, const int16_t bitmapWidth, const int16_t bitmapHeight, const int UInt, const int VInt, const uint8_t UFrac, const uint8_t VFrac) const;
    };

    class TextureMapper_L8_ARGB8888_NonOpaque_NearestNeighbor_GA : public DrawTextureMapScanLineBase32
    {
    public:
        virtual void drawTextureMapScanLineSubdivisions(int subdivisions, const int widthModLength, int pixelsToDraw, const int affineLength, float oneOverZRight, float UOverZRight, float VOverZRight, fixed16_16 U, fixed16_16 V, fixed16_16 deltaU, fixed16_16 deltaV, float ULeft, float VLeft, float URight, float VRight, float ZRight, const DrawingSurface& dest, const int destX, const int destY, const TextureSurface& texture, uint8_t alpha, const float dOneOverZdXAff, const float dUOverZdXAff, const float dVOverZdXAff);

    private:
        FORCE_INLINE_FUNCTION void writePixel(uint32_t* const destBits, const uint8_t* const textureBits8, const uint32_t* const palette32, const int16_t bitmapWidth, const int UInt, const int VInt, const uint8_t alpha) const;
    };

    class TextureMapper_L8_ARGB8888_NonOpaque_BilinearInterpolation_GA : public DrawTextureMapScanLineBase32
    {
    public:
        virtual void drawTextureMapScanLineSubdivisions(int subdivisions, const int widthModLength, int pixelsToDraw, const int affineLength, float oneOverZRight, float UOverZRight, float VOverZRight, fixed16_16 U, fixed16_16 V, fixed16_16 deltaU, fixed16_16 deltaV, float ULeft, float VLeft, float URight, float VRight, float ZRight, const DrawingSurface& dest, const int destX, const int destY, const TextureSurface& texture, uint8_t alpha, const float dOneOverZdXAff, const float dUOverZdXAff, const float dVOverZdXAff);

    private:
        FORCE_INLINE_FUNCTION void writePixel(uint32_t* const destBits, const uint8_t* const textureBits8, const uint32_t* const palette32, const int16_t bitmapWidth, const int UInt, const int VInt, const uint8_t UFrac, const uint8_t VFrac, const uint8_t alpha) const;
        void writePixelOnEdge(uint32_t* const destBits, const uint8_t* const textureBits8, const uint32_t* const palette32, const int16_t bitmapWidth, const int16_t bitmapHeight, const int UInt, const int VInt, const uint8_t UFrac, const uint8_t VFrac, const uint8_t alpha) const;
    };

    class TextureMapper_RGB565_NonOpaque_NearestNeighbor_GA : public DrawTextureMapScanLineBase32
    {
    public:
        virtual void drawTextureMapScanLineSubdivisions(int subdivisions, const int widthModLength, int pixelsToDraw, const int affineLength, float oneOverZRight, float UOverZRight, float VOverZRight, fixed16_16 U, fixed16_16 V, fixed16_16 deltaU, fixed16_16 deltaV, float ULeft, float VLeft, float URight, float VRight, float ZRight, const DrawingSurface& dest, const int destX, const int destY, const TextureSurface& texture, uint8_t alpha, const float dOneOverZdXAff, const float dUOverZdXAff, const float dVOverZdXAff);

    private:
        FORCE_INLINE_FUNCTION void writePixel(uint32_t* const destBits, const uint16_t* const textureBits, const uint8_t* alphaBits, const int16_t bitmapWidth, const int UInt, const int VInt, const uint8_t alpha) const;
    };

    class TextureMapper_RGB565_NonOpaque_NearestNeighbor_NoGA : public DrawTextureMapScanLineBase32
    {
    public:
        virtual void drawTextureMapScanLineSubdivisions(int subdivisions, const int widthModLength, int pixelsToDraw, const int affineLength, float oneOverZRight, float UOverZRight, float VOverZRight, fixed16_16 U, fixed16_16 V, fixed16_16 deltaU, fixed16_16 deltaV, float ULeft, float VLeft, float URight, float VRight, float ZRight, const DrawingSurface& dest, const int destX, const int destY, const TextureSurface& texture, uint8_t alpha, const float dOneOverZdXAff, const float dUOverZdXAff, const float dVOverZdXAff);

    private:
        FORCE_INLINE_FUNCTION void writePixel(uint32_t* const destBits, const uint16_t* const textureBits, const uint8_t* alphaBits, const int16_t bitmapWidth, const int UInt, const int VInt) const;
    };

    class TextureMapper_RGB565_NonOpaque_BilinearInterpolation_GA : public DrawTextureMapScanLineBase32
    {
    public:
        virtual void drawTextureMapScanLineSubdivisions(int subdivisions, const int widthModLength, int pixelsToDraw, const int affineLength, float oneOverZRight, float UOverZRight, float VOverZRight, fixed16_16 U, fixed16_16 V, fixed16_16 deltaU, fixed16_16 deltaV, float ULeft, float VLeft, float URight, float VRight, float ZRight, const DrawingSurface& dest, const int destX, const int destY, const TextureSurface& texture, uint8_t alpha, const float dOneOverZdXAff, const float dUOverZdXAff, const float dVOverZdXAff);

    private:
        FORCE_INLINE_FUNCTION void writePixel(uint32_t* const destBits, const uint16_t* const textureBits, const uint8_t* alphaBits, const int16_t bitmapWidth, const int UInt, const int VInt, const uint8_t UFrac, const uint8_t VFrac, const uint8_t alpha) const;
        void writePixelOnEdge(uint32_t* const destBits, const uint16_t* const textureBits, const uint8_t* alphaBits, const int16_t bitmapWidth, const int16_t bitmapHeight, const int UInt, const int VInt, const uint8_t UFrac, const uint8_t VFrac, const uint8_t alpha) const;
    };

    class TextureMapper_RGB565_NonOpaque_BilinearInterpolation_NoGA : public DrawTextureMapScanLineBase32
    {
    public:
        virtual void drawTextureMapScanLineSubdivisions(int subdivisions, const int widthModLength, int pixelsToDraw, const int affineLength, float oneOverZRight, float UOverZRight, float VOverZRight, fixed16_16 U, fixed16_16 V, fixed16_16 deltaU, fixed16_16 deltaV, float ULeft, float VLeft, float URight, float VRight, float ZRight, const DrawingSurface& dest, const int destX, const int destY, const TextureSurface& texture, uint8_t alpha, const float dOneOverZdXAff, const float dUOverZdXAff, const float dVOverZdXAff);

    private:
        FORCE_INLINE_FUNCTION void writePixel(uint32_t* const destBits, const uint16_t* const textureBits, const uint8_t* alphaBits, const int16_t bitmapWidth, const int UInt, const int VInt, const uint8_t UFrac, const uint8_t VFrac) const;
        void writePixelOnEdge(uint32_t* const destBits, const uint16_t* const textureBits, const uint8_t* alphaBits, const int16_t bitmapWidth, const int16_t bitmapHeight, const int UInt, const int VInt, const uint8_t UFrac, const uint8_t VFrac) const;
    };

    class TextureMapper_RGB565_Opaque_NearestNeighbor_GA : public DrawTextureMapScanLineBase32
    {
    public:
        virtual void drawTextureMapScanLineSubdivisions(int subdivisions, const int widthModLength, int pixelsToDraw, const int affineLength, float oneOverZRight, float UOverZRight, float VOverZRight, fixed16_16 U, fixed16_16 V, fixed16_16 deltaU, fixed16_16 deltaV, float ULeft, float VLeft, float URight, float VRight, float ZRight, const DrawingSurface& dest, const int destX, const int destY, const TextureSurface& texture, uint8_t alpha, const float dOneOverZdXAff, const float dUOverZdXAff, const float dVOverZdXAff);

    private:
        FORCE_INLINE_FUNCTION void writePixel(uint32_t* const destBits, const uint16_t* const textureBits, const int16_t bitmapWidth, const int UInt, const int VInt, const uint8_t alpha) const;
    };

    class TextureMapper_RGB565_Opaque_NearestNeighbor_NoGA : public DrawTextureMapScanLineBase32
    {
    public:
        virtual void drawTextureMapScanLineSubdivisions(int subdivisions, const int widthModLength, int pixelsToDraw, const int affineLength, float oneOverZRight, float UOverZRight, float VOverZRight, fixed16_16 U, fixed16_16 V, fixed16_16 deltaU, fixed16_16 deltaV, float ULeft, float VLeft, float URight, float VRight, float ZRight, const DrawingSurface& dest, const int destX, const int destY, const TextureSurface& texture, uint8_t alpha, const float dOneOverZdXAff, const float dUOverZdXAff, const float dVOverZdXAff);

    private:
        FORCE_INLINE_FUNCTION void writePixel(uint32_t* const destBits, const uint16_t* const textureBits, const int16_t bitmapWidth, const int UInt, const int VInt) const;
    };

    class TextureMapper_RGB565_Opaque_BilinearInterpolation_GA : public DrawTextureMapScanLineBase32
    {
    public:
        virtual void drawTextureMapScanLineSubdivisions(int subdivisions, const int widthModLength, int pixelsToDraw, const int affineLength, float oneOverZRight, float UOverZRight, float VOverZRight, fixed16_16 U, fixed16_16 V, fixed16_16 deltaU, fixed16_16 deltaV, float ULeft, float VLeft, float URight, float VRight, float ZRight, const DrawingSurface& dest, const int destX, const int destY, const TextureSurface& texture, uint8_t alpha, const float dOneOverZdXAff, const float dUOverZdXAff, const float dVOverZdXAff);

    private:
        FORCE_INLINE_FUNCTION void writePixel(uint32_t* const destBits, const uint16_t* const textureBits, const int16_t bitmapWidth, const int UInt, const int VInt, const uint8_t UFrac, const uint8_t VFrac, const uint8_t alpha) const;
        void writePixelOnEdge(uint32_t* const destBits, const uint16_t* const textureBits, const int16_t bitmapWidth, const int16_t bitmapHeight, const int UInt, const int VInt, const uint8_t UFrac, const uint8_t VFrac, const uint8_t alpha) const;
    };

    class TextureMapper_RGB565_Opaque_BilinearInterpolation_NoGA : public DrawTextureMapScanLineBase32
    {
    public:
        virtual void drawTextureMapScanLineSubdivisions(int subdivisions, const int widthModLength, int pixelsToDraw, const int affineLength, float oneOverZRight, float UOverZRight, float VOverZRight, fixed16_16 U, fixed16_16 V, fixed16_16 deltaU, fixed16_16 deltaV, float ULeft, float VLeft, float URight, float VRight, float ZRight, const DrawingSurface& dest, const int destX, const int destY, const TextureSurface& texture, uint8_t alpha, const float dOneOverZdXAff, const float dUOverZdXAff, const float dVOverZdXAff);

    private:
        FORCE_INLINE_FUNCTION void writePixel(uint32_t* const destBits, const uint16_t* const textureBits, const int16_t bitmapWidth, const int UInt, const int VInt, const uint8_t UFrac, const uint8_t VFrac) const;
        void writePixelOnEdge(uint32_t* const destBits, const uint16_t* const textureBits, const int16_t bitmapWidth, const int16_t bitmapHeight, const int UInt, const int VInt, const uint8_t UFrac, const uint8_t VFrac) const;
    };

    class TextureMapper_RGB888_Opaque_NearestNeighbor_GA : public DrawTextureMapScanLineBase32
    {
    public:
        virtual void drawTextureMapScanLineSubdivisions(int subdivisions, const int widthModLength, int pixelsToDraw, const int affineLength, float oneOverZRight, float UOverZRight, float VOverZRight, fixed16_16 U, fixed16_16 V, fixed16_16 deltaU, fixed16_16 deltaV, float ULeft, float VLeft, float URight, float VRight, float ZRight, const DrawingSurface& dest, const int destX, const int destY, const TextureSurface& texture, uint8_t alpha, const float dOneOverZdXAff, const float dUOverZdXAff, const float dVOverZdXAff);

    private:
        FORCE_INLINE_FUNCTION void writePixel(uint32_t* const destBits, const uint8_t* const textureBits8, const int16_t bitmapWidth, const int UInt, const int VInt, const uint8_t alpha) const;
    };

    class TextureMapper_RGB888_Opaque_NearestNeighbor_NoGA : public DrawTextureMapScanLineBase32
    {
    public:
        virtual void drawTextureMapScanLineSubdivisions(int subdivisions, const int widthModLength, int pixelsToDraw, const int affineLength, float oneOverZRight, float UOverZRight, float VOverZRight, fixed16_16 U, fixed16_16 V, fixed16_16 deltaU, fixed16_16 deltaV, float ULeft, float VLeft, float URight, float VRight, float ZRight, const DrawingSurface& dest, const int destX, const int destY, const TextureSurface& texture, uint8_t alpha, const float dOneOverZdXAff, const float dUOverZdXAff, const float dVOverZdXAff);

    private:
        FORCE_INLINE_FUNCTION void writePixel(uint32_t* const destBits, const uint8_t* const textureBits8, const int16_t bitmapWidth, const int UInt, const int VInt) const;
    };

    class TextureMapper_RGB888_Opaque_BilinearInterpolation_GA : public DrawTextureMapScanLineBase32
    {
    public:
        virtual void drawTextureMapScanLineSubdivisions(int subdivisions, const int widthModLength, int pixelsToDraw, const int affineLength, float oneOverZRight, float UOverZRight, float VOverZRight, fixed16_16 U, fixed16_16 V, fixed16_16 deltaU, fixed16_16 deltaV, float ULeft, float VLeft, float URight, float VRight, float ZRight, const DrawingSurface& dest, const int destX, const int destY, const TextureSurface& texture, uint8_t alpha, const float dOneOverZdXAff, const float dUOverZdXAff, const float dVOverZdXAff);

    private:
        FORCE_INLINE_FUNCTION void writePixel(uint32_t* const destBits, const uint8_t* const textureBits8, const int16_t bitmapWidth, const int UInt, const int VInt, const uint8_t UFrac, const uint8_t VFrac, const uint8_t alpha) const;
        void writePixelOnEdge(uint32_t* const destBits, const uint8_t* const textureBits8, const int16_t bitmapWidth, const int16_t bitmapHeight, const int UInt, const int VInt, const uint8_t UFrac, const uint8_t VFrac, const uint8_t alpha) const;
    };

    class TextureMapper_RGB888_Opaque_BilinearInterpolation_NoGA : public DrawTextureMapScanLineBase32
    {
    public:
        virtual void drawTextureMapScanLineSubdivisions(int subdivisions, const int widthModLength, int pixelsToDraw, const int affineLength, float oneOverZRight, float UOverZRight, float VOverZRight, fixed16_16 U, fixed16_16 V, fixed16_16 deltaU, fixed16_16 deltaV, float ULeft, float VLeft, float URight, float VRight, float ZRight, const DrawingSurface& dest, const int destX, const int destY, const TextureSurface& texture, uint8_t alpha, const float dOneOverZdXAff, const float dUOverZdXAff, const float dVOverZdXAff);

    private:
        FORCE_INLINE_FUNCTION void writePixel(uint32_t* const destBits, const uint8_t* const textureBits8, const int16_t bitmapWidth, const int UInt, const int VInt, const uint8_t UFrac, const uint8_t VFrac) const;
        void writePixelOnEdge(uint32_t* const destBits, const uint8_t* const textureBits8, const int16_t bitmapWidth, const int16_t bitmapHeight, const int UInt, const int VInt, const uint8_t UFrac, const uint8_t VFrac) const;
    };

    class TextureMapper_ARGB8888_NonOpaque_NearestNeighbor_GA : public DrawTextureMapScanLineBase32
    {
    public:
        virtual void drawTextureMapScanLineSubdivisions(int subdivisions, const int widthModLength, int pixelsToDraw, const int affineLength, float oneOverZRight, float UOverZRight, float VOverZRight, fixed16_16 U, fixed16_16 V, fixed16_16 deltaU, fixed16_16 deltaV, float ULeft, float VLeft, float URight, float VRight, float ZRight, const DrawingSurface& dest, const int destX, const int destY, const TextureSurface& texture, uint8_t alpha, const float dOneOverZdXAff, const float dUOverZdXAff, const float dVOverZdXAff);

    private:
        FORCE_INLINE_FUNCTION void writePixel(uint32_t* const destBits, const uint32_t* const textureBits32, const int16_t bitmapWidth, const int UInt, const int VInt, const uint8_t alpha) const;
    };

    class TextureMapper_ARGB8888_NonOpaque_BilinearInterpolation_GA : public DrawTextureMapScanLineBase32
    {
    public:
        virtual void drawTextureMapScanLineSubdivisions(int subdivisions, const int widthModLength, int pixelsToDraw, const int affineLength, float oneOverZRight, float UOverZRight, float VOverZRight, fixed16_16 U, fixed16_16 V, fixed16_16 deltaU, fixed16_16 deltaV, float ULeft, float VLeft, float URight, float VRight, float ZRight, const DrawingSurface& dest, const int destX, const int destY, const TextureSurface& texture, uint8_t alpha, const float dOneOverZdXAff, const float dUOverZdXAff, const float dVOverZdXAff);

    private:
        FORCE_INLINE_FUNCTION void writePixel(uint32_t* const destBits, const uint32_t* const textureBits32, const int16_t bitmapWidth, const int UInt, const int VInt, const uint8_t UFrac, const uint8_t VFrac, const uint8_t alpha) const;
        void writePixelOnEdge(uint32_t* const destBits, const uint32_t* const textureBits32, const int16_t bitmapWidth, const int16_t bitmapHeight, const int UInt, const int VInt, const uint8_t UFrac, const uint8_t VFrac, const uint8_t alpha) const;
    };

    class TextureMapper_A4_NearestNeighbor_GA : public DrawTextureMapScanLineBase32
    {
    public:
        virtual void drawTextureMapScanLineSubdivisions(int subdivisions, const int widthModLength, int pixelsToDraw, const int affineLength, float oneOverZRight, float UOverZRight, float VOverZRight, fixed16_16 U, fixed16_16 V, fixed16_16 deltaU, fixed16_16 deltaV, float ULeft, float VLeft, float URight, float VRight, float ZRight, const DrawingSurface& dest, const int destX, const int destY, const TextureSurface& texture, uint8_t alpha, const float dOneOverZdXAff, const float dUOverZdXAff, const float dVOverZdXAff);

    private:
        FORCE_INLINE_FUNCTION void writePixel(uint32_t* const destBits, const uint8_t a4, const uint8_t alpha) const;
    };

    class TextureMapper_A4_NearestNeighbor_NoGA : public DrawTextureMapScanLineBase32
    {
    public:
        virtual void drawTextureMapScanLineSubdivisions(int subdivisions, const int widthModLength, int pixelsToDraw, const int affineLength, float oneOverZRight, float UOverZRight, float VOverZRight, fixed16_16 U, fixed16_16 V, fixed16_16 deltaU, fixed16_16 deltaV, float ULeft, float VLeft, float URight, float VRight, float ZRight, const DrawingSurface& dest, const int destX, const int destY, const TextureSurface& texture, uint8_t alpha, const float dOneOverZdXAff, const float dUOverZdXAff, const float dVOverZdXAff);

    private:
        FORCE_INLINE_FUNCTION void writePixel(uint32_t* const destBits, const uint8_t a4) const;
    };

    class TextureMapper_A4_BilinearInterpolation_GA : public DrawTextureMapScanLineBase32
    {
    public:
        virtual void drawTextureMapScanLineSubdivisions(int subdivisions, const int widthModLength, int pixelsToDraw, const int affineLength, float oneOverZRight, float UOverZRight, float VOverZRight, fixed16_16 U, fixed16_16 V, fixed16_16 deltaU, fixed16_16 deltaV, float ULeft, float VLeft, float URight, float VRight, float ZRight, const DrawingSurface& dest, const int destX, const int destY, const TextureSurface& texture, uint8_t alpha, const float dOneOverZdXAff, const float dUOverZdXAff, const float dVOverZdXAff);

    private:
        FORCE_INLINE_FUNCTION void writePixel(uint32_t* const destBits, const uint16_t* const textureBits, const int16_t bitmapStride, const int UInt, const int VInt, const uint8_t UFrac, const uint8_t VFrac, const uint8_t alpha) const;
        void writePixelOnEdge(uint32_t* const destBits, const uint16_t* const textureBits, const int16_t bitmapStride, const int16_t bitmapWidth, const int16_t bitmapHeight, const int UInt, const int VInt, const uint8_t UFrac, const uint8_t VFrac, const uint8_t alpha) const;
    };

    class TextureMapper_A4_BilinearInterpolation_NoGA : public DrawTextureMapScanLineBase32
    {
    public:
        virtual void drawTextureMapScanLineSubdivisions(int subdivisions, const int widthModLength, int pixelsToDraw, const int affineLength, float oneOverZRight, float UOverZRight, float VOverZRight, fixed16_16 U, fixed16_16 V, fixed16_16 deltaU, fixed16_16 deltaV, float ULeft, float VLeft, float URight, float VRight, float ZRight, const DrawingSurface& dest, const int destX, const int destY, const TextureSurface& texture, uint8_t alpha, const float dOneOverZdXAff, const float dUOverZdXAff, const float dVOverZdXAff);

    private:
        FORCE_INLINE_FUNCTION void writePixel(uint32_t* const destBits, const uint16_t* const textureBits, const int16_t bitmapStride, const int UInt, const int VInt, const uint8_t UFrac, const uint8_t VFrac) const;
        void writePixelOnEdge(uint32_t* const destBits, const uint16_t* const textureBits, const int16_t bitmapStride, const int16_t bitmapWidth, const int16_t bitmapHeight, const int UInt, const int VInt, const uint8_t UFrac, const uint8_t VFrac) const;
    };

    class DecompressorL8Base
    {
    public:
        virtual ~DecompressorL8Base()
        {
        }

        virtual void blitCopyRGB565(const uint8_t* sourceData, const uint8_t* clutData, const Rect& source, const Rect& blitRect, uint8_t alpha) = 0;
        virtual void blitCopyRGB888(const uint8_t* sourceData, const uint8_t* clutData, const Rect& source, const Rect& blitRect, uint8_t alpha) = 0;
        virtual void blitCopyARGB8888(const uint8_t* sourceData, const uint8_t* clutData, const Rect& source, const Rect& blitRect, uint8_t alpha) = 0;
    };

    class DecompressorL8_L4 : public DecompressorL8Base
    {
    public:
        virtual void blitCopyRGB565(const uint8_t* sourceData, const uint8_t* clutData, const Rect& source, const Rect& blitRect, uint8_t alpha);
        virtual void blitCopyRGB888(const uint8_t* sourceData, const uint8_t* clutData, const Rect& source, const Rect& blitRect, uint8_t alpha);
        virtual void blitCopyARGB8888(const uint8_t* sourceData, const uint8_t* clutData, const Rect& source, const Rect& blitRect, uint8_t alpha);

    private:
        void blendPixelRGB565(const uint16_t imagePixel, uint32_t* fbPixel, const uint8_t alpha, const uint8_t ialpha) const;
        void blendPixelARGB8888(const uint8_t* imagePixel, uint32_t* fbPixel, const uint8_t alpha) const;
        void blendPixelRGB888(const uint8_t* imagePixel, uint32_t* fbPixel, const uint8_t alpha, const uint8_t ialpha) const;
    };

    class DecompressorL8_RLE : public DecompressorL8Base
    {
    public:
        virtual void blitCopyRGB565(const uint8_t* sourceData, const uint8_t* clutData, const Rect& source, const Rect& blitRect, uint8_t alpha);
        virtual void blitCopyRGB888(const uint8_t* sourceData, const uint8_t* clutData, const Rect& source, const Rect& blitRect, uint8_t alpha);
        virtual void blitCopyARGB8888(const uint8_t* sourceData, const uint8_t* clutData, const Rect& source, const Rect& blitRect, uint8_t alpha);

    private:
        static const uint16_t BLOCK_SIZE = 1024U;
    };

    class DecompressorL8_LZW9 : public DecompressorL8Base
    {
    public:
        virtual void blitCopyRGB565(const uint8_t* sourceData, const uint8_t* clutData, const Rect& source, const Rect& blitRect, uint8_t alpha);
        virtual void blitCopyRGB888(const uint8_t* sourceData, const uint8_t* clutData, const Rect& source, const Rect& blitRect, uint8_t alpha);
        virtual void blitCopyARGB8888(const uint8_t* sourceData, const uint8_t* clutData, const Rect& source, const Rect& blitRect, uint8_t alpha);

    private:
        static const uint16_t MAX_DICT_SIZE = 512U;
        static const uint16_t BLOCK_SIZE = 1024U;

        LZW9DictionaryEntry lzw9Dictionary[MAX_DICT_SIZE];
        uint8_t entryBuffer[64]; // Having 64byte max entry sizes entails 1 + 2 + ... + 63 + 64 = 2080px size image blocks
    };

    DecompressorL8Base* decompressorL8_L4;
    DecompressorL8Base* decompressorL8_RLE;
    DecompressorL8Base* decompressorL8_LZW9;
};

} // namespace touchgfx

#endif // TOUCHGFX_LCD32BPP_XRGB8888_HPP
