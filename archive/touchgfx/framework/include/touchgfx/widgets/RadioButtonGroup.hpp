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
#ifndef RADIOBUTTONGROUP_HPP
#define RADIOBUTTONGROUP_HPP

#include <touchgfx/widgets/RadioButton.hpp>
#include <touchgfx/Callback.hpp>
#include <cassert>

namespace touchgfx
{
/**
 * @class RadioButtonGroup RadioButtonGroup.hpp touchgfx/widgets/RadioButtonGroup.hpp
 *
 * @brief Class for handling a collection of RadioButtons.
 *
 *        Class for handling a collection of RadioButtons. The RadioButtonGroup handles the
 *        de-selection of radio buttons when a new selection occurs. A callback is executed when
 *        a new selection occurs reporting the newly selected RadioButton.
 *
 *        Template class: specify a CAPACITY, that is the number of RadioButtons to store.
 *
 * @tparam CAPACITY Type of the capacity.
 *
 * @see RadioButton
 */
template<uint16_t CAPACITY>
class RadioButtonGroup
{
public:

    /**
     * @fn RadioButtonGroup::RadioButtonGroup() : size(0), radioButtonClicked(this, &RadioButtonGroup::radioButtonClickedHandler), radioButtonUnselected(this, &RadioButtonGroup::radioButtonDeselectedHandler), radioButtonSelectedCallback(0), radioButtonDeselectedCallback(0)
     *
     * @brief Default constructor.
     *
     *        Default constructor.
     */
    RadioButtonGroup() :
        size(0),
        radioButtonClicked(this, &RadioButtonGroup::radioButtonClickedHandler),
        radioButtonUnselected(this, &RadioButtonGroup::radioButtonDeselectedHandler),
        radioButtonSelectedCallback(0),
        radioButtonDeselectedCallback(0)
    {
    }

    /**
     * @fn virtual RadioButtonGroup::~RadioButtonGroup()
     *
     * @brief Destructor.
     *
     *        Destructor.
     */
    virtual ~RadioButtonGroup()
    {
    }

    /**
     * @fn virtual void RadioButtonGroup::add(RadioButton& radioButton)
     *
     * @brief Add the RadioButton to the RadioButtonGroup.
     *
     *        Add the RadioButton to the RadioButtonGroup. Only add as many RadioButtons as the
     *        stated CAPACITY. Checked by an assert.
     *
     * @param [in] radioButton the RadioButton that is to be added.
     */
    virtual void add(RadioButton& radioButton)
    {
        assert(size < CAPACITY && "RadioButtonGroup capacity exceeded!");
        radioButton.setAction(radioButtonClicked);
        radioButton.setDeselectedAction(radioButtonUnselected);
        radioButtons[size++] = &radioButton;
    }

    /**
     * @fn virtual RadioButton* RadioButtonGroup::getRadioButton(uint16_t index) const
     *
     * @brief Gets the RadioButton at the specified index.
     *
     *        Gets the RadioButton at the specified index.
     *
     * @param index the index of the RadioButton to return.
     *
     * @return the RadioButton at the specified index. Returns 0 if illegal index.
     */
    virtual RadioButton* getRadioButton(uint16_t index) const
    {
        return (size > index) ? radioButtons[index] : 0;
    }

    /**
     * @fn virtual int32_t RadioButtonGroup::getSelectedRadioButtonIndex() const
     *
     * @brief Gets the index of the selected RadioButton.
     *
     *        Gets the index of the selected RadioButton.
     *
     * @return the index of the selected RadioButton. Returns -1 if no RadioButton is selected.
     */
    virtual int32_t getSelectedRadioButtonIndex() const
    {
        for (uint16_t i = 0; i < size; i++)
        {
            if (radioButtons[i]->getSelected())
            {
                return i;
            }
        }
        return -1;
    }

    /**
     * @fn virtual RadioButton* RadioButtonGroup::getSelectedRadioButton() const
     *
     * @brief Gets the selected RadioButton.
     *
     *        Gets the selected RadioButton.
     *
     * @return a pointer to the selected RadioButton. Returns 0 if no RadioButton is selected.
     */
    virtual RadioButton* getSelectedRadioButton() const
    {
        int32_t index = getSelectedRadioButtonIndex();
        return (index < 0) ? 0 : getRadioButton(index);
    }

    /**
     * @fn virtual void RadioButtonGroup::setSelected(RadioButton& radioButton)
     *
     * @brief Sets the specified RadioButton to be selected.
     *
     *        Sets the specified RadioButton to be selected and deselects all other. Do not
     *        call before all RadioButtons have been added to the RadioButtonGroup. Will call
     *        the radioButtonSelected callback.
     *
     * @param [in] radioButton the RadioButton to be selected.
     */
    virtual void setSelected(RadioButton& radioButton)
    {
        radioButton.setSelected(true);
        radioButtonClickedHandler(radioButton);
    }

