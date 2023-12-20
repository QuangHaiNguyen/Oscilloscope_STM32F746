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
#ifndef OSWRAPPERS_HPP
#define OSWRAPPERS_HPP

#include <touchgfx/hal/Types.hpp>

namespace touchgfx
{
/**
 * @class OSWrappers OSWrappers.hpp touchgfx/hal/OSWrappers.hpp
 *
 * @brief This class specifies OS wrappers for dealing with the frame buffer semaphore and the
 *        VSYNC signal.
 *
 *        This class specifies OS wrappers for dealing with the frame buffer semaphore and the
 *        VSYNC signal.
 */
class OSWrappers
{
public:

    /**
     * @fn static void OSWrappers::initialize();
     *
     * @brief Initialize frame buffer semaphore and queue/mutex for VSYNC signal.
     *
     *        Initialize frame buffer semaphore and queue/mutex for VSYNC signal.
     */
    static void initialize();

    /**
     * @fn static void OSWrappers::signalVSync();
     *
     * @brief Signal that a VSYNC has occurred.
     *
     *        Signal that a VSYNC has occurred. Should make the vsync queue/mutex available.
     *
     * @note This function is called from an ISR, and should (depending on OS) trigger a
     *       scheduling.
     */
    static void signalVSync();

    /**
     * @fn static void OSWrappers::waitForVSync();
     *
     * @brief This function blocks until a VSYNC occurs.
     *
     *        This function blocks until a VSYNC occurs.
     *
     * @note This function must first clear the mutex/queue and then wait for the next one to
     *       occur.
     */
    static void waitForVSync();

    /**
     * @fn static void OSWrappers::takeFrameBufferSemaphore();
     *
     * @brief Take the frame buffer semaphore.
     *
     *        Take the frame buffer semaphore. Blocks until semaphore is available.
     */
    static void takeFrameBufferSemaphore();

    /**
     * @fn static void OSWrappers::tryTakeFrameBufferSemaphore();
     *
     * @brief Attempt to obtain the frame buffer semaphore.
     *
     *        Attempt to obtain the frame buffer semaphore. If semaphore is not available, do
     *        nothing.
     *
     * @note must return immediately! This function does not care who has the taken the semaphore,
     *       it only serves to make sure that the semaphore is taken by someone.
     */
    static void tryTakeFrameBufferSemaphore();

    /**
     * @fn static void OSWrappers::giveFrameBufferSemaphore();
     *
     * @brief Release the frame buffer semaphore.
     *
     *        Release the frame buffer semaphore.
     */
    static void giveFrameBufferSemaphore();

    /**
     * @fn static void OSWrappers::giveFrameBufferSemaphoreFromISR();
     *
     * @brief Release the frame buffer semaphore in a way that is safe in interrupt context. Called
     *        from ISR.
     *
     *        Release the frame buffer semaphore in a way that is safe in interrupt context.
     *        Called from ISR.
     */
    static void giveFrameBufferSemaphoreFromISR();

    /**
     * @fn static void taskDelay(uint16_t ms);
     *
     * @brief A function that causes executing task to sleep for a number of milliseconds.
     *
     *        A function that causes executing task to sleep for a number of milliseconds.
     *        This function is OPTIONAL. It is only used by the TouchGFX in the case of
     *        a specific frame refresh strategy (REFRESH_STRATEGY_OPTIM_SINGLE_BUFFER_TFT_CTRL).
     *        Due to backwards compatibility, in order for this function to be useable by the HAL
     *        the function must be explicitly registered:
     *          hal.registerTaskDelayFunction(&OSWrappers::taskDelay)
     * @param ms The number of milliseconds to sleep
     * @see HAL::setFrameRefreshStrategy(FrameRefreshStrategy s)
     * @see HAL::registerTaskDelayFunction(void (*delayF)(uint16_t))
     */
    static void taskDelay(uint16_t ms);
};

} // namespace touchgfx
#endif // OSWRAPPERS_HPP
