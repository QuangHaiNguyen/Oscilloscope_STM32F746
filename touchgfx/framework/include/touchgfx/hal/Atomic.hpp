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
#ifndef ATOMIC_HPP
#define ATOMIC_HPP

/**
 * Defines a atomic write on suported platforms
 */

#if defined(WIN32)

#include <windows.h>
typedef LONG atomic_t;

/**
 * @fn inline void atomic_set(atomic_t& target, atomic_t value)
 *
 * @brief Makes a atomic write of value to target.
 *
 *        Makes a atomic write of value to target.
 *
 * @param [out] target The value to write to.
 * @param value        The value to write.
 */
inline void atomic_set(atomic_t& target, atomic_t value)
{
    InterlockedExchange(&target, value);
}

#elif defined(__GNUC__)

#include <csignal>
typedef sig_atomic_t atomic_t;

/**
 * @fn inline void atomic_set(atomic_t& target, atomic_t value)
 *
 * @brief Makes a atomic write of value to target.
 *
 *        Makes a atomic write of value to target.
 *
 * @param [out] target The value to write to.
 * @param value        The value to write.
 */
inline void atomic_set(atomic_t& target, atomic_t value)
{
    __sync_synchronize();
    target = value;
}

#elif defined(__IAR_SYSTEMS_ICC__)

typedef unsigned long atomic_t;

/**
 * @fn inline void atomic_set(atomic_t& target, atomic_t value)
 *
 * @brief Makes a atomic write of value to target.
 *
 *        Makes a atomic write of value to target.
 *
 * @note Assume that 32 bit writes are atomic.
 *
 * @param [out] target The value to write to.
 * @param value        The value to write.
 */
inline void atomic_set(atomic_t& target, atomic_t value)
{
    target = value;
}
#elif defined(__ARMCC_VERSION)

typedef unsigned long atomic_t;

/**
 * @fn inline void atomic_set(atomic_t& target, atomic_t value)
 *
 * @brief Makes a atomic write of value to target.
 *
 *        Makes a atomic write of value to target.
 *
 * @param [out] target The value to write to.
 * @param value        The value to write.
 */
inline void atomic_set(atomic_t& target, atomic_t value)
{
    target = value;
}
#else

#error "Compiler/platform not supported"

#endif

#endif // ATOMIC_HPP
