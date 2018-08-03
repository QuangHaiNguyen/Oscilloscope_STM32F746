/*
 * @brief LPCSPIFILIB driver functions and structures that are not visible
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

#include "spifilib_api.h"
#include "spifilib_chiphw.h"
#include <string.h>

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

/* Declare the version numbers */
#define LIBRARY_VERSION_MAJOR (0)
#define LIBRARY_VERSION_MINOR (6)

/* device node count and linked list header */
static uint32_t famCount = 0;
static SPIFI_DEV_FAMILY_T famHead = {0};

/* Number of supported devices */
#define NUMSUPPDEVS (sizeof(pPrvDevs) / sizeof(SPIFI_DEVDESC_T *))

/* Mapped error strings to error codes */
static const char *spifiErrStrings[SPIFI_ERR_LASTINDEX] = {
        "No error",
        "Device is busy",
        "General error",
        "Capability not supported",
        "Alignment error",
        "Device is locked",
        "Program error",
        "Erase error",
        "Program region not blank",
        "Page size exceeded",
        "Validation error",
        "Range exceeded",
        "Not Allowed in Memory Mode"
};

static const char noName[] = "Invalid index";

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Private functions
 ****************************************************************************/
static uint8_t checkExtendedMatch(SPIFI_DEV_DATA_T *pNode, SPIFI_DEVICE_ID_T *pID)
{
        uint32_t x;

        if (pID->extCount != pNode->pDevData->id.extCount) {
                return 0;
        }

        if (pNode->pDevData->id.extCount) {
                for (x = 0; x < pID->extCount; ++x) {
                        if (pNode->pDevData->id.extId[x] != pID->extId[x]) {
                                return 0;
                        }
                }
        }

        return 1;
}

static SPIFI_DEV_DATA_T *findDeviceMatch(SPIFI_DEV_DATA_T *pHead, SPIFI_DEVICE_ID_T *pID, uint8_t checkExtended)
{
        SPIFI_DEV_DATA_T *pNode;

        /* search the list looking for a match. Skip over head node since
             it is a dummy node and NEVER contains data */
        for (pNode = pHead->pNext; pNode != NULL; pNode = pNode->pNext) {
                /* Manufacturer and part match? */
                if ((pID->mfgId[0] == pNode->pDevData->id.mfgId[0])  &&
                        (pID->mfgId[1] == pNode->pDevData->id.mfgId[1]) &&
                        (pID->mfgId[2] == pNode->pDevData->id.mfgId[2])) {
                        /* If extended data check it */
                        uint8_t matchFound = 1;
                        if (checkExtended) {
                                matchFound = checkExtendedMatch(pNode, pID);

                        }
                        /* Match, time to exit */
                        if (matchFound) {
                                return pNode;
                        }
                }
        }
        return NULL;
}

/* Detect if this device exists at the passed base address, returns 0 if the
   device doesn't exist of the required memory allocation size for the device
   context if the device exists. */
static SPIFI_DEV_DATA_T *devDetect(uint32_t spifiCtrlAddr, SPIFI_DEV_FAMILY_T *familyNode)
{
        SPIFI_DEV_DATA_T *devNode;
        uint32_t idx;
        SPIFI_DEVICE_ID_T id;
        SPIFI_DEVICE_ID_T idVerify;

        /* Do not ask for extended ID information yet */
        id.extCount = 0;
        idVerify.extCount = 0;

        /* Read device ID three times to validate. First read on a hard reset isn't reliable */
        familyNode->pDesc->pPrvDevGetID(spifiCtrlAddr, &id);
        familyNode->pDesc->pPrvDevGetID(spifiCtrlAddr, &id);
        familyNode->pDesc->pPrvDevGetID(spifiCtrlAddr, &idVerify);

        /* Compare both reads to make sure they match. If any byte doesn't compare, abort. */
        for (idx = 0; idx < sizeof(id.mfgId); ++idx) {
                if (id.mfgId[idx] != idVerify.mfgId[idx]) {
                        return NULL;
                }
        }

        /* Find match for 3 bytes.  If found, check to see if there is extended id information */
        devNode = findDeviceMatch(familyNode->pDesc->pDevList, &id, 0);
        if ((devNode) && (devNode->pDevData->id.extCount)) {

                /* read ID + extended ID data */
                id.extCount = devNode->pDevData->id.extCount;
                familyNode->pDesc->pPrvDevGetID(spifiCtrlAddr, &id);

                /* Now get the node that matches JEDEC and extended data */
                devNode = findDeviceMatch(familyNode->pDesc->pDevList, &id, 1);
        }

        return devNode;
}

