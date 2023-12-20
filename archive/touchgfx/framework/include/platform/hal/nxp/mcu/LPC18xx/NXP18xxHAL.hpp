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
#ifndef NXP18XXHAL_HPP
#define NXP18XXHAL_HPP

#include <touchgfx/hal/HAL.hpp>
#include <platform/hal/nxp/mcu/LPC18xx/NXP18xxDMA.hpp>

#include <platform/driver/touch/TouchController.hpp>

/**
 * @class NXP18XXHAL NXP18xxHAL.hpp platform/hal/nxp/mcu/LPC18xx/NXP18xxHAL.hpp
 *
 * @brief HAL implementation for NXP LPC18XX.
 *
 *        HAL implementation for NXP LPC18XX.
 *
 * @sa HAL
 */
class NXP18XXHAL : public HAL
{
public:
    /**
     * @fn NXP18XXHAL::NXP18XXHAL(DMA_Interface& dma, LCD& display, TouchController& tc, uint16_t width, uint16_t height)
     *
     * @brief Constructor.
     *
     *        Constructor. Initializes members.
     *
     * @param [in,out] dma     Reference to DMA interface.
     * @param [in,out] display Reference to the LCD display.
     * @param [in,out] tc      Reference to Touch Controller driver.
     * @param width            Width of the display.
     * @param height           Height of the display.
     */
    NXP18XXHAL(DMA_Interface& dma, LCD& display, TouchController& tc, uint16_t width, uint16_t height) : HAL(dma, display, tc, width, height)
    {
    }

    /**
     * @fn virtual void NXP18XXHAL::configureInterrupts();
     *
     * @brief Sets the DMA and LCD interrupt priorities.
     *
     *        Sets the DMA and LCD interrupt priorities.
     */
    virtual void configureInterrupts();

    /**
     * @fn virtual void NXP18XXHAL::enableLCDControllerInterrupt();
     *
     * @brief Enables the LCD interrupt in the LCD controller.
     *
     *        Enables the LCD interrupt in the LCD controller.
     */
    virtual void enableLCDControllerInterrupt();

    /**
     * @fn virtual void NXP18XXHAL::enableInterrupts();
     *
     * @brief Enables the DMA and LCD interrupts.
     *
     *        Enables the DMA and LCD interrupts.
     */
    virtual void enableInterrupts();

    /**
     * @fn virtual void NXP18XXHAL::disableInterrupts();
     *
     * @brief Disables the DMA and LCD interrupts.
     *
     *        Disables the DMA and LCD interrupts.
     */
    virtual void disableInterrupts();

    /**
     *  @fn virtual void NXP18XXHAL::backPorchExited()
     *
     * @brief Reverses the order of tick and swap for this specific platform.
     *
     *        Reverses the order of tick and swap for this specific platform. Has to be called
     *        from within the LCD IRQ routine when the Back Porch Exit is reached.
     */
    virtual void backPorchExited()
    {
        tick();
        swapFrameBuffers();
    }

protected:
    /**
     * @fn virtual uint16_t* NXP18XXHAL::getTFTFrameBuffer() const;
     *
     * @brief Gets the frame buffer address used by the TFT controller.
     *
     *        Gets the frame buffer address used by the TFT controller.
     *
     * @return The address of the frame buffer currently being displayed on the TFT.
     */
    virtual uint16_t* getTFTFrameBuffer() const;

    /**
     * @fn virtual void NXP18XXHAL::setTFTFrameBuffer(uint16_t* adr);
     *
     * @brief Sets the frame buffer address used by the TFT controller.
     *
     *        Sets the frame buffer address used by the TFT controller.
     *
     * @param [in,out] adr New frame buffer address.
     */
    virtual void setTFTFrameBuffer(uint16_t* adr);
};

#endif // NXP18XXHAL_HPP
