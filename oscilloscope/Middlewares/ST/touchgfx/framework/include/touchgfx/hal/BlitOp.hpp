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
 * @file touchgfx/hal/BlitOp.hpp
 *
 * Declares constants for specifying blit operation capabilities.
 */
#ifndef TOUCHGFX_BLITOP_HPP
#define TOUCHGFX_BLITOP_HPP

#include <touchgfx/hal/Types.hpp>

namespace touchgfx
{
/** The Blit Operations. */
enum BlitOperations
{
    BLIT_OP_COPY = 1 << 0,                         ///< Copy the source to the destination
    BLIT_OP_FILL = 1 << 1,                         ///< Fill the destination with color
    BLIT_OP_COPY_WITH_ALPHA = 1 << 2,              ///< Copy the source to the destination using the given alpha
    BLIT_OP_FILL_WITH_ALPHA = 1 << 3,              ///< Fill the destination with color using the given alpha
    BLIT_OP_COPY_WITH_TRANSPARENT_PIXELS = 1 << 4, ///< Deprecated, ignored. (Copy the source to the destination, but not the transparent pixels)
    BLIT_OP_COPY_ARGB8888 = 1 << 5,                ///< Copy the source to the destination, performing per-pixel alpha blending
    BLIT_OP_COPY_ARGB8888_WITH_ALPHA = 1 << 6,     ///< Copy the source to the destination, performing per-pixel alpha blending and blending the result with an image-wide alpha
    BLIT_OP_COPY_L8 = 1 << 7,                      ///< Copy the L8 source to the destination using the given alpha
    BLIT_OP_COPY_A4 = 1 << 8,                      ///< Copy 4-bit source text to destination, performing per-pixel alpha blending
    BLIT_OP_COPY_A8 = 1 << 9,                      ///< Copy 8-bit source text to destination, performing per-pixel alpha blending
    BLIT_OP_COPY_16BIT = 1 << 10,                  ///< Copy 16-bit regardless of frame buffer format
    BLIT_OP_FILL_16BIT = 1 << 11                   ///< Fill 16-bit regardless of frame buffer format
};

/**
 * BlitOp instances carry the required information for performing operations on the LCD
 * (framebuffer) using DMA.
 */
struct BlitOp
{
    uint32_t operation;     ///< The operation to perform @see BlitOperations
    const uint16_t* pSrc;   ///< Pointer to the source (pixels or indexes)
    const uint8_t* pClut;   ///< Pointer to the source CLUT entires
    colortype color;        ///< Color to fill
    uint16_t* pDst;         ///< Pointer to the destination
    uint16_t nSteps;        ///< The number of pixels in a line
    uint16_t nLoops;        ///< The number of lines
    uint16_t srcLoopStride; ///< The number of bytes to stride the source after every loop
    uint16_t dstLoopStride; ///< The number of bytes to stride the destination after every loop
    uint8_t alpha;          ///< The alpha to use
    uint8_t srcFormat;      ///< The source format @see Bitmap::BitmapFormat
    uint8_t dstFormat;      ///< The destination format @see Bitmap::BitmapFormat
    bool replaceBgAlpha;    ///< Replace the background per pixel alpha value with 255 = solid
    bool replaceFgAlpha;    ///< Replace the fourground per pixel alpha value with 255 = solid
};

} // namespace touchgfx

#endif // TOUCHGFX_BLITOP_HPP