/* Detect first SPIFI FLASH device at the passed base address */
static SPIFI_DEV_FAMILY_T *spifiPartDetect(uint32_t spifiCtrlAddr, SPIFI_DEV_DATA_T * *devData)
{
        SPIFI_DEV_FAMILY_T *pNode;

        /* Loop through the library and check for detected devices.
             skip over head node because it is NEVER used. */
        for (pNode = famHead.pNext; pNode != NULL; pNode = pNode->pNext) {
                /* Match at this index */
                if ((*devData = devDetect(spifiCtrlAddr, pNode)) != NULL) {
                        return pNode;
                }
        }

        return NULL;
}

static uint32_t spifiCalculateHandleSize(SPIFI_DEV_FAMILY_T *devData)
{
        /* This is the size needed for the device context instance by the driver */
        return sizeof(SPIFI_HANDLE_T) + sizeof(SPIFI_INFODATA_T) +
                   devData->pDesc->prvDataSize;
}

/*****************************************************************************
 * Public functions
 ****************************************************************************/

SPIFI_DEV_FAMILY_T *spifiRegisterFamily(SPIFI_DEV_FAMILY_T *(*regFx)(void))
{
        SPIFI_DEV_FAMILY_T *pFam;

        /* Get the family node from the user */
        pFam = regFx();

        /* Insert the node into the beginning of the list */
        pFam->pNext = famHead.pNext;
        famHead.pNext = pFam;

        /* update the count of known families */
        ++famCount;

        /* Nothing to do here yet */
        return pFam;
}

/* Report the library version number */
uint16_t spifiGetLibVersion(void)
{
        return (LIBRARY_VERSION_MAJOR << 8) | LIBRARY_VERSION_MINOR;
}

/* Initialize the SPIFILIB driver */
SPIFI_ERR_T spifiInit(uint32_t spifiCtrlAddr, uint8_t reset)
{
        LPC_SPIFI_CHIPHW_T *pSpifiCtrlAddr = (LPC_SPIFI_CHIPHW_T *) spifiCtrlAddr;

        if (reset) {
                /* Reset controller */
                spifi_HW_ResetController(pSpifiCtrlAddr);

                /* Set intermediate data and memcmd registers. */
                spifi_HW_SetIDATA(pSpifiCtrlAddr, 0x0);
                spifi_HW_SetMEMCMD(pSpifiCtrlAddr, 0);

                spifi_HW_ResetController(pSpifiCtrlAddr);

                /* Setup SPIFI controller */
                spifi_HW_SetCtrl(pSpifiCtrlAddr,
                                                 (SPIFI_CTRL_TO(1000) |
                                                  SPIFI_CTRL_CSHI(15) |
                                                  SPIFI_CTRL_RFCLK(1) |
                                                  SPIFI_CTRL_FBCLK(1)));
        }

        /* Nothing to do here yet */
        return SPIFI_ERR_NONE;
}

/* Shutdown the SPIFILIB driver */
void spifiDeInit(void)
{
        /* Nothing to do here yet */
}

/* performs device specific initialization */
SPIFI_ERR_T spifiDevInit(SPIFI_HANDLE_T *pHandle)
{
        SPIFI_ERR_T retValue = SPIFI_ERR_NONE;

        /* call device specific initializtion if provided */
        if (pHandle->pDev->init) {
                retValue = pHandle->pDev->init(pHandle);
        }

        /* make sure the controller is not in memMode */
        spifiDevSetMemMode(pHandle, 0);

        return retValue;
}

/* performs device specific de-initialization */
SPIFI_ERR_T spifiDevDeInit(SPIFI_HANDLE_T *pHandle)
{
        SPIFI_ERR_T retValue = SPIFI_ERR_NONE;

        /* call device specific de-init if provided */
        if (pHandle->pDev->deInit) {
                retValue = pHandle->pDev->deInit(pHandle);
        }

        /* make sure the controller is in memMode */
        spifiDevSetMemMode(pHandle, 1);

        return retValue;
}

