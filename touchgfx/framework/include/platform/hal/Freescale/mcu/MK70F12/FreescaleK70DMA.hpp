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
#ifndef FREESCALEK70DMA_HPP
#define FREESCALEK70DMA_HPP

#include <touchgfx/hal/DMA.hpp>

#ifndef NO_USING_NAMESPACE_TOUCHGFX
using namespace touchgfx;
#endif

/**
 * @class FreescaleK70DMA FreescaleK70DMA.hpp platform/hal/Freescale/mcu/MK70F12/FreescaleK70DMA.hpp
 *
 * @brief This class specializes DMA_Interface for the Freescale K70 processors.
 *
 *        This class specializes DMA_Interface for the Freescale K70 processors.
 *
 * @sa DMA_Interface
 */
class FreescaleK70DMA : public DMA_Interface
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
     * @fn FreescaleK70DMA::FreescaleK70DMA();
     *
     * @brief Default constructor.
     *
     *        Default constructor.
     */
    FreescaleK70DMA();

    /**
     * @fn FreescaleK70DMA::~FreescaleK70DMA();
     *
     * @brief Destructor.
     *
     *        Destructor.
     */
    ~FreescaleK70DMA();

    /**
     * @fn virtual BlitOperations FreescaleK70DMA::getBlitCaps()
     *
     * @brief Gets the blit operation capabilities of this DMA.
     *
     *        Gets the blit operation capabilities of this DMA.
     *
     * @return BLIT_OP_COPY.
     */
    virtual BlitOperations getBlitCaps()
    {
        return static_cast<BlitOperations>(BLIT_OP_COPY);
    }

    /**
     * @fn void FreescaleK70DMA::initialize();
     *
     * @brief Perform hardware specific initialization.
     *
     *        Perform hardware specific initialization.
     */
    void initialize();

    /**
     * @fn uint8_t FreescaleK70DMA::isDmaQueueEmpty();
     *
     * @brief Query if DMA queue is empty.
     *
     *        Query if DMA queue is empty.
     *
     * @return 1 if DMA queue is empty, else 0.
     */
    uint8_t isDmaQueueEmpty();

    /**
     * @fn void FreescaleK70DMA::signalDMAInterrupt();
     *
     * @brief Signals DMA interrupt.
     *
     *        Signals DMA interrupt.
     */
    void signalDMAInterrupt();

protected:
    /**
     * @fn virtual void FreescaleK70DMA::setupDataCopy(const BlitOp& blitOp);
     *
     * @brief Configures the DMA for copying data to the frame buffer.
     *
     *        Configures the DMA for copying data to the frame buffer.
     *
     * @param blitOp Details on the copy to perform.
     */
    virtual void setupDataCopy(const BlitOp& blitOp);

    /**
     * @fn virtual void FreescaleK70DMA::setupDataFill(const BlitOp& blitOp);
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
     * @fn bool FreescaleK70DMA::finished();
     *
     * @brief Interrupt call-back: Used for DMA transfers.
     *
     *        Interrupt call-back: Used for DMA transfers, which are known to complete the
     *        current BlitOp processing.
     *
     * @return true always (since the BlitOp processing has been completed).
     */
    bool finished();

    static const unsigned int channel = 0; ///< The used DMA channel. Only channel 0 supports 2D transfers

    LockFreeDMA_Queue dma_queue;
    BlitOp            queue_storage[96];
    const BlitOp*     pBlitOp;      ///< Reference to current BlitOp (the queue is in-accessible from outside DMA_Interface)
    uint16_t          numLines;     ///< Number of lines remaining in current transfer.
    unsigned          use32Bit : 1; ///< set to non-zero when using 32bit transfers (optimization)

};

#endif // FREESCALEK70DMA_HPP
