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
#ifndef RADIOBUTTON_HPP
#define RADIOBUTTON_HPP

#include <touchgfx/widgets/AbstractButton.hpp>
#include <touchgfx/Bitmap.hpp>

namespace touchgfx
{
/**
 * @class RadioButton RadioButton.hpp touchgfx/widgets/RadioButton.hpp
 *
 * @brief Radio button with two states.
 *
 *        A radio button consists of four images, one for its not selected and one for
 *        selected. Each of these have an image for a pressed state. RadioButtons can be added
 *        to a RadioButtonGroup which handles the de-selection of radio buttons when a new
 *        selection is made.
 *
 * @see AbstractButton
 */
class RadioButton : public AbstractButton
{
public:

    /**
     * @fn RadioButton::RadioButton()
     *
     * @brief Default constructor.
     *
     *        Default constructor.
     */
    RadioButton() : AbstractButton(), bitmapUnselected(), bitmapUnselectedPressed(), bitmapSelected(), bitmapSelectedPressed(), alpha(255), selected(false), deselectionEnabled(false) { }

    /**
     * @fn virtual RadioButton::~RadioButton()
     *
     * @brief Destructor.
     *
     *        Destructor.
     */
    virtual ~RadioButton() { }

    /**
     * @fn virtual void RadioButton::draw(const Rect& invalidatedArea) const;
     *
     * @brief Draws the given invalidated area.
     *
     *        Draws the given invalidated area.
     *
     * @param invalidatedArea The rectangle to draw, with coordinates relative to this drawable.
     *
     * @see Drawable::draw()
     */
    virtual void draw(const Rect& invalidatedArea) const;

    /**
     * @fn virtual void RadioButton::handleClickEvent(const ClickEvent& event);
     *
     * @brief Updates the current state of the radio button.
     *
     *        Updates the current state of the radio button - pressed or released, selected or
     *        not selected - and invalidates it.
     *
     *        If a transition from the not selected to selected was made, the associated action
     *        is executed and then the Widget is invalidated.
     *
     * @param event Information about the click.
     *
     * @see Drawable::handleClickEvent()
     */
    virtual void handleClickEvent(const ClickEvent& event);

    /**
     * @fn virtual void RadioButton::setBitmaps(const Bitmap& bmpUnselected, const Bitmap& bmpUnselectedPressed, const Bitmap& bmpSelected, const Bitmap& bmpSelectedPressed);
     *
     * @brief Sets the bitmaps used by this button.
     *
     *        Sets the bitmaps used by this button. If no special pressed states are needed
     *        just specify the same bitmap for both pressed and non-pressed bitmaps.
     *
     * @param bmpUnselected        Bitmap to use when button is unselected.
     * @param bmpUnselectedPressed Bitmap to use when button is unselected and pressed.
     * @param bmpSelected          Bitmap to use when button is selected.
     * @param bmpSelectedPressed   Bitmap to use when button is selected and pressed.
     */
    virtual void setBitmaps(const Bitmap& bmpUnselected, const Bitmap& bmpUnselectedPressed, const Bitmap& bmpSelected, const Bitmap& bmpSelectedPressed);

    /**
     * @fn void RadioButton::setDeselectedAction(GenericCallback< const AbstractButton& >& callback)
     *
     * @brief Associates an action to be performed when the AbstractButton is deselected.
     *
     *        Associates an action to be performed when the AbstractButton is deselected.
     *
     * @param callback The callback to be executed. The callback will be given a reference to
     *                 the AbstractButton.
     *
     * @see GenericCallback
     */
    void setDeselectedAction(GenericCallback< const AbstractButton& >& callback)
    {
        deselectedAction = &callback;
    }

    /**
     * @fn virtual Rect RadioButton::getSolidRect() const;
     *
     * @brief Gets solid rectangle.
     *
     *        Gets solid rectangle.
     *
     * @return largest possible solid rect. Delegated to the largest solid rect of the radio
     *         button bitmap(s).
     */
    virtual Rect getSolidRect() const;

    /**
     * @fn void RadioButton::setAlpha(uint8_t alpha)
     *
     * @brief Sets the alpha channel for the image.
     *
     *        Sets the alpha channel for the image.
     *
     * @param alpha The alpha value. 255 = completely solid.
     */
    void setAlpha(uint8_t alpha)
    {
        this->alpha = alpha;
    }

    /**
     * @fn uint8_t RadioButton::getAlpha() const
     *
     * @brief Gets the current alpha value.
     *
     *        Gets the current alpha value.
     *
     * @return The current alpha value.
     */
    uint8_t getAlpha() const
    {
        return alpha;
    }

    /**
     * @fn void RadioButton::setDeselectionEnabled(bool state)
     *
     * @brief States whether or not it is possible to de-select the RadioButton by clicking it.
     *
     *        States whether or not it is possible to de-select the RadioButton by clicking it.
     *
     * @param state true if it should be possible to de-select by click.
     */
    void setDeselectionEnabled(bool state)
    {
        deselectionEnabled = state;
    }

    /**
     * @fn bool RadioButton::getDeselectionEnabled() const
     *
     * @brief Gets the current deselectionEnabled state.
     *
     *        Gets the current deselectionEnabled state.
     *
     * @return The current deselectionEnabled state.
     */
    bool getDeselectionEnabled() const
    {
        return deselectionEnabled;
    }

    /**
     * @fn void RadioButton::setSelected(bool newSelected);
     *
     * @brief Sets the radio buttons selected state.
     *
     *        Sets the radio buttons selected state.
     *
     * @param newSelected the new selected state.
     */
    void setSelected(bool newSelected);

    /**
     * @fn bool RadioButton::getSelected() const
     *
     * @brief Gets the current selected state.
     *
     *        Gets the current selected state.
     *
     * @return The current selected state.
     */
    bool getSelected() const
    {
        return selected;
    }

    /**
     * @fn Bitmap RadioButton::getCurrentlyDisplayedBitmap() const
     *
     * @brief Gets currently displayed bitmap.
     *
     *        Function to obtain the currently displayed bitmap, which depends on the radio
     *        button's pressed and selected state.
     *
     * @return The bitmap currently displayed.
     */
    Bitmap getCurrentlyDisplayedBitmap() const
    {
        return (selected ? (AbstractButton::pressed ?  bitmapSelectedPressed : bitmapSelected) : (AbstractButton::pressed ?  bitmapUnselectedPressed : bitmapUnselected));
    }

    /**
     * @fn virtual uint16_t RadioButton::getType() const
     *
     * @brief For GUI testing only.
     *
     *        For GUI testing only. Returns type of this drawable.
     *
     * @return TYPE_RADIOBUTTON.
     */
    virtual uint16_t getType() const
    {
        return (uint16_t)TYPE_RADIOBUTTON;
    }

protected:
    Bitmap  bitmapUnselected;        ///< The image to display when radio button unselected.
    Bitmap  bitmapUnselectedPressed; ///< The image to display when radio button unselected and pressed.
    Bitmap  bitmapSelected;          ///< The image to display when radio button selected.
    Bitmap  bitmapSelectedPressed;   ///< The image to display when radio button selected and pressed.
    uint8_t alpha;                   ///< The current alpha value. 255 denotes solid, 0 denotes completely transparent.
    bool    selected;                ///< The current selected state.
    bool    deselectionEnabled;      ///< Is de-selecting a selected radio button by clicking it enabled.

    GenericCallback< const AbstractButton& >* deselectedAction; ///< The callback to be executed when this AbstractButton is unselected.
};

} // namespace touchgfx

#endif // RADIOBUTTON_HPP
