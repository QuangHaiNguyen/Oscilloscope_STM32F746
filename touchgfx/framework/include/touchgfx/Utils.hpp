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
#ifndef UTILS_HPP
#define UTILS_HPP

#include <touchgfx/hal/Types.hpp>
#include <touchgfx/Bitmap.hpp>

namespace touchgfx
{
/**
 * Provides utility functions.
 */

/**
 * @fn void memset(void* data, uint8_t c, uint32_t size);
 *
 * @brief Simple implementation of the standard memset function.
 *
 *        Simple implementation of the standard memset function.
 *
 * @param [out] data Address of data to set.
 * @param c          Value to set.
 * @param size       Number of bytes to set.
 */
void memset(void* data, uint8_t c, uint32_t size);

/**
 * @fn RenderingVariant lookupNearestNeighborRenderVariant(const Bitmap& bitmap);
 *
 * @brief Returns the associated nearest neighbor render variant based on the bitmap format.
 *
 *        Returns the associated nearest neighbor render variant based on the bitmap format.
 *
 * @param bitmap The bitmap.
 *
 * @return A RenderingVariant based on the bitmap format.
 */
RenderingVariant lookupNearestNeighborRenderVariant(const Bitmap& bitmap);

/**
 * @fn RenderingVariant lookupBilinearRenderVariant(const Bitmap& bitmap);
 *
 * @brief Returns the associated bilinear render variant based on the bitmap format.
 *
 *        Returns the associated bilinear render variant based on the bitmap format.
 *
 * @param bitmap The bitmap.
 *
 * @return A RenderingVariant based on the bitmap format.
 */
RenderingVariant lookupBilinearRenderVariant(const Bitmap& bitmap);

/**
 * @fn template <class T> T abs(T d)
 *
 * @brief Simple implementation of the standard abs function
 *
 *        Simple implementation of the standard abs function.
 *
 * @tparam T The class type on which to perform the abs.
 * @param d The entity on which to perform the abs.
 *
 * @return The absolute (non-negative) value of d.
 */
template <class T>
T abs(T d)
{
    if (d < 0)
    {
        return -d;
    }
    return d;
}

} // namespace touchgfx

#endif // UTILS_HPP
