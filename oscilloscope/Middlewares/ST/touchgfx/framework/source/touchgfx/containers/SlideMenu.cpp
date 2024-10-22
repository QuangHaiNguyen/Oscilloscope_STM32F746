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

#include <touchgfx/Application.hpp>
#include <touchgfx/containers/SlideMenu.hpp>

namespace touchgfx
{
SlideMenu::SlideMenu()
    : Container(),
      menuContainer(),
      stateChangeButton(),
      background(),
      onStateChangeButtonClicked(this, &SlideMenu::stateChangeButtonClickedHandler),
      animationEndedCallback(this, &SlideMenu::animationEndedHandler),
      stateChangedCallback(0),
      stateChangedAnimationEndedCallback(0),
      currentState(COLLAPSED),
      expandDirection(EAST),
      animationEquation(EasingEquations::cubicEaseInOut),
      visiblePixelsWhenCollapsed(0),
      hiddenPixelsWhenExpanded(0),
      expandedStateTimeout(200),
      expandedStateTimer(0),
      animationDuration(10)
{
    Application::getInstance()->registerTimerWidget(this);

    stateChangeButton.setAction(onStateChangeButtonClicked);
    menuContainer.setMoveAnimationEndedAction(animationEndedCallback);

    Container::add(menuContainer);
    menuContainer.add(background);
    menuContainer.add(stateChangeButton);
}

SlideMenu::~SlideMenu()
{
    Application::getInstance()->unregisterTimerWidget(this);
}

void SlideMenu::setup(ExpandDirection newExpandDirection, const Bitmap& backgroundBMP, const Bitmap& stateChangeButtonBMP, const Bitmap& stateChangeButtonPressedBMP)
{
    int16_t backgroundX = 0;
    int16_t backgroundY = 0;
    int16_t buttonX = 0;
    int16_t buttonY = 0;

    switch (newExpandDirection)
    {
    case SOUTH:
        backgroundX = 0;
        backgroundY = 0;
        buttonX = (backgroundBMP.getWidth() - stateChangeButtonBMP.getWidth()) / 2;
        buttonY = backgroundBMP.getHeight();

        setVisiblePixelsWhenCollapsed(stateChangeButtonBMP.getHeight());
        break;
    case NORTH:
        backgroundX = 0;
        backgroundY = stateChangeButtonBMP.getHeight();
        buttonX = (backgroundBMP.getWidth() - stateChangeButtonBMP.getWidth()) / 2;
        buttonY = 0;

        setVisiblePixelsWhenCollapsed(stateChangeButtonBMP.getHeight());
        break;
    case EAST:
        backgroundX = 0;
        backgroundY = 0;
        buttonX = backgroundBMP.getWidth();
        buttonY = (backgroundBMP.getHeight() - stateChangeButtonBMP.getHeight()) / 2;

        setVisiblePixelsWhenCollapsed(stateChangeButtonBMP.getWidth());
        break;
    case WEST:
        backgroundX = stateChangeButtonBMP.getWidth();
        backgroundY = 0;
        buttonX = 0;
        buttonY = (backgroundBMP.getHeight() - stateChangeButtonBMP.getHeight()) / 2;

        setVisiblePixelsWhenCollapsed(stateChangeButtonBMP.getWidth());
        break;
    }

    setup(newExpandDirection, backgroundBMP, stateChangeButtonBMP, stateChangeButtonPressedBMP, backgroundX, backgroundY, buttonX, buttonY);
}

void SlideMenu::setup(ExpandDirection newExpandDirection, const Bitmap& backgroundBMP, int16_t backgroundX, int16_t backgroundY)
{
    setExpandDirection(newExpandDirection);

    background.setBitmap(backgroundBMP);
    background.setXY(backgroundX, backgroundY);

    const Rect boundingRect = background.getRect();
    // boundingRect.expandToFit(background.getRect());

    menuContainer.setWidth(boundingRect.right());
    menuContainer.setHeight(boundingRect.bottom());

    setWidthHeight(menuContainer);

    setExpandDirection(expandDirection);
    setState(currentState);

    invalidate();
}

void SlideMenu::setup(ExpandDirection newExpandDirection, const Bitmap& backgroundBMP, const Bitmap& stateChangeButtonBMP, const Bitmap& stateChangeButtonPressedBMP, int16_t backgroundX, int16_t backgroundY, int16_t stateChangeButtonX, int16_t stateChangeButtonY)
{
    setExpandDirection(newExpandDirection);

    background.setBitmap(backgroundBMP);
    background.setXY(backgroundX, backgroundY);

    stateChangeButton.setBitmaps(stateChangeButtonBMP, stateChangeButtonPressedBMP);
    stateChangeButton.setXY(stateChangeButtonX, stateChangeButtonY);

    Rect boundingRect;
    boundingRect.expandToFit(background.getRect());
    boundingRect.expandToFit(stateChangeButton.getRect());

    menuContainer.setWidth(boundingRect.right());
    menuContainer.setHeight(boundingRect.bottom());

    setWidthHeight(menuContainer);

    setExpandDirection(expandDirection);
    setState(currentState);

    invalidate();
}

void SlideMenu::setExpandDirection(ExpandDirection newExpandDirection)
{
    expandDirection = newExpandDirection;

    setState(currentState);
}

SlideMenu::ExpandDirection SlideMenu::getExpandDirection() const
{
    return expandDirection;
}

void SlideMenu::setVisiblePixelsWhenCollapsed(int16_t visiblePixels)
{
    visiblePixelsWhenCollapsed = visiblePixels;

    setState(currentState);
}

int16_t SlideMenu::getVisiblePixelsWhenCollapsed() const
{
    return visiblePixelsWhenCollapsed;
}

void SlideMenu::setHiddenPixelsWhenExpanded(int16_t hiddenPixels)
{
    hiddenPixelsWhenExpanded = hiddenPixels;

    setState(currentState);
}

int16_t SlideMenu::getHiddenPixelsWhenExpanded() const
{
    return hiddenPixelsWhenExpanded;
}

void SlideMenu::setExpandedStateTimeout(uint16_t timeout)
{
    expandedStateTimeout = timeout;
}

uint16_t SlideMenu::getExpandedStateTimeout() const
{
    return expandedStateTimeout;
}

void SlideMenu::setAnimationDuration(uint16_t duration)
{
    animationDuration = duration;
}

uint16_t SlideMenu::getAnimationDuration() const
{
    return animationDuration;
}

void SlideMenu::setAnimationEasingEquation(EasingEquation animationEasingEquation)
{
    animationEquation = animationEasingEquation;
}

EasingEquation SlideMenu::getAnimationEasingEquation() const
{
    return animationEquation;
}

void SlideMenu::setState(State newState)
{
    if (newState == COLLAPSED)
    {
        menuContainer.moveTo(getCollapsedXCoordinate(), getCollapsedYCoordinate());
    }
    else
    {
        menuContainer.moveTo(getExpandedXCoordinate(), getExpandedYCoordinate());
    }
    currentState = newState;
}

void SlideMenu::animateToState(State newState)
{
    if (animationDuration == 0)
    {
        setState(newState);
    }
    else if (newState != currentState)
    {
        if (currentState == COLLAPSED)
        {
            menuContainer.startMoveAnimation(getExpandedXCoordinate(), getExpandedYCoordinate(), animationDuration, animationEquation, animationEquation);
            currentState = EXPANDED;
        }
        else
        {
            menuContainer.startMoveAnimation(getCollapsedXCoordinate(), getCollapsedYCoordinate(), animationDuration, animationEquation, animationEquation);
            currentState = COLLAPSED;
        }

        // Disable stateChangeButton while animating
        stateChangeButton.setTouchable(false);
    }
}

SlideMenu::State SlideMenu::getState()
{
    return currentState;
}

void SlideMenu::resetExpandedStateTimer()
{
    expandedStateTimer = 0;
}

uint16_t SlideMenu::getExpandedStateTimer() const
{
    return expandedStateTimer;
}

int16_t SlideMenu::getBackgroundX() const
{
    return background.getX();
}

int16_t SlideMenu::getBackgroundY() const
{
    return background.getY();
}

int16_t SlideMenu::getStateChangeButtonX() const
{
    return stateChangeButton.getX();
}

int16_t SlideMenu::getStateChangeButtonY() const
{
    return stateChangeButton.getY();
}

void SlideMenu::setStateChangedCallback(GenericCallback<const SlideMenu&>& callback)
{
    stateChangedCallback = &callback;
}

void SlideMenu::setStateChangedAnimationEndedCallback(GenericCallback<const SlideMenu&>& callback)
{
    stateChangedAnimationEndedCallback = &callback;
}

void SlideMenu::handleTickEvent()
{
    if ((expandedStateTimeout != 0) && (currentState == EXPANDED) && !menuContainer.isMoveAnimationRunning())
    {
        expandedStateTimer++;

        if (expandedStateTimer > expandedStateTimeout)
        {
            animateToState(COLLAPSED);
        }
    }
}

void SlideMenu::add(Drawable& d)
{
    menuContainer.add(d);
}

void SlideMenu::remove(Drawable& d)
{
    menuContainer.remove(d);
}

void SlideMenu::stateChangeButtonClickedHandler(const AbstractButton& /*button*/)
{
    if (currentState == COLLAPSED)
    {
        animateToState(EXPANDED);
    }
    else
    {
        animateToState(COLLAPSED);
    }

    if ((stateChangedCallback != 0) && stateChangedCallback->isValid())
    {
        stateChangedCallback->execute(*this);
    }
}

void SlideMenu::animationEndedHandler(const MoveAnimator<Container>& /*container*/)
{
    resetExpandedStateTimer();
    stateChangeButton.setTouchable(true);

    if ((stateChangedAnimationEndedCallback != 0) && stateChangedAnimationEndedCallback->isValid())
    {
        stateChangedAnimationEndedCallback->execute(*this);
    }
}

int16_t SlideMenu::getCollapsedXCoordinate()
{
    switch (expandDirection)
    {
    case EAST:
        return -menuContainer.getWidth() + visiblePixelsWhenCollapsed;
    case WEST:
        return getWidth() - visiblePixelsWhenCollapsed;
    case SOUTH:
    case NORTH:
        break;
    }
    return 0;
}

int16_t SlideMenu::getCollapsedYCoordinate()
{
    switch (expandDirection)
    {
    case SOUTH:
        return -menuContainer.getHeight() + visiblePixelsWhenCollapsed;
    case NORTH:
        return getHeight() - visiblePixelsWhenCollapsed;
    case EAST:
    case WEST:
        break;
    }
    return 0;
}

int16_t SlideMenu::getExpandedXCoordinate()
{
    switch (expandDirection)
    {
    case EAST:
        return -hiddenPixelsWhenExpanded;
    case WEST:
        return hiddenPixelsWhenExpanded;
    case SOUTH:
    case NORTH:
        break;
    }
    return 0;
}

int16_t SlideMenu::getExpandedYCoordinate()
{
    switch (expandDirection)
    {
    case SOUTH:
        return -hiddenPixelsWhenExpanded;
    case NORTH:
        return hiddenPixelsWhenExpanded;
    case EAST:
    case WEST:
        break;
    }
    return 0;
}
} // namespace touchgfx
