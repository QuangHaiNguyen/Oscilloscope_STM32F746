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
#ifndef SNAPSHOTWIDGET_HPP
#define SNAPSHOTWIDGET_HPP

#include <touchgfx/widgets/Widget.hpp>

namespace touchgfx
{
/**
 * @class SnapshotWidget SnapshotWidget.hpp touchgfx/widgets/SnapshotWidget.hpp
 *
 * @brief A widget that is able to make a snapshot of the area the SnapshotWidget covers.
 *
 *        A widget that is able to make a snapshot of the area the SnapshotWidget covers. The
 *        SnapshotWidget will show the snapshot captured when it is drawn.
 *
 * @see Widget
 */
class SnapshotWidget : public Widget
{
public:

    /**
     * @fn SnapshotWidget::SnapshotWidget();
     *
     * @brief Default constructor.
     *
     *        Default constructor.
     */
    SnapshotWidget();

    /**
     * @fn virtual SnapshotWidget::~SnapshotWidget();
     *
     * @brief Destructor.
     *
     *        Destructor.
     */
    virtual ~SnapshotWidget();

    /**
     * @fn virtual void SnapshotWidget::draw(const Rect& invalidatedArea) const;
     *
     * @brief Draws the SnapshotWidget.
     *
     *        Draws the SnapshotWidget. It supports partial drawing, so it only redraws the
     *        area described by invalidatedArea.
     *
     * @param invalidatedArea The rectangle to draw, with coordinates relative to this drawable.
     */
    virtual void draw(const Rect& invalidatedArea) const;

    /**
     * @fn virtual Rect SnapshotWidget::getSolidRect() const;
     *
     * @brief Gets solid rectangle.
     *
     *        Gets solid rectangle.
     *
     * @return The solid rectangle.
     */
    virtual Rect getSolidRect() const;

    /**
     * @fn virtual void SnapshotWidget::makeSnapshot();
     *
     * @brief Makes a snapshot of the area the SnapshotWidget currently covers.
     *
     *        Makes a snapshot of the area the SnapshotWidget currently covers. This area is
     *        defined by setting the dimensions and the position of the SnapshotWidget.
     */
    virtual void makeSnapshot();

    /**
     * @fn void SnapshotWidget::setAlpha(const uint8_t a)
     *
     * @brief Sets the alpha value.
     *
     *        Sets the alpha value.
     *
     * @param a The alpha value.
     */
    void setAlpha(const uint8_t a)
    {
        alpha = a;
    }

    /**
     * @fn uint8_t SnapshotWidget::getAlpha() const
     *
     * @brief Gets the current alpha value.
     *
     *        Gets the current alpha value.
     *
     * @return The alpha value.
     */
    uint8_t getAlpha() const
    {
        return alpha;
    }

    /**
     * @fn virtual uint16_t SnapshotWidget::getType() const
     *
     * @brief For GUI testing only.
     *
     *        For GUI testing only. Returns type of this drawable.
     *
     * @return TYPE_SNAPSHOTWIDGET.
     */
    virtual uint16_t getType() const
    {
        return (uint16_t)TYPE_SNAPSHOTWIDGET;
    }

protected:
    uint16_t* fbCopy; ///< Pointer to the animation storage in which the pixel data for this snapshot is stored. Zero if no snapshot has been made.
    uint8_t   alpha;  ///< The alpha with which to draw this snapshot.
};

} // namespace touchgfx

#endif // SNAPSHOTWIDGET_HPP
