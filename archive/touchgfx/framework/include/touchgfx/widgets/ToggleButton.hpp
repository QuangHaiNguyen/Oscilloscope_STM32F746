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
#ifndef TOGGLEBUTTON_HPP
#define TOGGLEBUTTON_HPP

#include <touchgfx/widgets/Button.hpp>

namespace touchgfx
{
/**
 * @class ToggleButton ToggleButton.hpp touchgfx/widgets/ToggleButton.hpp
 *
 * @brief A ToggleButton is a Button specialization that swaps the two bitmaps when clicked.
 *
 *        A ToggleButton is a Button specialization that swaps the two bitmaps when clicked,
 *        such that the previous "pressed" bitmap, now becomes the one displayed when button is
 *        not pressed.
 *
 * @see Button
 */
class ToggleButton : public Button
{
public:

    /**
     * @fn ToggleButton::ToggleButton();
     *
     * @brief Default constructor.
     *
     *        Default constructor.
     */
    ToggleButton();

    /**
     * @fn virtual void ToggleButton::setBitmaps(const Bitmap& bmpReleased, const Bitmap& bmpPressed)
     *
     * @brief Sets the bitmaps.
     *
     *        Sets the bitmaps.
     *
     * @note This specific implementation remembers what bitmap was used as pressed, in order to
     *       support the ability to force the state.
     *
     * @param bmpReleased The bitmap to show in the "normal" state, ie when button is not pressed.
     * @param bmpPressed  The bitmap to show when the button is pressed.
     *
     * @see Button::setBitmaps
     */
    virtual void setBitmaps(const Bitmap& bmpReleased, const Bitmap& bmpPressed)
    {
        originalPressed = bmpPressed;
        Button::setBitmaps(bmpReleased, bmpPressed);
    }

    /**
     * @fn void ToggleButton::forceState(bool activeState);
     *
     * @brief Force the button into a specific state.
     *
     *        Use this function to force the button in one of the two possible states. If
     *        button is forced to the active state, then the pressed bitmap from the last call
     *        to setBitmaps becomes the one displayed when button is not pressed.
     *
     * @param activeState If true, display the bmpPressed bitmap when not pressed. If false display
     *                    the bmpReleased bitmap.
     */
    void forceState(bool activeState);

    /**
     * @fn bool ToggleButton::getState() const
     *
     * @brief Gets the state.
     *
     *        Gets the state.
     *
     * @return true if state is currently active.
     */
    bool getState() const
    {
        return up == originalPressed;
    }

    /**
     * @fn virtual void ToggleButton::handleClickEvent(const ClickEvent& event);
     *
     * @brief Overrides handleClickEvent.
     *
     *        Overrides handleClickEvent in order to swap the bitmaps after being clicked.
     *
     * @param event The event to handle.
     */
    virtual void handleClickEvent(const ClickEvent& event);

    /**
     * @fn virtual uint16_t ToggleButton::getType() const
     *
     * @brief For GUI testing only.
     *
     *        For GUI testing only. Returns type of this drawable.
     *
     * @return TYPE_TOGGLEBUTTON.
     */
    virtual uint16_t getType() const
    {
        return (uint16_t)TYPE_TOGGLEBUTTON;
    }
protected:
    Bitmap originalPressed; ///< Contains the bitmap that was originally being displayed when button is pressed.
};

} // namespace touchgfx

#endif // TOGGLEBUTTON_HPP
