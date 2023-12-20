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
#ifndef ANIMATIONTEXTUREMAPPER_HPP
#define ANIMATIONTEXTUREMAPPER_HPP

#include <touchgfx/widgets/TextureMapper.hpp>
#include <touchgfx/Callback.hpp>
#include <touchgfx/EasingEquations.hpp>
#include <touchgfx/Math3D.hpp>

namespace touchgfx
{
/**
 * @class AnimationTextureMapper AnimationTextureMapper.hpp touchgfx/widgets/AnimationTextureMapper.hpp
 *
 * @brief A texture mapper with animation capabilities.
 *
 *        A texture mapper with animation capabilities. Note that the angles of the
 *        TextureMapper is moved to the [0; 2PI] range at the beginning at the animation. The
 *        end angles should be relative to this and are limited to values in the range [-32.7;
 *        32.7].
 *
 * @see TextureMapper
 */
class AnimationTextureMapper : public TextureMapper
{
public:
    /**
     * @enum AnimationParameter
     *
     * @brief Values that represent different animation parameter.
     */
    enum AnimationParameter
    {
        X_ROTATION = 0, ///< Rotation around the X axis
        Y_ROTATION,     ///< Rotation around the Y axis
        Z_ROTATION,     ///< Rotation around the Z axis
        SCALE           ///< Scaling of the image
    };

    static const int NUMBER_OF_ANIMATION_PARAMETERS = SCALE + 1; ///< Number of animation parameters

    /**
     * @fn AnimationTextureMapper::AnimationTextureMapper();
     *
     * @brief Default constructor.
     *
     *        Default constructor.
     */
    AnimationTextureMapper();

    /**
     * @fn virtual AnimationTextureMapper::~AnimationTextureMapper()
     *
     * @brief Destructor.
     *
     *        Destructor. Destroys the AnimationTextureMapper.
     */
    virtual ~AnimationTextureMapper();

    /**
     * @fn void AnimationTextureMapper::setTextureMapperAnimationStepAction(GenericCallback<const AnimationTextureMapper& >& callback)
     *
     * @brief Associates an action to be performed when the animation steps.
     *
     *        Associates an action to be performed when the animation steps. Will not be called
     *        during delay period.
     *
     * @param callback The callback to be executed. The callback will be given a reference to
     *                 the AnimationTextureMapper.
     *
     * @see GenericCallback
     */
    void setTextureMapperAnimationStepAction(GenericCallback<const AnimationTextureMapper& >& callback);

    /**
     * @fn void AnimationTextureMapper::setTextureMapperAnimationEndedAction(GenericCallback<const AnimationTextureMapper& >& callback)
     *
     * @brief Associates an action to be performed when the animation ends.
     *
     *        Associates an action to be performed when the animation ends.
     *
     * @param callback The callback to be executed. The callback will be given a reference to
     *                 the AnimationTextureMapper.
     *
     * @see GenericCallback
     */
    void setTextureMapperAnimationEndedAction(GenericCallback<const AnimationTextureMapper& >& callback);

    /**
     * @fn virtual bool AnimationTextureMapper::isTextureMapperAnimationRunning() const
     *
     * @brief Gets whether or not the animation is running.
     *
     *        Gets whether or not the animation is running.
     *
     * @return true if the animation is running.
     */
    virtual bool isTextureMapperAnimationRunning() const;

    /**
     * @fn virtual void AnimationTextureMapper::setupAnimation(AnimationParameter parameter, float endValue, uint16_t duration, uint16_t delay, EasingEquation progressionEquation = &EasingEquations::linearEaseNone);
     *
     * @brief Sets up the animation for a specific parameter (angle/scale) for the next animation.
     *
     *        Sets up the animation for a specific parameter (angle/scale) for the next
     *        animation. The specific parameter is chosen using the AnimationType enum.
     *        AnimationTypes that are not setup using this method will keep their value during
     *        the animation.
     *
     * @param parameter           The parameter which animation details are being specified.
     * @param endValue            The end value for the parameter.
     * @param duration            The duration for the animation of this parameter. Specified in
     *                            ticks.
     * @param delay               The delay for the animation of this parameter. Specified in ticks.
     * @param progressionEquation the progression equation for the animation of this parameter.
     */
    virtual void setupAnimation(AnimationParameter parameter, float endValue, uint16_t duration, uint16_t delay, EasingEquation progressionEquation = &EasingEquations::linearEaseNone);

    /**
     * @fn virtual void AnimationTextureMapper::startAnimation();
     *
     * @brief Starts the animation.
     *
     *        Starts the animation from the current position to the specified end angles/scale.
     *        The progression of the angles/scale during the animation is described by the
     *        supplied EasingEquations.
     */
    virtual void startAnimation();

    /**
     * @fn virtual void AnimationTextureMapper::cancelMoveAnimation();
     *
     * @brief Cancel move animation.
     *
     *        Cancel move animation.
     */
    virtual void cancelMoveAnimation();

    /**
     * @fn virtual uint16_t AnimationTextureMapper::getAnimationStep()
     *
     * @brief Gets the current animation step.
     *
     * @return The current animation step.
     */
    virtual uint16_t getAnimationStep();

protected:
    /**
     * @fn virtual void AnimationTextureMapper::handleTickEvent()
     *
     * @brief The tick handler that handles the actual animation steps.
     *
     *        The tick handler that handles the actual animation steps.
     */
    virtual void handleTickEvent();

    /**
     * @struct AnimationSetting AnimationTextureMapper.hpp touchgfx/widgets/AnimationTextureMapper.hpp
     *
     * @brief Information about how a specific animation parameter should be animated.
     */
    struct AnimationSetting
    {
        bool           animationActive;              ///< Should this animation be performed
        float          animationStart;               ///< The animation start value
        float          animationEnd;                 ///< The animation end value
        uint16_t       animationDelay;               ///< A delay that is applied before animation start. Expressed in ticks.
        uint16_t       animationDuration;            ///< The complete duration of the animation. Expressed in ticks.
        EasingEquation animationProgressionEquation; ///< EasingEquation expressing the development of the value during the animation.
    };

    AnimationSetting animations[NUMBER_OF_ANIMATION_PARAMETERS];    ///< Descriptions of the animation of specific animation parameters

    GenericCallback<const AnimationTextureMapper& >* textureMapperAnimationStepCallback;  ///< Animation has performed a step Callback.
    GenericCallback<const AnimationTextureMapper& >* textureMapperAnimationEndedCallback; ///< Animation ended Callback.

    uint16_t animationCounter;   ///< Counter that is equal to the current step in the animation
    bool     animationRunning;   ///< Boolean that is true if the animation is running
};

} // namespace touchgfx

#endif // ANIMATIONTEXTUREMAPPER_HPP
