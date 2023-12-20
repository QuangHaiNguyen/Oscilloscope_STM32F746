/*
 * @brief Spansion Family driver
 *
 * @note
 * Copyright(C) NXP Semiconductors, 2014
 * All rights reserved.
 *
 * @par
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * LPC products.  This software is supplied "AS IS" without any warranties of
 * any kind, and NXP Semiconductors and its licenser disclaim any and
 * all warranties, express or implied, including all implied warranties of
 * merchantability, fitness for a particular purpose and non-infringement of
 * intellectual property rights.  NXP Semiconductors assumes no responsibility
 * or liability for the use of the software, conveys no license or rights under any
 * patent, copyright, mask work right, or any other intellectual property rights in
 * or to any products. NXP Semiconductors reserves the right to make changes
 * in the software without notification. NXP Semiconductors also makes no
 * representation or warranty that such application will be suitable for the
 * specified use without further testing or modification.
 *
 * @par
 * Permission to use, copy, modify, and distribute this software and its
 * documentation is hereby granted, under NXP Semiconductors' and its
 * licensor's relevant copyrights in the software, without fee, provided that it
 * is used in conjunction with NXP Semiconductors microcontrollers.  This
 * copyright, permission, and disclaimer notice must appear in all copies of
 * this code.
 */
#include <string.h>

#include "spifilib_dev.h"
#include "spifilib_chiphw.h"

/** @defgroup LPCSPIFILIB_CONFIG_SA25FLP LPCSPIFILIB Spansion S25FLxxxP family device support
 * @ingroup LPCSPIFILIB_DRIVERS
 * This driver includes support for the following devices.<br>
 * S25FL032P<br>
 * S25FL129P<br>
 *
 * Driver Feature Specifics:<br>
 * - 2 bytes status
 * - persistent erase status
 * - quad programming
 * - sub-block erase on subset of sectors (i.e NOT all sectors).
 * @{
 */

/**
 * @}
 */

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

/* Declare the base node */
static uint32_t devCount;
static SPIFI_DEV_DATA_T devHead = {0};

/* Required private data size for this driver */
#define PRVDATASIZE 0

/* Command definitions. Only used commands are defined. */
#define CMD_FAST_READ               0x0B                /**< Read Data bytes at Fast Speed */
#define CMD_QOR                     0x6B                /**< Quad Output Read */
#define CMD_RDID                    0x9F                /**< Read Identification */
#define CMD_WREN                    0x06                /**< Write Enable */
#define CMD_P4E                     0x20                /**< 4 KB Parameter Sector Erase */
#define CMD_BULK_ERASE              0xC7                /**< Bulk Erase */
#define CMD_SECT_ERASE              0xD8                /**< 64KB Sector Erase */
#define CMD_PAGE_PROGRAM            0x02                /**< Page Programming */
#define CMD_QUAD_PAGE_PROGRAM       0x32                /**< Quad Page Programming */
#define CMD_RDSR                    0x05                /**< Read Status Register */
#define CMD_WRITE_STATUS_REG        0x01                /**< Write (Status & Configuration) Register */
#define CMD_READ_CONFIG_REG         0x35                /**< Read Configuration Register (CFG) */
#define CMD_CLEAR_STATUS_REG        0x30                /**< Reset the Erase and Program Fail Flag (SR5 and SR6) and restore normal operation) */

/* Status register definitions */
/* Status Register Write Disable,
   1 = Protects when W# is low,
   0 = No protection, even when W# is low */
#define STATUS_SRWD                   (1 << 7)
/* Programming Error Occurred,
   0 = No Error,
   1 = Error occurred */
#define STATUS_P_ERR                  (1 << 6)
/* Erase Error Occurred,
   0 = No Error,
   1 = Error occurred */
#define STATUS_W_ERR                  (1 << 5)
/* Block protect bits,
   Protects upper half of address range in 5 sizes */
#define STATUS_BPMASK                 (7 << 2)
/* Write Enable Latch,
   1 = Device accepts Write Status Register, program, or erase commands,
   0 = Ignores Write Status Register, program, or erase commands */
#define STATUS_WEL                    (1 << 1)
/* Write in Progress,
   1 = Device Busy. A Write Status Register, program, or erase,
   0 = Ready. Device is in standby mode and can accept commands. */
#define STATUS_WIP                    (1 << 0)

