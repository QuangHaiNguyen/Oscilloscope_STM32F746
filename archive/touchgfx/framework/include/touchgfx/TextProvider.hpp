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
#ifndef TEXTPROVIDER_HPP
#define TEXTPROVIDER_HPP

#include <stdarg.h>
#include <touchgfx/Unicode.hpp>

namespace touchgfx
{
/**
 * @class TextProvider TextProvider.hpp touchgfx/TextProvider.hpp
 *
 * @brief The TextProvider is used in drawing basic and wildcard strings.
 *
 *        The TextProvider is used in drawing basic and wildcard strings. The TextProvider
 *        enables wildcard expansion of the string at the time it is written to the LCD.
 *
 *        It provides printf formatted text strings one character at the time, without the need
 *        for a user provided buffer to store the text string.
 */
class TextProvider
{
public:
    static const uint32_t MAX_32BIT_INTEGER_DIGITS = 33U; ///< Max number of digits used for the text representation of a 32 bit integer.

    /**
     * @fn TextProvider::TextProvider();
     *
     * @brief Default constructor.
     *
     *        Empty constructor. The user must call initialize() before characters can be
     *        provided.
     */
    TextProvider();

    /**
     * @fn void TextProvider::initialize(const Unicode::UnicodeChar* format, va_list pArg);
     *
     * @brief Initializes the TextProvider.
     *
     *        Initializes the TextProvider. Each '\2' character in the format is replaced by one UnicodeChar* argument from pArg.
     *
     * @param format The string to format.
     * @param pArg   Format arguments in the form of a va_list.
     */
    void initialize(const Unicode::UnicodeChar* format, va_list pArg);

    /**
     * @fn Unicode::UnicodeChar TextProvider::getNextChar(TextDirection direction = TEXT_DIRECTION_LTR);
     *
     * @brief Gets the next character.
     *
     *        Gets the next character.
     *
     * @return The next character of the expanded string or 0 if end of string i reached.
     *
     * @see TextProvider::getNextLigature()
     */
    Unicode::UnicodeChar getNextChar();

    /**
     * @fn Unicode::UnicodeChar TextProvider::getNextLigature(TextDirection direction);
     *
     * @brief Gets the next ligature.
     *
     *        Gets the next ligature. For most languages this is the same as getNextChar() but
     *        eg. Arabic has different ligatures for each character.
     *
     * @note Functions getNextLigature() and getNextChar() will advance through the same
     *       buffer and mixing the use of those functions is not recommended and may cause
     *       undesired results. Instead create two TextProviders and user getNextChar() on one
     *       and getNextLigature() on the other.
     *
     * @param direction The direction.
     *
     * @return The next character of the expanded string or 0 if end of string i reached.
     *
     * @see TextProvider::getNextChar()
     */
    Unicode::UnicodeChar getNextLigature(TextDirection direction);

private:
    Unicode::UnicodeChar getNextCharInternal();
    const Unicode::UnicodeChar* original_format_string;
    const Unicode::UnicodeChar* format;
    va_list pArg;
    const Unicode::UnicodeChar* substringPointer;
    bool isWritingWildcard;
    Unicode::UnicodeChar nextCharacter[2];

    typedef void (TextProvider::*UnicodeConverterInitFunctionPointer)();
    typedef Unicode::UnicodeChar(TextProvider::*UnicodeConverterFunctionPointer)(const Unicode::UnicodeChar currChar, const Unicode::UnicodeChar nextChar[2], const TextDirection direction);
    static UnicodeConverterInitFunctionPointer unicodeConverterInitFunction;
    static UnicodeConverterFunctionPointer unicodeConverterFunction;

    void initializeInternal();
    void unicodeConverterInit();
    Unicode::UnicodeChar unicodeConverter(const Unicode::UnicodeChar currChar, const Unicode::UnicodeChar nextChar[2], const TextDirection direction);
    bool isContextualBeginning;
    bool emitZeroWidthChar;
    static Unicode::UnicodeChar contextualForms0621_063a[][4];
    static Unicode::UnicodeChar contextualForms0641_064a[][4];
};

} // namespace touchgfx

#endif // TEXTPROVIDER_HPP
