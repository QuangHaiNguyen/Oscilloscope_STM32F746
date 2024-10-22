
#ifndef DEDICATEDBUFFERVIDEOCONTROLLER_HPP
#define DEDICATEDBUFFERVIDEOCONTROLLER_HPP

#include <touchgfx/hal/VideoController.hpp>
#include <touchgfx/widgets/VideoWidget.hpp>
#include <MJPEGDecoder.hpp>

#include <string.h>

#include "cmsis_os.h"
#if defined(osCMSIS) && (osCMSIS < 0x20000)
#define MUTEX_CREATE() osMutexCreate(0)
#define MUTEX_LOCK(m) osMutexWait(m, osWaitForever)
#define MUTEX_TYPE osMutexId
#define MUTEX_UNLOCK(m) osMutexRelease(m)
#define SEM_CREATE() osSemaphoreCreate(0, 1)
#define SEM_POST(s) osSemaphoreRelease(s)
#define SEM_TYPE osSemaphoreId
#define SEM_WAIT(s) osSemaphoreWait(s, osWaitForever)
#else
#define MUTEX_CREATE() osMutexNew(0)
#define MUTEX_LOCK(m) osMutexAcquire(m, osWaitForever)
#define MUTEX_TYPE osMutexId_t
#define MUTEX_UNLOCK(m) osMutexRelease(m)
#define SEM_CREATE() osSemaphoreNew(1, 0, 0)
#define SEM_POST(s) osSemaphoreRelease(s)
#define SEM_TYPE osSemaphoreId_t
#define SEM_WAIT(s) osSemaphoreAcquire(s, osWaitForever)
#endif

template <uint32_t no_streams, uint32_t width, uint32_t height, uint32_t stride, touchgfx::Bitmap::BitmapFormat output_format>
class DedicatedBufferController : public touchgfx::VideoController
{
public:
    DedicatedBufferController()
        : VideoController(), bufferRGB(0), sizeBufferRGB(0), topBufferRGB(0),
          firstDrawThisFrame(true), uiHasMutex(false), semDecode(0), mutexBuffers(0)
    {
        assert((no_streams > 0) && "Video: Number of streams zero!");

        // Clear arrays
        memset(decodeBuffers, 0, sizeof(decodeBuffers));
        memset(mjpegDecoders, 0, sizeof(mjpegDecoders));

        // Initialize synchronization primitives
        semDecode = SEM_CREATE(); // Binary semaphore
        mutexBuffers = MUTEX_CREATE();
    }

    Handle registerVideoWidget(touchgfx::VideoWidget& widget)
    {
        // Running in UI thread

        const uint32_t sizeOfOneDecodeBuffer = height * stride;

        // Find stream handle for Widget
        Handle handle = getFreeHandle();

        // Running in UI thread!!
        streams[handle].isActive = true;

        // Allocate one buffer for this stream, if possible
        if (topBufferRGB + sizeOfOneDecodeBuffer > (bufferRGB + sizeBufferRGB))
        {
            assert(0 && "registerVideoWidget: Unable to allocate RGB buffer!");
            return 0xFFFFFFFF;
        }

        decodeBuffers[handle] = (uint32_t*)topBufferRGB;
        topBufferRGB += sizeOfOneDecodeBuffer;

        // Set Widget buffer format and address
        widget.setVideoBufferFormat(output_format, width, height);
        widget.setVideoBuffer((uint8_t*)0);

        return handle;
    }

    void unregisterVideoWidget(const Handle handle)
    {
        // Running in UI thread

        // Lock mutex to wait for decoder thread to finish
        MUTEX_LOCK(mutexBuffers);
        MUTEX_UNLOCK(mutexBuffers);

        streams[handle].isActive = false;

        // If all handles are free, reset top pointer
        bool oneIsActive = false;
        for (uint32_t i = 0; i < no_streams; i++)
        {
            oneIsActive |= streams[i].isActive;
        }
        if (oneIsActive == false)
        {
            // Reset memory usage
            topBufferRGB = bufferRGB;
        }
    }

    uint32_t getCurrentFrameNumber(const Handle handle)
    {
        assert(handle < no_streams);
        const Stream& stream = streams[handle];
        return stream.frameNumber;
    }

