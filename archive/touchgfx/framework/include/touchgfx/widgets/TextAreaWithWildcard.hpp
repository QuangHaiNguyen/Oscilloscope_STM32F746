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
#ifndef TEXTAREAWITHWILDCARD_HPP
#define TEXTAREAWITHWILDCARD_HPP

#include <touchgfx/TextProvider.hpp>
#include <touchgfx/widgets/TextArea.hpp>

namespace touchgfx
{
/**
 * @class TextAreaWithWildcardBase TextAreaWithWildcard.hpp touchgfx/widgets/TextAreaWithWildcard.hpp
 *
 * @brief Base class for TextAreas displaying texts with one or more wildcards.
 *
 *        Base class for TextAreas displaying texts with one or more wildcards.
 *
 * @see TextAreaWithOneWildcard
 * @see TextAreaWithTwoWildcards
 */
class TextAreaWithWildcardBase : public TextArea
{
public:
    /**
     * @fn TextAreaWithWildcardBase::TextAreaWithWildcardBase()
     *
     * @brief Create an empty text area.
     *
     *        Create an empty text area.
     *
     * @note No text can be displayed until a font is set. Default color is black.
     */
    TextAreaWithWildcardBase() : TextArea() {  }

    /**
     * @fn int16_t TextAreaWithWildcardBase::calculateTextHeight(const Unicode::UnicodeChar* format, ...) const;
     *
     * @brief Gets the total height needed by the text.
     *
     *        Gets the total height needed by the text. Determined by number of lines and
     *        linespace. The number of wildcards in the text should match the number of values
     *        for the wildcards.
     *
     * @param format The text containing %s wildcards.
     * @param ...    Variable arguments providing additional information.
     *
     * @return the total height needed by the text.
     */
    int16_t calculateTextHeight(const Unicode::UnicodeChar* format, ...) const;
};

/**
 * @class TextAreaWithOneWildcard TextAreaWithWildcard.hpp touchgfx/widgets/TextAreaWithWildcard.hpp
 *
 * @brief TextArea with one wildcard.
 *
 *        TextArea with one wildcard. The format string (i.e. the text pointer set in
 *        TextArea::setText) is expected to contain a wildcard %s.
 *
 * @see TextAreaWithWildcardBase
 */
class TextAreaWithOneWildcard : public TextAreaWithWildcardBase
{
public:

    /**
     * @fn TextAreaWithOneWildcard::TextAreaWithOneWildcard()
     *
     * @brief Default constructor.
     *
     *        Create an empty text area.
     *
     * @note No text can be displayed until a font is set. Default color is black.
     */
    TextAreaWithOneWildcard() : TextAreaWithWildcardBase(), wildcard(0)
    {
    }

    /**
     * @fn virtual int16_t TextAreaWithOneWildcard::getTextHeight()
     *
     * @brief Gets text height.
     *
     *        Gets text height.
     *
     * @return The text height.
     */
    virtual int16_t getTextHeight()
    {
        return typedText.hasValidId() ? calculateTextHeight(typedText.getText(), wildcard) : 0;
    }

    /**
     * @fn virtual void TextAreaWithOneWildcard::draw(const Rect& area) const
     *
     * @brief Draws TextArea and its text.
     *
     *        Draws TextArea and its text if a Font is set and the TypedText associated with
     *        the TextArea is valid.
     *
     * @param area The invalidated area.
     */
    virtual void draw(const Rect& area) const
    {
        if (typedText.hasValidId())
        {
            const Font* fontToDraw = typedText.getFont();
            if (fontToDraw != 0)
            {
                LCD::StringVisuals visuals(fontToDraw, color, alpha, typedText.getAlignment(), linespace, rotation, typedText.getTextDirection(), indentation, wideTextAction);
                HAL::lcd().drawString(getAbsoluteRect(), area, visuals, typedText.getText(), wildcard);
            }
        }
    }

    /**
     * @fn void TextAreaWithOneWildcard::setWildcard(const Unicode::UnicodeChar* value)
     *
     * @brief Sets the wildcard in the text.
     *
     *        Sets the wildcard in the text.Must be a zero-terminated UnicodeChar array.
     *
     * @param value A pointer to the UnicodeChar to set the wildcard to.
     */
    void setWildcard(const Unicode::UnicodeChar* value)
    {
        wildcard = value;
    }

    /**
     * @fn const Unicode::UnicodeChar* TextAreaWithOneWildcard::getWildcard() const
     *
     * @brief Gets the wildcard in the text.
     *
     *        Gets the wildcard in the text.
     *
     * @return The wildcard used in the text.
     */
    const Unicode::UnicodeChar* getWildcard() const
    {
        return wildcard;
    }

    /**
     * @fn virtual uint16_t TextAreaWithOneWildcard::getTextWidth() const
     *
     * @brief Gets the width in pixels of the current associated text.
     *
     *         Gets the width in pixels of the current associated text in the current selected
     *         language. In case of multi-lined text the width of the widest line is returned.
     *
     * @return The width in pixels of the current text.
     */
    virtual uint16_t getTextWidth() const
    {
        return typedText.hasValidId() ? typedText.getFont()->getStringWidth(typedText.getTextDirection(), typedText.getText(), wildcard) : 0;
    }

