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
#ifndef UNICODE_HPP
#define UNICODE_HPP

#include <stdarg.h>
#include <touchgfx/hal/Types.hpp>

namespace touchgfx
{
/**
 * @class Unicode Unicode.hpp touchgfx/Unicode.hpp
 *
 * @brief This class provides simple helper functions for working with 16-bit strings.
 */
class Unicode
{
public:

    /**
     * @typedef uint16_t UnicodeChar
     *
     * @brief Use the UnicodeChar typename when referring to strings.
     *
     *        Use the UnicodeChar typename when referring to strings.
     */
    typedef uint16_t UnicodeChar;

    static const UnicodeChar EMPTY[1];  ///< An empty string, which should be used instead of a null-pointer to indicate that the a string has no value.

    /**
     * @fn static uint16_t Unicode::strlen(const UnicodeChar* str);
     *
     * @brief Gets the length of a 0-terminated unicode string.
     *
     *        Gets the length of a 0-terminated unicode string.
     *
     * @param str The string in question.
     *
     * @return Length of string.
     */
    static uint16_t strlen(const UnicodeChar* str);

    /**
     * @fn static uint16_t Unicode::strlen(const char* str);
     *
     * @brief Gets the length of a 0-terminated string.
     *
     *        Gets the length of a 0-terminated string.
     *
     * @param str The string.
     *
     * @return Length of string.
     */
    static uint16_t strlen(const char* str);

    /**
     * @fn static uint16_t Unicode::strncpy(UnicodeChar* RESTRICT dst, const UnicodeChar* RESTRICT src, uint16_t maxchars);
     *
     * @brief Copy a string to a destination buffer, UnicodeChar to UnicodeChar version.
     *
     *        Copy a string to a destination buffer, UnicodeChar to UnicodeChar version. Stops if it
     *        encounters a zero-termination, in which case the zero-termination is included in
     *        the destination string. Otherwise copies maxchars.
     *
     * @param [out] dst The destination buffer. Must have a size of at least maxchars.
     * @param [in] src  The source string (UnicodeChars)
     * @param maxchars  Maximum number of characters to copy.
     *
     * @return The number of characters copied (excluding zero-termination if encountered)
     *
     * @warning If there is no null-termination among the first n UnicodeChars of src,
     *          the string placed in destination will NOT be zero-terminated!
     */
    static uint16_t strncpy(UnicodeChar* RESTRICT dst, const UnicodeChar* RESTRICT src, uint16_t maxchars);

    /**
     * @fn static uint16_t Unicode::strncpy(UnicodeChar* RESTRICT dst, const char* RESTRICT src, uint16_t maxchars);
     *
     * @brief Copy a string to a destination buffer, char to UnicodeChar version.
     *
     *        Copy a string to a destination buffer, char to UnicodeChar version. Stops if it encounters a
     *        zero-termination, in which case the zero-termination is included in the
     *        destination string. Otherwise copies maxchars.
     *
     * @param [out] dst The destination buffer. Must have a size of at least maxchars.
     * @param [in] src  The source string as an array of chars.
     * @param maxchars  Maximum number of characters to copy.
     *
     * @return The number of characters copied (excluding zero-termination if encountered)
     *
     * @warning If there is no null-termination among the first n bytes of src, the
     *          string placed in destination will NOT be zero-terminated!
     */
    static uint16_t strncpy(UnicodeChar* RESTRICT dst, const char* RESTRICT src, uint16_t maxchars);

    /**
     * @fn static void Unicode::itoa(int32_t value, UnicodeChar* buffer, uint16_t bufferSize, int radix);
     *
     * @brief Integer to ASCII conversion.
     *
     *        Integer to ASCII conversion.
     *
     * @param value        to convert.
     * @param [out] buffer to place result in.
     * @param bufferSize   Size of buffer (number of 16-bit values).
     * @param radix        to use (8 for octal, 10 for decimal, 16 for hex)
     */
    static void itoa(int32_t value, UnicodeChar* buffer, uint16_t bufferSize, int radix);

    /**
     * @fn static void Unicode::utoa(uint32_t value, UnicodeChar* buffer, uint16_t bufferSize, int radix);
     *
     * @brief Integer to ASCII conversion.
     *
     *        Integer to ASCII conversion.
     *
     * @param value        to convert.
     * @param [out] buffer to place result in.
     * @param bufferSize   Size of buffer (number of 16-bit values).
     * @param radix        to use (8 for octal, 10 for decimal, 16 for hex)
     */
    static void utoa(uint32_t value, UnicodeChar* buffer, uint16_t bufferSize, int radix);