    void setFrameRate(const Handle handle, uint32_t ui_frames, uint32_t video_frames)
    {
        // Running in UI thread

        assert(handle < no_streams);
        Stream& stream = streams[handle];

        // Reset counters
        stream.frameCount = 0;
        stream.tickCount = 0;

        // Save requested frame rate ratio
        stream.frame_rate_ticks = ui_frames;
        stream.frame_rate_video = video_frames;
    }

    void setVideoData(const Handle handle, const uint8_t* movie, const uint32_t length)
    {
        assert(handle < no_streams);
        Stream& stream = streams[handle];

        // Reset stream frame number
        stream.frameNumber = 0;

        mjpegDecoders[handle]->setVideoData(movie, length);
    }

    void setVideoData(const Handle handle, VideoDataReader& reader)
    {
        mjpegDecoders[handle]->setVideoData(reader);
    }

    void setCommand(const Handle handle, Command cmd, uint32_t param)
    {
        // Running in UI thread

        assert(handle < no_streams);
        Stream& stream = streams[handle];

        switch (cmd)
        {
        case PLAY:
            // Cannot Play without movie
            if (mjpegDecoders[handle]->hasVideo())
            {
                stream.isPlaying = true;
                stream.hasMoreFrames = true; // Assume more frames, lowered on end
                // Reset counters
                stream.frameCount = 0;
                stream.tickCount = 0;
            }
            break;
        case PAUSE:
            stream.isPlaying = false;
            break;
        case SEEK:
            stream.seek_to_frame = param;
            // Reset counters
            stream.frameCount = 0;
            stream.tickCount = 0;
            break;
        case STOP:
            stream.isPlaying = false;
            stream.seek_to_frame = 1;
            break;
        case SET_REPEAT:
            stream.repeat = (param > 0);
            break;
        }
    }

    bool updateFrame(const Handle handle, touchgfx::VideoWidget& widget)
    {
        // Running in UI thread

        assert(handle < no_streams);
        Stream& stream = streams[handle];

        // Increase tickCount if playing
        if (stream.isPlaying)
        {
            stream.tickCount++;
        }

        // Invalidate Widget if new frame was decoded
        if (stream.doInvalidateOnNextTick)
        {
            widget.setVideoBuffer((uint8_t*)decodeBuffers[handle]);
            widget.invalidate();
            stream.doInvalidateOnNextTick = false;
            if (!stream.hasMoreFrames && !stream.repeat)
            {
                stream.isPlaying = false;
            }
            return stream.hasMoreFrames;
        }
        return true;
    }

    void draw(const Handle handle, const touchgfx::Rect& invalidatedArea, const touchgfx::VideoWidget& widget)
    {
        // Running in UI thread

        // Take the mutex in the first call to draw in this UI frame
        // This will make UI thread wait for the decoder thread if it is still decoding
        if (firstDrawThisFrame)
        {
            MUTEX_LOCK(mutexBuffers);
            firstDrawThisFrame = false;
            uiHasMutex = true;
        }
    }

    void setRGBBuffer(uint8_t* buffer, size_t sizeOfBuffer)
    {
        // Running in UI thread / main

        bufferRGB = buffer;
        topBufferRGB = bufferRGB;
        sizeBufferRGB = sizeOfBuffer;
    }

    void addDecoder(MJPEGDecoder& decoder, uint32_t index)
    {
        // Running in UI thread / main

        assert(index < no_streams);
        mjpegDecoders[index] = &decoder;
    }

    void endFrame()
    {
        // Running in UI thread

        // Check if we should invalidate in next frame
        for (uint32_t i = 0; i < no_streams; i++)
        {
            Stream& stream = streams[i];
            if (stream.isPlaying)
            {
                if (decodeForNextTick(stream))
                {
                    stream.doDecode = true;
                }
            }
        }

        // Mark next draw as first
        firstDrawThisFrame = true;

        // Signal decoder thread to wake up
        SEM_POST(semDecode);
        // Unlock the buffer mutex. FreeRTOS does not allow unlocking a mutex you did not lock
        if (uiHasMutex)
        {
            MUTEX_UNLOCK(mutexBuffers);
            uiHasMutex = false;
        }
    }