/* 4kB param sector location,
    1 = top of memory
    0 = bottom of memory */
#define CONFIG_TBPARM                 (1 << 2)

/* Sets Quad mode in configuration register
     1 = enable Quad mode
     0 = disable Quad mode */
#define STATUS_QUAD_ENABLE            (1 << 1)

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Private functions
 ****************************************************************************/

/* Software write Enable */
static void prvSetWREN(LPC_SPIFI_CHIPHW_T *pSpifiCtrlAddr)
{
        spifi_HW_SetCmd(pSpifiCtrlAddr,
                                        (SPIFI_CMD_OPCODE(CMD_WREN) |
                                         SPIFI_CMD_FIELDFORM(SPIFI_FIELDFORM_ALL_SERIAL) |
                                         SPIFI_CMD_FRAMEFORM(SPIFI_FRAMEFORM_OP)));

        spifi_HW_WaitCMD(pSpifiCtrlAddr);
}

/* Software clear status */
static void prvClearStatus(LPC_SPIFI_CHIPHW_T *pSpifiCtrlAddr)
{
        spifi_HW_SetCmd(pSpifiCtrlAddr,
                                        (SPIFI_CMD_OPCODE(CMD_CLEAR_STATUS_REG) |
                                         SPIFI_CMD_FIELDFORM(SPIFI_FIELDFORM_ALL_SERIAL) |
                                         SPIFI_CMD_FRAMEFORM(SPIFI_FRAMEFORM_OP)));

        spifi_HW_WaitCMD(pSpifiCtrlAddr);
}

static uint8_t prvGetStatusRegister(LPC_SPIFI_CHIPHW_T *pSpifiCtrlAddr)
{
        uint8_t statRegister;

        spifi_HW_SetCmd(pSpifiCtrlAddr,
                                        (SPIFI_CMD_OPCODE(CMD_RDSR) |
                                         SPIFI_CMD_DATALEN(1) |
                                         SPIFI_CMD_FIELDFORM(SPIFI_FIELDFORM_ALL_SERIAL) |
                                         SPIFI_CMD_FRAMEFORM(SPIFI_FRAMEFORM_OP)));

        statRegister = spifi_HW_GetData8(pSpifiCtrlAddr);

        /* Wait for command to complete */
        spifi_HW_WaitCMD(pSpifiCtrlAddr);

        return statRegister;
}

/* Wait for device to complete operation (go unbusy) */
static void prvWaitUnBusy(LPC_SPIFI_CHIPHW_T *pSpifiCtrlAddr)
{
        /* Device wait for device to go unbusy */
        while ((prvGetStatusRegister(pSpifiCtrlAddr) & STATUS_WIP) != 0) {}
}

/* Write Status / Config Register */
static void prvSetStatusRegister(LPC_SPIFI_CHIPHW_T *pSpifiCtrlAddr, uint8_t status)
{
        prvSetWREN(pSpifiCtrlAddr);
        spifi_HW_SetCmd(pSpifiCtrlAddr,
                                        (SPIFI_CMD_OPCODE(CMD_WRITE_STATUS_REG) |
                                         SPIFI_CMD_DATALEN(1) |
                                         SPIFI_CMD_DOUT(1) |
                                         SPIFI_CMD_FIELDFORM(SPIFI_FIELDFORM_ALL_SERIAL) |
                                         SPIFI_CMD_FRAMEFORM(SPIFI_FRAMEFORM_OP)));

        /* Write the data out */
        spifi_HW_SetData8(pSpifiCtrlAddr, status);

        /* Wait for Controler to finish command */
        spifi_HW_WaitCMD(pSpifiCtrlAddr);

        /* Wait for flash controller to finish command */
        prvWaitUnBusy(pSpifiCtrlAddr);
}