    /**
     * @fn static int Unicode::atoi(const UnicodeChar* s);
     *
     * @brief String to integer conversion.
     *
     *        String to integer conversion. Starts conversion at the start of the string.
     *        Running digits from here are converted.
     *
     * @param s DECIMAL zero-terminated unicode string to convert.
     *
     * @return The converted integer value of the string, 0 if the string does not start with a
     *         digit.
     */
    static int atoi(const UnicodeChar* s);

    /**
     * @fn static UnicodeChar* Unicode::snprintf(UnicodeChar* dst, uint16_t dstSize, const UnicodeChar* format, ...);
     *
     * @brief Formats a string and adds null termination.
     *
     *        Formats a string and adds null termination. The string is formatted like when
     *        printf is used.
     *
     *        Support formats: \%c (element type: char), \%s (element type: zero-terminated
     *        UnicodeChar list), \%\% (The '\%' character), \%u, \%i, \%d, \%o, \%x (all these are
     *        integers formatted in radix 10, 10, 10, 8, 16 respectively).
     *
     *        The number formats (\%u, \%i, \%d, \%o and \%x) all support \%[0][length]X to specify
     *        the size of the generated field (length) and whether the number should be
     *        prefixed with zeros (or blanks).
     *
     * @param [out] dst Buffer to place the result in.
     * @param dstSize   Size of the buffer.
     * @param format    The string to format.
     * @param ...       Variable arguments providing additional information.
     *
     * @return pointer to the first element in the buffer.
     *
     * @see snprintfFloat, snprintfFloats
     */
    static UnicodeChar* snprintf(UnicodeChar* dst, uint16_t dstSize, const UnicodeChar* format, ...);

    /**
     * @fn static UnicodeChar* Unicode::vsnprintf(UnicodeChar* dst, uint16_t dstSize, const UnicodeChar* format, va_list pArg);
     *
     * @brief Variant of snprintf.
     *
     *        Variant of snprintf. See snprintf for details.
     *
     * @param [in] dst If non-null, destination for the.
     * @param dstSize  Size of the destination.
     * @param format   Describes the format to use.
     * @param pArg     The argument.
     *
     * @return null if it fails, else an UnicodeChar*.
     *
     * @see snprintf
     */
    static UnicodeChar* vsnprintf(UnicodeChar* dst, uint16_t dstSize, const UnicodeChar* format, va_list pArg);

    /**
     * @fn static UnicodeChar* Unicode::snprintf(UnicodeChar* dst, uint16_t dstSize, const char* format, ...);
     *
     * @brief Variant of snprintf.
     *
     *        Variant of snprintf.
     *
     *        Support formats: \%c (element type: char), \%s (element type: zero-terminated
     *        UnicodeChar list), \%\% (The '\%' character), \%u, \%i, \%d, \%o, \%x (all these are
     *        integers formatted in radix 10, 10, 10, 8, 16 respectively).
     *
     *        The number formats (\%u, \%i, \%d, \%o and \%x) all support
     *
     *            %[flags][width][.precision]X
     *
     *        Where flags can be:
     *
     *        <pre>
     *        '-': left justify the field (see width).
     *        '+': force sign.
     *        ' ': insert space if value is positive.
     *        '0': left pad with zeros instead of spaces (see width).
     *        </pre>
     *
     *        Where width is the desired width of the output. If the value is larger, more
     *        characters may be generated, but not more than the parameter dstSize. If width is
     *        '*' the actual width is read from the parameters passed to this function.
     *
     *        Where precision is the number of number of digits after the decimal point,
     *        default is 3. Use "%.f" to not generate any numbers after the decimal point. If
     *        precision is '*' the actual precision is read from the parameters passed to this
     *        function.
     *
     * @param [out] dst   Buffer to place the result in.
     * @param dstSize     Size of the buffer.
     * @param [in] format The string to format.
     * @param ...         Variable arguments providing additional information.
     *
     * @return pointer to the first element in the buffer.
     *
     * @note \%f is not supported by this function because floats are converted to doubles when
     *       given as parameters in a variable argument list (va_list). Use snprintfFloat or
     *       snprintfFloats instead.
     * @warning The format string is internally copied from at char* to a UnicodeChar*. This buffer
     *          has a limit of 63 characters, so if the format is longer than 63 characters, the
     *          caller must do this copying to prevent an assert from triggering:
     *          \code{.cpp}
     *          touchgfx::Unicode::UnicodeChar tmpfmt[200];
     *          touchgfx::Unicode::strncpy(tmpfmt, "Very, very, very, very, very, very, very, very, very long format %i", 200);
     *          touchgfx::Unicode::snprintf(dst, dstSize, tmpfmt, ...);
     *          \endcode
     *
     * @see snprintfFloat, snprintfFloats
     */
    static UnicodeChar* snprintf(UnicodeChar* dst, uint16_t dstSize, const char* format, ...);

