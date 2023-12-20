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
#ifndef TEXTAREA_HPP
#define TEXTAREA_HPP

#include <touchgfx/hal/Types.hpp>
#include <touchgfx/Font.hpp>
#include <touchgfx/FontManager.hpp>
#include <touchgfx/widgets/Widget.hpp>
#include <touchgfx/Unicode.hpp>
#include <touchgfx/lcd/LCD.hpp>
#include <touchgfx/TypedText.hpp>

namespace touchgfx
{
/**
 * @class TextArea TextArea.hpp touchgfx/widgets/TextArea.hpp
 *
 * @brief This widget is capable of showing a text area on the screen.
 *
 *        This widget is capable of showing a text area on the screen. A TextArea can display a
 *        TypedText. Optional configuration include text color.
 *
 *        Example text_example shows how to use a TextArea.
 *
 * @note A TextArea just holds a pointer to the text displayed. The developer must ensure that the
 *       pointer remains valid when drawing.
 *
 * @see TypedText for information about text
 * @see TextAreaWithOneWildcard, @see TextAreaWithTwoWildcards for displaying texts containing
 *      wildcards.
 */
class TextArea : public Widget
{
public:

    /**
     * @fn TextArea::TextArea()
     *
     * @brief Default constructor.
     *
     *        Create an empty TextArea. Default color is black.
     */
    TextArea() : Widget(), typedText(TYPED_TEXT_INVALID), color(0), linespace(0), alpha(255), indentation(0), rotation(TEXT_ROTATE_0), wideTextAction(WIDE_TEXT_NONE)
    { }

    /**
     * @fn virtual Rect TextArea::getSolidRect() const
     *
     * @brief Gets solid rectangle.
     *
     *        Gets solid rectangle.
     *
     * @return the largest solid rectangle for this widget. For a TextArea, this is an empty area.
     */
    virtual Rect getSolidRect() const
    {
        return Rect(0, 0, 0, 0);
    }

    /**
     * @fn inline void TextArea::setColor(colortype color)
     *
     * @brief Sets the color of the text.
     *
     *        Sets the color of the text.
     *
     * @param color The color to use.
     */
    inline void setColor(colortype color)
    {
        this->color = color;
    }

    /**
     * @fn inline colortype TextArea::getColor() const
     *
     * @brief Gets the color of the text.
     *
     *        Gets the color of the text.
     *
     * @return The color to used for drawing the text.
     */
    inline colortype getColor() const
    {
        return color;
    }

    /**
     * @fn void TextArea::setAlpha(uint8_t alpha)
     *
     * @brief Sets the alpha value of the text.
     *
     *        Sets the alpha value of the text.
     *
     * @param alpha The alpha value. 255 = completely solid. 0 = invisible.
     */
    void setAlpha(uint8_t alpha)
    {
        this->alpha = alpha;
    }

    /**
     * @fn uint8_t TextArea::getAlpha() const
     *
     * @brief Gets the alpha value of the text.
     *
     *        Gets the alpha value of the text.
     *
     * @return The alpha value. 255 = completely solid. 0 = invisible.
     */
    uint8_t getAlpha() const
    {
        return alpha;
    }

    /**
     * @fn virtual void TextArea::setBaselineY(int16_t baselineY)
     *
     * @brief Adjusts the TextArea y coordinate to place the text at the specified baseline.
     *
     *        Adjusts the text areas y coordinate so the text will have its baseline at the
     *        specified value. The placements is relative to the specified TypedText so if this
     *        changes you have to set the baseline again. Note that setTypedText must be called
     *        prior to setting the baseline.
     *
     * @param baselineY The y coordinate of the baseline.
     */
    virtual void setBaselineY(int16_t baselineY)
    {
        setY(baselineY - getTypedText().getFont()->getFontHeight());
    }