    /**
     * @fn virtual uint16_t TextAreaWithOneWildcard::getType() const
     *
     * @brief For GUI testing only.
     *
     *        For GUI testing only. Returns type of this drawable.
     *
     * @return TYPE_TEXTAREAWITHONEWILDCARD.
     */
    virtual uint16_t getType() const
    {
        return (uint16_t)TYPE_TEXTAREAWITHONEWILDCARD;
    }

protected:
    const Unicode::UnicodeChar* wildcard; ///< Pointer to the wildcard string. Must be zero-terminated.
};

/**
 * @class TextAreaWithTwoWildcards TextAreaWithWildcard.hpp touchgfx/widgets/TextAreaWithWildcard.hpp
 *
 * @brief TextArea with two wildcards.
 *
 *        TextArea with two wildcards. The format string (i.e. the text pointer set in
 *        TextArea::setText) is expected to contain two wildcards %s.
 *
 * @see TextAreaWithWildcardBase
 */
class TextAreaWithTwoWildcards : public TextAreaWithWildcardBase
{
public:

    /**
     * @fn TextAreaWithTwoWildcards::TextAreaWithTwoWildcards()
     *
     * @brief Default constructor.
     *
     *        Create an empty text area.
     *
     * @note No text can be displayed until a font is set. Default color is black.
     */
    TextAreaWithTwoWildcards() : TextAreaWithWildcardBase(), wc1(0), wc2(0)
    {
    }

    /**
     * @fn virtual int16_t TextAreaWithTwoWildcards::getTextHeight()
     *
     * @brief Gets text height.
     *
     *        Gets text height.
     *
     * @return The text height.
     */
    virtual int16_t getTextHeight()
    {
        return typedText.hasValidId() ? calculateTextHeight(typedText.getText(), wc1, wc2) : 0;
    }

    /**
     * @fn virtual void TextAreaWithTwoWildcards::draw(const Rect& area) const
     *
     * @brief Draws TextArea and its text.
     *
     *        Draws TextArea and its text if a Font is set and the TypedText associated with
     *        the TextArea is valid.
     *
     * @param area The invalidated area.
     */
    virtual void draw(const Rect& area) const
    {
        if (typedText.hasValidId())
        {
            const Font* fontToDraw = typedText.getFont();
            if (fontToDraw != 0)
            {
                LCD::StringVisuals visuals(fontToDraw, color, alpha, typedText.getAlignment(), linespace, rotation, typedText.getTextDirection(), indentation, wideTextAction);
                HAL::lcd().drawString(getAbsoluteRect(), area, visuals, typedText.getText(), wc1, wc2);
            }
        }
    }

    /**
     * @fn void TextAreaWithTwoWildcards::setWildcard1(const Unicode::UnicodeChar* value)
     *
     * @brief Sets the first wildcard in the text.
     *
     *        Sets the first wildcard in the text. Must be a zero-terminated UnicodeChar array.
     *
     * @param value A pointer to the UnicodeChar to set the wildcard to.
     */
    void setWildcard1(const Unicode::UnicodeChar* value)
    {
        wc1 = value;
    }

    /**
     * @fn const Unicode::UnicodeChar* TextAreaWithTwoWildcards::getWildcard1() const
     *
     * @brief Gets the first wildcard in the text.
     *
     *        Gets the first wildcard in the text.
     *
     * @return The first wildcard from a TextArea with two wildcards.
     */
    const Unicode::UnicodeChar* getWildcard1() const
    {
        return wc1;
    }

    /**
     * @fn void TextAreaWithTwoWildcards::setWildcard2(const Unicode::UnicodeChar* value)
     *
     * @brief Sets the second wildcard in the text.
     *
     *        Sets the second wildcard in the text. Must be a zero-terminated UnicodeChar array.
     *
     * @param value A pointer to the UnicodeChar to set the wildcard to.
     */
    void setWildcard2(const Unicode::UnicodeChar* value)
    {
        wc2 = value;
    }

    /**
     * @fn const Unicode::UnicodeChar* TextAreaWithTwoWildcards::getWildcard2() const
     *
     * @brief Gets the second wildcard in the text.
     *
     *        Gets the second wildcard in the text.
     *
     * @return The second wildcard from a TextArea with two wildcards.
     */
    const Unicode::UnicodeChar* getWildcard2() const
    {
        return wc2;
    }

    /**
     * @fn virtual uint16_t TextAreaWithTwoWildcards::getTextWidth() const
     *
     * @brief Gets the width in pixels of the current associated text.
     *
     *           Gets the width in pixels of the current associated text in the current
     *           selected language. In case of multi-lined text the width of the widest line is
     *           returned.
     *
     * @return The width in pixels of the current text.
     */
    virtual uint16_t getTextWidth() const
    {
        return typedText.hasValidId() ? typedText.getFont()->getStringWidth(typedText.getTextDirection(), typedText.getText(), wc1, wc2) : 0;
    }

    /**
     * @fn virtual uint16_t TextAreaWithTwoWildcards::getType() const
     *
     * @brief For GUI testing only.
     *
     *        For GUI testing only. Returns type of this drawable.
     *
     * @return TYPE_TEXTAREAWITHTWOWILDCARDS.
     */
    virtual uint16_t getType() const
    {
        return (uint16_t)TYPE_TEXTAREAWITHTWOWILDCARDS;
    }
protected:
    const Unicode::UnicodeChar* wc1; ///< Pointer to the first wildcard string. Must be zero-terminated.
    const Unicode::UnicodeChar* wc2; ///< Pointer to the second wildcard string. Must be zero-terminated.
};

} // namespace touchgfx

#endif // TEXTAREAWITHWILDCARD_HPP
