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
#ifndef SLIDETRANSITION_HPP
#define SLIDETRANSITION_HPP

#include <touchgfx/hal/HAL.hpp>
#include <touchgfx/containers/Container.hpp>
#include <touchgfx/transitions/Transition.hpp>
#include <touchgfx/widgets/SnapshotWidget.hpp>
#include <touchgfx/hal/Types.hpp>
#include <touchgfx/EasingEquations.hpp>

namespace touchgfx
{

class Container;

/**
 * @class SlideTransition SlideTransition.hpp touchgfx/transitions/SlideTransition.hpp
 *
 * @brief A Transition that slides from one screen to the next.
 *
 *        A Transition that slides from one screen to the next. It does so by moving a
 *        SnapShotWidget with a snapshot of the Screen transitioning away from, and by moving
 *        the contents of Screen transitioning to.
 *
 * @tparam templateDirection Type of the template direction.
 *
 * @see Transition
 */
template <Direction templateDirection>
class SlideTransition : public Transition
{
public:

    /**
     * @fn SlideTransition::SlideTransition(const uint8_t transitionSteps = 20) : Transition(), snapshot(), snapshotPtr(&snapshot), handleTickCallback(this, &SlideTransition::tickMoveDrawable), direction(templateDirection), animationSteps(transitionSteps), animationCounter(0), calculatedValue(0)
     *
     * @brief Constructor.
     *
     *        Constructor.
     *
     * @param transitionSteps Number of steps in the transition animation.
     */
    SlideTransition(const uint8_t transitionSteps = 20)
        : Transition(),
          snapshot(),
          snapshotPtr(&snapshot),
          handleTickCallback(this, &SlideTransition::tickMoveDrawable),
          direction(templateDirection),
          animationSteps(transitionSteps),
          animationCounter(0),
          calculatedValue(0)
    {
        if (HAL::USE_ANIMATION_STORAGE)
        {
            snapshot.setPosition(0, 0, HAL::DISPLAY_WIDTH, HAL::DISPLAY_HEIGHT);
            snapshot.makeSnapshot();

            switch (direction)
            {
            case EAST:
                targetValue = -HAL::DISPLAY_WIDTH;
                break;
            case WEST:
                targetValue = HAL::DISPLAY_WIDTH;
                break;
            case NORTH:
                targetValue = HAL::DISPLAY_HEIGHT;
                break;
            case SOUTH:
                targetValue = -HAL::DISPLAY_HEIGHT;
                break;
            default:
                done = true;
                // Nothing to do here
                break;
            }
        }
    }

    /**
     * @fn virtual SlideTransition::~SlideTransition()
     *
     * @brief Destructor.
     *
     *        Destructor.
     */
    virtual ~SlideTransition()
    {
    }

    /**
     * @fn virtual void SlideTransition::handleTickEvent()
     *
     * @brief Handles the tick event when transitioning.
     *
     *        Handles the tick event when transitioning. It moves the contents of the Screen's
     *        container and a SnapshotWidget with a snapshot of the previous Screen. The
     *        direction of the transition determines the direction the contents of the
     *        container and the SnapshotWidget moves.
     */
    virtual void handleTickEvent()
    {
        if (!HAL::USE_ANIMATION_STORAGE)
        {
            done = true;
            return;
        }

        Transition::handleTickEvent();

        // Calculate new position or stop animation
        animationCounter++;
        if (animationCounter <= animationSteps)
        {
            // Calculate value in [0;targetValue]
            calculatedValue = EasingEquations::cubicEaseOut(animationCounter, 0, targetValue, animationSteps);

            // Note: Result of "calculatedValue & 1" is compiler dependent for negative values of calculatedValue
            if (calculatedValue % 2)
            {
                // Optimization: calculatedValue is odd, add 1/-1 to move drawables modulo 32 bits in framebuffer
                calculatedValue += (calculatedValue > 0 ? 1 : -1);
            }
        }
        else
        {
            // Final step: stop the animation
            done = true;
            animationCounter = 0;
            return;
        }

        // Move snapshot
        switch (direction)
        {
        case EAST:
        case WEST:
            // Convert to delta value relative to current X
            calculatedValue -= snapshot.getX();
            snapshot.moveRelative(calculatedValue, 0);
            break;
        case NORTH:
        case SOUTH:
            // Convert to delta value relative to current Y
            calculatedValue -= snapshot.getY();
            snapshot.moveRelative(0, calculatedValue);
            break;
        default:
            done = true;
            // Nothing to do here
            break;
        }

        // Move children with delta value for X or Y
        screenContainer->forEachChild(&handleTickCallback);
    }