/* Read Status Register*/
static uint16_t prvGetStatusConfigRegister(LPC_SPIFI_CHIPHW_T *pSpifiCtrlAddr)
{
        uint16_t statConfigRegisters;

        spifi_HW_SetCmd(pSpifiCtrlAddr,
                                        (SPIFI_CMD_OPCODE(CMD_RDSR) |
                                         SPIFI_CMD_DATALEN(1) |
                                         SPIFI_CMD_FIELDFORM(SPIFI_FIELDFORM_ALL_SERIAL) |
                                         SPIFI_CMD_FRAMEFORM(SPIFI_FRAMEFORM_OP)));

        statConfigRegisters = (((uint16_t) spifi_HW_GetData8(pSpifiCtrlAddr)) << 8);

        /* Wait for command to complete */
        spifi_HW_WaitCMD(pSpifiCtrlAddr);

        spifi_HW_SetCmd(pSpifiCtrlAddr,
                                        (SPIFI_CMD_OPCODE(CMD_READ_CONFIG_REG) |
                                         SPIFI_CMD_DATALEN(1) |
                                         SPIFI_CMD_FIELDFORM(SPIFI_FIELDFORM_ALL_SERIAL) |
                                         SPIFI_CMD_FRAMEFORM(SPIFI_FRAMEFORM_OP)));

        statConfigRegisters |= spifi_HW_GetData8(pSpifiCtrlAddr);

        /* Wait for command to complete */
        spifi_HW_WaitCMD(pSpifiCtrlAddr);

        return statConfigRegisters;
}

/* Write Status / Config Register */
static void prvSetStatusConfigRegisters(LPC_SPIFI_CHIPHW_T *pSpifiCtrlAddr, uint16_t statusConfig)
{
        prvSetWREN(pSpifiCtrlAddr);
        spifi_HW_SetCmd(pSpifiCtrlAddr,
                                        (SPIFI_CMD_OPCODE(CMD_WRITE_STATUS_REG) |
                                         SPIFI_CMD_DATALEN(2) |
                                         SPIFI_CMD_DOUT(1) |
                                         SPIFI_CMD_FIELDFORM(SPIFI_FIELDFORM_ALL_SERIAL) |
                                         SPIFI_CMD_FRAMEFORM(SPIFI_FRAMEFORM_OP)));

        spifi_HW_SetData8(pSpifiCtrlAddr, (uint8_t) (statusConfig >> 8));
        spifi_HW_SetData8(pSpifiCtrlAddr, (uint8_t) statusConfig);

        /* Wait for LPC controller to finish command */
        spifi_HW_WaitCMD(pSpifiCtrlAddr);

        /* Wait for flash controller to finish command */
        prvWaitUnBusy(pSpifiCtrlAddr);
}

/* Set / Clear Quad mode */
static void prvSetQuadMode(LPC_SPIFI_CHIPHW_T *pSpifiCtrlAddr, uint8_t enQuadMode)
{
        uint16_t statusConfig;

        statusConfig = prvGetStatusConfigRegister(pSpifiCtrlAddr);

        /* Set or clear the quad enable bit */
        if (enQuadMode) {
                statusConfig |= STATUS_QUAD_ENABLE;
        }
        else {
                statusConfig &= ~STATUS_QUAD_ENABLE;
        }

        /* write status / config */
        prvSetStatusConfigRegisters(pSpifiCtrlAddr, statusConfig);
}

/* Checks to see if the device is writable and not busy */
static SPIFI_ERR_T prvCheckWriteState(LPC_SPIFI_CHIPHW_T *pSpifiCtrlAddr)
{
        uint16_t stat;

        /* Get status */
        stat = prvGetStatusRegister(pSpifiCtrlAddr);

        /* Exit if blocks are locked or WIP in progress */
        if ((stat & STATUS_BPMASK) != 0) {
                return SPIFI_ERR_LOCKED;
        }
        else if ((stat & STATUS_WIP) != 0) {
                return SPIFI_ERR_BUSY;
        }

        return SPIFI_ERR_NONE;
}

/* Read Identification */
static void deviceGetID(uint32_t spifiAddr, SPIFI_DEVICE_ID_T *pID)
{
        uint8_t idx;
        LPC_SPIFI_CHIPHW_T *pSpifiCtrlAddr = (LPC_SPIFI_CHIPHW_T *) spifiAddr;
        
        /* Read ID command, plus read 3 bytes on data */
        spifi_HW_SetCmd(pSpifiCtrlAddr,
                                        (SPIFI_CMD_OPCODE(CMD_RDID) |
                                         SPIFI_CMD_DATALEN(3 + pID->extCount) |
                                         SPIFI_CMD_FIELDFORM(SPIFI_FIELDFORM_ALL_SERIAL) |
                                         SPIFI_CMD_FRAMEFORM(SPIFI_FRAMEFORM_OP)));

        /* Get info from the device */
        pID->mfgId[0] = spifi_HW_GetData8(pSpifiCtrlAddr);      /* Manufacturers ID */
        pID->mfgId[1] = spifi_HW_GetData8(pSpifiCtrlAddr);      /* Memory Type */
        pID->mfgId[2] = spifi_HW_GetData8(pSpifiCtrlAddr);      /* Memmory Capacity */

        /* Read the specified number of extended bytes */
        for (idx = 0; idx < pID->extCount; ++idx) {
                pID->extId[idx] = spifi_HW_GetData8(pSpifiCtrlAddr);
        }

        spifi_HW_WaitCMD(pSpifiCtrlAddr);
}

