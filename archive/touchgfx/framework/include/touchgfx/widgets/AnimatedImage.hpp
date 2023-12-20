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
#ifndef ANIMATEDIMAGE_HPP
#define ANIMATEDIMAGE_HPP

#include <touchgfx/widgets/Image.hpp>

namespace touchgfx
{
/**
 * @class AnimatedImage AnimatedImage.hpp touchgfx/widgets/AnimatedImage.hpp
 *
 * @brief A widget capable of basic animation using a range of bitmaps.
 *
 *        The AnimatedImage is capable of running the animation from start to end or in reverse
 *        order, end to start. It is capable doing a single animation or looping the animation
 *        until stopped or paused. See animation_example far a demonstration of how to use of
 *        this widget.
 */
class AnimatedImage : public Image
{
public:

    /**
     * @fn AnimatedImage::AnimatedImage(const BitmapId& start, const BitmapId& end, const uint8_t& updateInterval = 1)
     *
     * @brief Constructs an AnimatedImage.
     *
     *        The start and the end specifies the range of bitmaps to be used for animation.
     *        The update interval defines how often the animation should be updated. The
     *        animation will iterate over the bitmaps that lies between the IDs of start and
     *        end, both included.
     *
     * @param start          Defines the start of the range of images in the animation.
     * @param end            Defines the end of the range of images in the animation.
     * @param updateInterval Defines the number of ticks between each animation step. Higher value
     *                       results in a slower animation.
     */
    AnimatedImage(const BitmapId& start, const BitmapId& end, const uint8_t& updateInterval = 1) :
        Image(Bitmap(start)),
        animationDoneAction(0),
        startId(start),
        endId(end),
        updateTicksInterval(updateInterval),
        ticksSinceUpdate(0),
        reverse(false),
        loopAnimation(false),
        running(false)
    {
    }

    /**
     * @fn AnimatedImage::AnimatedImage(const uint8_t& updateInterval = 1)
     *
     * @brief Constructor.
     *
     *        Constructs an AnimatedImage without initializing bitmaps.
     *
     * @note The bitmaps to display must be configured through set setBitmaps function before this
     *       widget displays anything.
     *
     * @param updateInterval Defines the number of ticks between each animation step. Higher value
     *                       results in a slower animation.
     */
    AnimatedImage(const uint8_t& updateInterval = 1) :
        animationDoneAction(0),
        startId(BITMAP_INVALID),
        endId(BITMAP_INVALID),
        updateTicksInterval(),
        ticksSinceUpdate(0),
        reverse(false),
        loopAnimation(false),
        running(false)
    { }

    /**
     * @fn virtual void AnimatedImage::startAnimation(const bool& rev, const bool& reset = false, const bool& loop = false);
     *
     * @brief Starts the animation.
     *
     *        Starts the animation.
     *
     * @param rev   Defines if the animation should be performed in reverse order.
     * @param reset Defines if the animation should reset and start from the first (or last if
     *              reverse order) bitmap.
     * @param loop  Defines if the animation should loop or do a single animation.
     */
    virtual void startAnimation(const bool& rev, const bool& reset = false, const bool& loop = false);

    /**
     * @fn virtual void AnimatedImage::stopAnimation();
     *
     * @brief Stops and resets the animation.
     *
     *        Stops and resets the animation.
     */
    virtual void stopAnimation();

    /**
     * @fn virtual void AnimatedImage::pauseAnimation();
     *
     * @brief Toggles the running state of an animation.
     *
     *        Toggles the running state of an animation. Pauses the animation if the animation
     *        is running. Continues the animation if previously paused.
     */
    virtual void pauseAnimation();

    virtual void handleTickEvent();

    /**
     * @fn void AnimatedImage::setDoneAction(GenericCallback< const AnimatedImage& >& callback)
     *
     * @brief Associates an action to be performed when the animation of the AnimatedImage is done.
     *
     *        Associates an action to be performed when the animation of the AnimatedImage is
     *        done.
     *
     * @param callback The callback is executed when done. The callback is given the animated
     *                 image.
     */
    void setDoneAction(GenericCallback< const AnimatedImage& >& callback)
    {
        animationDoneAction = &callback;
    }

    /**
     * @fn bool AnimatedImage::isRunning()
     *
     * @brief Gets the running state of the AnimatedImage.
     *
     *        Gets the running state of the AnimatedImage.
     *
     * @return true if the animation is currently running, false otherwise.
     */
    bool isRunning()
    {
        return running;
    };

    /**
     * @fn bool AnimatedImage::isReverse()
     *
     * @brief Query if this object is running in reverse.
     *
     *        Query if this object is running in reverse.
     *
     * @return true if the animation is performed in reverse order.
     */
    bool isReverse()
    {
        return reverse;
    };

    /**
     * @fn void AnimatedImage::setBitmaps(BitmapId start, BitmapId end);
     *
     * @brief Sets the bitmaps that are used by the animation.
     *
     *        Sets the bitmaps that are used by the animation.
     *
     *        The animation will iterate over the bitmaps that lies between the IDs of start
     *        and end, both included.
     *
     * @param start Defines the start of the range of images in the animation.
     * @param end   Defines the end of the range of images in the animation.
     */
    void setBitmaps(BitmapId start, BitmapId end);

    /**
     * @fn void AnimatedImage::setUpdateTicksInterval(uint8_t updateInterval);
     *
     * @brief Sets the update interval.
     *
     *        Sets the update interval.
     *
     * @param updateInterval Defines the number of ticks between each animation step. Higher value
     *                       results in a slower animation.
     */
    void setUpdateTicksInterval(uint8_t updateInterval);

    /**
     * @fn virtual uint16_t AnimatedImage::getType() const
     *
     * @brief For GUI testing only.
     *
     *        For GUI testing only. Returns type of this drawable.
     *
     * @return TYPE_ANIMATEDIMAGE.
     */
    virtual uint16_t getType() const
    {
        return (uint16_t)TYPE_ANIMATEDIMAGE;
    }

protected:
    GenericCallback<const AnimatedImage&>* animationDoneAction; ///< Pointer to the callback being executed when animation is done.

    BitmapId startId;             ///< Id of first bitmap in animation.
    BitmapId endId;               ///< Id of second bitmap in animation.S
    uint8_t  updateTicksInterval; ///< Number of ticks required between each animation update (image change).
    uint8_t  ticksSinceUpdate;    ///< Number of ticks since last animation update.
    bool     reverse;             ///< If true, run in reverse direction (last to first).
    bool     loopAnimation;       ///< If true, continuously loop animation.
    bool     running;             ///< If true, animation is running.

    /**
     * @fn virtual void AnimatedImage::setBitmap(const Bitmap& bmp)
     *
     * @brief Is not public available. Use setBitmaps instead.
     *
     *        Is not public available. Use setBitmaps instead. Internally in AnimatedImage use
     *        Image::setBitmap(...).
     *
     * @param bmp The bitmap.
     */
    virtual void setBitmap(const Bitmap& bmp) {}
};

} // namespace touchgfx

#endif // ANIMATEDIMAGE_HPP
