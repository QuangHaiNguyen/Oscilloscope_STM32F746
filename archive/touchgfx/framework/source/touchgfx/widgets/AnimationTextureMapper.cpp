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
#include <touchgfx/widgets/AnimationTextureMapper.hpp>

namespace touchgfx
{

AnimationTextureMapper::AnimationTextureMapper() :
    TextureMapper(),
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

AnimationTextureMapper::~AnimationTextureMapper()
{
}

void AnimationTextureMapper::setTextureMapperAnimationStepAction(GenericCallback<const AnimationTextureMapper& >& callback)
{
    textureMapperAnimationStepCallback = &callback;
}

void AnimationTextureMapper::setTextureMapperAnimationEndedAction(GenericCallback<const AnimationTextureMapper& >& callback)
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
}

void AnimationTextureMapper::cancelMoveAnimation()
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
    if (animationRunning)
    {
        bool activeAnimation = false;

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

            if ((animationCounter >= animations[i].animationDelay) &&
                    (animationCounter <= (uint32_t)(animations[i].animationDelay + animations[i].animationDuration)))
            {
                activeAnimation = true;

                // Adjust the used animationCounter for the startup delay
                uint32_t actualAnimationCounter = animationCounter - animations[i].animationDelay;

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

                float delta = directionModifier * (animations[i].animationProgressionEquation(actualAnimationCounter, 0, distance, animations[i].animationDuration) / 1000.f);

                switch ((AnimationParameter) i)
                {
                case X_ROTATION:
                    newXAngle = animations[X_ROTATION].animationStart + delta;
                    break;
                case Y_ROTATION:
                    newYAngle = animations[Y_ROTATION].animationStart + delta;
                    break;
                case Z_ROTATION:
                    newZAngle = animations[Z_ROTATION].animationStart + delta;
                    break;
                case SCALE:
                    newScale = animations[SCALE].animationStart + delta;
                    break;
                default:
                    break;
                }
            }
        }

        if (activeAnimation)
        {
            updateAngles(newXAngle, newYAngle, newZAngle);
            setScale(newScale);

            if (textureMapperAnimationStepCallback && textureMapperAnimationStepCallback->isValid())
            {
                textureMapperAnimationStepCallback->execute(*this);
            }

            animationCounter++;
        }
        else
        {
            // End of animation
            cancelMoveAnimation();

            if (textureMapperAnimationEndedCallback && textureMapperAnimationEndedCallback->isValid())
            {
                textureMapperAnimationEndedCallback->execute(*this);
            }
        }
    }
}

}