/* Converts a device status to an OR'ed API status */
static uint32_t deviceGetStatus(SPIFI_HANDLE_T *pHandle)
{
        uint16_t devStat;
        uint32_t status = 0;
        LPC_SPIFI_CHIPHW_T *pSpifiCtrlAddr = (LPC_SPIFI_CHIPHW_T *) pHandle->pInfoData->spifiCtrlAddr;

        /* Read device status word */
        devStat = prvGetStatusRegister(pSpifiCtrlAddr);

        /* Convert to standard status values */
        if ((devStat & (STATUS_P_ERR | STATUS_W_ERR)) != 0) {
                if ((devStat & STATUS_P_ERR) != 0) {
                        status |= SPIFI_STAT_PROGERR;
                }
                if ((devStat & STATUS_W_ERR) != 0) {
                        status |= SPIFI_STAT_ERASEERR;
                }

                /* Clear latched status */
                prvClearStatus(pSpifiCtrlAddr);
        }
        if ((devStat & STATUS_BPMASK) != 0) {
                if ((devStat & STATUS_BPMASK) == STATUS_BPMASK) {
                        status |= SPIFI_STAT_FULLLOCK;
                }
                else {
                        status |= SPIFI_STAT_PARTLOCK;
                }
        }
        if ((devStat & STATUS_WIP) != 0) {
                status |= SPIFI_STAT_BUSY;
        }

        return status;
}

/* Enter memory mode */
static uint32_t deviceGetMemoryModeCmd(SPIFI_HANDLE_T *pHandle, uint8_t enMMode)
{
        uint32_t cmdValue;

        if (pHandle->pInfoData->opts & SPIFI_OPT_USE_QUAD) {
                cmdValue =
                        (SPIFI_CMD_OPCODE(CMD_QOR) |
                         SPIFI_CMD_DOUT(0) |
                         SPIFI_CMD_INTER(1) |
                         SPIFI_CMD_FIELDFORM(SPIFI_FIELDFORM_SERIAL_OPCODE_ADDRESS) |
                         SPIFI_CMD_FRAMEFORM(SPIFI_FRAMEFORM_OP_3ADDRESS));
        }
        else {
                cmdValue =
                        (SPIFI_CMD_OPCODE(CMD_FAST_READ) |
                         SPIFI_CMD_DOUT(0) |
                         SPIFI_CMD_INTER(1) |
                         SPIFI_CMD_FIELDFORM(SPIFI_FIELDFORM_ALL_SERIAL) |
                         SPIFI_CMD_FRAMEFORM(SPIFI_FRAMEFORM_OP_3ADDRESS));
        }

        return cmdValue;
}

/* lock/ unlock commands */
static SPIFI_ERR_T deviceLockCmd(SPIFI_HANDLE_T *pHandle, SPIFI_PCMD_LOCK_UNLOCK_T cmd, uint32_t data)
{
    SPIFI_ERR_T status = SPIFI_ERR_NOTSUPPORTED;
    
    switch (cmd) {
        case SPIFI_PCMD_UNLOCK_DEVICE:
        case SPIFI_PCMD_LOCK_DEVICE:
        {
            uint8_t stat;
      
            LPC_SPIFI_CHIPHW_T *pSpifiCtrlAddr = (LPC_SPIFI_CHIPHW_T *) pHandle->pInfoData->spifiCtrlAddr;

            /* Get current status */
            stat = prvGetStatusRegister(pSpifiCtrlAddr);

            if (cmd == SPIFI_PCMD_UNLOCK_DEVICE) {
               /* Clear lock bits only if they are locked */
                if ((stat & STATUS_BPMASK) != 0) {
                    stat &= ~STATUS_BPMASK;
                    /* Write updated value back to status register */
                    prvSetStatusRegister(pSpifiCtrlAddr, stat);
                }
            }
            else {
               /* Clear lock bits only if they are locked */
                if ((stat & STATUS_BPMASK) != STATUS_BPMASK) {
                    stat |= STATUS_BPMASK;
                    /* Write updated value back to status register */
                    prvSetStatusRegister(pSpifiCtrlAddr, stat);
                }
            }
            status = SPIFI_ERR_NONE;
        }
            break;
        
        case SPIFI_PCMD_UNLOCK_BLOCK:
        case SPIFI_PCMD_LOCK_BLOCK:
            break;
    }
    return status;    
}

