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
#ifndef SNAPPER_HPP
#define SNAPPER_HPP

#include <touchgfx/hal/Types.hpp>
#include <touchgfx/Callback.hpp>
#include <touchgfx/Drawable.hpp>
#include <touchgfx/mixins/Draggable.hpp>

namespace touchgfx
{
/**
 * @class Snapper Snapper.hpp touchgfx/mixins/Snapper.hpp
 *
 * @brief A mix-in that will make class T draggable and able to snap to a position.
 *
 *        A mix-in that will make class T draggable and able to snap to a position when a drag
 *        operation has ended. The mix-in is able to perform callbacks when the snapper gets
 *        dragged and when the Snapper snaps to its snap position.
 *
 * @tparam T specifies the type to enable the Snap behavior to.
 *
 * @see Draggable&lt;T&gt;
 */
template <class T>
class Snapper : public Draggable<T>
{

public:

    /**
     * @fn Snapper::Snapper()
     *
     * @brief Default constructor.
     *
     *        Default constructor.
     */
    Snapper() : Draggable<T>(), snapPosition(0, 0), dragAction(0), snappedAction(0)
    {
    }

    /**
     * @fn virtual Snapper::~Snapper()
     *
     * @brief Destructor.
     *
     *        Destructor.
     */
    virtual ~Snapper()
    {
    }

    /**
     * @fn virtual void Snapper::handleDragEvent(const DragEvent& evt)
     *
     * @brief Called when dragging the Snapper.
     *
     *        Called when dragging the Snapper. It will delegate the event if a GenericCallback
     *        is set with setDragAction.
     *
     * @param evt The drag event.
     */
    virtual void handleDragEvent(const DragEvent& evt)
    {
        Draggable<T>::handleDragEvent(evt);

        if (dragAction && dragAction->isValid())
        {
            dragAction->execute(evt);
        }
    }

    /**
     * @fn virtual void Snapper::handleClickEvent(const ClickEvent& evt)
     *
     * @brief Handles the click events when the Snapper is clicked.
     *
     *         Handles the click events when the Snapper is clicked. It saves its current
     *         position as the snap position if the Snapper is pressed. This happens when the
     *         drag operation starts.
     *
     *         The snapper will then move to the snap position when the click is released. This
     *         happens when the drag operation ends.
     *
     * @param evt The click event.
     */
    virtual void handleClickEvent(const ClickEvent& evt)
    {
        T::handleClickEvent(evt);

        if (evt.getType() == ClickEvent::RELEASED)
        {
            if (snappedAction && snappedAction->isValid())
            {
                snappedAction->execute();
            }

            T::moveTo(snapPosition.first, snapPosition.second);
        }
        else if (evt.getType() == ClickEvent::PRESSED)
        {
            snapPosition.first = T::getX();
            snapPosition.second = T::getY();
        }
    }

    /**
     * @fn void Snapper::setSnapPosition(int16_t x, int16_t y)
     *
     * @brief Sets the position the Snapper should snap to.
     *
     *        Sets the position the Snapper should snap to. This position will be overridden
     *        with the Snappers current position when the Snapper is pressed.
     *
     * @param x The x coordinate.
     * @param y The y coordinate.
     */
    void setSnapPosition(int16_t x, int16_t y)
    {
        snapPosition.first = x;
        snapPosition.second = y;
    }

    /**
     * @fn void Snapper::setDragAction(GenericCallback< const DragEvent& >& callback)
     *
     * @brief Associates an action to be performed when the Snapper is dragged.
     *
     *        Associates an action to be performed when the Snapper is dragged.
     *
     * @param callback The callback will be executed with the DragEvent.
     *
     * @see GenericCallback
     */
    void setDragAction(GenericCallback< const DragEvent& >& callback)
    {
        dragAction = &callback;
    }

    /**
     * @fn void Snapper::setSnappedAction(GenericCallback<>& callback)
     *
     * @brief Associates an action to be performed when the Snapper is snapped.
     *
     *        Associates an action to be performed when the Snapper is snapped.
     *
     * @param [in] callback The callback to be executed on snap.
     *
     * @see GenericCallback
     */
    void setSnappedAction(GenericCallback<>& callback)
    {
        snappedAction = &callback;
    }

private:
    Pair<int16_t, int16_t> snapPosition;
    GenericCallback<const DragEvent&>* dragAction;
    GenericCallback<>* snappedAction;
};

} //namespace touchgfx

#endif // SNAPPER_HPP