    /**
     * @fn virtual void TextArea::setXBaselineY(int16_t x, int16_t baselineY)
     *
     * @brief Adjusts the TextArea y coordinate to place the text at the specified baseline.
     *
     *        Adjusts the text areas y coordinate so the text will have its baseline at the
     *        specified value. The placements is relative to the specified TypedText so if this
     *        changes you have to set the baseline again. Note that setTypedText must be called
     *        prior to setting the baseline. The specified x coordinate will be used as the x
     *        coordinate of the TextArea.
     *
     * @param x         The x coordinate of the TextArea.
     * @param baselineY The y coordinate of the baseline.
     */
    virtual void setXBaselineY(int16_t x, int16_t baselineY)
    {
        setX(x);
        setBaselineY(baselineY);
    }

    /**
     * @fn inline void TextArea::setLinespacing(uint16_t space)
     *
     * @brief Sets the line spacing of the TextArea.
     *
     *        Sets the line spacing of the TextArea.
     *
     * @param space The line spacing of use in the TextArea.
     */
    inline void setLinespacing(uint16_t space)
    {
        linespace = space;
    }

    /**
     * @fn inline uint16_t TextArea::getLinespacing() const
     *
     * @brief Gets the line spacing of the TextArea.
     *
     *        Gets the line spacing of the TextArea.
     *
     * @return The line spacing.
     */
    inline uint16_t getLinespacing() const
    {
        return linespace;
    }

    /**
     * @fn inline void TextArea::setIndentation(uint8_t indent)
     *
     * @brief Sets the indentation for the text.
     *
     *        Sets the indentation for the text. This is very useful when a font is an italic
     *        font where letters such as "j" and "g" extend a lot to the left under the
     *        previous characters. if a line starts with a "j" or "g" this letter would either
     *        have to be pushed to the right to be able to see all of it, e.g. using spaces
     *        which would ruin a multi line text which is left aligned. This could be solved by
     *        changing a textarea.setPosition(50,50,100,100) to textarea.setPosition(45,50,110,
     *        100) followed by a textarea.setIndentation(5). Characters that do not extend to
     *        the left under the previous characters will be drawn in the same position in
     *        either case, but "j" and "g" will be aligned with other lines.
     *
     *        The function getMaxPixelsLeft() will give you the maximum number of pixels any
     *        glyph in the font extends to the left.
     *
     * @param indent The indentation from left (when left aligned text) and right (when right
     *               aligned text).
     *
     * @see getMaxPixelsLeft
     */
    inline void setIndentation(uint8_t indent)
    {
        indentation = indent;
    }

    /**
     * @fn inline uint8_t TextArea::getIndentation()
     *
     * @brief Gets the indentation.
     *
     *        Gets the indentation.
     *
     * @return The indentation.
     *
     * @see setIndetation
     */
    inline uint8_t getIndentation()
    {
        return indentation;
    }

    /**
     * @fn virtual int16_t TextArea::getTextHeight();
     *
     * @brief Gets the total height needed by the text.
     *
     *        Gets the total height needed by the text, taking number of lines and line spacing
     *        into consideration.
     *
     * @return the total height needed by the text.
     */
    virtual int16_t getTextHeight();

    /**
     * @fn virtual uint16_t TextArea::getTextWidth() const;
     *
     * @brief Gets the width in pixels of the current associated text in the current selected
     *        language.
     *
     *        Gets the width in pixels of the current associated text in the current selected
     *        language. In case of multi-lined text the width of the widest line is returned.
     *
     * @return The width in pixels of the current text.
     */
    virtual uint16_t getTextWidth() const;

    /**
     * @fn virtual void TextArea::draw(const Rect& area) const;
     *
     * @brief Draws the text.
     *
     *        Draws the text. Called automatically.
     *
     * @param area The invalidated area.
     */
    virtual void draw(const Rect& area) const;

    /**
     * @fn void TextArea::setTypedText(TypedText t);
     *
     * @brief Sets the TypedText of the text area.
     *
     *        Sets the TypedText of the text area. If no prior size has been set the TextArea
     *        will be resized to fit the new TypedText.
     *
     * @param t The TypedText for this widget to display.
     */
    void setTypedText(TypedText t);