/* Converts a SPIFILIB error code into a meaningful string */
const char *spifiReturnErrString(SPIFI_ERR_T errCode)
{
        if (((unsigned int) errCode) < SPIFI_ERR_LASTINDEX) {
                return spifiErrStrings[errCode];
        }

        return noName;
}

/* Returns information on the device */
uint32_t spifiDevGetInfo(SPIFI_HANDLE_T *pHandle, SPIFI_INFO_ID_T infoId)
{
        uint32_t val;

        switch (infoId) {
        case SPIFI_INFO_DEVSIZE:
                val = pHandle->pInfoData->numBlocks * pHandle->pInfoData->blockSize;
                break;

        case SPIFI_INFO_ERASE_BLOCKS:
                val = pHandle->pInfoData->numBlocks;
                break;

        case SPIFI_INFO_ERASE_BLOCKSIZE:
                val = pHandle->pInfoData->blockSize;
                break;

        case SPIFI_INFO_ERASE_SUBBLOCKS:
                val = pHandle->pInfoData->numSubBlocks;
                break;

        case SPIFI_INFO_ERASE_SUBBLOCKSIZE:
                val = pHandle->pInfoData->subBlockSize;
                break;

        case SPIFI_INFO_PAGESIZE:
                val = pHandle->pInfoData->pageSize;
                break;

        case SPIFI_INFO_MAXREADSIZE:
                val = pHandle->pInfoData->maxReadSize;
                break;

        case SPIFI_INFO_MAXCLOCK:
                val = pHandle->pInfoData->maxClkRate;
                break;

        case SPIFI_INFO_CAPS:
                val = pHandle->pInfoData->caps;
                break;

        case SPIFI_INFO_STATUS:
                val = pHandle->pDev->getStatus(pHandle);
                break;

        case SPIFI_INFO_OPTIONS:
                val = pHandle->pInfoData->opts;
                break;

        default:
                val = 0;
                break;
        }

        return val;
}

/* Returns status of memory mode */
uint8_t spifiDevGetMemoryMode(SPIFI_HANDLE_T *pHandle)
{
        LPC_SPIFI_CHIPHW_T *pSpifiCtrlAddr = (LPC_SPIFI_CHIPHW_T *) pHandle->pInfoData->spifiCtrlAddr;

        return (spifi_HW_GetStat(pSpifiCtrlAddr) & SPIFI_STAT_MCINIT) != 0;
}

SPIFI_ERR_T spifiDevSetMemMode(SPIFI_HANDLE_T *pHandle, uint8_t enMMode)
{
        uint32_t cmdValue;
        LPC_SPIFI_CHIPHW_T *pSpifiCtrlAddr = (LPC_SPIFI_CHIPHW_T *) pHandle->pInfoData->spifiCtrlAddr;

        /* RESET the memMode controller */
        spifi_HW_ResetController(pSpifiCtrlAddr);

        /* Wait for HW to acknowledge the reset. */
        spifi_HW_WaitRESET(pSpifiCtrlAddr);

        if (enMMode) {
                /* Get the device specific memory mode command */
                cmdValue = pHandle->pDev->getMemModeCmd(pHandle, enMMode);

                /* Specify the intermediate data byte. */
                spifi_HW_SetIDATA(pSpifiCtrlAddr, 0xA0);

                /* Set the appropriate values in the command reg. */
                spifi_HW_SetCmd(pSpifiCtrlAddr, cmdValue);
                spifi_HW_WaitCMD(pSpifiCtrlAddr);
                spifi_HW_SetMEMCMD(pSpifiCtrlAddr, cmdValue);
        }
        else {
                spifi_HW_SetIDATA(pSpifiCtrlAddr, 0x0);
                spifi_HW_SetMEMCMD(pSpifiCtrlAddr, 0);

                /* RESET the memMode controller */
                spifi_HW_ResetController(pSpifiCtrlAddr);

                /* Wait for HW to acknowledge the reset. */
                spifi_HW_WaitRESET(pSpifiCtrlAddr);
        }
        return SPIFI_ERR_NONE;
}

