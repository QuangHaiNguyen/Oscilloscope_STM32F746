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
#ifndef TOUCHGFXINIT_HPP
#define TOUCHGFXINIT_HPP

#include <touchgfx/hal/HAL.hpp>
#include <touchgfx/hal/DMA.hpp>
#include <platform/driver/touch/TouchController.hpp>

#include <texts/TypedTextDatabase.hpp>
#include <fonts/ApplicationFontProvider.hpp>
#include <gui/common/FrontendHeap.hpp>
#include <BitmapDatabase.hpp>

static ApplicationFontProvider fontProvider;

/**
 * @brief The global touchgfx namespace. All TouchGFX framework classes and global functions are placed in this namespace.
 */
namespace touchgfx
{

/// @cond

static Texts texts; ///< The texts

template <class T>
HAL& getHAL(DMA_Interface& dma, LCD& display, TouchController& tc, int16_t width, int16_t height)
{
    static T hal(dma, display, tc, width, height);
    return hal;
}
/// @endcond

/**
 * @globalfn
 */

/**
 * @fn template <class HALType> HAL& touchgfx_generic_init(DMA_Interface& dma, LCD& display, TouchController& tc, int16_t width, int16_t height, uint16_t* bitmapCache, uint32_t bitmapCacheSize, uint32_t numberOfDynamicBitmaps = 0)
 *
 * @brief TouchGFX generic initialize.
 *
 *        TouchGFX generic initialize.
 *
 * @tparam HALType The class type of the HAL subclass used for this port.
 * @param [in] dma               Reference to the DMA implementation object to use. Can be of type
 *                               NoDMA to disable the use of DMA for rendering.
 * @param [in] display           Reference to the LCD renderer implementation (subclass of LCD).
 *                               Could be either LCD16bpp for RGB565 UIs, or LCD1bpp for
 *                               monochrome UIs or LCD24bpp for 24bit displays using RGB888 UIs.
 * @param [in] tc                Reference to the touch controller driver (or NoTouchController to
 *                               disable touch input).
 * @param width                  The \a native display width of the actual display, in pixels.
 *                               This value is irrespective of whether the concrete UI should be
 *                               portrait or landscape mode. It must match what the display itself
 *                               is configured as.
 * @param height                 The \a native display height of the actual display, in pixels.
 *                               This value is irrespective of whether the concrete UI should be
 *                               portrait or landscape mode. It must match what the display itself
 *                               is configured as.
 * @param [in] bitmapCache       Optional pointer to starting address of a memory region in which
 *                               to place the bitmap cache. Usually in external RAM. Pass 0 if
 *                               bitmap caching is not used.
 * @param bitmapCacheSize        Size of bitmap cache in bytes. Pass 0 if bitmap cache is not used.
 * @param numberOfDynamicBitmaps Number of dynamic bitmaps.
 *
 * @return A reference to the allocated (and initialized) HAL object.
 */
template <class HALType>
HAL& touchgfx_generic_init(DMA_Interface& dma, LCD& display, TouchController& tc, int16_t width, int16_t height,
                           uint16_t* bitmapCache, uint32_t bitmapCacheSize, uint32_t numberOfDynamicBitmaps = 0)
{
    HAL& hal = getHAL<HALType>(dma, display, tc, width, height);
    hal.initialize();

    Bitmap::registerBitmapDatabase(BitmapDatabase::getInstance(),
                                   BitmapDatabase::getInstanceSize(),
                                   bitmapCache,
                                   bitmapCacheSize,
                                   numberOfDynamicBitmaps);

    TypedText::registerTexts(&texts);
    Texts::setLanguage(0);

    FontManager::setFontProvider(&fontProvider);

    FrontendHeap& heap = FrontendHeap::getInstance();
    (void)heap; // we need to obtain the reference above to initialize the frontend heap.

    hal.registerEventListener(*(Application::getInstance()));

    return hal;
}

} // namespace touchgfx
#endif // TOUCHGFXINIT_HPP
