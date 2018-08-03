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
#ifndef SYS_CONFIG_H
#define SYS_CONFIG_H

/** @ingroup BOARD_HITEX_EVA_4350_OPTIONS
 * @{
 */

/* Build for 43xx chip family */
#define CHIP_LPC43XX

/* Build for full MII interface */
// #define USE_RMII

/* Un-comment DEBUG_ENABLE for IO support via the UART */
//#define DEBUG_ENABLE

/* Enable DEBUG_SEMIHOSTING along with DEBUG to enable IO support
   via semihosting */
// #define DEBUG_SEMIHOSTING

/* Board UART used for debug output */
#define DEBUG_UART LPC_USART0

/* Crystal frequency into device */
#define CRYSTAL_MAIN_FREQ_IN 12000000

/* Crystal frequency into device for RTC/32K input */
#define CRYSTAL_32K_FREQ_IN 32768

/* Frequency on external clock in pin */
#define EXTERNAL_CLKIN_FREQ_IN 0

/* Default CPU clock frequency */
#define MAX_CLOCK_FREQ (204000000)

/* Audio and USB default PLL rates (configured in SystemInit()) */
#define CGU_AUDIO_PLL_RATE (0)
#define CGU_USB_PLL_RATE (480000000)

/**
 * @}
 */

#endif // SYS_CONFIG_H