/* Return the number of supported device families in this driver */
uint32_t spifiGetSuppFamilyCount(void)
{
        /* return number of registered devices */
        return famCount;
}

/* Return the driver family name for a specific index */
const char *spifiGetSuppFamilyName(uint32_t index)
{
        uint32_t idx;
        SPIFI_DEV_FAMILY_T *pNode;

        if (index >= famCount) {
                return noName;
        }

        /* cycle through the list of families skipping over head node since it
           is NEVER used. Once we break out of this loop pNode should be
           pointing at the correct node.  */
        pNode = famHead.pNext;
        for (idx = 0; idx < index; ++idx) {
                pNode = pNode->pNext;
        }

        return pNode->pDesc->pFamName;
}

/* Detect and return memory needed for device handle at passed address */
uint32_t spifiGetHandleMemSize(uint32_t spifiCtrlAddr)
{
        uint32_t bytesNeeded = 0;
        SPIFI_DEV_FAMILY_T *detectedPart;
        SPIFI_DEV_DATA_T *devData;

        /* Find first device at the base address */
        detectedPart = spifiPartDetect(spifiCtrlAddr, &devData);
        if (detectedPart) {
                /* This is the size needed for the device context instance by the driver */
                bytesNeeded = spifiCalculateHandleSize(detectedPart);
        }

        return bytesNeeded;
}

/* Initialize driver and hardware for a specific device */
SPIFI_HANDLE_T *spifiInitDevice(void *pMem, uint32_t sizePMem, uint32_t spifiCtrlAddr, uint32_t baseAddr)
{
        SPIFI_DEV_FAMILY_T *detectedPart;
        SPIFI_DEV_DATA_T *devData;
        SPIFI_HANDLE_T *pSpifiHandle;
        uint32_t *pMem32 = (uint32_t *) pMem;

        /* Is the passed buffer size aligned on a 32-bit boundary? */
        if (((uint32_t) pMem32 & 0x3) != 0) {
                return NULL;
        }

        /* Detect the device at at the base address and abort on error. */
        detectedPart = spifiPartDetect(spifiCtrlAddr, &devData);
        if (!detectedPart) {
                return NULL;
        }

        /* Is passed memory space big enough? */
        if (spifiCalculateHandleSize(detectedPart) > sizePMem) {
                return NULL;
        }

        /* Setup handle */
        pSpifiHandle = (SPIFI_HANDLE_T *) pMem;

        /* Clear entire device context areas */
        memset(pMem, 0, sizePMem);

        /* Setup device info region */
        pMem32 += (sizeof(SPIFI_HANDLE_T) / sizeof(uint32_t));
        pSpifiHandle->pInfoData = (SPIFI_INFODATA_T *) pMem32;

        /* Copy the detected device specific data into the handle */
        memcpy(&pSpifiHandle->pInfoData->id, &devData->pDevData->id, sizeof(pSpifiHandle->pInfoData->id));

        /* Setup device private data region */
        pMem32 += (sizeof(SPIFI_INFODATA_T) / sizeof(uint32_t));
        pSpifiHandle->pDevData = (void *) pMem32;

        /* Setup device specific data */
        pSpifiHandle->pInfoData->spifiCtrlAddr = spifiCtrlAddr;
        pSpifiHandle->pInfoData->baseAddr = baseAddr;
        pSpifiHandle->pInfoData->numBlocks = devData->pDevData->blks;
        pSpifiHandle->pInfoData->blockSize = devData->pDevData->blkSize;
        pSpifiHandle->pInfoData->numSubBlocks = devData->pDevData->subBlks;
        pSpifiHandle->pInfoData->subBlockSize = devData->pDevData->subBlkSize;
        pSpifiHandle->pInfoData->pageSize = devData->pDevData->pageSize;
        pSpifiHandle->pInfoData->maxReadSize = devData->pDevData->maxReadSize;
        pSpifiHandle->pInfoData->maxClkRate = devData->pDevData->maxClkRate;
        pSpifiHandle->pInfoData->caps = devData->pDevData->caps;
    pSpifiHandle->pInfoData->pDevName = devData->pDevData->pDevName;
    
        /* Call device setup */
        pSpifiHandle->pInfoData->lastErr = detectedPart->pDesc->pPrvDevSetup(pSpifiHandle, spifiCtrlAddr, baseAddr);

        if (pSpifiHandle->pInfoData->lastErr != SPIFI_ERR_NONE) {
                return NULL;
        }
    
        /* Call the device specific init */
    if (pSpifiHandle->pDev->init) {
        pSpifiHandle->pInfoData->lastErr = pSpifiHandle->pDev->init(pSpifiHandle);
        if (pSpifiHandle->pInfoData->lastErr != SPIFI_ERR_NONE) {
            return NULL;
        }
    }

        return pSpifiHandle;
}

