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
#ifndef RENDERINGBUFFER_HPP
#define RENDERINGBUFFER_HPP

namespace touchgfx
{
/**
 * @class RenderingBuffer RenderingBuffer.hpp touchgfx/canvas_widget_renderer/RenderingBuffer.hpp
 *
 * @brief Rendering buffer wrapper.
 *
 *        Rendering buffer wrapper. This class does not know anything about memory
 *        organizations, all it does it keeps an array of pointers to each pixel row. The
 *        general rules of rendering are as follows.
 *
 *        1. Allocate or create somehow a rendering buffer itself. Since
 *           the library does not depend on any particular platform or architecture it was
 *           decided that it's your responsibility to create and destroy rendering buffers
 *           properly. You can use any available mechanism to create it - you can use a system
 *           API function, simple memory allocation, or even statically defined array. You also
 *           should know the memory organization (or possible variants)
 *           in your system. For example, there's an R,G,B or B,G,R organizations with one byte
 *           per component (three bytes per pixel) is used very often. So, if you intend to use
 *           class render_bgr24, for example, you should allocate at least width*height*3 bytes
 *           of memory.
 *
 *        2. Create a RenderingBuffer object and then call method attach(). It requires
 *           a pointer to the buffer itself, width and height of the buffer in pixels, and the
 *           length of the row in bytes. All these values must properly correspond to the
 *           memory organization. The argument stride is used because in reality the row length
 *           in bytes does not obligatory correspond with the width of the image in pixels,
 *           i.e. it cannot be simply calculated as width_in_pixels * bytes_per_pixel. For
 *           example, it must be aligned to 4 bytes in Windows bitmaps. Method attach() can be
 *           called more than once. The execution time of it is very little, still it allocates
 *           memory of heigh * sizeof(char*) bytes and has a loop while (height--) {...}, so
 *           it's unreasonable to call it every time before drawing any single pixel :-)
 *
 *        3. Create an object (or a number of objects) of a rendering class, such as
 *           renderer_bgr24_solid, renderer_bgr24_image and so on. These classes require a
 *           pointer to the RenderingBuffer object, but they do not perform any considerable
 *           operations except storing this pointer. So, rendering objects can be created on
 *           demand almost any time. These objects know about concrete memory organization
 *           (this knowledge is hard coded), so actually, the memory you allocated or created
 *           in clause 1 should actually be in correspondence to the needs of the rendering
 *           class.
 *
 *        4. Render your image using rendering classes, for example, Rasterizer
 *
 *        5. Display the result, or store it, or whatever. It's also your
 *           responsibility and depends on the platform.
 */
class RenderingBuffer
{
public:
    /**
     * @fn RenderingBuffer::RenderingBuffer()
     *
     * @brief Default constructor.
     *
     *        Default constructor.
     */
    RenderingBuffer();

    /**
     * @fn RenderingBuffer::~RenderingBuffer();
     *
     * @brief Destructor.
     *
     *        Destructor.
     */
    ~RenderingBuffer();

    /**
     * @fn RenderingBuffer::RenderingBuffer(unsigned char* buf_, unsigned char xAdjust_, unsigned width_, unsigned height_, int stride_);
     *
     * @brief Constructor.
     *
     *        Constructor.
     *
     * @param [in] buf_ Pointer to the frame buffer where the image is rendered.
     * @param xAdjust_  Horizontal adjustment of the x coordinate, used when bits per pixel is
     *                  less than eight which implies that a uint8_t pointer cannot precisely
     *                  address the start of the frame buffer.
     * @param width_    The width of the frame buffer to write.
     * @param height_   The height of the frame buffer to write.
     * @param stride_   How much to add the a pointer inside the frame buffer to advance to the
     *                  next line in the frame buffer.
     */
    RenderingBuffer(unsigned char* buf_,
                    unsigned char xAdjust_,
                    unsigned width_,
                    unsigned height_,
                    int      stride_);

    /**
     * @fn void RenderingBuffer::attach(unsigned char* buf_, unsigned char xAdjust_, unsigned width_, unsigned height_, int stride_);
     *
     * @brief Attaches a buffer.
     *
     *        Attaches a buffer. Can be used if the buffer is not ready when the Rendering
     *        buffer is created initially.
     *
     * @param [in] buf_ Pointer to the frame buffer where the image is rendered.
     * @param xAdjust_  Horizontal adjustment of the x coordinate, used when bits per pixel is
     *                  less than eight which implies that a uint8_t pointer cannot precisely
     *                  address the start of the frame buffer.
     * @param width_    The width of the frame buffer to write.
     * @param height_   The height of the frame buffer to write.
     * @param stride_   How much to add the a pointer inside the frame buffer to advance to the
     *                  next line in the frame buffer.
     */
    void attach(unsigned char* buf_,
                unsigned char xAdjust_,
                unsigned width_,
                unsigned height_,
                int      stride_);

    /**
     * @fn unsigned char RenderingBuffer::getXAdjust() const
     *
     * @brief Gets x coordinate adjust.
     *
     *        Gets x coordinate adjust.
     *
     * @return The x coordinate adjust.
     */
    unsigned char getXAdjust() const
    {
        return xAdjust;
    }

    /**
     * @fn unsigned RenderingBuffer::getWidth() const
     *
     * @brief Gets the width.
     *
     *        Gets the width.
     *
     * @return The width.
     */
    unsigned getWidth() const
    {
        return width;
    }

    /**
     * @fn unsigned RenderingBuffer::getHeight() const
     *
     * @brief Gets the height.
     *
     *        Gets the height.
     *
     * @return The height.
     */
    unsigned getHeight() const
    {
        return height;
    }

    /**
     * @fn bool RenderingBuffer::inbox(int x, int y) const
     *
     * @brief Tests if a given coordinate is inside the RenderingBuffer.
     *
     *        Tests if a given coordinate is inside the RenderingBuffer.
     *
     * @param x The x coordinate.
     * @param y The y coordinate.
     *
     * @return true if (x,y) is inside the RenderingBuffer, false otherwise.
     */
    bool inbox(int x, int y) const
    {
        return x >= 0 && y >= 0 && x < int(width) && y < int(height);
    }

    /**
     * @fn unsigned char* RenderingBuffer::row(unsigned y)
     *
     * @brief Gets a pointer to the given row in the RenderingBuffer.
     *
     *        Gets a pointer to the given row in the RenderingBuffer.
     *
     * @param y The line number, ie the row.
     *
     * @return The pointer to the start of the given line in the RenderingBuffer.
     */
    unsigned char* row(unsigned y)
    {
        return buf + stride * y;
    }

    /**
     * @fn const unsigned char* RenderingBuffer::row(unsigned y) const
     *
     * @brief Gets a pointer to the given row in the RenderingBuffer.
     *
     *        Gets a pointer to the given row in the RenderingBuffer.
     *
     * @param y The line number, ie the row.
     *
     * @return The pointer to the start of the given line in the RenderingBuffer.
     */
    const unsigned char* row(unsigned y) const
    {
        return buf + stride * y;
    }

private:
    RenderingBuffer(const RenderingBuffer&);

    unsigned char*  buf;
    unsigned char   xAdjust;
    unsigned        width;
    unsigned        height;
    int             stride;
};

} // namespace touchgfx

#endif // RENDERINGBUFFER_HPP
