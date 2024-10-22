/******************************************************************************
* Copyright (c) 2018(-2023) STMicroelectronics.
* All rights reserved.
*
* This file is part of the TouchGFX 4.22.1 distribution.
*
* This software is licensed under terms that can be found in the LICENSE file in
* the root directory of this software component.
* If no LICENSE file comes with this software, it is provided AS-IS.
*
*******************************************************************************/

#include <touchgfx/Utils.hpp>
#include <touchgfx/containers/scrollers/ScrollList.hpp>

namespace touchgfx
{
ScrollList::ScrollList()
    : ScrollBase(),
      paddingAfterLastItem(0),
      snapping(false),
      windowSize(1)
{
}

void ScrollList::setWidth(int16_t width)
{
    ScrollBase::setWidth(width);
    if (getHorizontal())
    {
        setWindowSize(windowSize);
    }
}

void ScrollList::setHeight(int16_t height)
{
    ScrollBase::setHeight(height);
    if (!getHorizontal())
    {
        setWindowSize(windowSize);
    }
}

void ScrollList::setDrawableSize(int16_t drawableSize, int16_t drawableMargin)
{
    ScrollBase::setDrawableSize(drawableSize, drawableMargin);
    setWindowSize(windowSize);
}

void ScrollList::setDrawables(DrawableListItemsInterface& drawableListItems, GenericCallback<DrawableListItemsInterface*, int16_t, int16_t>& updateDrawableCallback)
{
    stopAnimation();
    numberOfDrawables = drawableListItems.getNumberOfDrawables();
    list.setDrawables(drawableListItems, 0, updateDrawableCallback);
    setOffset(0);
}

void ScrollList::setWindowSize(int16_t items)
{
    if (itemSize > 0)
    {
        const int16_t widgetSize = getHorizontal() ? getWidth() : getHeight();
        const int16_t activeWidgetSize = widgetSize - (distanceBeforeAlignedItem + paddingAfterLastItem);
        const int16_t numberOfVisibleItems = (activeWidgetSize + itemSize / 2) / itemSize; // Round up
        items = MIN(items, numberOfVisibleItems);                                          // No more than numberOfVisibleItems
    }
    windowSize = MAX(1, items); // No less than 1
    animateToPosition(keepOffsetInsideLimits(getOffset(), 0));
}

void ScrollList::setPadding(int16_t paddingBefore, int16_t paddingAfter)
{
    const int32_t currentOffset = getOffset();
    distanceBeforeAlignedItem = paddingBefore;
    paddingAfterLastItem = paddingAfter;
    setOffset(currentOffset);
    list.refreshDrawables();
}

int16_t ScrollList::getPaddingBefore() const
{
    return distanceBeforeAlignedItem;
}

int16_t ScrollList::getPaddingAfter() const
{
    return paddingAfterLastItem;
}

void ScrollList::setSnapping(bool snap)
{
    snapping = snap;
    if (snapping)
    {
        setOffset(getNearestAlignedOffset(getOffset()));
    }
}

bool ScrollList::getSnapping() const
{
    return snapping;
}

int32_t ScrollList::getPositionForItem(int16_t itemIndex)
{
    const int32_t currentOffset = getNormalizedOffset(getOffset());
    if (itemIndex < 0 || itemIndex >= list.getNumberOfItems() || itemSize == 0)
    {
        return currentOffset;
    }
    const int32_t itemOffset = -itemIndex * itemSize;
    // Get the visible size
    const int16_t widgetSize = getHorizontal() ? getWidth() : getHeight();
    const int16_t activeWidgetSize = widgetSize - (distanceBeforeAlignedItem + paddingAfterLastItem);
    if (list.getCircular())
    {
        int32_t offset = currentOffset;
        // Important this is a do-while of visibleSize < itemSize in which case we need to check at least one time
        do
        {
            const int16_t i = (-getNormalizedOffset(offset)) / itemSize; // Item index of first
            if (itemIndex == i)
            {
                return currentOffset;
            }
            offset -= itemSize;
        } while (offset >= currentOffset - (activeWidgetSize - itemSize));
        const int32_t allItemsSize = list.getNumberOfItems() * itemSize;
        // Either scroll left from the first item or right from the last item. Find out which is closest
        int32_t leftScrollDistance = itemOffset - currentOffset;
        const int32_t leftScrollDistance2 = (itemOffset + allItemsSize) - currentOffset;
        const int32_t rightItemOffset = getNormalizedOffset(currentOffset - (activeWidgetSize - itemSize));
        int32_t rightScrollDistance = rightItemOffset - itemOffset;
        const int32_t rightScrollDistance2 = rightItemOffset - (itemOffset - allItemsSize);
        if (abs(leftScrollDistance2) < abs(leftScrollDistance))
        {
            leftScrollDistance = leftScrollDistance2;
        }
        if (abs(rightScrollDistance2) < abs(rightScrollDistance))
        {
            rightScrollDistance = rightScrollDistance2;
        }
        if (abs(rightScrollDistance) < abs(leftScrollDistance))
        {
            return currentOffset - rightScrollDistance;
        }
        return currentOffset + leftScrollDistance;
    }

    if (itemOffset > currentOffset) // First item on screen is higher than the itemIndex. Scroll itemIndex to top position
    {
        return itemOffset;
    }
    const int16_t numberOfVisibleItems = activeWidgetSize / itemSize;
    int32_t itemOffsetAtEnd = itemOffset;
    if (numberOfVisibleItems > 0)
    {
        if (snapping)
        {
            itemOffsetAtEnd = itemOffset + (numberOfVisibleItems - 1) * itemSize;
        }
        else
        {
            itemOffsetAtEnd = itemOffset + activeWidgetSize - itemSize;
        }
    }
    if (itemOffsetAtEnd < currentOffset)
    {
        return itemOffsetAtEnd;
    }
    return currentOffset;
}

void ScrollList::handleClickEvent(const ClickEvent& event)
{
    ScrollBase::handleClickEvent(event);
    if (event.getType() == ClickEvent::PRESSED)
    {
        xClick = event.getX();
        yClick = event.getY();
        initialSwipeOffset = getOffset();

        setOffset(getNearestAlignedOffset(initialSwipeOffset));
        if (itemPressedCallback && itemPressedCallback->isValid())
        {
            const int16_t click = (getHorizontal() ? xClick : yClick);
            int32_t offset = click - getOffset();
            const int32_t listSize = getNumberOfItems() * itemSize;
            if (getCircular())
            {
                offset += listSize;
                offset %= listSize;
            }
            if (offset >= 0 && offset < listSize)
            {
                const int16_t item = offset / itemSize;
                itemPressedCallback->execute(item);
            }
        }
    }
    else if (event.getType() == ClickEvent::RELEASED)
    {
        if (currentAnimationState == NO_ANIMATION)
        {
            // For a tiny drag, start by re-aligning (no animation(!))
            setOffset(getNearestAlignedOffset(getOffset()));
            if (itemSelectedCallback && itemSelectedCallback->isValid())
            {
                const int16_t click = (getHorizontal() ? xClick : yClick);
                int32_t offset = click - getOffset();
                const int32_t listSize = getNumberOfItems() * itemSize;
                if (getCircular())
                {
                    offset += listSize;
                    offset %= listSize;
                }
                else
                {
                    offset -= distanceBeforeAlignedItem;
                }
                if (offset >= 0 && offset < listSize)
                {
                    const int16_t item = offset / itemSize;
                    itemSelectedCallback->execute(item);
                }
            }
        }
        else if (currentAnimationState == ANIMATING_DRAG)
        {
            // click + drag + release. Find best Y to scroll to
            animateToPosition(getNearestAlignedOffset(getOffset()));
        }
    }
}

int32_t ScrollList::getNearestAlignedOffset(int32_t offset) const
{
    if (snapping)
    {
        // ScrollBase implementation will snap
        return ScrollBase::getNearestAlignedOffset(offset);
    }

    return keepOffsetInsideLimits(offset, 0);
}

int32_t ScrollList::keepOffsetInsideLimits(int32_t newOffset, int16_t overShoot) const
{
    if (!getCircular())
    {
        newOffset = MIN(newOffset, overShoot);
        const int maxOffToTheStart = windowSize < getNumberOfItems() ? getNumberOfItems() - windowSize : 0;
        newOffset = MAX(newOffset, -(itemSize * maxOffToTheStart) - overShoot);
    }
    return newOffset;
}
} // namespace touchgfx