    /**
     * @fn virtual void RadioButtonGroup::setDeselectionEnabled(bool deselectionEnabled)
     *
     * @brief Sets whether or not it is possible to deselect RadioButtons by clicking them when
     *        they are selected.
     *
     *        Sets whether or not it is possible to deselect RadioButtons by clicking them when
     *        they are selected.
     *
     * @param deselectionEnabled true if it should be possible to deselect by click.
     */
    virtual void setDeselectionEnabled(bool deselectionEnabled)
    {
        for (uint16_t i = 0; i < size; i++)
        {
            radioButtons[i]->setDeselectionEnabled(deselectionEnabled);
        }
    }

    /**
     * @fn virtual bool RadioButtonGroup::getDeselectionEnabled() const
     *
     * @brief Gets the current deselectionEnabled state.
     *
     *        Gets the current deselectionEnabled state.
     *
     * @return The current deselectionEnabled state.
     */
    virtual bool getDeselectionEnabled() const
    {
        return (size > 0) ? radioButtons[0]->getDeselectionEnabled() : false;
    }

    /**
     * @fn void RadioButtonGroup::setRadioButtonSelectedHandler(GenericCallback< const AbstractButton& >& callback)
     *
     * @brief Associate an action with a radio button.
     *
     *        Associates an action to be performed when a radio button belonging to this group
     *        is selected.
     *
     * @param callback The callback to be executed. The callback will be given a reference to
     *                 the RadioButton that was selected.
     *
     * @see GenericCallback
     */
    void setRadioButtonSelectedHandler(GenericCallback< const AbstractButton& >& callback)
    {
        radioButtonSelectedCallback = &callback;
    }

    /**
     * @fn void RadioButtonGroup::setRadioButtonDeselectedHandler(GenericCallback< const AbstractButton& >& callback)
     *
     * @brief Associate an action with a radio button.
     *
     *        Associates an action to be performed when a radio button belonging to this group
     *        transition from selected to unselected.
     *
     * @param callback The callback to be executed. The callback will be given a reference to
     *                 the RadioButton that was selected.
     *
     * @see GenericCallback
     */
    void setRadioButtonDeselectedHandler(GenericCallback< const AbstractButton& >& callback)
    {
        radioButtonDeselectedCallback = &callback;
    }

protected:
    RadioButton* radioButtons[CAPACITY]; ///< The list of added RadioButtons.
    uint16_t     size;                   ///< The current number of added RadioButtons.

    Callback<RadioButtonGroup, const AbstractButton& > radioButtonClicked;    ///< Callback that is attached to the RadioButtons.
    Callback<RadioButtonGroup, const AbstractButton& > radioButtonUnselected; ///< Callback that is attached to the RadioButtons.

    GenericCallback<const AbstractButton& >* radioButtonSelectedCallback;   ///< The callback to be executed when a radio button belonging to this group is selected.
    GenericCallback<const AbstractButton& >* radioButtonDeselectedCallback; ///< The callback to be executed when a radio button belonging to this group is deselected.

    /**
     * @fn virtual void RadioButtonGroup::radioButtonClickedHandler(const AbstractButton& radioButton)
     *
     * @brief Handles the event that a RadioButton has been selected.
     *
     *        Handles the event that a RadioButton has been selected. deselects all other
     *        RadioButtons.
     *
     * @param radioButton the RadioButton that has been selected.
     */
    virtual void radioButtonClickedHandler(const AbstractButton& radioButton)
    {
        // Deselect other radio buttons
        for (uint16_t i = 0; i < size; i++)
        {
            if (radioButtons[i] != &radioButton)
            {
                if (radioButtons[i]->getSelected())
                {
                    radioButtons[i]->setSelected(false);
                }
            }
        }

        if (radioButtonSelectedCallback && radioButtonSelectedCallback->isValid())
        {
            radioButtonSelectedCallback->execute(radioButton);
        }
    }

    /**
     * @fn virtual void RadioButtonGroup::radioButtonDeselectedHandler(const AbstractButton& radioButton)
     *
     * @brief Handles the event that a RadioButton has been deselected.
     *
     *        Handles the event that a RadioButton has been deselected.
     *
     * @param radioButton the RadioButton that has been deselected.
     */
    virtual void radioButtonDeselectedHandler(const AbstractButton& radioButton)
    {
        if (radioButtonDeselectedCallback && radioButtonDeselectedCallback->isValid())
        {
            radioButtonDeselectedCallback->execute(radioButton);
        }
    }

};

} // namespace touchgfx

#endif // RADIOBUTTONGROUP_HPP
