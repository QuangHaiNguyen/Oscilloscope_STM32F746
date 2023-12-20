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
#ifndef CLICKLISTENER_HPP
#define CLICKLISTENER_HPP

#include <touchgfx/Callback.hpp>

namespace touchgfx
{
/**
 * @class ClickListener ClickListener.hpp touchgfx/mixins/ClickListener.hpp
 *
 * @brief Mix-in class that extends a class with a click action event.
 *
 *        Mix-in class that extends a class with a click action event that is called when the
 *        class receives a click event.
 *
 * @tparam T specifies the type to extend with the ClickListener behavior.
 */
template<class T>
class ClickListener : public T
{
public:

    /**
     * @fn ClickListener::ClickListener()
     *
     * @brief Default constructor.
     *
     *        Default constructor.
     */
    ClickListener() : T(), clickAction(0)
    {
        T::setTouchable(true);
    }

    /**
     * @fn virtual ClickListener::~ClickListener()
     *
     * @brief Destructor.
     */
    virtual ~ClickListener() { }

    /**
     * @fn virtual void ClickListener::handleClickEvent(const ClickEvent& event)
     *
     * @brief Ensures that the clickEvent is propagated to the super class.
     *
     *         Ensures that the clickEvent is propagated to the super class T and to the
     *         clickAction listener.
     *
     * @param event Information about the click.
     *
     * @see Drawable::handleClickEvent()
     */
    virtual void handleClickEvent(const ClickEvent& event)
    {
        T::handleClickEvent(event);
        if (clickAction && clickAction->isValid())
        {
            clickAction->execute(*this, event);
        }
    }

    /**
     * @fn void ClickListener::setClickAction(GenericCallback< const T&, const ClickEvent& >& callback)
     *
     * @brief Associates an action to be performed when the class T is clicked.
     *
     *        Associates an action to be performed when the class T is clicked.
     *
     * @param callback The callback to be executed. The callback will be given a reference to T.
     *
     * @see GenericCallback
     */
    void setClickAction(GenericCallback< const T&, const ClickEvent& >& callback)
    {
        clickAction = &callback;
    }

protected:
    GenericCallback<const T&, const ClickEvent&>* clickAction;  ///< The callback to be executed when T is clicked
};

} //namespace touchgfx

#endif // CLICKLISTENER_HPP
