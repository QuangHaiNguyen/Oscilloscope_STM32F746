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
#ifndef SCANLINE_HPP
#define SCANLINE_HPP

#include <touchgfx/canvas_widget_renderer/CanvasWidgetRenderer.hpp>

namespace touchgfx
{
/**
 * @class Scanline Scanline.hpp touchgfx/canvas_widget_renderer/Scanline.hpp
 *
 * @brief This class is used to transfer data from class Outline (or a similar one)
 *        to the rendering buffer.
 *
 *        This class is used to transfer data from class Outline (or a similar one)
 *        to the rendering buffer. It's organized very simple. The class stores information of
 *        horizontal spans to render it into a pixel-map buffer. Each span has initial X,
 *        length, and an array of bytes that determine the alpha values for each pixel. So, the
 *        restriction of using this class is 256 levels of Anti-Aliasing, which is quite enough
 *        for any practical purpose. Before using this class you should know the minimal and
 *        maximal pixel coordinates of your scanline. The protocol of using is: 1. reset()
 *        2. addCell() / addSpan() - accumulate scanline. You pass y coordinate
 *           into these functions in order to make scanline know the last Y. Before calling
 *           addCell() / addSpan() you should check with method isReady(y)
 *           if the last Y has changed. It also checks if the scanline is not empty. When
 *           forming one scanline the next x coordinate must be always greater than the last
 *           stored one, i.e. it works only with ordered coordinates.
 *        3. If the current scanline isReady() you should render it and then call
 *           resetSpans() before adding new cells/spans.
 *        4. Rendering:
 *
 *        Scanline provides an iterator class that allows you to extract the spans and the
 *        cover values for each pixel. Be aware that clipping has not been done yet, so you
 *        should perform it yourself. Use Scanline::iterator to render spans:
 *        -------------------------------------------------------------------------
 *
 *        int baseX = scanline.getBaseX();   // base X. Should be added to the span's X
 *                                           // "scanline" is a const reference to the //
 *                                           scanline passed in.
 *
 *        int y = scanline.y();                    // y coordinate of the scanline
 *
 *        ************************************
 *        ...Perform vertical clipping here...
 *        ************************************
 *
 *        Scanline::iterator span(scanline);
 *
 *        unsigned char* row = renderingBuffer->row(y); // The the address of the beginning
 *                                             // of the current row
 *
 *        unsigned num_spans = scanline.getNumSpans(); // Number of spans. It's guaranteed that
 *                                             // numSpans is always greater than 0.
 *
 *        do
 *        {
 *            int x = span.next() + baseX;        // The beginning X of the span
 *
 *            const int8u covers* = span.getCovers(); // The array of the cover values
 *
 *            int numPix = span.getNumPix();       // Number of pixels of the span.
 *                                                 // Always greater than 0, still we // should
 *                                                 use "int" instead of // "unsigned" because
 *                                                 it's more // convenient for clipping
 *
 *            **************************************
 *            ...Perform horizontal clipping here...
 *            ...you have x, covers, and pix_Fromcount...
 *            **************************************
 *
 *            unsigned char* dst = row + x;  // Calculate the start address of the row.
 *                                           // In this case we assume a simple // grayscale
 *                                           image 1-byte per pixel.
 *            do
 *            {
 *                *dst++ = *covers++;        // Hypotetical rendering.
 *            }
 *            while (--numPix);
 *        }
 *        while (--numSpans);  // numSpans cannot be 0, so this loop is quite safe
 *
 *        The question is: why should we accumulate the whole scanline when we could render
 *        just separate spans when they're ready? That's because using the scanline is in
 *        general faster. When is consists of more than one span the conditions for the
 *        processor cash system are better, because switching between two different areas of
 *        memory (that can be large ones) occurs less frequently.
 */
class Scanline
{
public:
    enum { AA_SHIFT = 8 };

    /**
     * @class iterator Scanline.hpp touchgfx/canvas_widget_renderer/Scanline.hpp
     *
     * @brief An iterator to help go through all the elements that make up a Scanline.
     *
     *        An iterator to help go through all the elements that make up a Scanline. Each
     *        part of the Scanline has a different Cover.
     */
    class iterator
    {
    public:

        /**
         * @fn iterator::iterator(const Scanline& scanline)
         *
         * @brief Constructor.
         *
         *        Constructor. Creates an iterator to help go through all the Scanline parts of
         *        the polygon on a single Scanline.
         *
         * @param scanline The scanline to iterate.
         */
        iterator(const Scanline& scanline) :
            covers(scanline.covers),
            curCount(scanline.counts),
            curStartIndex(scanline.startIndices)
        {
        }

        /**
         * @fn int iterator::next()
         *
         * @brief Gets the next element on the Scanline.
         *
         *        Gets the next element on the Scanline.
         *
         * @return An the next index in the array of Scanline elements.
         */
        int next()
        {
            ++curCount;
            ++curStartIndex;
            return int(*curStartIndex);
        }

        /**
         * @fn int iterator::getNumPix() const
         *
         * @brief Gets number of consecutive pixels in the current run on the Scanline.
         *
         *        Gets number of consecutive pixels in the current run on the Scanline.
         *
         * @return The number of consecutive pixels.
         */
        int getNumPix() const
        {
            return int(*curCount);
        }

