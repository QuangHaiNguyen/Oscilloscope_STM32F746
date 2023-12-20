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
#ifndef TRANSITION_HPP
#define TRANSITION_HPP

#include <touchgfx/hal/Types.hpp>
#include <touchgfx/lcd/LCD.hpp>

namespace touchgfx
{

class Container;
class SnapshotWidget;

/**
 * @class Transition Transition.hpp touchgfx/transitions/Transition.hpp
 *
 * @brief The Transition class is the base class for Transitions.
 *
 *        The Transition class is the base class for Transitions. Implementations of Transition
 *        defines what happens when transitioning between Screens, which typically involves
 *        visual effects. An example of a transition implementation can be seen in example
 *        custom_transition_example. The most basic transition is the NoTransition class that
 *        does a transition without any visual effects.
 *
 * @see NoTransition
 * @see SlideTransition
 */
class Transition
{
public:

    /**
     * @fn Transition::Transition()
     *
     * @brief Default constructor.
     *
     *        Constructs the Transition.
     */
    Transition() :
        screenContainer(0), done(false)
    {
    }

    /**
     * @fn virtual Transition::~Transition()
     *
     * @brief Destructor.
     *
     *        Destructor.
     */
    virtual ~Transition()
    {
    }

    /**
     * @fn virtual void Transition::handleTickEvent()
     *
     * @brief Called for every tick when transitioning.
     *
     *        Called for every tick when transitioning. Base does nothing.
     */
    virtual void handleTickEvent() { }

    /**
     * @fn bool Transition::isDone() const
     *
     * @brief Query if the transition is done transitioning.
     *
     *        Query if the transition is done transitioning. It is the responsibility of the
     *        inheriting class to set the underlying done flag.
     *
     * @return True if the transition is done, false otherwise.
     */
    bool isDone() const
    {
        return done;
    }

    /**
     * @fn virtual void Transition::tearDown()
     *
     * @brief Tears down the Animation.
     *
     *        Tears down the Animation. Called before the d.tor. when the application changes
     *        the transition. Base version does nothing.
     */
    virtual void tearDown() { }

    /**
     * @fn virtual void Transition::init()
     *
     * @brief Initializes the transition.
     *
     *        Initializes the transition. Called after the c.tor. when the application changes
     *        the transition. Base version does nothing.
     */
    virtual void init() { }

    /**
     * @fn virtual void Transition::setScreenContainer(Container& cont)
     *
     * @brief Sets the screen container.
     *
     *        Sets the screen container. Is used by Screen to enable the transition to access
     *        the container.
     *
     * @param [in] cont The container the transition should have access to.
     */
    virtual void setScreenContainer(Container& cont)
    {
        screenContainer = &cont;
    }

protected:
    Container* screenContainer; ///< The screen Container of the Screen transitioning to.
    bool       done;            ///< Flag that indicates when the transition is done. This should be set by implementing classes.
};

} // namespace touchgfx
#endif // TRANSITION_HPP