/* Bulk Erase*/
static SPIFI_ERR_T deviceEraseAll(SPIFI_HANDLE_T *pHandle)
{
        SPIFI_ERR_T status;
        LPC_SPIFI_CHIPHW_T *pSpifiCtrlAddr = (LPC_SPIFI_CHIPHW_T *) pHandle->pInfoData->spifiCtrlAddr;

        status = prvCheckWriteState(pSpifiCtrlAddr);
        if (status == SPIFI_ERR_NONE) {
                prvSetWREN(pSpifiCtrlAddr);
                spifi_HW_SetCmd(pSpifiCtrlAddr,
                                                (SPIFI_CMD_OPCODE(CMD_BULK_ERASE) |
                                                 SPIFI_CMD_FIELDFORM(SPIFI_FIELDFORM_ALL_SERIAL) |
                                                 SPIFI_CMD_FRAMEFORM(SPIFI_FRAMEFORM_OP)));

                spifi_HW_WaitCMD(pSpifiCtrlAddr);

                /* Device wait for device to go unbusy */
                prvWaitUnBusy(pSpifiCtrlAddr);
        }

        return status;
}

/* Erase a block by block number */
static SPIFI_ERR_T deviceEraseBlock(SPIFI_HANDLE_T *pHandle, uint32_t blockNum)
{
        uint16_t stat;
        uint32_t addr;
        SPIFI_ERR_T status = SPIFI_ERR_RANGE;
        LPC_SPIFI_CHIPHW_T *pSpifiCtrlAddr = (LPC_SPIFI_CHIPHW_T *) pHandle->pInfoData->spifiCtrlAddr;

        if (blockNum < pHandle->pInfoData->numBlocks) {
                status = prvCheckWriteState(pSpifiCtrlAddr);
                if (status == SPIFI_ERR_NONE) {
                        addr = blockNum * pHandle->pInfoData->blockSize;
                        prvClearStatus(pSpifiCtrlAddr);
                        prvSetWREN(pSpifiCtrlAddr);

                        spifi_HW_SetAddr(pSpifiCtrlAddr, addr);
                        spifi_HW_SetCmd(pSpifiCtrlAddr,
                                                        (SPIFI_CMD_OPCODE(CMD_SECT_ERASE) |
                                                         SPIFI_CMD_FIELDFORM(SPIFI_FIELDFORM_ALL_SERIAL) |
                                                         SPIFI_CMD_FRAMEFORM(SPIFI_FRAMEFORM_OP_3ADDRESS)));

                        spifi_HW_WaitCMD(pSpifiCtrlAddr);

                        /* If blocking is disabled, exit now */
                        if ((pHandle->pInfoData->opts & SPIFI_OPT_NOBLOCK) == 0) {
                                /* Device wait for device to go unbusy */
                                prvWaitUnBusy(pSpifiCtrlAddr);

                                /* Read status and check error bits */
                                stat = deviceGetStatus(pHandle);
                                if ((stat & SPIFI_STAT_ERASEERR) != 0) {
                                        status = SPIFI_ERR_ERASEERR;
                                }
                        }
                }
        }

        return status;
}