    /**
     * @fn static UnicodeChar* Unicode::vsnprintf(UnicodeChar* dst, uint16_t dstSize, const char* format, va_list pArg);
     *
     * @brief Variant of snprintf.
     *
     *        Variant of snprintf. See snprintf for details.
     *
     * @param [in] dst If non-null, destination for the.
     * @param dstSize  Size of the destination.
     * @param format   Describes the format to use.
     * @param pArg     The argument.
     *
     * @return null if it fails, else an UnicodeChar*.
     *
     * @see snprintf
     */
    static UnicodeChar* vsnprintf(UnicodeChar* dst, uint16_t dstSize, const char* format, va_list pArg);

    /**
     * @fn static UnicodeChar* Unicode::snprintfFloats(UnicodeChar* dst, uint16_t dstSize, const UnicodeChar* format, const float* values);
     *
     * @brief Variant of snprintf for floats only.
     *
     *        Variant of snprintf for floats only.
     *
     *        The format supports several \%f with flags/modifiers. The following is supported:
     *
     *            %[flags][width][.precision]f
     *
     *        Where flags can be:
     *
     *        <pre>
     *        '-': left justify the field (see width).
     *        '+': force sign.
     *        ' ': insert space if value is positive
     *        '\#': insert decimal point even if there are not decimals
     *        '0': left pad with zeros instead of spaces (see width)
     *        </pre>
     *
     *        Where width is the desired width of the output. If the value is larger, more
     *        characters may be generated, but not more than the parameter dstSize. If width is
     *        '*' the actual width is read from the list of values passed to this function.
     *
     *        Where precision is the number of number of digits after the decimal point,
     *        default is 3. Use "%.f" to not generate any numbers after the decimal point. If
     *        precision is '*' the actual precision is read from the list of values passed to
     *        this function.
     *        \code{.cpp}
     *        float param1[3] = { 6.0f, 4.0f, 3.14159f };
     *        Unicode::snprintfFloats(buffer, 20, "%*.*f", param1);
     *        // buffer="3.1416"
     *        float param2[2] = { 3.14159f, -123.4f };
     *        Unicode::snprintfFloats(buffer, 20, "%f %f", param2);
     *        // buffer="3.142 -123.400"
     *        \endcode
     *
     * @param [out] dst   Buffer to place the result in.
     * @param dstSize     Size of the buffer.
     * @param [in] format The string to format.
     * @param [in] values Variable arguments providing additional information.
     *
     * @return pointer to the first element in the buffer.
     *
     * @see snprintf, snprintfFloat
     */
    static UnicodeChar* snprintfFloats(UnicodeChar* dst, uint16_t dstSize, const UnicodeChar* format, const float* values);

