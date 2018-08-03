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
#ifndef MCUINSTRUMENTATION_HPP
#define MCUINSTRUMENTATION_HPP

#include <stdint.h>

namespace touchgfx
{
/**
 * @class MCUInstrumentation MCUInstrumentation.hpp platform/core/MCUInstrumentation.hpp
 *
 * @brief Interface for instrumenting processors to measure MCU load via measured CPU cycles.
 *
 *        Interface for instrumenting processors to measure MCU load via measured CPU
 *        cycles.
 */
class MCUInstrumentation
{
public:
    /**
     * @fn MCUInstrumentation::MCUInstrumentation()
     *
     * @brief Constructor.
     *
     *        Constructor. Initializes members.
     */
    MCUInstrumentation():
        cc_consumed(0),
        cc_in(0)
    {
    }

    /**
     * @fn virtual void MCUInstrumentation::init() = 0;
     *
     * @brief Initialize.
     *
     *        Initialize.
     */
    virtual void init() = 0;

    /**
     * @fn virtual MCUInstrumentation::~MCUInstrumentation()
     *
     * @brief Virtual destructor.
     *
     *        Virtual destructor.
     */
    virtual ~MCUInstrumentation() {}

    /**
     * @fn virtual unsigned int MCUInstrumentation::getElapsedUS(unsigned int start, unsigned int now, unsigned int clockfrequency) = 0;
     *
     * @brief Gets elapsed microseconds based on clock frequency.
     *
     *        Gets elapsed microseconds based on clock frequency.
     *
     * @param start          Start time.
     * @param now            Current time.
     * @param clockfrequency Clock frequency of the system expressed in MHz.
     *
     * @return Elapsed microseconds start and now.
     */
    virtual unsigned int getElapsedUS(unsigned int start, unsigned int now, unsigned int clockfrequency) = 0;

    /**
     * @fn virtual unsigned int MCUInstrumentation::getCPUCycles(void) = 0;
     *
     * @brief Gets CPU cycles from register.
     *
     *        Gets CPU cycles from register.
     *
     * @return CPU cycles.
     */
    virtual unsigned int getCPUCycles(void) = 0;

    /**
     * @fn virtual void MCUInstrumentation::setMCUActive(bool active)
     *
     * @brief Sets MCU activity high.
     *
     *        Sets MCU activity high.
     *
     * @param active if True, inactive otherwise.
     */
    virtual void setMCUActive(bool active)
    {
        if (active) //idle task sched out
        {
            uint32_t cc_temp = getCPUCycles() - cc_in;
            cc_consumed += cc_temp;
        }
        else //idle task sched in
        {
            cc_in = getCPUCycles();
        }
    }

    /**
     * @fn virtual uint32_t MCUInstrumentation::getCCConsumed()
     *
     * @brief Gets number of consumed clock cycles.
     *
     *        Gets number of consumed clock cycles.
     *
     * @return clock cycles.
     */
    virtual uint32_t getCCConsumed()
    {
        return cc_consumed;
    }

    /**
     * @fn virtual void MCUInstrumentation::setCCConsumed(uint32_t val)
     *
     * @brief Sets number of consumed clock cycles.
     *
     *        Sets number of consumed clock cycles.
     *
     * @param val number of clock cycles.
     */
    virtual void setCCConsumed(uint32_t val)
    {
        cc_consumed = val;
    }

protected:
    volatile uint32_t cc_consumed; ///< Amount of consumed CPU cycles.
    volatile uint32_t cc_in;       ///< Current CPU cycles.
};

} // namespace touchgfx

#endif // MCUINSTRUMENTATION_HPP
