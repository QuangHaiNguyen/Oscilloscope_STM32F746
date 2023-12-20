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
#ifndef TEXTPROGRESS_HPP
#define TEXTPROGRESS_HPP

#include <touchgfx/containers/progress_indicators/AbstractProgressIndicator.hpp>
#include <touchgfx/widgets/TextAreaWithWildcard.hpp>

namespace touchgfx
{
/**
 * @class TextProgress TextProgress.hpp touchgfx/containers/progress_indicators/TextProgress.hpp
 *
 * @brief A text progress.
 *
 *        A text progress will display progress as a number with a given number of decimals.
 *
 * @note The implementation does not use floating point variables to calculate the progress.
 */
class TextProgress : public AbstractProgressIndicator
{
public:

    /**
     * @fn TextProgress::TextProgress();
     *
     * @brief Default constructor.
     *
     *        Default constructor.
     */
    TextProgress();

    /**
     * @fn virtual TextProgress::~TextProgress();
     *
     * @brief Destructor.
     *
     *        Destructor.
     */
    virtual ~TextProgress();

    /**
     * @fn virtual void TextProgress::setProgressIndicatorPosition(int16_t x, int16_t y, int16_t width, int16_t height);
     *
     * @brief Sets the position and dimension of the text progress indicator.
     *
     *        Sets the position and dimension of the text progress indicator relative to the
     *        background image.
     *
     * @param x      The x coordinate.
     * @param y      The y coordinate.
     * @param width  The width of the text progress indicator.
     * @param height The height of the text progress indicator.
     */
    virtual void setProgressIndicatorPosition(int16_t x, int16_t y, int16_t width, int16_t height);

    /**
     * @fn virtual void TextProgress::setTypedText(TypedText t);
     *
     * @brief Sets the typed text.
     *
     *        Sets the typed text. The text should have one wildcard and could for example "&lt;
     *        progress&gt;%".
     *
     * @param t The TypedText to process.
     *
     * @see getTypedText
     */
    virtual void setTypedText(const TypedText& t);

    /**
     * @fn virtual TypedText TextProgress::getTypedText() const;
     *
     * @brief Gets the typed text.
     *
     *        Gets the typed text.
     *
     * @return The typed text.
     *
     * @see setTypedText
     */
    virtual TypedText getTypedText() const;

    /**
     * @fn virtual void TextProgress::setColor(colortype color);
     *
     * @brief Sets the color.
     *
     *        Sets the color of the text in the used text area.
     *
     * @param color The color.
     *
     * @see getColor
     * @see TextArea::setColor
     */
    virtual void setColor(colortype color);

    /**
     * @fn virtual colortype TextProgress::getColor() const;
     *
     * @brief Gets the color.
     *
     *        Gets the color of the text in the used text area.
     *
     * @return The color.
     */
    virtual colortype getColor() const;

    /**
     * @fn virtual void TextProgress::setAlpha(uint8_t alpha);
     *
     * @brief Sets the alpha.
     *
     *        Sets the alpha of the text area.
     *
     * @param alpha The alpha.
     *
     * @see getAlpha
     * @see TextArea::setAlpha
     */
    virtual void setAlpha(uint8_t alpha);

    /**
     * @fn virtual uint8_t TextProgress::getAlpha() const;
     *
     * @brief Gets the alpha.
     *
     *        Gets the alpha of the text area.
     *
     * @return The alpha.
     *
     * @see setAlpha
     * @see TextArea::getAlpha
     */
    virtual uint8_t getAlpha() const;

    virtual void setValue(int value);

    /**
     * @fn virtual void TextProgress::setNumberOfDecimals(uint16_t numberOfDecimals);
     *
     * @brief Sets number of decimals.
     *
     *        Sets number of decimals when displaying progress.
     *
     * @param numberOfDecimals Number of decimals. Only up to two decimals is supported.
     *
     * @see getNumberOfDecimals
     */
    virtual void setNumberOfDecimals(uint16_t numberOfDecimals);

    /**
     * @fn virtual uint16_t TextProgress::getNumberOfDecimals() const;
     *
     * @brief Gets number of decimals.
     *
     *        Gets number of decimals.
     *
     * @return The number of decimals.
     *
     * @see setNumberOfDecimals
     */
    virtual uint16_t getNumberOfDecimals() const;

protected:
    TextAreaWithOneWildcard textArea;   ///< The text area
    Unicode::UnicodeChar textBuffer[9]; ///< Room for 100.0000
    uint16_t decimals;                  ///< The number of decimals
};
}

#endif // TEXTPROGRESS_HPP
