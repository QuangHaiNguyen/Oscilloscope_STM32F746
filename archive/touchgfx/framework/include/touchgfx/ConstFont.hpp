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
#ifndef CONSTFONT_HPP
#define CONSTFONT_HPP

#include "Font.hpp"

namespace touchgfx
{
/**
 * @class ConstFont ConstFont.hpp touchgfx/ConstFont.hpp
 *
 * @brief A ConstFont is a Font implementation that has its contents defined at compile-time and
 *        usually placed in read-only memory.
 *
 *        A ConstFont is a Font implementation that has its contents defined at compile-time
 *        and usually placed in read-only memory.
 *
 * @note Pure virtual class. Create an application-
 *          specific implementation of getPixelData.
 *
 * @see Font
 */
class ConstFont : public Font
{
public:

    /**
     * @fn ConstFont::ConstFont(const GlyphNode* list, uint16_t size, uint16_t height, uint8_t pixBelowBase, uint8_t bitsPerPixel, uint8_t maxLeft, uint8_t maxRight, const Unicode::UnicodeChar fallbackChar);
     *
     * @brief Creates a font instance.
     *
     *        Creates a font instance.
     *
     * @param list         The array of glyphs known to this font.
     * @param size         The number of glyphs in list.
     * @param height       The height in pixels of the highest character in this font.
     * @param pixBelowBase The maximum number of pixels that can be drawn below the baseline in
     *                     this font.
     * @param bitsPerPixel The number of bits per pixel in this font.
     * @param maxLeft      The maximum a character extends to the left.
     * @param maxRight     The maximum a character extends to the right.
     * @param fallbackChar The fallback character for the typography in case no glyph is
     *                     available.
     */
    ConstFont(const GlyphNode* list, uint16_t size, uint16_t height, uint8_t pixBelowBase, uint8_t bitsPerPixel, uint8_t maxLeft, uint8_t maxRight, const Unicode::UnicodeChar fallbackChar);

    using Font::getGlyph;

    /**
     * @fn virtual const GlyphNode* ConstFont::getGlyph(Unicode::UnicodeChar unicode, const uint8_t*& pixelData, uint8_t& bitsPerPixel) const;
     *
     * @brief Gets the glyph data associated with the specified unicode.
     *
     *        Gets the glyph data associated with the specified unicode. An implementation of
     *        Font::getGlyph. Searches the glyph list for the specified font.
     *
     *        Complexity O(log n)
     *
     * @param unicode            The character to look up.
     * @param pixelData          Pointer to the pixel data for the glyph if the glyph is found.
     *                           This is set by this method.
     * @param [out] bitsPerPixel Reference where to place the number of bits per pixel.
     *
     * @return A pointer to the glyph node or null if the glyph was not found.
     */
    virtual const GlyphNode* getGlyph(Unicode::UnicodeChar unicode, const uint8_t*& pixelData, uint8_t& bitsPerPixel) const;

    /**
     * @fn virtual const uint8_t* ConstFont::getPixelData(const GlyphNode* glyph) const = 0;
     *
     * @brief Gets the pixel date associated with this glyph.
     *
     *        Gets the pixel date associated with this glyph.
     *
     * @param glyph The glyph to get the pixels data from.
     *
     * @return Pointer to the pixel data of this glyph.
     */
    virtual const uint8_t* getPixelData(const GlyphNode* glyph) const = 0;

    /**
     * @fn virtual int8_t ConstFont::getKerning(Unicode::UnicodeChar prevChar, const GlyphNode* glyph) const = 0;
     *
     * @brief Gets the kerning distance between two characters.
     *
     *        Gets the kerning distance between two characters.
     *
     * @param prevChar The unicode value of the previous character.
     * @param glyph    the glyph object for the current character.
     *
     * @return The kerning distance between prevChar and glyph char.
     */
    virtual int8_t getKerning(Unicode::UnicodeChar prevChar, const GlyphNode* glyph) const = 0;

protected:

    /**
     * @fn const GlyphNode* ConstFont::find(Unicode::UnicodeChar unicode) const;
     *
     * @brief Finds the glyph data associated with the specified unicode.
     *
     *        Finds the glyph data associated with the specified unicode.
     *
     * @param unicode The character to look up.
     *
     * @return A pointer to the glyph node or null if the glyph was not found.
     */
    const GlyphNode* find(Unicode::UnicodeChar unicode) const;

    const GlyphNode* glyphList; ///< The list of glyphs
    uint16_t         listSize;  ///< The size of the list of glyphs

private:
    ConstFont() : Font(0, 0, 0, 0, 0, 0) { }
};

} // namespace touchgfx
#endif // CONSTFONT_HPP