    /**
     * @fn virtual void SlideTransition::tearDown()
     *
     * @brief Tear down.
     *
     *        Tear down.
     *
     * @see Transition::teadDown()
     */
    virtual void tearDown()
    {
        if (HAL::USE_ANIMATION_STORAGE)
        {
            screenContainer->remove(snapshot);
        }
    }

    /**
     * @fn virtual void SlideTransition::init()
     *
     * @brief Initializes this object.
     *
     *        Initializes this object.
     *
     * @see Transition::init()
     */
    virtual void init()
    {
        if (HAL::USE_ANIMATION_STORAGE)
        {
            Transition::init();

            Callback<SlideTransition, Drawable&> initCallback(this, &SlideTransition::initMoveDrawable);
            screenContainer->forEachChild(&initCallback);

            screenContainer->add(snapshot);
        }
    }

protected:

    /**
     * @fn virtual void SlideTransition::initMoveDrawable(Drawable& d)
     *
     * @brief Moves the Drawable to its initial position.
     *
     *        Moves the Drawable to its initial position.
     *
     * @param [in] d The Drawable to move.
     */
    virtual void initMoveDrawable(Drawable& d)
    {
        switch (direction)
        {
        case EAST:
            d.moveRelative(HAL::DISPLAY_WIDTH, 0);
            break;
        case WEST:
            d.moveRelative(-HAL::DISPLAY_WIDTH, 0);
            break;
        case NORTH:
            d.moveRelative(0, -HAL::DISPLAY_HEIGHT);
            break;
        case SOUTH:
            d.moveRelative(0, HAL::DISPLAY_HEIGHT);
            break;
        default:
            // Nothing to do here
            break;
        }
    }

    /**
     * @fn virtual void SlideTransition::tickMoveDrawable(Drawable& d)
     *
     * @brief Moves the Drawable.
     *
     *        Moves the Drawable.
     *
     * @param [in] d The Drawable to move.
     */
    virtual void tickMoveDrawable(Drawable& d)
    {
        if (&d == snapshotPtr)
        {
            return;
        }

        switch (direction)
        {
        case EAST:
        case WEST:
            d.moveRelative(calculatedValue, 0);
            break;
        case NORTH:
        case SOUTH:
            d.moveRelative(0, calculatedValue);
            break;
        default:
            // Special case, do not move. Class NoTransition can be used instead.
            done = true;
            break;
        }
    }

    SnapshotWidget  snapshot;    ///< The SnapshotWidget that is moved when transitioning.
    SnapshotWidget* snapshotPtr; ///< Pointer pointing to the snapshot used in this transition.The snapshot pointer

private:
    Callback<SlideTransition, Drawable&> handleTickCallback;    ///< Callback used for tickMoveDrawable().

    Direction     direction;        ///< The direction of the transition.
    const uint8_t animationSteps;   ///< Number of steps the transition should move per complete animation.
    uint8_t       animationCounter; ///< Current step in the transition animation.
    int16_t       targetValue;      ///< The target value for the transition animation.
    int16_t       calculatedValue;  ///< The calculated X or Y value for the snapshot and the children.
};

} // namespace touchgfx
#endif // SLIDETRANSITION_HPP
