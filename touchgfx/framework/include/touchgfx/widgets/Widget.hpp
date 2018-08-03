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
#ifndef WIDGET_HPP
#define WIDGET_HPP

#include <assert.h>
#include <touchgfx/Drawable.hpp>
namespace touchgfx
{
/**
 * @class Widget Widget.hpp touchgfx/widgets/Widget.hpp
 *
 * @brief A Widget is a Drawable leaf (i.e. not a container).
 *
 *        A Widget is a Drawable leaf (i.e. not a container). It does not currently contain any
 *        implementation code, since the Drawable base class handles everything related to leaf
 *        nodes. Extend this when implementing custom widgets.
 *
 * @see Drawable
 */
class Widget : public Drawable
{
public:
    /**
     * @fn Widget::Widget()
     *
     * @brief Default constructor.
     *
     *        Default constructor.
     */
    Widget() : Drawable() { }

    /**
     * @fn virtual Widget::~Widget()
     *
     * @brief Destructor.
     *
     *        Destructor.
     */
    virtual ~Widget() { }

    /**
     * @fn virtual void Widget::getLastChild(int16_t x, int16_t y, Drawable** last)
     *
     * @brief Function for obtaining the the last child of this widget that intersects with the
     *        specified point.
     *
     *        Function for obtaining the the last child of this widget that intersects with the
     *        specified point. Used in input event handling for obtaining the appropriate drawable
     *        that should receive the event. Note that input events must be delegated to the last
     *        drawable of the tree (meaning highest z-order / front-most drawable).
     *
     *        Only containers can have children, so this implementation simply yields itself as
     *        result. The container implementation will filter children that do not intersect with
     *        the point or are not visible/enabled, so performing those checks are unnecessary.
     *
     * @param x          The point of intersection expressed in coordinates relative to the parent.
     * @param y          The y coordinate.
     * @param [out] last Result will be placed here.
     */
    virtual void getLastChild(int16_t x, int16_t y, Drawable** last)
    {
        if (isVisible() && isTouchable())
        {
            *last = this;
        }
    }

    /**
     * @fn virtual uint16_t Widget::getType() const
     *
     * @brief For GUI testing only.
     *
     *        For GUI testing only. Returns type of this drawable.
     *
     * @return TYPE_WIDGET.
     */
    virtual uint16_t getType() const
    {
        return (uint16_t)TYPE_WIDGET;
    }
};

} // namespace touchgfx

#endif // WIDGET_HPP