/* Erase a block by sub-block number */
static SPIFI_ERR_T deviceEraseSubBlock(SPIFI_HANDLE_T *pHandle, uint32_t subBlockNum)
{
        uint16_t stat;
        uint32_t addr;
        SPIFI_ERR_T status = SPIFI_ERR_RANGE;
        LPC_SPIFI_CHIPHW_T *pSpifiCtrlAddr = (LPC_SPIFI_CHIPHW_T *) pHandle->pInfoData->spifiCtrlAddr;
    
        if (subBlockNum < pHandle->pInfoData->numSubBlocks) {
                status = prvCheckWriteState(pSpifiCtrlAddr);
                if (status == SPIFI_ERR_NONE) {
                        addr = (subBlockNum * pHandle->pInfoData->subBlockSize) + pHandle->pInfoData->baseAddr;
                        prvClearStatus(pSpifiCtrlAddr);
                        prvSetWREN(pSpifiCtrlAddr);

                        spifi_HW_SetAddr(pSpifiCtrlAddr, addr);
                        spifi_HW_SetCmd(pSpifiCtrlAddr,
                                                        (SPIFI_CMD_OPCODE(CMD_P4E) |
                                                         SPIFI_CMD_FIELDFORM(SPIFI_FIELDFORM_ALL_SERIAL) |
                                                         SPIFI_CMD_FRAMEFORM(SPIFI_FRAMEFORM_OP_3ADDRESS)));

                        spifi_HW_WaitCMD(pSpifiCtrlAddr);

                        /* If blocking is disabled, exit now */
                        if ((pHandle->pInfoData->opts & SPIFI_OPT_NOBLOCK) == 0) {
                                /* Device wait for device to go unbusy */
                                prvWaitUnBusy(pSpifiCtrlAddr);

                                /* Read status and check error bits */
                                stat = deviceGetStatus(pHandle);
                                if ((stat & SPIFI_STAT_ERASEERR) != 0) {
                                        status = SPIFI_ERR_ERASEERR;
                                }
                        }
                }
        }

        return status;
}

/* Program a region */
static SPIFI_ERR_T devicePageProgram(SPIFI_HANDLE_T *pHandle, uint32_t addr, const uint32_t *writeBuff, uint32_t bytes)
{
        uint16_t stat;
        uint32_t cmdValue;
        uint8_t *writeBuff8 = (uint8_t *) writeBuff;
        SPIFI_ERR_T status = SPIFI_ERR_PAGESIZE;
        LPC_SPIFI_CHIPHW_T *pSpifiCtrlAddr = (LPC_SPIFI_CHIPHW_T *) pHandle->pInfoData->spifiCtrlAddr;

        if (bytes <= pHandle->pInfoData->pageSize) {
                status = prvCheckWriteState(pSpifiCtrlAddr);
                if (status == SPIFI_ERR_NONE) {
                        prvClearStatus(pSpifiCtrlAddr);
                        prvSetWREN(pSpifiCtrlAddr);

                        if (pHandle->pInfoData->opts & SPIFI_OPT_USE_QUAD) {
                                cmdValue = (SPIFI_CMD_OPCODE(CMD_QUAD_PAGE_PROGRAM) |
                                                        SPIFI_CMD_DATALEN(bytes) |
                                                        SPIFI_CMD_DOUT(1) |
                                                        SPIFI_CMD_FIELDFORM(SPIFI_FIELDFORM_SERIAL_OPCODE_ADDRESS) |
                                                        SPIFI_CMD_FRAMEFORM(SPIFI_FRAMEFORM_OP_3ADDRESS));
                        }
                        else {
                                cmdValue = (SPIFI_CMD_OPCODE(CMD_PAGE_PROGRAM) |
                                                        SPIFI_CMD_DATALEN(bytes) |
                                                        SPIFI_CMD_DOUT(1) |
                                                        SPIFI_CMD_FIELDFORM(SPIFI_FIELDFORM_ALL_SERIAL) |
                                                        SPIFI_CMD_FRAMEFORM(SPIFI_FRAMEFORM_OP_3ADDRESS));
                        }

                        spifi_HW_SetAddr(pSpifiCtrlAddr, addr);
                        spifi_HW_SetCmd(pSpifiCtrlAddr, cmdValue);

                        /* Write data */
                        while (bytes) {
                                spifi_HW_SetData8(pSpifiCtrlAddr, *writeBuff8);
                                ++writeBuff8;
                                --bytes;

                        }
                }

                spifi_HW_WaitCMD(pSpifiCtrlAddr);

                /* If block is disabled, exit now */
                if ((pHandle->pInfoData->opts & SPIFI_OPT_NOBLOCK) == 0) {
                        /* Device wait for device to go unbusy */
                        prvWaitUnBusy(pSpifiCtrlAddr);

                        /* Read status and check error bits */
                        stat = deviceGetStatus(pHandle);
                        if ((stat & SPIFI_STAT_PROGERR) != 0) {
                                status = SPIFI_ERR_PROGERR;
                        }
                }
        }

        return status;
}

