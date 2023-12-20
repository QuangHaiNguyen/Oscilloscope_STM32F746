/**********************************************************************
* $Id$          lpc177x_8x_eeprom.h                     2011-06-02
*//**
* @file         lpc177x_8x_eeprom.h
* @brief        Contains all macro definitions and function prototypes
*                       support for EEPROM firmware library on LPC177x_8x
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
/** @defgroup EEPROM    EEPROM (Electrically Erasable Programmable Read-Only Memory)
 * @ingroup LPC177x_8xCMSIS_FwLib_Drivers
 * @{
 */

#ifndef __LPC177X_8X_EEPROM_H_
#define __LPC177X_8X_EEPROM_H_

/* Includes ------------------------------------------------------------------- */
#include "lpc177x_8x.h"
#include "lpc_types.h"


#ifdef __cplusplus
extern "C"
{
#endif

/* Private macros ------------------------------------------------------------- */
/** @defgroup EEPROM_Private_Macros EEPROM Private Macros
 * @{
 */

/* -------------------------- BIT DEFINITIONS --------------------------------- */
/*********************************************************************//**
 * Macro defines for EEPROM command register
 **********************************************************************/
#define EEPROM_CMD_8_BIT_READ                   (0)
#define EEPROM_CMD_16_BIT_READ                  (1)
#define EEPROM_CMD_32_BIT_READ                  (2)
#define EEPROM_CMD_8_BIT_WRITE                  (3)
#define EEPROM_CMD_16_BIT_WRITE                 (4)
#define EEPROM_CMD_32_BIT_WRITE                 (5)
#define EEPROM_CMD_ERASE_PRG_PAGE               (6)

#define EEPROM_CMD_RDPREFETCH                   (1<<3)

#define EEPROM_PAGE_SIZE                                64
#define EEPROM_PAGE_NUM                                 64

/*********************************************************************//**
 * Macro defines for EEPROM address register
 **********************************************************************/
#define EEPROM_PAGE_OFFSET(n)                   (n&0x3F)
#define EEPROM_PAGE_ADRESS(n)                   ((n&0x3F)<<6)

/*********************************************************************//**
 * Macro defines for EEPROM write data register
 **********************************************************************/
#define EEPROM_WDATA_8_BIT(n)                   (n&0x000000FF)
#define EEPROM_WDATA_16_BIT(n)                  (n&0x0000FFFF)
#define EEPROM_WDATA_32_BIT(n)                  (n&0xFFFFFFFF)

/*********************************************************************//**
 * Macro defines for EEPROM read data register
 **********************************************************************/
#define EEPROM_RDATA_8_BIT(n)                   (n&0x000000FF)
#define EEPROM_RDATA_16_BIT(n)                  (n&0x0000FFFF)
#define EEPROM_RDATA_32_BIT(n)                  (n&0xFFFFFFFF)

/*********************************************************************//**
 * Macro defines for EEPROM power down register
 **********************************************************************/
#define EEPROM_PWRDWN                                   (1<<0)

#define EEPROM_ENDOF_RW                                 (26)
#define EEPROM_ENDOF_PROG                               (28)

/**
 * @}
 */


/* Public Types --------------------------------------------------------------- */
/** @defgroup EEPROM_Public_Types EEPROM Public Types
 * @{
 */

typedef enum
{
        MODE_8_BIT = 0,
        MODE_16_BIT,
        MODE_32_BIT
}EEPROM_Mode_Type;


/**
 * @}
 */


/* Public Functions ----------------------------------------------------------- */
/** @defgroup EEPROM_Public_Functions EEPROM Public Functions
 * @{
 */

void EEPROM_Init(void);
void EEPROM_Write(uint16_t page_offset, uint16_t page_address, void* data, EEPROM_Mode_Type mode, uint32_t size);
void EEPROM_Read(uint16_t page_offset, uint16_t page_address, void* data, EEPROM_Mode_Type mode, uint32_t size);
void EEPROM_Erase(uint32_t address);
void EEPROM_PowerDown(FunctionalState NewState);

/**
 * @}
 */


#ifdef __cplusplus
}
#endif


#endif /* __LPC177X_8X_EEPROM_H_ */

/**
 * @}
 */

/* --------------------------------- End Of File ------------------------------ */
