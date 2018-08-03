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
#ifndef NXP1788DMA_HPP
#define NXP1788DMA_HPP

#include <touchgfx/hal/DMA.hpp>
#include "lpc177x_8x_gpdma.h"

#ifndef NO_USING_NAMESPACE_TOUCHGFX
using namespace touchgfx;
#endif

/**
 * @class NXP1788DMA NXP1788DMA.hpp platform/hal/nxp/mcu/LPC17xx/NXP1788DMA.hpp
 *
 * @brief This class specializes DMA_Interface for the NXP 1788 processor.
 *
 *        This class specializes DMA_Interface for the NXP 1788 processor.
 *
 * @sa DMA_Interface
 */
class NXP1788DMA : public DMA_Interface
{
    /**
     * @typedef DMA_Interface Base
     *
     * @brief Defines an alias representing the base.
     *
     *        Defines an alias representing the base.
     */
    typedef DMA_Interface Base;

public:
    /**
     * @fn NXP1788DMA::NXP1788DMA(GPDMA_LLI_Type* lliMemory);
     *
     * @brief Constructor.
     *
     *        Constructor.
     *
     * @param [in,out] lliMemory Pointer to an array containing the number of elements equivalent
     *                           to the height of the display in pixels.
     */
    NXP1788DMA(GPDMA_LLI_Type* lliMemory);

    /**
     * @fn NXP1788DMA::~NXP1788DMA();
     *
     * @brief Destructor.
     *
     *        Destructor.
     */
    ~NXP1788DMA();

    /**
     * @fn virtual BlitOperations NXP1788DMA::getBlitCaps()
     *
     * @brief Gets blit capabilities.
     *
     *        Gets blit capabilities.
     *
     * @return the blit operation capabilities of this DMA (BLIT_OP_COPY)
     */
    virtual BlitOperations getBlitCaps()
    {
        return static_cast<BlitOperations>(BLIT_OP_COPY | BLIT_OP_FILL);
    }

    /**
     * @fn void NXP1788DMA::initialize();
     *
     * @brief Perform hardware specific initialization.
     *
     *        Perform hardware specific initialization.
     */
    void initialize();

    /**
     * @fn void NXP1788DMA::signalDMAInterrupt();
     *
     * @brief Raises a DMA interrupt signal.
     *
     *        Raises a DMA interrupt signal.
     */
    void signalDMAInterrupt();

protected:
    /**
     * @fn virtual void NXP1788DMA::setupDataCopy(const BlitOp& blitOp);
     *
     * @brief Configures the DMA for copying data to the frame-buffer.
     *
     *        Configures the DMA for copying data to the frame-buffer.
     *
     * @param blitOp Details on the copy to perform.
     */
    virtual void setupDataCopy(const BlitOp& blitOp);

    /**
     * @fn virtual void NXP1788DMA::setupDataFill(const BlitOp& blitOp);
     *
     * @brief Configures the DMA for "filling" the frame-buffer with a single color.
     *
     *        Configures the DMA for "filling" the frame-buffer with a single color.
     *
     * @param blitOp Details on the "fill" to perform.
     */
    virtual void setupDataFill(const BlitOp& blitOp);

private:
    static const unsigned int channel = 0; ///< The used DMA channel. Only channel 0 supports 2D transfers

    GPDMA_LLI_Type*   MyLLI;        ///< GPDMA scatter-gather linked list memory
    LockFreeDMA_Queue dma_queue;
    BlitOp            queue_storage[96];
    const BlitOp*     pBlitOp;      ///< Reference to current BlitOp (the queue is in-accessible from outside DMA_Interface)
    uint16_t          numLines;     ///< Number of lines remaining in current transfer.
    unsigned          use32Bit : 1; ///< set to non-zero when using 32bit transfers (optimization)
};

#endif // NXP1788DMA_HPP