    /**
     * @fn TypedText TextArea::getTypedText() const
     *
     * @brief Gets the TypedText of the text area
     *
     *        Gets the TypedText of the text area.
     *
     * @return The currently used TypedText.
     */
    TypedText getTypedText() const
    {
        return typedText;
    }

    /**
     * @fn void TextArea::setRotation(const TextRotation rotation = TEXT_ROTATE_0)
     *
     * @brief Sets rotation of the text in the TextArea.
     *
     *        Sets rotation of the text in the TextArea. The value TEXT_ROTATE_0 is the default
     *        for normal text. The value TEXT_ROTATE_90 will rotate the text clockwise, thus
     *        writing from the top of the display and down. Similarly TEXT_ROTATE_180 and
     *        TEXT_ROTATE_270 is further rotate 90 degrees clockwise.
     *
     * @param rotation The rotation of the text.
     */
    void setRotation(const TextRotation rotation = TEXT_ROTATE_0)
    {
        this->rotation = rotation;
    }

    /**
     * @fn TextRotation TextArea::getRotation() const
     *
     * @brief Gets rotation of the text in the TextArea.
     *
     *        Gets rotation of the text in the TextArea.
     *
     * @return The rotation of the text.
     */
    TextRotation getRotation() const
    {
        return rotation;
    }

    /**
     * @fn void TextArea::resizeToCurrentText();
     *
     * @brief Sets the dimensions of the TextArea.
     *
     *         Sets the dimensions of the TextArea to match the width and height of the current
     *         associated text for the current selected language.
     */
    void resizeToCurrentText();

    /**
     * @fn void TextArea::resizeHeightToCurrentText();
     *
     * @brief Sets the hight of the TextArea.
     *
     *         Sets the height of the TextArea to match the height of the current associated
     *         text for the current selected language. This is espicially useful for texts with
     *         WordWrap enabled.
     *
     * @see resizeToCurrentText, setWordWrap
     */
    void resizeHeightToCurrentText();

    /**
     * @fn void TextArea::setWideTextAction(WideTextAction action)
     *
     * @brief Sets wide text action.
     *
     *        Sets wide text action. Defines what to do if a line of text is wider than the
     *        text area.
     *
     * @param action The action to perform for wide lines of text.
     *
     * @see getWideTextACtion
     * @see resizeHeightToCurrentText
     */
    void setWideTextAction(WideTextAction action)
    {
        wideTextAction = action;
    }

    /**
     * @fn WideTextAction TextArea::getWideTextAction() const
     *
     * @brief Gets word wrap.
     *
     *        Gets word wrap preciously set using setWordWrap.
     *
     * @return true if it succeeds, false if it fails.
     *
     * @see setWordWrap
     */
    WideTextAction getWideTextAction() const
    {
        return wideTextAction;
    }

    /**
     * @fn virtual uint16_t TextArea::getType() const
     *
     * @brief For GUI testing only.
     *
     *        For GUI testing only. Returns type of this drawable.
     *
     * @return TYPE_TEXTAREA.
     */
    virtual uint16_t getType() const
    {
        return (uint16_t)TYPE_TEXTAREA;
    }

protected:
    TypedText      typedText;      ///< The TypedText to display
    colortype      color;          ///< The color to use.
    uint16_t       linespace;      ///< The line spacing to use, in pixels, in case the text contains newlines.
    uint8_t        alpha;          ///< The alpha to use.
    uint8_t        indentation;    ///< The indentation of the text inside the text area
    TextRotation   rotation;       ///< The text rotation to use
    WideTextAction wideTextAction; ///< What to do if the text is wider than the text area

private:
    int16_t getTextHeightInternal(const Unicode::UnicodeChar* format, ...) const;
};

} // namespace touchgfx

#endif // TEXTAREA_HPP
