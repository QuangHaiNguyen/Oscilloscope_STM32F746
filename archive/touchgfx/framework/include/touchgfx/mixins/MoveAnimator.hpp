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
#ifndef MOVEANIMATOR_HPP
#define MOVEANIMATOR_HPP

#include <touchgfx/Application.hpp>
#include <touchgfx/Callback.hpp>
#include <touchgfx/EasingEquations.hpp>

namespace touchgfx
{
/**
 * @class MoveAnimator MoveAnimator.hpp touchgfx/mixins/MoveAnimator.hpp
 *
 * @brief A MoveAnimator makes the template class T able to animate a movement.
 *
 *        A MoveAnimator makes the template class T able to animate a movement from its current
 *        position to a specified end position. The movement in both the X and Y direction can
 *        be described by supplying EasingEquations. The MoveAnimator performs a callback when
 *        the animation has finished.
 *
 *        This mixin can be used on any Drawable.
 *
 * @tparam T Specifies the type should have the move animation capability.
 */
template<class T>
class MoveAnimator : public T
{
public:

    /**
     * @fn MoveAnimator::MoveAnimator()
     *
     * @brief Default constructor.
     *
     *        Default constructor. Creates and initialize the MoveAnimator.
     */
    MoveAnimator() :
        T(),
        moveAnimationRunning(false),
        moveAnimationCounter(0),
        moveAnimationDelay(0),
        moveAnimationEndedCallback(0)
    {
    }

    /**
     * @fn virtual MoveAnimator::~MoveAnimator()
     *
     * @brief Destructor.
     *
     *        Destructor. Destroys the MoveAnimator.
     */
    virtual ~MoveAnimator()
    {
    }

    /**
     * @fn void MoveAnimator::setMoveAnimationEndedAction(GenericCallback<const MoveAnimator<T>& >& callback)
     *
     * @brief Associates an action to be performed when the animation ends.
     *
     *        Associates an action to be performed when the animation ends.
     *
     * @param callback The callback to be executed. The callback will be given a reference to
     *                 the MoveAnimator.
     *
     * @see GenericCallback
     */
    void setMoveAnimationEndedAction(GenericCallback<const MoveAnimator<T>& >& callback)
    {
        moveAnimationEndedCallback = &callback;
    }

    /**
     * @fn virtual void MoveAnimator::setMoveAnimationDelay(uint16_t delay)
     *
     * @brief Sets a delay on animations done by the MoveAnimator.
     *
     *        Sets a delay on animations done by the MoveAnimator.
     *
     * @param delay The delay in ticks.
     */
    virtual void setMoveAnimationDelay(uint16_t delay)
    {
        moveAnimationDelay = delay;
    }

    /**
     * @fn virtual uint16_t MoveAnimator::getMoveAnimationDelay() const
     *
     * @brief Gets the current animation delay.
     *
     *        Gets the current animation delay.
     *
     * @return The current animation delay.
     */
    virtual uint16_t getMoveAnimationDelay() const
    {
        return moveAnimationDelay;
    }

    /**
     * @fn virtual bool MoveAnimator::isRunning() const
     *
     * @brief Gets whether or not the move animation is running.
     *
     *        Gets whether or not the move animation is running.
     *
     * @return true if the move animation is running.
     */
    virtual bool isRunning() const
    {
        return moveAnimationRunning;
    }

    /**
     * @fn void MoveAnimator::startMoveAnimation(int16_t endX, int16_t endY, uint16_t duration, EasingEquation xProgressionEquation = &EasingEquations::linearEaseNone, EasingEquation yProgressionEquation = &EasingEquations::linearEaseNone)
     *
     * @brief Starts the move animation.
     *
     *        Starts the move animation from the current position to the specified end
     *        position. The development of the position (X, Y) during the animation is
     *        described by the supplied EasingEquations.
     *
     * @param endX                 The X position of T at animation end. Relative to the
     *                             container or view that holds T.
     * @param endY                 The Y position of T at animation end. Relative to the
     *                             container or view that holds T.
     * @param duration             The duration of the animation measured in ticks.
     * @param xProgressionEquation The equation that describes the development of the X position
     *                             during the animation. Default =
     *                             EasingEquations::linearEaseNone.
     * @param yProgressionEquation The equation that describes the development of the Y position
     *                             during the animation. Default =
     *                             EasingEquations::linearEaseNone.
     */
    void startMoveAnimation(int16_t endX, int16_t endY, uint16_t duration, EasingEquation xProgressionEquation = &EasingEquations::linearEaseNone, EasingEquation yProgressionEquation = &EasingEquations::linearEaseNone)
    {
        Application::getInstance()->registerTimerWidget(this);

        moveAnimationCounter = 0;
        moveAnimationStartX = T::getX();
        moveAnimationStartY = T::getY();
        moveAnimationEndX = endX;
        moveAnimationEndY = endY;
        moveAnimationDuration = duration;
        moveAnimationXEquation = xProgressionEquation;
        moveAnimationYEquation = yProgressionEquation;

        moveAnimationRunning = true;

        if (moveAnimationDelay == 0 && moveAnimationDuration == 0)
        {
            nextMoveAnimationStep(); // Set end position and shut down
        }
    }

