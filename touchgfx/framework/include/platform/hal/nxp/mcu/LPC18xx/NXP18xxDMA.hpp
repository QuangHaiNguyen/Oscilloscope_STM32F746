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
#ifndef NXP18XXDMA_HPP
#define NXP18XXDMA_HPP

#include <touchgfx/hal/DMA.hpp>
#include <lpc18xx_gpdma.h>

#ifndef NO_USING_NAMESPACE_TOUCHGFX
using namespace touchgfx;
#endif

/**
 * @class NXP18XXDMA NXP18xxDMA.hpp platform/hal/nxp/mcu/LPC18xx/NXP18xxDMA.hpp
 *
 * @brief This class specializes DMA_Interface for the NXP 18xx processor.
 *
 *        This class specializes DMA_Interface for the NXP 18xx processor.
 *
 * @sa DMA_Interface
 */
class NXP18XXDMA : public DMA_Interface
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
     * @fn NXP18XXDMA::NXP18XXDMA(GPDMA_LLI_Type* lliMemory);
     *
     * @brief Constructor.
     *
     *        Constructor.
     *
     * @param [in,out] lliMemory Pointer to an array containing the number of elements equivalent
     *                           to the height of the display in pixels.
     */
    NXP18XXDMA(GPDMA_LLI_Type* lliMemory);

    /**
     * @fn NXP18XXDMA::~NXP18XXDMA();
     *
     * @brief Destructor.
     *
     *        Destructor.
     */
    ~NXP18XXDMA();

    /**
     * @fn virtual BlitOperations NXP18XXDMA::getBlitCaps()
     *
     * @brief Gets blit capabilities.
     *
     *        Gets blit capabilities.
     *
     * @return the blit capabilities of this DMA (BLIT_OP_COPY).
     */
    virtual BlitOperations getBlitCaps()
    {
        return static_cast<BlitOperations>(BLIT_OP_COPY | BLIT_OP_FILL);
    }

    /**
     * @fn void NXP18XXDMA::initialize();
     *
     * @brief Perform hardware specific initialization.
     *
     *        Perform hardware specific initialization.
     */
    void initialize();

    /**
     * @fn uint8_t NXP18XXDMA::isDmaQueueEmpty();
     *
     * @brief Determines whether or not the DMA queue is empty.
     *
     *        Determines whether or not the DMA queue is empty.
     *
     * @return 1 if DMA queue is empty, else 0.
     */
    uint8_t isDmaQueueEmpty();

    /**
     * @fn void NXP18XXDMA::signalDMAInterrupt();
     *
     * @brief Signal DMA interrupt.
     *
     *        Signal DMA interrupt.
     *
     * @note Should be from DMA interrupt handler.
     */
    void signalDMAInterrupt();

protected:
    /**
     * @fn virtual void NXP18XXDMA::setupDataCopy(const BlitOp& blitOp);
     *
     * @brief Configures the DMA for copying data to the frame-buffer.
     *
     *        Configures the DMA for copying data to the frame-buffer.
     *
     * @param blitOp Details on the copy to perform.
     */
    virtual void setupDataCopy(const BlitOp& blitOp);

    /**
     * @fn virtual void NXP18XXDMA::setupDataFill(const BlitOp& blitOp);
     *
     * @brief Configures the DMA for "filling" the frame-buffer with a single color.
     *
     *        Configures the DMA for "filling" the frame-buffer with a single color.
     *
     * @param blitOp Details on the "fill" to perform.
     */
    virtual void setupDataFill(const BlitOp& blitOp);

private:
    /**
     * @fn bool NXP18XXDMA::finished();
     *
     * @brief Interrupt call-back: Used for DMA transfers, which are known to complete the current
     *        BlitOp processing.
     *
     *        Interrupt call-back: Used for DMA transfers, which are known to complete the
     *        current BlitOp processing.
     *
     * @return true always (since the BlitOp processing has been completed).
     */
    bool finished();

    static const unsigned int channel = 0; ///< The used DMA channel. Only channel 0 supports 2D transfers

    GPDMA_LLI_Type*   MyLLI;        ///< GPDMA container for LPC43xx;
    LockFreeDMA_Queue dma_queue;
    BlitOp            queue_storage[96];
    const BlitOp*     pBlitOp;      ///< Reference to current BlitOp (the queue is in-accessible from outside DMA_Interface)
    uint16_t          numLines;     ///< Number of lines remaining in current transfer.
    unsigned          use32Bit : 1; ///< set to non-zero when using 32bit transfers (optimization)
};

#endif // NXP18XXDMA_HPP
