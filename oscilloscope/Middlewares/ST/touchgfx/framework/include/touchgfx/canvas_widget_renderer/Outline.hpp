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

/**
 * @file touchgfx/canvas_widget_renderer/Outline.hpp
 *
 * Declares the touchgfx::Outline class. Used internally by CanvasWidgetRenderer.
 */
#ifndef TOUCHGFX_OUTLINE_HPP
#define TOUCHGFX_OUTLINE_HPP

#include <touchgfx/canvas_widget_renderer/Cell.hpp>

/// @cond
namespace touchgfx
{
/**
 * An internal class that implements the main rasterization algorithm. Used in the Rasterizer.
 * Should not be used directly.
 */
class Outline
{
public:
    /** Defines an alias representing the outline flags. */
    typedef unsigned int OutlineFlags_t;

    static const OutlineFlags_t OUTLINE_NOT_CLOSED = 1U;    ///< If this bit is set in flags, the current Outline has not yet been closed. Used for automatic closing an Outline before rendering the Outline.
    static const OutlineFlags_t OUTLINE_SORT_REQUIRED = 2U; ///< If this bit is set in flags, Cell objects have been added to the Outline requiring the Cell list needs to be sorted.

    /** Initializes a new instance of the Outline class. */
    Outline();

    /** Finalizes an instance of the Outline class. */
    virtual ~Outline();

    /**
     * Resets this object. This implies removing the current Cell objects and preparing for
     * a new Outline.
     */
    void reset();

    /**
     * Move a virtual pen to the specified coordinate.
     *
     * @param  x The x coordinate.
     * @param  y The y coordinate.
     */
    void moveTo(int x, int y);

    /**
     * Create a line from the current virtual pen coordinate to the given coordinate
     * creating an Outline.
     *
     * @param  x The x coordinate.
     * @param  y The y coordinate.
     */
    void lineTo(int x, int y);

    /**
     * Gets number of cells registered in the current drawn path for the Outline.
     *
     * @return The number of cells.
     */
    unsigned getNumCells() const
    {
        return numCells;
    }

    /**
     * Gets a pointer to the the Cell objects in the Outline. If the Outline is not closed,
     * it is closed. If the Outline is unsorted, it will be quick sorted first.
     *
     * @return A pointer to the sorted list of Cell objects in the Outline.
     */
    const Cell* closeOutlineAndSortCells();

    /**
     * Sets maximum render y coordinate. This is used to avoid registering any Cell that has
     * a y coordinate less than zero of higher than the given y.
     *
     * @param  y The max y coordinate to render for the Outline.
     */
    void setMaxRender(int x, int y)
    {
        maxRenderX = x;
        maxRenderY = y;
    }

    /**
     * Determines if there was enough memory to register the entire outline, of if the
     * outline was too complex.
     *
     * @return false if the buffer for Outline Cell objects was too small.
     */
    FORCE_INLINE_FUNCTION bool wasOutlineTooComplex()
    {
        return numCells > maxCells;
    }

private:
    /**
     * Sets coordinate of the current Cell.
     *
     * @param  x The x coordinate.
     * @param  y The y coordinate.
     */
    void setCurCell(int x, int y);

    /**
     * Sets coordinate of the current Cell without checking if x,y differs from the curCell's x,y.
     *
     * @param  x The x coordinate.
     * @param  y The y coordinate.
     */
    void setCurCellNew(int x, int y);

    /** Adds current cell to the Outline. */
    void addCurCell();

    /**
     * Render the scanline in the special case where the line is on the same scanline,
     * though it might not be 100% horizontal as the fraction of the y endpoints might
     * differ making the line tilt ever so slightly.
     *
     * @param  ey The entire part of the from/to y coordinate (same for both from y and to y as
     *            it is a horizontal scanline).
     * @param  x1 The from x coordinate in poly format.
     * @param  y1 The from y coordinate fraction part in poly format.
     * @param  x2 The to x coordinate in poly format.
     * @param  y2 The to y coordinate fraction part in poly format.
     */
    void renderScanline(int ey, int x1, int y1, int x2, int y2);

    /**
     * Render the line. This is the general case which handles all cases regardless of the
     * position of the from coordinate and the to coordinate.
     *
     * @param  x1 The from x coordinate in poly format.
     * @param  y1 The from y coordinate in poly format.
     * @param  x2 The to x coordinate in poly format.
     * @param  y2 The to y coordinate in poly format.
     */
    void renderLine(int x1, int y1, int x2, int y2);

    FORCE_INLINE_FUNCTION static void swapCells(Cell* a, Cell* b)
    {
        const Cell temp = *a;
        *a = *b;
        *b = temp;
    }

    FORCE_INLINE_FUNCTION static bool lessThan(const Cell* a, const Cell* b)
    {
        return a->y < b->y || (a->y == b->y && a->x < b->x);
    }

    /**
     * Quick sort Outline cells.
     *
     * @param [in] start The first Cell object in the Cell array to sort.
     * @param      num   Number of Cell objects to sort.
     */
    static void qsortCells(Cell* const start, unsigned num);

    unsigned maxCells;
    unsigned numCells;
    Cell* cells;
    Cell* curCellPtr;
    int curX;
    int curY;
    int closeX;
    int closeY;
    unsigned int flags;
    int maxRenderX;
    int maxRenderY;
#ifdef SIMULATOR
    unsigned maxNumCells;
#endif
};

} // namespace touchgfx

/// @endcond

#endif // TOUCHGFX_OUTLINE_HPP
