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
#include <touchgfx/Utils.hpp>
#include <touchgfx/hal/HAL.hpp>
#include <touchgfx/widgets/VideoWidget.hpp>

namespace touchgfx
{
VideoWidget::VideoWidget()
    : Widget(),
      movieEndedAction(0), buffer(0), format(Bitmap::RGB888),
      bufferWidth(0), bufferHeight(0), videoWidth(0), videoHeight(0)
{
    handle = VideoController::getInstance().registerVideoWidget(*this);
    Application::getInstance()->registerTimerWidget(this);
}

VideoWidget::~VideoWidget()
{
    VideoController::getInstance().unregisterVideoWidget(handle);
    Application::getInstance()->unregisterTimerWidget(this);
}

void VideoWidget::play() const
{
    VideoController::getInstance().setCommand(handle, VideoController::PLAY, 0);
}

void VideoWidget::pause() const
{
    VideoController::getInstance().setCommand(handle, VideoController::PAUSE, 0);
}

void VideoWidget::stop() const
{
    VideoController::getInstance().setCommand(handle, VideoController::STOP, 0);
}

bool VideoWidget::isPlaying() const
{
    return VideoController::getInstance().getIsPlaying(handle);
}

void VideoWidget::seek(uint32_t frameNumber) const
{
    VideoController::getInstance().setCommand(handle, VideoController::SEEK, frameNumber);
}

void VideoWidget::showFrame(uint32_t frameNumber) const
{
    if (isPlaying())
    {
        seek(frameNumber);
    }
    else
    {
        VideoController::getInstance().setCommand(handle, VideoController::SHOW, frameNumber);
    }
}

void VideoWidget::setRepeat(bool repeat) const
{
    VideoController::getInstance().setCommand(handle, VideoController::SET_REPEAT, repeat ? 1 : 0);
}

void VideoWidget::handleTickEvent()
{
    const bool hasMoreFrames = VideoController::getInstance().updateFrame(handle, *this);
    if (!hasMoreFrames)
    {
        // Now showing last frame
        if (movieEndedAction && movieEndedAction->isValid())
        {
            movieEndedAction->execute(*this);
        }
    }
}

Rect VideoWidget::getSolidRect() const
{
    // For Dedicated or DoubleBuffered video we have a bufferWidth > 0
    // But maybe no buffer yet (only after decoding first frame)
    if (bufferWidth > 0)
    {
        // Solid inside video area if we have a buffer, otherwise transparent
        if (buffer != 0)
        {
            return Rect(0, 0, MIN(videoWidth, bufferWidth), MIN(videoHeight, bufferHeight));
        }
        return Rect();
    }

    //Direct to framebuffer, solid inside the video area
    return Rect(0, 0, videoWidth, videoHeight);
}

void VideoWidget::draw(const Rect& invalidatedArea) const
{
    // Only draw the invalidated area inside the video x buffer
    Rect invVideoRect(0, 0, videoWidth, videoHeight);
    invVideoRect &= invalidatedArea;

    // Inform controller that we are drawing
    VideoController::getInstance().draw(handle, invVideoRect, *this);

    // If we have a buffer, draw that
    if (buffer != 0)
    {
        // Limit to buffer size
        invVideoRect &= Rect(0, 0, bufferWidth, bufferHeight);

        // Convert to lcd coordinates. Width is buffer stride.
        Rect absolute = getAbsoluteRect();
        absolute.width = bufferWidth;
        // Copy to LCD, always solid
        HAL::lcd().blitCopy(buffer, format, absolute, invVideoRect, 255, false);
    }
}

uint32_t VideoWidget::getCurrentFrameNumber() const
{
    return VideoController::getInstance().getCurrentFrameNumber(handle);
}

void VideoWidget::setFrameRate(uint32_t ui_frames, uint32_t video_frames) const
{
    VideoController::getInstance().setFrameRate(handle, ui_frames, video_frames);
}

void VideoWidget::setVideoData(const uint8_t* movie, const uint32_t length)
{
    VideoController::getInstance().setVideoData(handle, movie, length);

    readVideoInformation();
}

void VideoWidget::setVideoData(VideoDataReader& reader)
{
    VideoController::getInstance().setVideoData(handle, reader);

    readVideoInformation();
}

void VideoWidget::getVideoInformation(VideoInformation* data) const
{
    VideoController::getInstance().getVideoInformation(handle, data);
}

void VideoWidget::readVideoInformation()
{
    // Set frame rate to movie speed (assuming 60 ui frames in a second)
    VideoInformation videoInformation;
    VideoController::getInstance().getVideoInformation(handle, &videoInformation);
    videoWidth = (int16_t)videoInformation.frame_width;
    videoHeight = (int16_t)videoInformation.frame_height;
    const uint32_t videoFramesIn1000ms = 1000 / videoInformation.ms_between_frames;
    setFrameRate(60, videoFramesIn1000ms);
}

} // namespace touchgfx