SPIFI_ERR_T spifiDevSetOpts(SPIFI_HANDLE_T *pHandle, uint32_t options, uint8_t set)
{
        /* default to not supported */
        SPIFI_ERR_T retValue = SPIFI_ERR_NOTSUPPORTED;
        uint8_t memMode;

        if ((options & pHandle->pInfoData->caps) == options) {

                /* first get the current memory mode */
                memMode = spifiDevGetMemoryMode(pHandle);

                /* Set the option in the driver so other routines will act accordingly */
                if (set) {
                        pHandle->pInfoData->opts |= options;
                }
                else {
                        pHandle->pInfoData->opts &= ~options;
                }

                /* Perform device specific setup for the option */
                retValue = pHandle->pDev->setOpts(pHandle, options, set);

                /* If changing the state of Quad support update memory mode */
                if (options & SPIFI_OPT_USE_QUAD) {
                        spifiDevSetMemMode(pHandle, memMode);
                }
        }
        return retValue;
}

/* Returns the address mapped to an block number */
uint32_t spifiGetAddrFromBlock(SPIFI_HANDLE_T *pHandle, uint32_t blockNum)
{
        uint32_t baseAddr = 0xFFFFFFFF;

        if (blockNum < pHandle->pInfoData->numBlocks) {
                baseAddr = pHandle->pInfoData->baseAddr + (blockNum * pHandle->pInfoData->blockSize);
        }

        return baseAddr;
}

/* Returns the starting address of a sub-block number */
uint32_t spifiGetAddrFromSubBlock(SPIFI_HANDLE_T *pHandle, uint32_t subBlockNum)
{
        uint32_t baseAddr = ~0UL;

    /* If the device provides a specific method for calculating the address use it. */
    if (!pHandle->pDev->subBlockCmd) {
        /* If sub-blocks are not supported (.e numSubBlocks = 0) then return error */
        if (subBlockNum < pHandle->pInfoData->numSubBlocks) {
            baseAddr = pHandle->pInfoData->baseAddr + (subBlockNum * pHandle->pInfoData->subBlockSize);
        }
    }
    else {
        baseAddr = pHandle->pDev->subBlockCmd(pHandle, SPIFI_PCMD_SUB_BLOCK_TO_ADDR, subBlockNum);
    } 
    
        return baseAddr;
}

/* Returns the block number the passedd= address is located in */
uint32_t spifiGetBlockFromAddr(SPIFI_HANDLE_T *pHandle, uint32_t addr)
{
        uint32_t block;
        block = (addr - pHandle->pInfoData->baseAddr) / pHandle->pInfoData->blockSize;

        if (block >= pHandle->pInfoData->numBlocks) {
                return ~0UL;
        }

        return block;
}

/* Returns the sub-block number the passed address is located in */
uint32_t spifiGetSubBlockFromAddr(SPIFI_HANDLE_T *pHandle, uint32_t addr)
{
        uint32_t subBlock;

    /* If device does not support sub-blocks return error */
    if (!pHandle->pInfoData->subBlockSize) {
        return ~0UL;
    }
    
    if (!pHandle->pDev->subBlockCmd) {
        subBlock = (addr - pHandle->pInfoData->baseAddr) / pHandle->pInfoData->subBlockSize;

        if (subBlock >= pHandle->pInfoData->numSubBlocks) {
            return ~0UL;
        }
    }
    else {
        subBlock = pHandle->pDev->subBlockCmd(pHandle, SPIFI_PCMD_ADDR_TO_SUB_BLOCK, addr);
    }

        return subBlock;
}