    /**
     * @fn void MoveAnimator::cancelMoveAnimation()
     *
     * @brief Cancel move animation.
     *
     *        Cancel move animation.
     */
    void cancelMoveAnimation()
    {
        Application::getInstance()->unregisterTimerWidget(this);
        moveAnimationRunning = false;
    }

protected:

    /**
     * @fn virtual void MoveAnimator::handleTickEvent()
     *
     * @brief The tick handler that handles the actual animation steps.
     *
     *        The tick handler that handles the actual animation steps.
     */
    virtual void handleTickEvent()
    {
        T::handleTickEvent();

        nextMoveAnimationStep();
    }

    /**
     * @fn void MoveAnimator::nextMoveAnimationStep()
     *
     * @brief Execute next step in move animation.
     *
     *        Execute next step in move animation and stop the timer if necessary.
     */
    void nextMoveAnimationStep()
    {
        if (moveAnimationRunning)
        {
            if (moveAnimationCounter < moveAnimationDelay)
            {
                // Just wait for the delay time to pass
                moveAnimationCounter++;
            }
            else
            {
                if (moveAnimationCounter <= (uint32_t)(moveAnimationDelay + moveAnimationDuration))
                {
                    // Adjust the used animationCounter for the startup delay
                    uint32_t actualAnimationCounter = moveAnimationCounter - moveAnimationDelay;

                    int16_t deltaX = moveAnimationXEquation(actualAnimationCounter, 0, moveAnimationEndX - moveAnimationStartX, moveAnimationDuration);
                    int16_t deltaY = moveAnimationYEquation(actualAnimationCounter, 0, moveAnimationEndY - moveAnimationStartY, moveAnimationDuration);

                    T::moveTo(moveAnimationStartX + deltaX, moveAnimationStartY + deltaY);
                    moveAnimationCounter++;
                }
                if (moveAnimationCounter > (uint32_t)(moveAnimationDelay + moveAnimationDuration))
                {
                    // End of animation
                    moveAnimationRunning = false;
                    moveAnimationCounter = 0;
                    Application::getInstance()->unregisterTimerWidget(this);

                    if (moveAnimationEndedCallback && moveAnimationEndedCallback->isValid())
                    {
                        moveAnimationEndedCallback->execute(*this);
                    }
                }
            }
        }
    }

protected:
    bool           moveAnimationRunning;   ///< Boolean that is true if the animation is running
    uint16_t       moveAnimationCounter;   ///< Counter that is equal to the current step in the animation
    uint16_t       moveAnimationDelay;     ///< A delay that is applied before animation start. Expressed in ticks.
    uint16_t       moveAnimationDuration;  ///< The complete duration of the animation. Expressed in ticks.
    int16_t        moveAnimationStartX;    ///< The X value at the beginning of the animation.
    int16_t        moveAnimationStartY;    ///< The Y value at the beginning of the animation.
    int16_t        moveAnimationEndX;      ///< The X value at the end of the animation.
    int16_t        moveAnimationEndY;      ///< The Y value at the end of the animation.
    EasingEquation moveAnimationXEquation; ///< EasingEquation expressing the development of the X value during the animation.
    EasingEquation moveAnimationYEquation; ///< EasingEquation expressing the development of the Y value during the animation.

    GenericCallback<const MoveAnimator<T>& >* moveAnimationEndedCallback; ///< Animation ended Callback.
};

} //namespace touchgfx
#endif // MOVEANIMATOR_HPP