        /**
         * @fn const uint8_t* iterator::getCovers() const
         *
         * @brief Gets the covers in the current run on the Scanline.
         *
         *        Gets the covers in the current run on the Scanline.
         *
         * @return array of covers of each individual pixel.
         */
        const uint8_t* getCovers() const
        {
            return covers + *curStartIndex;
        }

    private:
        const uint8_t*  covers;
        const uint16_t* curCount;
        const uint16_t* curStartIndex;
    };

    friend class iterator;

    /**
     * @fn Scanline::Scanline();
     *
     * @brief Default constructor.
     *
     *        Default constructor. Initiate a Scanline by setting up pointers to store covers,
     *        and counts.
     */
    Scanline();

    /**
     * @fn Scanline::~Scanline()
     *
     * @brief Destructor.
     *
     *        Destructor.
     */
    virtual ~Scanline() { }

    /**
     * @fn void Scanline::reset();
     *
     * @brief Resets the Scanline object in preparation for the handling the next Scanline.
     *
     *        Resets the Scanline object in preparation for the handling the next Scanline.
     */
    void reset();

    /**
     * @fn void Scanline::resetSpans();
     *
     * @brief Resets the spans in preparation for the next Scanline.
     *
     *        Resets the spans in preparation for the next Scanline. Identical to calling
     *        reset()
     *        without changing the dx_ and dy_ parameters from the previous call to reset().
     */
    void resetSpans();

    /**
     * @fn void Scanline::addCell(int x, int y, unsigned cover);
     *
     * @brief Adds a single cell to the current Scanline.
     *
     *        Adds a single cell to the current Scanline. Works just like invoking addSpan()
     *        with a len=1.
     *
     * @param x     The x coordinate.
     * @param y     The y coordinate.
     * @param cover The cover.
     */
    void addCell(int x, int y, unsigned cover);

    /**
     * @fn void Scanline::addSpan(int x, int y, unsigned len, unsigned cover);
     *
     * @brief Adds a span of cells to the current Scanline.
     *
     *        Adds a span of cells to the current Scanline. Works like calling addCell() len
     *        times.
     *
     * @param x     The x coordinate.
     * @param y     The y coordinate.
     * @param len   The length.
     * @param cover The cover.
     */
    void addSpan(int x, int y, unsigned len, unsigned cover);

    /**
     * @fn int Scanline::isReady(int y) const;
     *
     * @brief Checks if a Scanline is ready for rendering.
     *
     *        Checks if a Scanline is ready for rendering. A Scanline is ready for rendering
     *        when the y coordinate has changed. Since all the cells are sorted, a change in
     *        the y coordinate means that we have moved to the next Scanline and thus the
     *        collected data for the Scanline must be rendered before we register cells for the
     *        next Scanline.
     *
     * @param y The y coordinate.
     *
     * @return True if the given y coordinate differs from the y coordinate for the cells in the
     *         current Scanline.
     */
    int isReady(int y) const;

    /**
     * @fn int Scanline::getY() const
     *
     * @brief Gets y coordinate, i.e. the vertical offset of the Scanline.
     *
     *        Gets y coordinate, i.e. the vertical offset of the Scanline. This allows easy
     *        positioning of the Outline. The y coordinate is setup through function reset().
     *
     * @return The y coordinate.
     */
    int getY() const
    {
        return lastY;
    }

    /**
     * @fn unsigned Scanline::getNumSpans() const
     *
     * @brief Gets number spans in the Scanline.
     *
     *        Gets number spans in the Scanline.
     *
     * @return The number spans.
     */
    unsigned getNumSpans() const
    {
        return numSpans;
    }

private:
    Scanline(const Scanline&);
    const Scanline& operator = (const Scanline&);

    int       lastX;
    int       lastY;
    unsigned  numSpans;
    uint16_t* curStartIndex;
    uint16_t* curCount;

    uint8_t*  covers;
    uint16_t* startIndices;
    uint16_t* counts;
};

FORCE_INLINE_FUNCTION void Scanline::resetSpans()
{
    lastX         = 0x7FFF;
    lastY         = 0x7FFF;
    curCount      = counts;
    curStartIndex = startIndices;
    numSpans      = 0;
}

FORCE_INLINE_FUNCTION void Scanline::addCell(int x, int y, unsigned cover)
{
    if (x < 0)
    {
        // Starts before scanline start
        return;
    }
    if (unsigned(x) >= CanvasWidgetRenderer::getScanlineWidth())
    {
        // Starts after scanline end
        return;
    }

    covers[x] = (unsigned char)cover;
    if (x == lastX + 1)
    {
        (*curCount)++;
    }
    else
    {
        *++curCount = 1;
        *++curStartIndex = x;
        numSpans++;
    }
    lastX = x;
    lastY = y;
}

FORCE_INLINE_FUNCTION int Scanline::isReady(int y) const
{
    return numSpans && (y ^ lastY);
}

} // namespace touchgfx

#endif // SCANLINE_HPP
