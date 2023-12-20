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
#ifndef DRAGGABLE_HPP
#define DRAGGABLE_HPP

#include <touchgfx/Callback.hpp>

namespace touchgfx
{
/**
 * @class Draggable Draggable.hpp touchgfx/mixins/Draggable.hpp
 *
 * @brief Mix-in class that extends a class to become draggable.
 *
 *        Mix-in class that extends a class to become draggable.
 *
 * @tparam T specifies the type to extend with the draggable behavior.
 */
template<class T>
class Draggable : public T
{
public:

    /**
     * @fn Draggable::Draggable()
     *
     * @brief Default constructor.
     *
     *        Default constructor.
     */
    Draggable() : T()
    {
        T::setTouchable(true);
    }

    /**
     * @fn virtual Draggable::~Draggable()
     *
     * @brief Destructor.
     *
     *        Destructor.
     */
    virtual ~Draggable() { }

    /**
     * @fn virtual void Draggable::handleDragEvent(const DragEvent& evt)
     *
     * @brief Called when dragging the draggable object.
     *
     *        Called when dragging the draggable object. The object is moved according to the
     *        drag event.
     *
     * @param evt The drag event.
     */
    virtual void handleDragEvent(const DragEvent& evt)
    {
        T::handleDragEvent(evt);
        T::moveRelative(evt.getDeltaX(), evt.getDeltaY());
    }
};

} //namespace touchgfx
#endif // DRAGGABLE_HPP
