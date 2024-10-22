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
#include <touchgfx/widgets/AnimationTextureMapper.hpp>

namespace touchgfx
{
AnimationTextureMapper::AnimationTextureMapper()
    : TextureMapper(),
      textureMapperAnimationStepCallback(0),
      textureMapperAnimationEndedCallback(0),
      animationCounter(0),
      animationRunning(false)
{
    for (int i = 0; i < NUMBER_OF_ANIMATION_PARAMETERS; i++)
    {
        animations[i].animationActive = false;
    }
}

void AnimationTextureMapper::setTextureMapperAnimationStepAction(GenericCallback<const AnimationTextureMapper&>& callback)
{
    textureMapperAnimationStepCallback = &callback;
}

void AnimationTextureMapper::setTextureMapperAnimationEndedAction(GenericCallback<const AnimationTextureMapper&>& callback)
{
    textureMapperAnimationEndedCallback = &callback;
}

bool AnimationTextureMapper::isTextureMapperAnimationRunning() const
{
    return animationRunning;
}

void AnimationTextureMapper::setupAnimation(AnimationParameter parameter, float endValue, uint16_t duration, uint16_t delay, EasingEquation progressionEquation /*= &EasingEquations::linearEaseNone*/)
{
    animations[parameter].animationActive = true;
    animations[parameter].animationEnd = endValue;
    animations[parameter].animationDuration = duration;
    animations[parameter].animationDelay = delay;
    animations[parameter].animationProgressionEquation = progressionEquation;
}

void AnimationTextureMapper::startAnimation()
{
    Application::getInstance()->registerTimerWidget(this);

    animationCounter = 0;

    animations[X_ROTATION].animationStart = xAngle;
    animations[Y_ROTATION].animationStart = yAngle;
    animations[Z_ROTATION].animationStart = zAngle;
    animations[SCALE].animationStart = scale;

    animationRunning = true;

    for (int i = 0; i < NUMBER_OF_ANIMATION_PARAMETERS; i++)
    {
        if (animations[i].animationActive && animations[i].animationDelay + animations[i].animationDuration > 0)
        {
            return; // Animation needs to run, return
        }
    }
    // No active animations or all active animations have zero steps, execute now!
    handleTickEvent();
}

void AnimationTextureMapper::cancelAnimationTextureMapperAnimation()
{
    Application::getInstance()->unregisterTimerWidget(this);
    animationRunning = false;

    for (int i = 0; i < NUMBER_OF_ANIMATION_PARAMETERS; i++)
    {
        animations[i].animationActive = false;
    }
}

uint16_t AnimationTextureMapper::getAnimationStep()
{
    return animationCounter;
}

void AnimationTextureMapper::handleTickEvent()
{
    if (!animationRunning)
    {
        return;
    }
    bool newAngleAssigned = false;
    bool newScaleAssigned = false;
    bool activeAnimationExists = false;

    animationCounter++;

    float newXAngle = xAngle;
    float newYAngle = yAngle;
    float newZAngle = zAngle;
    float newScale = scale;

    for (int i = 0; i < NUMBER_OF_ANIMATION_PARAMETERS; i++)
    {
        if (!(animations[i].animationActive))
        {
            continue;
        }

        if (animationCounter >= animations[i].animationDelay)
        {
            // Adjust the used animationCounter for the startup delay
            const uint32_t actualAnimationCounter = animationCounter - animations[i].animationDelay;

            int directionModifier;
            int16_t distance;

            if (animations[i].animationEnd > animations[i].animationStart)
            {
                directionModifier = 1;
                distance = (int16_t)((animations[i].animationEnd - animations[i].animationStart) * 1000);
            }
            else
            {
                directionModifier = -1;
                distance = (int16_t)((animations[i].animationStart - animations[i].animationEnd) * 1000);
            }

            const float delta = (float)directionModifier * (animations[i].animationProgressionEquation(actualAnimationCounter, 0, distance, animations[i].animationDuration) / 1000.f);

            switch ((AnimationParameter)i)
            {
            case X_ROTATION:
                newXAngle = animations[X_ROTATION].animationStart + delta;
                newAngleAssigned = true;
                break;
            case Y_ROTATION:
                newYAngle = animations[Y_ROTATION].animationStart + delta;
                newAngleAssigned = true;
                break;
            case Z_ROTATION:
                newZAngle = animations[Z_ROTATION].animationStart + delta;
                newAngleAssigned = true;
                break;
            case SCALE:
                newScale = animations[SCALE].animationStart + delta;
                newScaleAssigned = true;
                break;
            }
        }
        if (animationCounter >= (uint32_t)(animations[i].animationDelay + animations[i].animationDuration))
        {
            animations[i].animationActive = false;
        }
        else
        {
            activeAnimationExists = true;
        }
    }

    if (newAngleAssigned || newScaleAssigned)
    {
        if (newAngleAssigned)
        {
            updateAngles(newXAngle, newYAngle, newZAngle);
        }
        if (newScaleAssigned)
        {
            updateScale(newScale);
        }

        if (textureMapperAnimationStepCallback && textureMapperAnimationStepCallback->isValid())
        {
            textureMapperAnimationStepCallback->execute(*this);
        }
    }
    if (!activeAnimationExists)
    {
        // End of animation
        cancelAnimationTextureMapperAnimation();

        if (textureMapperAnimationEndedCallback && textureMapperAnimationEndedCallback->isValid())
        {
            textureMapperAnimationEndedCallback->execute(*this);
        }
    }
}
} // namespace touchgfx
