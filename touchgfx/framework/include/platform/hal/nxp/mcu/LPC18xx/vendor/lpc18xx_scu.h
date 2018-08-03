/**********************************************************************
* $Id: lpc18xx_scu.h 48117 2011-10-20 12:13:01Z anderslu $              lpc18xx_scu.h           2011-06-02
*//**
* @file         lpc18xx_scu.h
* @brief        Contains all macro definitions and function prototypes
*                       support for SCU firmware library on LPC18xx
* @version      1.0
* @date         02. June. 2011
* @author       NXP MCU SW Application Team
*
* Copyright(C) 2011, NXP Semiconductor
* All rights reserved.
*
***********************************************************************
* Software that is described herein is for illustrative purposes only
* which provides customers with programming information regarding the
* products. This software is supplied "AS IS" without any warranties.
* NXP Semiconductors assumes no responsibility or liability for the
* use of the software, conveys no license or title under any patent,
* copyright, or mask work right to the product. NXP Semiconductors
* reserves the right to make changes in the software without
* notification. NXP Semiconductors also make no representation or
* warranty that such application will be suitable for the specified
* use without further testing or modification.
**********************************************************************/

/* Peripheral group ----------------------------------------------------------- */
/** @defgroup SCU       SCU
 * @ingroup LPC1800CMSIS_FwLib_Drivers
 * @{
 */

#ifndef __SCU_H
#define __SCU_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Private macros ------------------------------------------------------------- */
/** @defgroup SCT_Private_Macros SCT Private Macros
 * @{
 */

/** Port offset definition */
#define PORT_OFFSET     0x80
/** Pin offset definition */
#define PIN_OFFSET      0x04

/* Pin modes */
#define MD_PUP                  (0x0<<2)                /** Pull-up enabled     */
#define MD_BUK                  (0x1<<2)                /** Plain input                 */
#define MD_PLN                  (0x2<<2)                /** Repeater mode               */
#define MD_PDN                  (0x3<<2)                /** Pull-down enabled   */


/* Pin function */
#define FUNC0                   0x0                             /** Function 0  */
#define FUNC1                   0x1                             /** Function 1  */
#define FUNC2                   0x2                             /** Function 2  */
#define FUNC3                   0x3                             /** Function 3  */

/**
 * @}
 */

/* Public Functions ----------------------------------------------------------- */
/** @defgroup SCU_Public_Functions SCU Public Functions
 * @{
 */

void scu_pinmux(uint8_t port, uint8_t pin, uint8_t mode, uint8_t func);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* end __SCU_H */

/**
 * @}
 */

/* --------------------------------- End Of File ------------------------------ */