/* Returns the first sub-block in hte passed block */
uint32_t spifiGetSubBlockFromBlock(SPIFI_HANDLE_T *pHandle, uint32_t blockNum)
{
        uint32_t subBlock = ~0UL;

    if (!pHandle->pDev->subBlockCmd) {
        /* If the blockNum passed is larger than this device, 
           or if sub-blocks are not supported report error */
        if ((blockNum >= pHandle->pInfoData->numBlocks) ||
           (!pHandle->pInfoData->subBlockSize)) {
            return subBlock;
        }
        /* Calculate the sub-block number based on detected params */
        subBlock = (blockNum * (pHandle->pInfoData->blockSize / pHandle->pInfoData->subBlockSize));
    }
    else {
        
        subBlock = pHandle->pDev->subBlockCmd(pHandle, SPIFI_PCMD_BLOCK_TO_SUB_BLOCK, blockNum);
        }

        return subBlock;
}

/* Program the device with the passed buffer */
SPIFI_ERR_T spifiProgram(SPIFI_HANDLE_T *pHandle, uint32_t addr, const uint32_t *writeBuff, uint32_t bytes)
{
        uint32_t sendBytes;
        SPIFI_ERR_T err = SPIFI_ERR_NONE;

        /* Program using up to page size */
        while ((bytes > 0) && (err == SPIFI_ERR_NONE)) {
                sendBytes = bytes;
                if (sendBytes > pHandle->pInfoData->pageSize) {
                        sendBytes = pHandle->pInfoData->pageSize;
                }

                err = pHandle->pDev->pageProgram(pHandle, addr, writeBuff, sendBytes);
                addr += sendBytes;
                writeBuff += (sendBytes / sizeof(uint32_t));
                bytes -= sendBytes;
        }

        return err;
}

/* Read the device into the passed buffer */
SPIFI_ERR_T spifiRead(SPIFI_HANDLE_T *pHandle, uint32_t addr, uint32_t *readBuff, uint32_t bytes)
{
        uint32_t readBytes;
        SPIFI_ERR_T err = SPIFI_ERR_NONE;

        /* Read using up to the maximum read size */
        while ((bytes > 0) && (err == SPIFI_ERR_NONE)) {
                readBytes = bytes;
                if (readBytes > pHandle->pInfoData->maxReadSize) {
                        readBytes = pHandle->pInfoData->maxReadSize;
                }

                err = pHandle->pDev->read(pHandle, addr, readBuff, readBytes);
                addr += readBytes;
                readBuff += (readBytes / sizeof(uint32_t));
                bytes -= readBytes;
        }

        return err;
}

/* Erase multiple blocks */
SPIFI_ERR_T spifiErase(SPIFI_HANDLE_T *pHandle, uint32_t firstBlock, uint32_t numBlocks)
{
        SPIFI_ERR_T err = SPIFI_ERR_NONE;

        if ((firstBlock + numBlocks) > pHandle->pInfoData->numBlocks) {
                return SPIFI_ERR_RANGE;
        }

        /* Only perform erase if numBlocks is != 0 */
        for (; (numBlocks); ++firstBlock, --numBlocks) {
                err = pHandle->pDev->eraseBlock(pHandle, firstBlock);
                if (err != SPIFI_ERR_NONE) {
                        break;
                }
        }

        return err;
}

/* Erase multiple blocks by address range */
SPIFI_ERR_T spifiEraseByAddr(SPIFI_HANDLE_T *pHandle, uint32_t firstAddr, uint32_t lastAddr)
{
        uint32_t firstBlock, lastBlock;
        SPIFI_ERR_T err = SPIFI_ERR_RANGE;

        /* Get block numbers for addresses */
        firstBlock = spifiGetBlockFromAddr(pHandle, firstAddr);
        lastBlock = spifiGetBlockFromAddr(pHandle, lastAddr);

        /* Limit to legal address range */
        if ((firstBlock != ~0UL) && (lastBlock != ~0UL)) {
                err = spifiErase(pHandle, firstBlock, ((lastBlock - firstBlock) + 1));
        }

        return err;
}
