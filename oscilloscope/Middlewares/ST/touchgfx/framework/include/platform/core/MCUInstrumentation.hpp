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
 * @file platform/core/MCUInstrumentation.hpp
 *
 * Declares the touchgfx::MCUInstrumentation interface class.
 */
#ifndef TOUCHGFX_MCUINSTRUMENTATION_HPP
#define TOUCHGFX_MCUINSTRUMENTATION_HPP

#include <touchgfx/hal/Types.hpp>

namespace touchgfx
{
/** Interface for instrumenting processors to measure MCU load via measured CPU cycles. */
class MCUInstrumentation
{
public:
    /** Initializes a new instance of the MCUInstrumentation class. */
    MCUInstrumentation()
        : cc_consumed(0),
          cc_in(0)
    {
    }

    /** Initialize. */
    virtual void init() = 0;

    /** Finalizes an instance of the MCUInstrumentation class. */
    virtual ~MCUInstrumentation()
    {
    }

    /**
     * Gets elapsed microseconds based on clock frequency.
     *
     * @param  start          Start time.
     * @param  now            Current time.
     * @param  clockfrequency Clock frequency of the system expressed in MHz.
     *
     * @return Elapsed microseconds start and now.
     */
    virtual unsigned int getElapsedUS(unsigned int start, unsigned int now, unsigned int clockfrequency) = 0;

    /**
     * Gets CPU cycles from register.
     *
     * @return CPU cycles.
     */
    virtual unsigned int getCPUCycles(void) = 0;

    /**
     * Sets MCU activity high.
     *
     * @param  active if True, inactive otherwise.
     */
    virtual void setMCUActive(bool active)
    {
        if (active) // Idle task sched out
        {
            uint32_t cc_temp = getCPUCycles() - cc_in;
            cc_consumed += cc_temp;
        }
        else // Idle task sched in
        {
            cc_in = getCPUCycles();
        }
    }

    /**
     * Gets number of consumed clock cycles.
     *
     * @return clock cycles.
     */
    virtual uint32_t getCCConsumed()
    {
        return cc_consumed;
    }

    /**
     * Sets number of consumed clock cycles.
     *
     * @param  val number of clock cycles.
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

#endif // TOUCHGFX_MCUINSTRUMENTATION_HPP
