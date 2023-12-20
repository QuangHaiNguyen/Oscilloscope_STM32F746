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
#ifndef NODMA_HPP
#define NODMA_HPP

#include <touchgfx/hal/DMA.hpp>
#include <assert.h>

namespace touchgfx
{
/**
 * @class NoDMA NoDMA.hpp touchgfx/hal/NoDMA.hpp
 *
 * @brief This is an "empty" DMA subclass that does nothing except assert if accidentally used.
 *
 *        This is an "empty" DMA subclass that does nothing except assert if accidentally used.
 *        An instance of this object can be used if DMA support is not desired.
 *
 * @see DMA_Interface
 */
class NoDMA : public DMA_Interface
{
public:

    /**
     * @fn NoDMA::NoDMA()
     *
     * @brief Default constructor.
     *
     *        Constructs a NoDMA object, with a queue of 1 element.
     */
    NoDMA() : DMA_Interface(q), q(&b, 1) { }

    virtual ~NoDMA() { }

    /**
     * @fn virtual BlitOperations NoDMA::getBlitCaps()
     *
     * @brief No blit operations supported by this DMA implementation.
     *
     *        No blit operations supported by this DMA implementation.
     *
     * @return Zero (no blit ops supported).
     */
    virtual BlitOperations getBlitCaps()
    {
        return static_cast<BlitOperations>(0);
    }

    /**
     * @fn virtual void NoDMA::setupDataCopy(const BlitOp& blitOp)
     *
     * @brief Asserts if used.
     *
     *        Asserts if used.
     *
     * @param blitOp The blit operation to be performed by this DMA instance.
     */
    virtual void setupDataCopy(const BlitOp& blitOp)
    {
        assert(0 && "DMA operation not supported");
    }

    /**
     * @fn virtual void NoDMA::setupDataFill(const BlitOp& blitOp)
     *
     * @brief Asserts if used.
     *
     *        Asserts if used.
     *
     * @param blitOp The blit operation to be performed by this DMA instance.
     */
    virtual void setupDataFill(const BlitOp& blitOp)
    {
        assert(0 && "DMA operation not supported");
    }

    /**
     * @fn virtual void NoDMA::signalDMAInterrupt()
     *
     * @brief Does nothing.
     *
     *        Does nothing.
     */
    virtual void signalDMAInterrupt()
    {
    }

    /**
     * @fn virtual void NoDMA::flush()
     *
     * @brief Does nothing.
     *
     *        Block until all DMA transfers are complete. Since this particular DMA does not do
     *        anything, return immediately.
     */
    virtual void flush() { }

private:
    LockFreeDMA_Queue q;
    BlitOp            b;
};

} // namespace touchgfx
#endif // NODMA_HPP
