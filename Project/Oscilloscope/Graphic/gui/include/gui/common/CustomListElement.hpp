/******************************************************************************
 *
 * @brief     This file is part of the TouchGFX 4.5.1 evaluation distribution.
 *
 * @author    Draupner Graphics A/S <http://www.touchgfx.com>
 *
 ******************************************************************************
 *
 * @section Copyright
 *
 * This file is free software and is provided for example purposes. You may
 * use, copy, and modify within the terms and conditions of the license
 * agreement.
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
#ifndef CUSTOM_LIST_ELEMENT_HPP
#define CUSTOM_LIST_ELEMENT_HPP

#include <touchgfx/containers/Container.hpp>
#include <touchgfx/widgets/Image.hpp>
#include <touchgfx/widgets/Button.hpp>
#include <touchgfx/Bitmap.hpp>

using namespace touchgfx;

class MainView;

/**
 * CustomListElement is the representation of each entry in our list.
 * The list element contains an image and a button.
 */
class CustomListElement : public Container
{
public:
    CustomListElement();

    /**
     * Initialize this containers widgets
     */
    void setupListElement(const Bitmap& bmp);

    /**
     * Setup the view callback
     */
    void setAction(GenericCallback< CustomListElement& >& callback)
    {
        viewCallback = &callback;
    }

    /**
     * Handler of button click events
     */
    void buttonClicked(const AbstractButton& source);

private:
    Button triggerButton;

    /**
     * Callback that is called when the button is clicked
     */
    Callback<CustomListElement, const AbstractButton&> buttonClickedCallback;

    /**
     * Callback that that notifies the creator of the list element of an event
     */
    GenericCallback< CustomListElement& >* viewCallback;
};

#endif /* CUSTOM_LIST_ELEMENT_HPP */