    /**
     * @fn static UnicodeChar* Unicode::snprintfFloat(UnicodeChar* dst, uint16_t dstSize, const UnicodeChar* format, const float value)
     *
     * @brief Variant of snprintf.
     *
     *        Variant of snprintf for one float only.
     *
     *        The number format supports only one \%f with flags/modifiers. The following is
     *        supported:
     *
     *            %[flags][width][.precision]f
     *
     *        Where flags can be:
     *
     *        <pre>
     *        '-': left justify the field (see width).
     *        '+': force sign.
     *        ' ': insert space if value is positive.
     *        '\#': insert decimal point even if there are not decimals.
     *        '0': left pad with zeros instead of spaces (see width).
     *        </pre>
     *
     *        Where width is the desired width of the output. If the value is larger, more
     *        characters may be generated, but not more than the parameter dstSize.
     *
     *        Where precision is the number of number of digits after the decimal point,
     *        default is 3. Use "%.f" to not generate any numbers after the decimal point.
     *        \code{.cpp}
     *        Unicode::UnicodeChar buffer[20];
     *        Unicode::snprintfFloat(buffer, 20, "%6.4f", 3.14159f);
     *        // buffer="3.1416" Unicode::snprintfFloat(buffer, 20, "%#6.f", 3.14159f);
     *        // buffer="    3." Unicode::snprintfFloat(buffer, 20, "%6f", 3.14159f);
     *        // buffer=" 3.142" Unicode::snprintfFloat(buffer, 20, "%+06.f", 3.14159f);
     *        // buffer="+00003"
     *        \endcode
     *
     *        If more control over the output is needed, see snprintfFloats which can have more
     *        than a single "%f" in the string and also supports "*" in place of a number.
     *
     * @param [out] dst   Buffer to place the result in.
     * @param dstSize     Size of the buffer.
     * @param [in] format The string to format.
     * @param value       Variable arguments providing additional information.
     *
     * @return pointer to the first element in the buffer.
     *
     * @see snprintf, snprintfFloats
     */
    static UnicodeChar* snprintfFloat(UnicodeChar* dst, uint16_t dstSize, const UnicodeChar* format, const float value)
    {
        return snprintfFloats(dst, dstSize, format, &value);
    }

    /**
     * @fn static UnicodeChar* Unicode::snprintfFloats(UnicodeChar* dst, uint16_t dstSize, const char* format, const float* values);
     *
     * @brief Variant of snprintf for floats only.
     *
     *        Variant of snprintf for floats only.
     *
     *        The format supports several \%f with flags/modifiers. The following is supported:
     *
     *            %[flags][width][.precision]f
     *
     *        Where flags can be:
     *
     *        <pre>
     *        '-': left justify the field (see width).
     *        '+': force sign.
     *        ' ': insert space if value is positive.
     *        '\#': insert decimal point even if there are not decimals.
     *        '0': left pad with zeros instead of spaces (see width).
     *        </pre>
     *
     *        Where width is the desired width of the output. If the value is larger, more
     *        characters may be generated, but not more than the parameter dstSize. If width is
     *        '*' the actual width is read from the list of values passed to this function.
     *
     *        Where precision is the number of number of digits after the decimal point,
     *        default is 3. Use "%.f" to not generate any numbers after the decimal point. If
     *        precision is '*' the actual precision is read from the list of values passed to
     *        this function.
     *        \code{.cpp}
     *        float param1[3] = { 6.0f, 4.0f, 3.14159f };
     *        Unicode::snprintfFloats(buffer, 20, "%*.*f", param1);
     *        // buffer="3.1416"
     *        float param2[2] = { 3.14159f, -123.4f };
     *        Unicode::snprintfFloats(buffer, 20, "%f %f", param2);
     *        // buffer="3.142 -123.400"
     *        \endcode
     *
     * @param [out] dst   Buffer to place the result in.
     * @param dstSize     Size of the buffer.
     * @param [in] format The string to format.
     * @param [in] values Variable arguments providing additional information.
     *
     * @return pointer to the first element in the buffer.
     *
     * @warning The format string is internally copied from at char* to a UnicodeChar*. This buffer
     *          has a limit of 63 characters, so if the format is longer than 63 characters, the
     *          caller must do this copying to prevent an assert from triggering:
     *          \code{.cpp}
     *          touchgfx::Unicode::UnicodeChar tmpfmt[200];
     *          touchgfx::Unicode::strncpy(tmpfmt, "Very, very, very, very, very, very, very, very, very long format %f", 200);
     *          touchgfx::Unicode::snprintfFloats(dst, dstSize, tmpfmt, values);
     *          \endcode
     *
     * @see snprintf, snprintfFloat
     */
    static UnicodeChar* snprintfFloats(UnicodeChar* dst, uint16_t dstSize, const char* format, const float* values);