    void decoderTaskEntry()
    {
        // Running in Decoder thread!!

        while (1)
        {
            // Wait for synchronisation signal from UI thread
            SEM_WAIT(semDecode);

            // Lock out the UI by taking the mutex
            MUTEX_LOCK(mutexBuffers);

            // Update stream flags
            for (uint32_t i = 0; i < no_streams; i++)
            {
                Stream& stream = streams[i];
                if (stream.doDecode)
                {
                    stream.doInvalidateOnNextTick = true;
                }
            }

            // Now decode all streams marked for invalidation in next tick
            for (uint32_t i = 0; i < no_streams; i++)
            {
                Stream& stream = streams[i];
                if (stream.doDecode)
                {
                    MJPEGDecoder* const decoder = mjpegDecoders[i];
                    // Seek or increment video frame
                    if (stream.seek_to_frame > 0)
                    {
                        decoder->gotoFrame(stream.seek_to_frame);
                        stream.seek_to_frame = 0;
                    }

                    // Decode frame
                    stream.hasMoreFrames = decoder->decodeNextFrame((uint8_t*)decodeBuffers[i], width, height, stride);
                    stream.frameCount++;

                    // Update frame number
                    if (stream.hasMoreFrames)
                    {
                        stream.frameNumber = decoder->getCurrentFrameNumber() - 1; // Points to frame after this
                    }
                    else
                    {
                        stream.frameNumber = 1;
                    }
                    stream.doDecode = false;
                }
            }

            // Release the stream and decode buffer and let UI continue
            MUTEX_UNLOCK(mutexBuffers);
        }
    }

    void getVideoInformation(const Handle handle, touchgfx::VideoInformation* data)
    {
        assert(handle < no_streams);
        mjpegDecoders[handle]->getVideoInfo(data);
    }

    bool getIsPlaying(const Handle handle)
    {
        assert(handle < no_streams);
        Stream& stream = streams[handle];
        return stream.isPlaying;
    }

private:
    class Stream
    {
    public:
        Stream() : frameNumber(0), frameCount(0), tickCount(0), frame_rate_video(0), frame_rate_ticks(0),
            seek_to_frame(0),
            isActive(false), doInvalidateOnNextTick(false), hasMoreFrames(false), repeat(true), doDecode(false) {}
        uint32_t frameNumber;      // Video frame number shown
        uint32_t frameCount;       // Video frame counter (for frame rate)
        uint32_t tickCount;        // UI frames since play
        uint32_t frame_rate_video; // Ratio of frames wanted divider
        uint32_t frame_rate_ticks; // Ratio of frames wanted counter
        uint32_t seek_to_frame;    // Requested next frame number
        bool isActive;
        bool isPlaying;
        bool doInvalidateOnNextTick;
        bool hasMoreFrames;
        bool repeat;
        bool doDecode;
    };

    MJPEGDecoder* mjpegDecoders[no_streams];
    uint32_t* decodeBuffers[no_streams];
    Stream streams[no_streams];
    uint8_t* bufferRGB;
    size_t sizeBufferRGB;        // Size in Bytes
    uint8_t* topBufferRGB;       // Pointer to unused memory in buffer
    bool firstDrawThisFrame;     // Only used by UI thread
    bool uiHasMutex;             // High if mutex was taken (i.e. was drawn)

    SEM_TYPE semDecode;          // Post by UI, wait by Decoder thread
    MUTEX_TYPE mutexBuffers;     // Mutual exclusion of the video buffers and stream data

    /**
     * Return true, if new video frame should be decoded for the next tick (keep video decode framerate low)
     */
    bool decodeForNextTick(const Stream& stream)
    {
        // Running in UI thread

        // Compare tickCount/frameCount to frame_rate_ticks/frame_rate_video
        if ((stream.tickCount * stream.frame_rate_video) >= (stream.frame_rate_ticks * stream.frameCount))
        {
            return true;
        }
        return false;
    }

    Handle getFreeHandle()
    {
        // Running in UI thread

        for (uint32_t i = 0; i < no_streams; i++)
        {
            if (streams[i].isActive == false)
            {
                // Reset stream parameters
                streams[i] = Stream();

                return static_cast<VideoController::Handle>(i);
            }
        }

        assert(0 && "Unable to find free video stream handle!");
        return static_cast<VideoController::Handle>(0);
    }
};

#endif // TOUCHGFX_DEDICATEDBUFFERVIDEOCONTROLLER_HPP
