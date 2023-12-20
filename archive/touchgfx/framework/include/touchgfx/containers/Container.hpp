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
#ifndef CONTAINER_HPP
#define CONTAINER_HPP

#include <touchgfx/Drawable.hpp>
#include <touchgfx/Callback.hpp>

namespace touchgfx
{
/**
 * @class Container Container.hpp touchgfx/containers/Container.hpp
 *
 * @brief A Container is a Drawable that can have child nodes.
 *
 *        A Container is a Drawable that can have child nodes. The z-order of children is
 *        determined by the order in which Drawables are added to the container - the Drawable
 *        added last will be front-most on the screen.
 *
 *        This class overrides a few functions in Drawable in order to traverse child nodes.
 *
 *        Note that containers act as view ports - that is, only the parts of children that
 *        intersect with the geometry of the container will be visible (e.g. setting a
 *        container's width to 0 will render all children invisible).
 *
 * @see Drawable
 */
class Container : public Drawable
{
public:

    /**
     * @fn Container::Container()
     *
     * @brief Default constructor.
     *
     *        Default constructor.
     */
    Container() : Drawable(),
        firstChild(0)
    {
    }

    /**
     * @fn virtual Container::~Container()
     *
     * @brief Destructor.
     *
     *        Destructor.
     */
    virtual ~Container() { }

    /**
     * @fn virtual void Container::add(Drawable& d);
     *
     * @brief Adds a Drawable instance as child to this Container.
     *
     *        Adds a Drawable instance as child to this Container.
     *
     * @note Never add a drawable more than once (will loop forever)!
     *
     * @param [in] d The Drawable to add.
     */
    virtual void add(Drawable& d);

    /**
     * @fn virtual void Container::remove(Drawable& d);
     *
     * @brief Removes a Drawable instance from the list of children.
     *
     *        Removes a Drawable instance from the list of children.
     *
     * @note This is safe to call even if d is not a child (in which case nothing happens).
     *
     * @param [in] d The Drawable to remove.
     */
    virtual void remove(Drawable& d);

    /**
     * @fn virtual void Container::removeAll();
     *
     * @brief Removes all children by resetting their parent and sibling pointers.
     *
     *        Removes all children by resetting their parent and sibling pointers.
     */
    virtual void removeAll();

    /**
     * @fn virtual void Container::unlink();
     *
     * @brief Removes all children by unlinking the first child.
     *
     *        Removes all children by unlinking the first child. The parent and sibling
     *        pointers of the children are not reset.
     */
    virtual void unlink();

    /**
     * @fn virtual bool Container::contains(const Drawable& d);
     *
     * @brief Query if this object contains the given drawable.
     *
     *        Query if this object contains the given drawable.
     *
     * @param d The Drawable to look for.
     *
     * @return True if the specified Drawable instance is direct child of this container, false
     *         otherwise.
     */
    virtual bool contains(const Drawable& d);

    /**
     * @fn virtual void Container::insert(Drawable* previous, Drawable& d);
     *
     * @brief Inserts a Drawable instance after the specified child node.
     *
     *        Inserts a Drawable instance after the specified child node.
     *
     * @note As with add, do not add the same drawable twice.
     *
     * @param [in] previous The Drawable to look foor.
     * @param [in] d        The Drawable to insert.
     */
    virtual void insert(Drawable* previous, Drawable& d);

    /**
     * @fn virtual void Container::getLastChild(int16_t x, int16_t y, Drawable** last);
     *
     * @brief Gets the last child of this container.
     *
     *        Gets the last (=highest Z-order) child of this container that is enabled, visible
     *        and intersects with the specified point. Recursive function.
     *
     * @param x          The x coordinate of the intersection.
     * @param y          The y coordinate of the intersection.
     * @param [out] last out parameter in which the result is placed.
     *
     * @see Drawable::getLastChild
     */
    virtual void getLastChild(int16_t x, int16_t y, Drawable** last);

    /**
     * @fn virtual void Container::draw(const Rect& invalidatedArea) const;
     *
     * @brief Draws the children of this container.
     *
     *        Draws the children of this container. Only used when JSMOC is disabled.
     *
     * @param invalidatedArea The area to draw.
     */
    virtual void draw(const Rect& invalidatedArea) const;

    /**
     * @fn virtual Rect Container::getSolidRect() const;
     *
     * @brief Gets solid rectangle of a Container.
     *
     *        Gets solid rectangle of a Container. JSMOC does not operate directly on
     *        containers.
     *
     * @return An empty rectangle per default.
     */
    virtual Rect getSolidRect() const;

    /**
     * @fn virtual void Container::forEachChild(GenericCallback<Drawable&>* function);
     *
     * @brief Calls the specified function for each child in the container.
     *
     *        Calls the specified function for each child in the container. Function to call
     *        must have the following prototype: void T::func(Drawable&amp;)
     *
     * @param [in] function The function to be executed for each child.
     *
     * @see ListLayout::insert for a usage example.
     */
    virtual void forEachChild(GenericCallback<Drawable&>* function);

    /**
     * @fn virtual uint16_t Container::getType() const
     *
     * @brief For GUI testing only.
     *
     *        For GUI testing only. Returns type of this drawable.
     *
     * @return TYPE_CONTAINER.
     */
    virtual uint16_t getType() const
    {
        return (uint16_t)TYPE_CONTAINER;
    }

protected:

    /**
     * @fn virtual Rect Container::getContainedArea() const;
     *
     * @brief Gets a rectangle describing the total area covered by the children of this container.
     *
     *        Gets a rectangle describing the total area covered by the children of this
     *        container.
     *
     * @return Rectangle covering all children.
     */
    virtual Rect getContainedArea() const;

    /**
     * @fn virtual void Container::moveChildrenRelative(int16_t deltaX, int16_t deltaY);
     *
     * @brief Calls moveRelative on all children.
     *
     *        Calls moveRelative on all children.
     *
     * @param deltaX Horizontal displacement.
     * @param deltaY Vertical displacement.
     */
    virtual void moveChildrenRelative(int16_t deltaX, int16_t deltaY);

    Drawable* firstChild; ///< Pointer to the first child of this container. Subsequent children can be found through firstChild->nextSibling.

    friend class Screen;
    virtual void setupDrawChain(const Rect& invalidatedArea, Drawable** nextPreviousElement);
};

} // namespace touchgfx
#endif // CONTAINER_HPP