    /**
     * @fn static UnicodeChar* Unicode::snprintfFloat(UnicodeChar* dst, uint16_t dstSize, const char* format, const float value)
     *
     * @brief Variant of snprintf.
     *
     *        Variant of snprintf for one float only.
     *
     *        The number format supports only one \%f with flags/modifiers. The following is
     *        supported:
     *
     *            %[flags][width][.precision]f
     *
     *        Where flags can be:
     *
     *        <pre>
     *        '-': left justify the field (see width).
     *        '+': force sign.
     *        ' ': insert space if value is positive.
     *        '\#': insert decimal point even if there are not decimals.
     *        '0': left pad with zeros instead of spaces (see width).
     *        </pre>
     *
     *        Where width is the desired width of the output. If the value is larger, more
     *        characters may be generated, but not more than the parameter dstSize.
     *
     *        Where precision is the number of number of digits after the decimal point,
     *        default is 3. Use "%.f" to not generate any numbers after the decimal point.
     *        \code{.cpp}
     *        Unicode::UnicodeChar buffer[20];
     *        Unicode::snprintfFloat(buffer, 20, "%6.4f", 3.14159f);
     *        // buffer="3.1416"
     *        Unicode::snprintfFloat(buffer, 20, "%#6.f", 3.14159f);
     *        // buffer="    3."
     *        Unicode::snprintfFloat(buffer, 20, "%6f", 3.14159f);
     *        // buffer=" 3.142"
     *        Unicode::snprintfFloat(buffer, 20, "%+06.f", 3.14159f);
     *        // buffer="+00003"
     *        \endcode
     *
     *        If more control over the output is needed, see snprintfFloats which can have more
     *        than a single "%f" in the string and also supports "*" in place of a number.
     *
     * @param [out] dst   Buffer to place the result in.
     * @param dstSize     Size of the buffer.
     * @param [in] format The string to format.
     * @param value       Variable arguments providing additional information.
     *
     * @return pointer to the first element in the buffer.
     *
     * @see snprintf, snprintfFloats
     */
    static UnicodeChar* snprintfFloat(UnicodeChar* dst, uint16_t dstSize, const char* format, const float value)
    {
        return snprintfFloats(dst, dstSize, format, &value);
    }

    /**
     * @fn static int Unicode::strncmp(const UnicodeChar* RESTRICT str1, const UnicodeChar* RESTRICT str2, uint16_t maxchars);
     *
     * @brief Compares up to maxchars characters of the string str1 to those of the string str2.
     *
     *        Compares up to maxchars characters of the string str1 to those of the string
     *        str2. This function starts comparing the first character of each string. If they
     *        are equal to each other, it continues with the following pairs until the
     *        characters differ, until a terminating null-character is reached, or until
     *        maxchars characters match in both strings, whichever happens first.
     *
     * @param str1     The first string.
     * @param str2     The second string.
     * @param maxchars The maximum number of chars to compare.
     *
     * @return Returns an integral value indicating the relationship between the strings: A zero
     *         value indicates that the characters compared in both strings are all equal. A
     *         value greater than zero indicates that the first character that does not match
     *         has a greater value in str1 than in str2; And a value less than zero indicates
     *         the opposite.
     */
    static int strncmp(const UnicodeChar* RESTRICT str1, const UnicodeChar* RESTRICT str2, uint16_t maxchars);

    /**
     * @fn static int Unicode::strncmp_ignore_white_spaces(const UnicodeChar* RESTRICT str1, const UnicodeChar* RESTRICT str2, uint16_t maxchars);
     *
     * @brief Like strncmp except that ignore any spaces in the two strings.
     *
     *        Like strncmp except that ignore any spaces in the two strings.
     *
     * @param str1     The first string.
     * @param str2     The second string.
     * @param maxchars The maximum number of chars to compare.
     *
     * @return Returns an integral value indicating the relationship between the strings: A zero
     *         value indicates that the characters compared in both strings are all equal. A
     *         value greater than zero indicates that the first character that does not match
     *         has a greater value in str1 than in str2; And a value less than zero indicates
     *         the opposite.
     */
    static int strncmp_ignore_white_spaces(const UnicodeChar* RESTRICT str1, const UnicodeChar* RESTRICT str2, uint16_t maxchars);

private:
    static void composeString(const UnicodeChar*& bufptr, const UnicodeChar sign, const UnicodeChar* format, bool hasPrecision, bool zeroPrefix, int precision, bool hasWidth, int width, bool alignLeft, int& charNumber, uint16_t dstSize, UnicodeChar* dst);

    static void parseFlagsAndPrecision(const UnicodeChar*& format, UnicodeChar& sign, bool& alignLeft, bool& forceDecimalPoint, bool& zeroPrefix, bool& hasWidth, int& width, bool& hasPrecision, int& precision);

    static const UnicodeChar* skip_white_spaces(const UnicodeChar* str);
};

} // namespace touchgfx

#endif // UNICODE_HPP