/* Read a region */
static SPIFI_ERR_T deviceRead(SPIFI_HANDLE_T *pHandle, uint32_t addr, uint32_t *readBuff, uint32_t bytes)
{
        uint8_t *readBuff8 = (uint8_t *) readBuff;
        SPIFI_ERR_T status = SPIFI_ERR_RANGE;
        LPC_SPIFI_CHIPHW_T *pSpifiCtrlAddr = (LPC_SPIFI_CHIPHW_T *) pHandle->pInfoData->spifiCtrlAddr;
        uint32_t cmdValue;

        /* Limit read to controller data limit in bytes */
        if (bytes < pHandle->pInfoData->maxReadSize) {

                /* Choose how to program the read command */
                if (pHandle->pInfoData->opts & SPIFI_OPT_USE_QUAD) {
                        cmdValue =
                                (SPIFI_CMD_OPCODE(CMD_QOR) |
                                 SPIFI_CMD_DATALEN(bytes) |
                                 SPIFI_CMD_DOUT(0) |
                                 SPIFI_CMD_INTER(1) |
                                 SPIFI_CMD_FIELDFORM(SPIFI_FIELDFORM_SERIAL_OPCODE_ADDRESS) |
                                 SPIFI_CMD_FRAMEFORM(SPIFI_FRAMEFORM_OP_3ADDRESS));
                }
                else {
                        cmdValue =
                                (SPIFI_CMD_OPCODE(CMD_FAST_READ) |
                                 SPIFI_CMD_DATALEN(bytes) |
                                 SPIFI_CMD_DOUT(0) |
                                 SPIFI_CMD_INTER(1) |
                                 SPIFI_CMD_FIELDFORM(SPIFI_FIELDFORM_ALL_SERIAL) |
                                 SPIFI_CMD_FRAMEFORM(SPIFI_FRAMEFORM_OP_3ADDRESS));
                }

                spifi_HW_SetAddr(pSpifiCtrlAddr, addr);
                spifi_HW_SetCmd(pSpifiCtrlAddr, cmdValue);

                /* Read data */
                while (bytes) {
                        *readBuff8 = spifi_HW_GetData8(pSpifiCtrlAddr);
                        ++readBuff8;
                        --bytes;
                }

                spifi_HW_WaitCMD(pSpifiCtrlAddr);
                status = SPIFI_ERR_NONE;
        }

        return status;
}

/* Set or unset driver options. Caller already updated driver options. */
static SPIFI_ERR_T deviceSetUnsetOptions(SPIFI_HANDLE_T *pHandle, uint32_t options, uint8_t set)
{
        LPC_SPIFI_CHIPHW_T *pSpifiCtrlAddr = (LPC_SPIFI_CHIPHW_T *) pHandle->pInfoData->spifiCtrlAddr;

        /* Enable / Disable quad mode */
        if (options & SPIFI_OPT_USE_QUAD) {
                prvSetQuadMode(pSpifiCtrlAddr, set);
        }

        return SPIFI_ERR_NONE;
}

/* Enable or disable software write protect state */
static SPIFI_ERR_T deviceReset(SPIFI_HANDLE_T *pHandle)
{
        return SPIFI_ERR_NOTSUPPORTED;
}


/* Setup a device */
static SPIFI_ERR_T devSetup(SPIFI_HANDLE_T *pHandle, uint32_t spifiCtrlAddr, uint32_t baseAddr)
{
        /* Spansion S25 device function table */
        static  SPIFI_DEV_T fxTable;

        /* Init the private device functions */
        fxTable.init = NULL;            /* No specific init required for this family */
        fxTable.deInit = NULL;  /* No specific deInit required for this family */
    
        fxTable.lockCmd = deviceLockCmd;
        fxTable.eraseAll = deviceEraseAll;
        fxTable.eraseBlock = deviceEraseBlock;
        fxTable.eraseSubBlock = deviceEraseSubBlock;
        fxTable.pageProgram = devicePageProgram;
        fxTable.read = deviceRead;
        fxTable.setOpts = deviceSetUnsetOptions;
        fxTable.reset = deviceReset;
        fxTable.getMemModeCmd = deviceGetMemoryModeCmd;
        fxTable.getStatus = deviceGetStatus;
    fxTable.subBlockCmd = NULL;

        /* save pointer to device function table */
        pHandle->pDev = &fxTable;

        return SPIFI_ERR_NONE;
}

