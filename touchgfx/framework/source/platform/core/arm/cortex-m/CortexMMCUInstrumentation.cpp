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
#include <platform/core/arm/cortex-m/CortexMMCUInstrumentation.hpp>
#include <touchgfx/hal/HAL.hpp>

namespace touchgfx
{

void CortexMMCUInstrumentation::init()
{
    // See: http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.ddi0337e/CEGHJDCF.html
    //
    //      [24]  Read/write  TRCENA  This bit must be set to 1 to enable use of the trace and debug blocks:
    //                                    Data Watchpoint and Trace (DWT)
    //                                    Instrumentation Trace Macrocell (ITM)
    //                                    Embedded Trace Macrocell (ETM)
    //                                    Trace Port Interface Unit (TPIU).
    //                                    This enables control of power usage unless tracing is required. The application can enable this, for ITM use, or use by a debugger.

    *((volatile unsigned int*)0xE000EDFC) |= 0x01000000;
    *((volatile unsigned int*)0xE0001000) |= 1;
}

//Board specific clockfrequency
unsigned int CortexMMCUInstrumentation::getElapsedUS(unsigned int start, unsigned int now, unsigned int clockfrequency)
{
    return ((now - start) + (clockfrequency / 2)) / clockfrequency;
}


unsigned int CortexMMCUInstrumentation::getCPUCycles()
{
    return *((volatile unsigned int*)0xE0001004);
}

void CortexMMCUInstrumentation::setMCUActive(bool active)
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

}
