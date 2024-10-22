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
 * @file touchgfx/containers/clock/DigitalClock.hpp
 *
 * Declares the touchgfx::DigitalClock class.
 */
#ifndef TOUCHGFX_DIGITALCLOCK_HPP
#define TOUCHGFX_DIGITALCLOCK_HPP

#include <touchgfx/TypedText.hpp>
#include <touchgfx/containers/clock/AbstractClock.hpp>
#include <touchgfx/hal/Types.hpp>
#include <touchgfx/widgets/TextAreaWithWildcard.hpp>

namespace touchgfx
{
/**
 * A digital clock. Can be set in either 12 or 24 hour mode. Seconds are optional. Width and
 * height must be set manually to match the typography and alignment specified in the
 * text database. The Digital Clock requires a typedText with one wildcard and uses the
 * following characters (not including quotes)
 * "AMP :0123456789" These must be present in the text database with the same typography
 * as the wildcard text. Leading zero for the hour indicator can be enabled/disable by
 * the displayLeadingZeroForHourIndicator method.
 */
class DigitalClock : public AbstractClock
{
public:
    /** Values that represent different display modes. */
    enum DisplayMode
    {
        DISPLAY_12_HOUR_NO_SECONDS, ///< 12 Hour clock. Seconds are not displayed
        DISPLAY_24_HOUR_NO_SECONDS, ///< 24 Hour clock. Seconds are not displayed
        DISPLAY_12_HOUR,            ///< 12 Hour clock. Seconds are displayed
        DISPLAY_24_HOUR             ///< 24 Hour clock. Seconds are displayed
    };

    DigitalClock();

    virtual void setWidth(int16_t width);

    virtual void setHeight(int16_t height);

    /**
     * Adjusts the DigitalClock y coordinate so the text will have its baseline at the
     * specified value. The placements is relative to the specified TypedText so if the
     * TypedText is changed, you have to set the baseline again.
     *
     * @param  baselineY The y coordinate of the baseline of the text.
     *
     * @note that setTypedText must be called prior to setting the baseline.
     */
    virtual void setBaselineY(int16_t baselineY);

    /**
     * Sets the typed text of the DigitalClock. Expects a TypedText with one wildcard and
     * that the following characters are defined for the typography of the TypedText:
     * - 12 hour clock: "AMP :0123456789"
     * - 24 hour clock: ":0123456789"
     *
     * @param  typedText Describes the typed text to use.
     *
     * @note Automatically invalidates the DigitalClock.
     */
    virtual void setTypedText(TypedText typedText);

    /**
     * Sets the color of the text.
     *
     * @param  color The new text color.
     *
     * @note Automatically invalidates the DigitalClock.
     */
    virtual void setColor(colortype color);

    /**
     * Gets the color of the text.
     *
     * @return The color.
     */
    virtual colortype getColor() const;

    /**
     * Sets the display mode to 12/24 hour clock with or without seconds.
     *
     * @param  dm The new display mode.
     *
     * @see DisplayMode, getDisplayMode
     */
    virtual void setDisplayMode(DisplayMode dm)
    {
        displayMode = dm;
    }

    /**
     * Gets the current display mode.
     *
     * @return The display mode.
     *
     * @see DisplayMode, setDisplayMode
     */
    virtual DisplayMode getDisplayMode() const
    {
        return displayMode;
    }

    /**
     * Sets whether to display a leading zero for the hour indicator or not, when the hour
     * value only has one digit. For example 8 can be displayed as "8:"
     * (displayLeadingZero=false) or "08:" (displayLeadingZero=true).
     *
     * Default value for this setting is false.
     *
     * @param  displayLeadingZero true = show leading zero. false = do not show leading zero.
     *
     * @note This does not affect the display of minutes or seconds.
     */
    void displayLeadingZeroForHourIndicator(bool displayLeadingZero);

    /**
     * @copydoc Image::setAlpha
     */
    virtual void setAlpha(uint8_t newAlpha);

    /**
     * @copydoc Image::getAlpha
     */
    virtual uint8_t getAlpha() const;

    /**
     * Gets text width of the currently displayed DigitalClock.
     *
     * @return The text width of the currently displayed DigitalClock.
     */
    virtual uint16_t getTextWidth() const
    {
        return text.getTextWidth();
    }

    virtual void invalidateContent() const
    {
        if (getAlpha() > 0)
        {
            AbstractClock::invalidateContent();
        }
    }

protected:
    static const int BUFFER_SIZE = 12; ///< Buffer size of the wild card, worst case is "12:59:59 AM" (12 chars)

    DisplayMode displayMode;             ///< The current display mode
    bool useLeadingZeroForHourIndicator; ///< Print a leading zero if the hour is less than 10

    TextAreaWithOneWildcard text;             ///< The clock text
    Unicode::UnicodeChar buffer[BUFFER_SIZE]; ///< Wild card buffer for the clock text

    virtual void updateClock();
};

} // namespace touchgfx

#endif // TOUCHGFX_DIGITALCLOCK_HPP