static SPIFI_ERR_T devRegister(void *pDevFamily, SPIFI_DEV_DATA_T *pDevData)
{
        /* insert into the beginning of the list */
        pDevData->pNext = devHead.pNext;
        devHead.pNext = pDevData;

        /* update the number of devices in the list */
        ++devCount;

        /* Nothing to do here yet */
        return SPIFI_ERR_NONE;
}

/*****************************************************************************
 * Public functions
 ****************************************************************************/
SPIFI_DEV_FAMILY_T *SPIFI_REG_FAMILY_Spansion_2Byte_PStatus(void)
{
        static SPIFI_DEV_FAMILY_T handle;
        static SPIFI_FAM_DESC_T desc;

        /* Store the device specific info so it can be returned */
        desc.pFamName = "Spansion S25FL032P based family";
        desc.pDevList = &devHead;
        desc.prvDataSize = PRVDATASIZE;
        desc.pPrvDevGetID = deviceGetID;
        desc.pPrvDevSetup = devSetup;
        desc.pPrvDevRegister = devRegister;

        /* Save the descriptor in the handle */
        handle.pDesc = &desc;

        /* Make sure that the base list is empty and the count reflects 0 */
        devHead.pNext = NULL;
        devCount = 0;

        {
                /* ID Spansion S25FL032P */
                static const SPIFI_DEV_PDATA_T pData = {
            "S25FL032P",
                        {{0x01, 0x02, 0x15}, 0, {0}},   /* JEDEC ID, extCount, ext data  */
                        64,                                             /* # of blocks */
                        0x10000,                                /* block size */
                        0,                                          /* # of sub-blocks  32 (This device only has sub-block erase in first/last 2 blocks) */
                        0,                                      /* sub-block size  0x1000 */
                        0x100,                                  /* page size */
                        32768,                                  /* max single read bytes */
                        40000000,                               /* max clock rate in Hz */
                        (SPIFI_CAP_QUAD | SPIFI_CAP_FULLLOCK | SPIFI_CAP_NOBLOCK)       /* capabilitites */
                };
                static SPIFI_DEV_DATA_T data;

                data.pDevData = &pData;
                devRegister(&handle, &data);
        }

        {
                /* Spansion S25FL129P 64k Sector */
                static const SPIFI_DEV_PDATA_T pData = {
            "S25FL129P 64kSec",
                        {{0x01, 0x20, 0x18, }, 2, {0x4D, 0x01}},/* JEDEC ID, extCount, ext data  */
                        256,                                    /* # of blocks */
                        0x10000,                                /* block size */
                        0,                                          /* # of sub-blocks 32 (This device only has sub-block erase in first/last 2 blocks) */
                        0,                                          /* sub-block size 0x1000 */
                        0x100,                                  /* page size */
                        32768,                                  /* max single read bytes */
                        40000000,                               /* max clock rate in Hz */
                        (SPIFI_CAP_QUAD | SPIFI_CAP_FULLLOCK | SPIFI_CAP_NOBLOCK)
                };
                static SPIFI_DEV_DATA_T data;

                data.pDevData = &pData;
                devRegister(&handle, &data);
        }
        {
                /* Spansion S25FL129P 256k Sector */
                static const SPIFI_DEV_PDATA_T pData = {
            "S25FL129P 256kSec",
                        {{0x01, 0x20, 0x18, }, 2, {0x4D, 0x0}}, /* JEDEC ID, extCount, ext data  */
                        64,                                             /* # of blocks */
                        0x40000,                                /* block size */
                        0,                                          /* # of sub-blocks (Does NOT support sub-block erase) */
                        0,                                      /* sub-block size */
                        0x100,                                  /* page size */
                        32768,                                  /* max single read bytes */
                        40000000,                               /* max clock rate in Hz */
                        (SPIFI_CAP_QUAD | SPIFI_CAP_FULLLOCK | SPIFI_CAP_NOBLOCK)
                };
                static SPIFI_DEV_DATA_T data;

                data.pDevData = &pData;
                devRegister(&handle, &data);
        }

        /* finally return the handle */
        return &handle;
}
