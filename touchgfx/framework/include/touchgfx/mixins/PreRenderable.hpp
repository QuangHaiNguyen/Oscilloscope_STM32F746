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
#ifndef PRERENDERABLE_HPP
#define PRERENDERABLE_HPP

#include <touchgfx/Drawable.hpp>
#include <touchgfx/hal/HAL.hpp>

namespace touchgfx
{
/**
 * @class PreRenderable PreRenderable.hpp touchgfx/mixins/PreRenderable.hpp
 *
 * @brief This mixin can be used on any Drawable.
 *
 *        This mixin can be used on any Drawable. It provides a preRender function, which will
 *        cache the current visual appearance of the Drawable to be cache in a memory region.
 *        Subsequent calls to draw() on this Drawable will result in a simple memcpy of the
 *        cached memory instead of the normal draw operation. This mixin can therefore be used
 *        on Drawables whose visual appearance is static and the normal draw operation takes a
 *        long time to compute.
 *
 * @note The actual uses of this mixin are rare, and the class is mainly provided for example
 *       purposes.
 *
 * @tparam T The type of Drawable to add this functionality to.
 */
template <class T>
class PreRenderable : public T
{
public:

    /**
     * @fn PreRenderable::PreRenderable()
     *
     * @brief Default constructor.
     *
     *        Default constructor. Initializes the PreRenderable.
     */
    PreRenderable() : preRenderedAddress(0) { }

    /**
     * @fn void PreRenderable::draw(const Rect& invalidatedArea) const
     *
     * @brief Overrides the draw function.
     *
     *        Overrides the draw function. If preRender has been called, perform a memcpy of
     *        the cached version. If not, just call the base class version of draw.
     *
     * @param invalidatedArea The subregion of this Drawable which needs to be redrawn.
     */
    void draw(const Rect& invalidatedArea) const
    {
        if (isPreRendered())
        {
            Rect dirty = invalidatedArea;
            Rect meAbs = T::getRect();

            uint16_t width = T::rect.width;
            int cols = dirty.width;
            int rows = dirty.height;
            int offsetPos = dirty.x + width * dirty.y;
            uint16_t* src = (uint16_t*)preRenderedAddress;

            HAL::getInstance()->blitCopy(src + offsetPos, meAbs.x + dirty.x, meAbs.y + dirty.y, cols, rows, width, 255, false);
        }
        else
        {
            T::draw(invalidatedArea);
        }
    }

    /**
     * @fn virtual void PreRenderable::setupDrawChain(const Rect& invalidatedArea, Drawable** nextPreviousElement)
     *
     * @brief Add to draw chain.
     *
     * @note For TouchGFX internal use only.
     *
     * @param invalidatedArea              Include drawables that intersect with this area only.
     * @param [in,out] nextPreviousElement Modifiable element in linked list.
     */
    virtual void setupDrawChain(const Rect& invalidatedArea, Drawable** nextPreviousElement)
    {
        T::resetDrawChainCache();
        if (isPreRendered())
        {
            if (!T::isVisible())
            {
                return;
            }
            T::nextDrawChainElement = *nextPreviousElement;
            *nextPreviousElement = this;
        }
        else
        {
            T::setupDrawChain(invalidatedArea, nextPreviousElement);
        }
    }

    /**
     * @fn bool PreRenderable::isPreRendered() const
     *
     * @brief Whether or not the snapshot of the widget been taken.
     *
     * @return Is the widget rendered.
     */
    bool isPreRendered() const
    {
        return preRenderedAddress != 0;
    }

    /**
     * @fn void PreRenderable::preRender()
     *
     * @brief Takes a snapshot of the current visual appearance of this widget.
     *
     *        Takes a snapshot of the current visual appearance of this widget. All subsequent
     *        calls to draw on this mixin will result in the snapshot being draw.
     */
    void preRender()
    {
        if (HAL::getInstance()->getBlitCaps() & BLIT_OP_COPY)
        {
            Rect meAbs = T::getRect();
            T::translateRectToAbsolute(meAbs);
            preRenderedAddress = HAL::getInstance()->copyFBRegionToMemory(meAbs);
        }
    }
private:
    uint16_t* preRenderedAddress;
};

} // namespace touchgfx
#endif // PRERENDERABLE_HPP
