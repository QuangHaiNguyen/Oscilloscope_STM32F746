/**********************************************************************
* $Id$          lpc177x_8x_crc.h                        2011-06-02
*//**
* @file         lpc177x_8x_crc.h
* @brief        Contains all macro definitions and function prototypes
*                       support for CRC firmware library on LPC177x_8x
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
/** @defgroup CRC       CRC (Cyclic Redundancy Check)
 * @ingroup LPC177x_8xCMSIS_FwLib_Drivers
 * @{
 */

#ifndef __LPC177X_8X_CRC_H_
#define __LPC177X_8X_CRC_H_

/* Includes ------------------------------------------------------------------- */
#include "lpc177x_8x.h"
#include "lpc_types.h"


#ifdef __cplusplus
extern "C"
{
#endif

/* Private macros ------------------------------------------------------------- */
/** @defgroup CRC_Private_Macros CRC Private Macros
 * @{
 */

/* -------------------------- BIT DEFINITIONS ----------------------------------- */
/*********************************************************************//**
 * Macro defines for CRC mode register
 **********************************************************************/
#define CRC_BIT_RVS_WR                  (1<<2)
#define CRC_CMPL_WR                             (1<<3)
#define CRC_BIT_RVS_SUM                 (1<<4)
#define CRC_CMPL_SUM                    (1<<5)


/**
 * @}
 */
/* Private types ------------------------------------------------------------- */
typedef enum
{
        CRC_POLY_CRCCCITT = 0,                  /** CRC CCITT polynomial */
        CRC_POLY_CRC16,                                 /** CRC-16 polynomial */
        CRC_POLY_CRC32                                  /** CRC-32 polynomial */
}CRC_Type;

typedef enum
{
        CRC_WR_8BIT = 1,                                /** 8-bit write: 1-cycle operation */
        CRC_WR_16BIT = 2,                                       /** 16-bit write: 2-cycle operation */
        CRC_WR_32BIT = 4,                                       /** 32-bit write: 4-cycle operation */
}CRC_WR_SIZE;

/* Public Functions ----------------------------------------------------------- */
/** @defgroup CRC_Public_Functions CRC Public Functions
 * @{
 */
void CRC_Init(CRC_Type CRCType);
void CRC_Reset(void);
uint32_t CRC_CalcDataChecksum(uint32_t data, CRC_WR_SIZE SizeType);
uint32_t CRC_CalcBlockChecksum(void *blockdata, uint32_t blocksize, CRC_WR_SIZE SizeType);

/**
 * @}
 */


#ifdef __cplusplus
}
#endif


#endif /* __LPC177X_8X_CRC_H_ */

/**
 * @}
 */

/* --------------------------------- End Of File ------------------------------ */
