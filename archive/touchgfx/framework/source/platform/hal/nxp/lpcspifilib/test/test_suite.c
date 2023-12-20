/*
 * @brief LPCSPIFILIB test_suite
 *
 * @note
 * Copyright(C) NXP Semiconductors, 2013
 * All rights reserved.
 *
 * @par
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * LPC products.  This software is supplied "AS IS" without any warranties of
 * any kind, and NXP Semiconductors and its licensor disclaim any and
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

#include "test_suite.h"

#include <string.h>
#include "board.h"
#include "spifilib_api.h"
#include "stopwatch.h"

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/
typedef struct {
        SPIFI_HANDLE_T *pSpifi;
        uint32_t caps;
        uint32_t numBlocks;
        uint32_t numSubBlocks;
        uint32_t pageSize;
        uint32_t blockSize;
        uint32_t subBlockSize;
} DEVICE_TEST_DATA_T;

typedef struct {
        uint32_t buffer[TEST_BUFFSIZE / sizeof(uint32_t)];
} TEST_BUFFER_T;

/* Declare test buffers, 32-bit aligned */
static TEST_BUFFER_T testBuff[TEST_BUFFER_COUNT];

static uint32_t blinkToggleRate = 500;

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Private functions
 ****************************************************************************/
static void populateDeviceData(SPIFI_HANDLE_T *pSpifi, DEVICE_TEST_DATA_T *devData, uint32_t pageLimit)
{
        /* save pointer to device */
        devData->pSpifi = pSpifi;

        /* Get device specific information */
        devData->caps = spifiDevGetInfo(pSpifi, SPIFI_INFO_CAPS);
        devData->pageSize = spifiDevGetInfo(pSpifi, SPIFI_INFO_PAGESIZE);
        devData->numBlocks = spifiDevGetInfo(pSpifi, SPIFI_INFO_ERASE_BLOCKS);
        devData->numSubBlocks = spifiDevGetInfo(pSpifi, SPIFI_INFO_ERASE_SUBBLOCKS);
        devData->blockSize = spifiDevGetInfo(pSpifi, SPIFI_INFO_ERASE_BLOCKSIZE);
        devData->subBlockSize = spifiDevGetInfo(pSpifi, SPIFI_INFO_ERASE_SUBBLOCKSIZE);
    
        /* It would be better to use malloc() to create a buffer size to
           the program page */
        if ((pageLimit) && (pageLimit < devData->pageSize)) {
                /* Very large page device, write a partial buffer instead */
                devData->pageSize = pageLimit;
        }
}

static void bulkEraseDevice(DEVICE_TEST_DATA_T *devData, uint8_t useAddrMode, uint8_t verify)
{
        SPIFI_ERR_T errCode;

        /* Erase full device */
        if (verify) {
                DEBUGOUT("Erasing device via %s... ", useAddrMode ? "Addr" : "BULK");
        }

        if (useAddrMode) {
                uint32_t beginAddr = spifiGetAddrFromBlock(devData->pSpifi, 0);
                uint32_t endAddr = beginAddr + ((devData->numBlocks * devData->blockSize) - 1);

                errCode = spifiEraseByAddr(devData->pSpifi, beginAddr, endAddr);

        }
        else {
                errCode = spifiDevEraseAll(devData->pSpifi);

        }
        if (errCode != SPIFI_ERR_NONE) {
                test_suiteError(useAddrMode ? "BULK EraseByAddr" : "BULK Erase", errCode);
        }

        /* Verify entire device is erased */
        if (verify) {
                test_suiteVerifyDeviceErased(devData->pSpifi, true);
        }
}

static void rangeCheckEraseDevice(DEVICE_TEST_DATA_T *devData, uint8_t useAddrMode)
{
        SPIFI_ERR_T errCode;

        DEBUGOUT("Range checking EraseBy%s... ", useAddrMode ? "Addr" : "BULK");

        /* Check out of bounds address */
        if (useAddrMode) {
                uint32_t beginAddr = spifiGetAddrFromBlock(devData->pSpifi, 0);
                uint32_t endAddr = beginAddr + ((devData->numBlocks * devData->blockSize) - 1);

                errCode = spifiEraseByAddr(devData->pSpifi, beginAddr, endAddr + 1);
                if (SPIFI_ERR_RANGE != errCode) {
                        test_suiteError("Erase range check Addr mode", errCode);
                }
        }
        else {
                /* check out of bounds block */
                uint32_t endBlock = devData->numBlocks;

                errCode = spifiErase(devData->pSpifi, 0, endBlock + 1);
                if (SPIFI_ERR_RANGE != errCode) {
                        test_suiteError("Erase range check BLOCK mode", errCode);
                }
        }
}

static void setMemModeVerify(SPIFI_HANDLE_T *pSpifi, uint8_t newState)
{
        /* set the specified state */
        spifiDevSetMemMode(pSpifi, newState);

        /* Check to make sure it is in the mode we just set it to */
        if (spifiDevGetMemoryMode(pSpifi) != newState) {
                test_suiteError("Inconsistent MEMMODE", SPIFI_ERR_GEN);
        }
}

/*****************************************************************************
 * Public functions
 ****************************************************************************/
void test_suiteLibHelperBattery(SPIFI_HANDLE_T *pSpifi)
{
        uint32_t hwBaseAddress;
        uint32_t minAddress;
        uint32_t maxAddress;
        uint32_t testBlockNum;
        DEVICE_TEST_DATA_T devData;

        populateDeviceData(pSpifi, &devData, 0);
        hwBaseAddress = pSpifi->pInfoData->baseAddr;

        DEBUGOUT("Begin LibHelper Test Battery\r\n");

        /* First check basic consitency of device params */
        DEBUGOUT("Checking Block Sub-Block size consistency... ");
    if (devData.caps & SPIFI_CAP_SUBBLKERASE) {
        if ((devData.numSubBlocks == 0) ||
            (devData.subBlockSize == 0)) {
            test_suiteError("LibDeviceParamsBattery->block,sub-block consistency)", SPIFI_ERR_GEN);
        } 
    }
    else {
        if ((devData.numSubBlocks != 0) ||
            (devData.subBlockSize != 0)) {
                test_suiteError("LibDeviceParamsBattery->block,sub-block consistency)", SPIFI_ERR_GEN);
        }
    }
        DEBUGOUT("Passed\r\n");

        /* Next check the baseAddress */
        DEBUGOUT("Checking device base Address... ");
        if (hwBaseAddress == 0xffffffff) {
                test_suiteError("LibDeviceParamsBattery->baseAddr consistency)", SPIFI_ERR_GEN);
        }
        DEBUGOUT("Passed\r\n");

        /* Test spifiGetAddrFromBlock() */
        DEBUGOUT("Testing spifiGetAddrFromBlock()... ");
        if (spifiGetAddrFromBlock(pSpifi, 0) != hwBaseAddress) {
                test_suiteError("LibHelperBattery->spifiGetAddrFromBlock(min)", SPIFI_ERR_GEN);
        }
        testBlockNum = devData.numBlocks - 1;
        if (spifiGetAddrFromBlock(pSpifi, testBlockNum) != ((testBlockNum * devData.blockSize) + hwBaseAddress)) {
                test_suiteError("LibHelperBattery->spifiGetAddrFromBlock(max)", SPIFI_ERR_GEN);
        }
        if (spifiGetAddrFromBlock(pSpifi, (devData.numBlocks)) != 0xffffffff) {
                test_suiteError("LibHelperBattery->spifiGetAddrFromBlock(max+1)", SPIFI_ERR_GEN);
        }
        testBlockNum = devData.numBlocks / 2;   /* choose a block somewhere in the middle */
        if (spifiGetAddrFromBlock(pSpifi, testBlockNum) != ((testBlockNum * devData.blockSize) + hwBaseAddress)) {
                test_suiteError("LibHelperBattery->spifiGetAddrFromBlock(numBlocks/2)", SPIFI_ERR_GEN);
        }
        DEBUGOUT("Passed\r\n");

    /*Test spifiGetBlockFromAddr() */
        DEBUGOUT("Testing spifiGetBlockFromAddr()... ");
        minAddress = hwBaseAddress;
        maxAddress = hwBaseAddress + ((devData.numBlocks * devData.blockSize) - 1);
        if (spifiGetBlockFromAddr(pSpifi, minAddress) != 0) {
                test_suiteError("LibHelperBattery->spifiGetBlockFromAddr(min)", SPIFI_ERR_GEN);
        }
        if ((minAddress) && ((spifiGetBlockFromAddr(pSpifi, (minAddress - 1))) != 0xffffffff)) {
                test_suiteError("LibHelperBattery->spifiGetBlockFromAddr(min-1)", SPIFI_ERR_GEN);
        }
        if ((minAddress) && ((spifiGetBlockFromAddr(pSpifi, 0)) != 0xffffffff)) {
                test_suiteError("LibHelperBattery->spifiGetBlockFromAddr(0)", SPIFI_ERR_GEN);
        }
        if (spifiGetBlockFromAddr(pSpifi, maxAddress) != (devData.numBlocks - 1)) {
                test_suiteError("LibHelperBattery->spifiGetBlockFromAddr(max)", SPIFI_ERR_GEN);
        }
        if (spifiGetBlockFromAddr(pSpifi, (maxAddress + 1)) != 0xffffffff) {
                test_suiteError("LibHelperBattery->spifiGetBlockFromAddr(max+1)", SPIFI_ERR_GEN);
        }
        if (spifiGetBlockFromAddr(pSpifi, 0xffffffff) != 0xffffffff) {
                test_suiteError("LibHelperBattery->spifiGetBlockFromAddr(0xffffffff)", SPIFI_ERR_GEN);
        }
        if (spifiGetBlockFromAddr(pSpifi, (minAddress + devData.blockSize)) != 1) {
                test_suiteError("LibHelperBattery->spifiGetBlockFromAddr(block1Addr)", SPIFI_ERR_GEN);
        }
        if (spifiGetBlockFromAddr(pSpifi, ((minAddress + devData.blockSize) - 1)) != 0) {
                test_suiteError("LibHelperBattery->spifiGetBlockFromAddr(block0Addr)", SPIFI_ERR_GEN);
        }
        DEBUGOUT("Passed\r\n");
    
        /*Test spifiGetAddrFromSubBlock() if sub blocks are supported */
    if (!(devData.caps & SPIFI_CAP_SUBBLKERASE)){
        DEBUGOUT("Sub-Block Erase NOT supported! Checking return values for conversion routines... ");
        if (spifiGetAddrFromSubBlock(pSpifi, 1) != 0xffffffff) {
                    test_suiteError("LibHelperBattery->spifiGetAddrFromSubBlock(1) != 0xffffffff", SPIFI_ERR_GEN);
            }
        if (spifiGetSubBlockFromAddr(pSpifi, 0) != 0xffffffff) {
            test_suiteError("LibHelperBattery->spifiGetSubBlockFromAddr(0) != 0xffffffff", SPIFI_ERR_GEN);
        }
        if (spifiGetSubBlockFromBlock(pSpifi, 1) != 0xffffffff) {
                test_suiteError("LibHelperBattery->spifiGetSubBlockFromBlock(1)", SPIFI_ERR_GEN);
        }
        DEBUGOUT("Passed\r\n");
    }
        else {
            DEBUGOUT("Testing spifiGetAddrFromSubBlock()... ");
            if (spifiGetAddrFromSubBlock(pSpifi, 0) != hwBaseAddress) {
                    test_suiteError("LibHelperBattery->spifiGetAddrFromSubBlock(min)", SPIFI_ERR_GEN);
            }
            testBlockNum = devData.numSubBlocks - 1;
            if (spifiGetAddrFromSubBlock(pSpifi, testBlockNum) != ((testBlockNum * devData.subBlockSize) + hwBaseAddress)) {
                    test_suiteError("LibHelperBattery->spifiGetAddrFromSubBlock(max)", SPIFI_ERR_GEN);
            }
            if (spifiGetAddrFromSubBlock(pSpifi, (devData.numSubBlocks)) != 0xffffffff) {
                    test_suiteError("LibHelperBattery->spifiGetAddrFromSubBlock(max+1)", SPIFI_ERR_GEN);
            }
            testBlockNum = devData.numSubBlocks / 2;/* choose a block somewhere in the middle */
            if (spifiGetAddrFromSubBlock(pSpifi, testBlockNum) != ((testBlockNum * devData.subBlockSize) + hwBaseAddress)) {
                    test_suiteError("LibHelperBattery->spifiGetAddrFromSubBlock(numSubBlocks/2)", SPIFI_ERR_GEN);
            }
            ++testBlockNum;     /* choose a block somewhere in the middle + 1 */
            if (spifiGetAddrFromSubBlock(pSpifi, testBlockNum) != ((testBlockNum * devData.subBlockSize) + hwBaseAddress)) {
                    test_suiteError("LibHelperBattery->spifiGetAddrFromSubBlock((numSubBlocks/2)+1)", SPIFI_ERR_GEN);
            }
            DEBUGOUT("Passed\r\n");
    
        /*Test spifiGetSubBlockFromAddr() */
        DEBUGOUT("Testing spifiGetSubBlockFromAddr()... ");
        minAddress = hwBaseAddress;
        maxAddress = hwBaseAddress + ((devData.numBlocks * devData.blockSize) - 1);
        if (spifiGetSubBlockFromAddr(pSpifi, minAddress) != 0) {
            test_suiteError("LibHelperBattery->spifiGetSubBlockFromAddr(min)", SPIFI_ERR_GEN);
        }
        if ((minAddress) && ((spifiGetSubBlockFromAddr(pSpifi, (minAddress - 1))) != 0xffffffff)) {
            test_suiteError("LibHelperBattery->spifiGetSubBlockFromAddr(min-1)", SPIFI_ERR_GEN);
        }
        if ((minAddress) && ((spifiGetSubBlockFromAddr(pSpifi, 0)) != 0xffffffff)) {
            test_suiteError("LibHelperBattery->spifiGetSubBlockFromAddr(0)", SPIFI_ERR_GEN);
        }
        if (spifiGetSubBlockFromAddr(pSpifi, maxAddress) != (devData.numSubBlocks - 1)) {
            test_suiteError("LibHelperBattery->spifiGetSubBlockFromAddr(max)", SPIFI_ERR_GEN);
        }
        if (spifiGetSubBlockFromAddr(pSpifi, (maxAddress + 1)) != 0xffffffff) {
            test_suiteError("LibHelperBattery->spifiGetSubBlockFromAddr(max+1)", SPIFI_ERR_GEN);
        }
        if (spifiGetSubBlockFromAddr(pSpifi, 0xffffffff) != 0xffffffff) {
            test_suiteError("LibHelperBattery->spifiGetSubBlockFromAddr(0xffffffff)", SPIFI_ERR_GEN);
        }
        if (spifiGetSubBlockFromAddr(pSpifi, (minAddress + devData.subBlockSize)) != 1) {
            test_suiteError("LibHelperBattery->spifiGetSubBlockFromAddr(subBlock1Addr)", SPIFI_ERR_GEN);
        }
        if (spifiGetSubBlockFromAddr(pSpifi, ((minAddress + devData.subBlockSize) - 1)) != 0) {
            test_suiteError("LibHelperBattery->spifiGetSubBlockFromAddr(subBlock0Addr)", SPIFI_ERR_GEN);
        }
        DEBUGOUT("Passed\r\n");

        /*Test spifiGetSubBlockFromBlock() */
        {
            uint32_t maxBlock = devData.numBlocks - 1;
            uint32_t subBlocksInBlock = devData.blockSize / devData.subBlockSize;

            DEBUGOUT("Testing spifiGetSubBlockFromBlock()... ");
            if (spifiGetSubBlockFromBlock(pSpifi, 0) != 0 ) {
                test_suiteError("LibHelperBattery->spifiGetSubBlockFromBlock(min)", SPIFI_ERR_GEN);
            }
            if (spifiGetSubBlockFromBlock(pSpifi, maxBlock) != (devData.numSubBlocks - subBlocksInBlock)) {
                test_suiteError("LibHelperBattery->spifiGetSubBlockFromBlock(max)", SPIFI_ERR_GEN);
            }
            if (spifiGetSubBlockFromBlock(pSpifi, (maxBlock + 1)) != 0xffffffff) {
                test_suiteError("LibHelperBattery->spifiGetSubBlockFromBlock(max+1)", SPIFI_ERR_GEN);
            }
            if (spifiGetSubBlockFromBlock(pSpifi, 0xffffffff) != 0xffffffff) {
                test_suiteError("LibHelperBattery->spifiGetSubBlockFromBlock(0xffffffff)", SPIFI_ERR_GEN);
            }
            if (spifiGetSubBlockFromBlock(pSpifi, 1) != subBlocksInBlock) {
                test_suiteError("LibHelperBattery->spifiGetSubBlockFromBlock(1)", SPIFI_ERR_GEN);
            }
            DEBUGOUT("Passed\r\n");
        }
    }
    
        /* Finally check memMode reporting */
        setMemModeVerify(pSpifi, true);
        setMemModeVerify(pSpifi, false);

        DEBUGOUT("LibHelper Battery Test Complete!\r\n\r\n");
}

void test_suiteLibEraseBattery(SPIFI_HANDLE_T *pSpifi, uint8_t useAddrMode)
{
        DEVICE_TEST_DATA_T devData;
        uint32_t idx;
        uint32_t testBlock;
        SPIFI_ERR_T errCode;

        DEBUGOUT("Begin LibErase Test Battery(addrMode:%d)\r\n", useAddrMode);

        populateDeviceData(pSpifi, &devData, TEST_BUFFSIZE);

        /* Generate some data */
        test_suiteFillBuffer(test_suiteGetBuffer(TEST_TX_BUFFER_ID), 0x5a5a5a5a, TEST_BUFFSIZE);

        /* Make sure we are not in mem mode and turn off QUAD for now */
        spifiDevSetOpts(pSpifi, SPIFI_OPT_USE_QUAD, false);
        spifiDevSetMemMode(pSpifi, false);

        if (spifiDevGetMemoryMode(pSpifi)) {
                test_suiteError("Inconsistent MEMMODE", SPIFI_ERR_GEN);
        }

        /* UnLock device */
        errCode = spifiDevUnlockDevice(pSpifi);
        if (errCode != SPIFI_ERR_NONE) {
                test_suiteError("LibEraseBattery (unlockDevice)", errCode);
        }

        /* Test bulk erase with verify */
        bulkEraseDevice(&devData, useAddrMode, true);

        for (testBlock = 0; testBlock < devData.numBlocks; testBlock += (devData.numBlocks - 1)) {

                /* Write block x and read it back verifying that basic write / read works. */
                DEBUGOUT("Write-Verify Block %d... ", testBlock);
                test_suiteWriteBlock(pSpifi, testBlock, TEST_TX_BUFFER_ID, devData.pageSize);
                test_suiteVerifyPattern(pSpifi, spifiGetAddrFromBlock(pSpifi, testBlock), devData.blockSize, 0x5a);
                DEBUGOUT("Passed\r\n");

                /* now do some basic range checking... */
                rangeCheckEraseDevice(&devData, useAddrMode);
                test_suiteVerifyPattern(pSpifi, spifiGetAddrFromBlock(pSpifi, testBlock), devData.blockSize, 0x5a);
                DEBUGOUT("Passed\r\n");

                /* Verify that request to erase x blocks does nothing */
                DEBUGOUT("Checking erase block %d len=0 does nothing... ", testBlock);
                test_suiteEraseBlocks(pSpifi, testBlock, 0);
                test_suiteVerifyPattern(pSpifi, spifiGetAddrFromBlock(pSpifi, testBlock), devData.blockSize, 0x5a);
                DEBUGOUT("Passed\r\n");

                /* Erase block x and verify entire device is still erased */
                DEBUGOUT("Erasing block %d... ", testBlock);
                if (useAddrMode) {
                        uint32_t beginAddress = spifiGetAddrFromBlock(pSpifi, testBlock);
                        test_suiteEraseByAddr(pSpifi, beginAddress, ((beginAddress + devData.blockSize) - 1));
                }
                else {
                        test_suiteEraseBlocks(pSpifi, testBlock, 1);
                }
                test_suiteVerifyBlockErased(pSpifi, testBlock, true);
        }

        /* Now write block 1,2 and 3 and verify. */
        for (idx = 1; idx < 4; ++idx) {
                test_suiteWriteBlock(pSpifi, idx, TEST_TX_BUFFER_ID, devData.pageSize);
        }

        /* Now verify they were written correctly, */
        DEBUGOUT("Writing blocks 1, 2 and 3... ");
        for (idx = 1; idx < 4; ++idx) {
                test_suiteVerifyPattern(pSpifi, spifiGetAddrFromBlock(pSpifi, idx), devData.blockSize, 0x5a);
        }
        DEBUGOUT("Passed.\r\n");

        /* now erase block 2 and verify */
        DEBUGOUT("Erasing block 2 ... ");
        if (useAddrMode) {
                test_suiteEraseByAddr(pSpifi, spifiGetAddrFromBlock(pSpifi, 2), (spifiGetAddrFromBlock(pSpifi, 3) - 1));
        }
        else {
                test_suiteEraseBlocks(pSpifi, 2, 1);
        }
        test_suiteVerifyBlockErased(pSpifi, 2, false);

        /* now verify block 1 and 3 are unchanged */
        for (idx = 1; idx < 4; idx += 2) {
                test_suiteVerifyPattern(pSpifi, spifiGetAddrFromBlock(pSpifi, idx), devData.blockSize, 0x5a);
        }
        DEBUGOUT("Passed.\r\n");

        /* now erase blocks 1 and 3 and verify whole device is erased */
        DEBUGOUT("Erasing blocks 1 and 3... ");
        for (idx = 1; idx < 4; idx += 2) {
                if (useAddrMode) {
                        test_suiteEraseByAddr(pSpifi,
                                                                  spifiGetAddrFromBlock(pSpifi, idx), (spifiGetAddrFromBlock(pSpifi, idx + 1) - 1));
                }
                else {
                        test_suiteEraseBlocks(pSpifi, idx, 1);
                }
        }
        test_suiteVerifyDeviceErased(pSpifi, true);

        /* if device support sub-block erase test it.
           Current lib does not support using address mode for sub-blocks */
        if ((devData.caps & SPIFI_CAP_SUBBLKERASE) && (devData.numSubBlocks > 2)) {
        
                uint32_t subBlock = devData.numSubBlocks / 2; /* set the test sub-block somewhere in the middle */
        
        DEBUGOUT("Verifing programmed block... ");
        /* Write the fill pattern to the sub-blocks before and after test sub-block */
        test_suiteWriteSubBlock(pSpifi, (subBlock - 1), TEST_TX_BUFFER_ID, (devData.subBlockSize * 3));

                /* Read the sub blocks and verify they contain pattern */
                for (idx = 0; idx < 3; ++idx) {
                        errCode = spifiRead(pSpifi,
                                                                spifiGetAddrFromSubBlock(pSpifi, ((subBlock + idx) - 1)),
                                                                test_suiteGetBuffer(TEST_RX_BUFFER_ID),
                                                                TEST_BUFFSIZE);
                        if (errCode != SPIFI_ERR_NONE) {
                                test_suiteError("Step 1 Read", errCode);
                        }
                        test_suiteVerifyBufferFilledWithValue(TEST_RX_BUFFER_ID, TEST_BUFFSIZE, 0x5a);
                }
                DEBUGOUT("Passed.\r\n");

                /* next erase and verify the erase was successful */
                DEBUGOUT("Erasing subblock %d... ", (subBlock));
                test_suiteEraseSubBlocks(pSpifi, (subBlock), 1);
                test_suiteVerifySubBlockErased(pSpifi, (subBlock), 0);
                DEBUGOUT("Passed.\r\n");

                /* Read the adjacent sub blocks and verify they still have data */
                DEBUGOUT("Verifying adjacent sub-blocks... ");
                for (idx = 0; idx < 3; idx += 2) {
                        errCode = spifiRead(pSpifi,
                                                                spifiGetAddrFromSubBlock(pSpifi, ((subBlock + idx) - 1)),
                                                                test_suiteGetBuffer(TEST_RX_BUFFER_ID),
                                                                TEST_BUFFSIZE);
                        if (errCode != SPIFI_ERR_NONE) {
                                test_suiteError("spifiRead", errCode);
                        }
                        test_suiteVerifyBufferFilledWithValue(TEST_RX_BUFFER_ID, TEST_BUFFSIZE, 0x5a);
                }
                DEBUGOUT("Passed.\r\n");
        }

        DEBUGOUT("FIXME: Test lock/unlock\r\n");
        DEBUGOUT("LibErase Test Battery(addrMode:%d) Complete!\r\n\r\n", useAddrMode);
}

void test_suiteDataBattery(SPIFI_HANDLE_T *pSpifi, uint8_t enableQuadRead, uint8_t enableQuadWrite)
{
        DEVICE_TEST_DATA_T devData;
        SPIFI_ERR_T errCode;

        DEBUGOUT("Begin Data Test Battery(qRead=%d qWrite=%d)\r\n", enableQuadRead, enableQuadWrite);
        /* Make sure we are not in mem mode */
        spifiDevSetMemMode(pSpifi, false);

        /* populate the test data structure */
        populateDeviceData(pSpifi, &devData, TEST_BUFFSIZE);

        /* UnLock device */
        errCode = spifiDevUnlockDevice(pSpifi);
        if (errCode != SPIFI_ERR_NONE) {
                test_suiteError("LibDataBattery (unlockDevice)", errCode);
        }

        /* Start by erasing entire device using block mode */
        spifiDevSetOpts(pSpifi, SPIFI_OPT_USE_QUAD, enableQuadRead);
        bulkEraseDevice(&devData, false, true);

        DEBUGOUT("Filling device with address pattern... ");
        spifiDevSetOpts(pSpifi, SPIFI_OPT_USE_QUAD, enableQuadWrite);
        test_suiteFillDeviceWithAddressPattern(pSpifi, true);

        DEBUGOUT("Verify via:devRead... ");
        spifiDevSetOpts(pSpifi, SPIFI_OPT_USE_QUAD, enableQuadRead);
        test_suiteValidateDeviceWithAddressPattern(pSpifi, true);

        DEBUGOUT("Verify via:addr mode... ");

        spifiDevSetMemMode(pSpifi, true);
        test_suiteValidateDeviceWithAddressPattern(pSpifi, true);

        DEBUGOUT("Data Test Battery(qRead=%d qWrite=%d) Complete! \r\n\r\n", enableQuadRead, enableQuadWrite);
}

void test_suiteMemModeTestBattery(SPIFI_HANDLE_T *pSpifi, uint32_t baseAddr, uint8_t enableQuadRead, uint8_t enableQuadWrite)
{
        DEBUGOUT("Begin MemMode Test Battery(qRead=%d qWrite=%d)\r\n", enableQuadRead, enableQuadWrite);
        /* To enable QUAD mode with memoryMode, it must be enabled and
           memMode must be re-enabled */
        spifiDevSetMemMode(pSpifi, true);
        spifiDevSetOpts(pSpifi, SPIFI_OPT_USE_QUAD, enableQuadRead);

        DEBUGOUT("Timed read:%d mSeconds\r\n", test_suiteTimedReadBlock(baseAddr));
        DEBUGOUT("MemMode Test Battery(qRead=%d qWrite=%d) Complete! \r\n\r\n", enableQuadRead, enableQuadWrite);
}

void test_suiteSetErrorBlinkRate(uint32_t newValue)
{
        blinkToggleRate = newValue;
}

uint32_t test_suiteGetErrorBlinkRate(void)
{
        return blinkToggleRate;
}

/* Displays error message and dead loops */
void test_suiteError(char *str, SPIFI_ERR_T errNum)
{
        DEBUGOUT("\r\n%s() Error:%d %s\r\n", str, errNum, spifiReturnErrString(errNum));

        /* change the blink rate to fast to indicate error */
        test_suiteSetErrorBlinkRate(100);

        /* Loop forever */
        while (1) {
                __WFI();
        }
}

uint32_t *test_suiteGetBuffer(TEST_BUFFERID_T bufId)
{
        return testBuff[bufId].buffer;
}

/* Fill buffer with some data */
void test_suiteFillBuffer(uint32_t *pBuffer, uint32_t seed, uint32_t bytes)
{
        uint32_t idx = 0;
        while (bytes > 0) {
                pBuffer[idx] = seed;
                ++idx;
                bytes -= sizeof(uint32_t);
        }
}

/* Compares 2 memory regions for differences */
void test_suiteCompareTestBuffers(TEST_BUFFERID_T buf1Id, TEST_BUFFERID_T buf2Id, uint32_t bytes)
{
        uint8_t *b1 = (uint8_t *) test_suiteGetBuffer(buf1Id);
        uint8_t *b2 = (uint8_t *) test_suiteGetBuffer(buf2Id);
        uint32_t index;

        for (index = 0; index < bytes; ++index) {
                if (b1[index] != b2[index]) {
                        DEBUGOUT("Compare Failure index:%d\r\n", index);
                        test_suiteError("Compare failure 1", SPIFI_ERR_GEN);
                }
        }
}

/* Fill the device with address pattern (i.e mem address) */
void test_suiteFillDeviceWithAddressPattern(SPIFI_HANDLE_T *pSpifi, uint8_t verbose)
{
        uint32_t idx;
        uint32_t maxWrite;
        uint32_t pageAddress;
        uint32_t bytesToWrite;
        uint32_t startTime = 0;
        uint32_t *buffer;
        SPIFI_ERR_T errCode;

        maxWrite = spifiDevGetInfo(pSpifi, SPIFI_INFO_PAGESIZE);
        pageAddress = spifiGetAddrFromBlock(pSpifi, 0);
        bytesToWrite = (spifiDevGetInfo(pSpifi, SPIFI_INFO_ERASE_BLOCKSIZE) *
                                        spifiDevGetInfo(pSpifi, SPIFI_INFO_ERASE_BLOCKS));

        if (verbose) {
                StopWatch_Init();
                startTime = StopWatch_Start();
        }

        buffer = test_suiteGetBuffer(TEST_TEMP_BUFFER_ID);
        if (maxWrite > TEST_BUFFSIZE) {
                maxWrite = TEST_BUFFSIZE;
        }
        while (bytesToWrite) {
                if (maxWrite > bytesToWrite) {
                        maxWrite = bytesToWrite;
                }
                /* Fill the buffer with data i.e addresses */
                for (idx = 0; idx < maxWrite; idx += sizeof(uint32_t)) {
                        buffer[idx / sizeof(uint32_t)] = (pageAddress + idx);
                }
                errCode = spifiDevPageProgram(pSpifi, pageAddress, buffer, maxWrite);
                if (errCode != SPIFI_ERR_NONE) {
                        test_suiteError("test_suiteWriteBlock", errCode);
                }
                pageAddress += maxWrite;
                bytesToWrite -= maxWrite;
        }
        if (verbose) {
                DEBUGOUT("%d mSec\r\n", StopWatch_TicksToMs(StopWatch_Elapsed(startTime)));
        }
}

/* Verify that the address pattern is written into memory */
void test_suiteValidateDeviceWithAddressPattern(SPIFI_HANDLE_T *pSpifi, uint8_t verbose)
{
        uint32_t idx;
        uint32_t maxRead;
        uint32_t address;
        uint32_t bytesToRead;
        uint32_t startTime = 0;
        uint32_t *buffer;
        SPIFI_ERR_T errCode;

        address = spifiGetAddrFromBlock(pSpifi, 0);
        bytesToRead = (spifiDevGetInfo(pSpifi, SPIFI_INFO_ERASE_BLOCKSIZE) *
                                   spifiDevGetInfo(pSpifi, SPIFI_INFO_ERASE_BLOCKS));

        /* Initialize the stopwatch */
        if (verbose) {
                StopWatch_Init();
                startTime = StopWatch_Start();
        }

        /* determine how to test */
        if (spifiDevGetMemoryMode(pSpifi)) {
                uint32_t readValue;

                for (idx = 0; idx < bytesToRead; idx += sizeof(uint32_t)) {
                        readValue = *((uint32_t *) (address + idx));
                        if (readValue != (address + idx)) {
                                DEBUGOUT("\r\nError Addr:0x%08x Val:0x%08x", (address + idx), readValue);
                                test_suiteError("test_suiteValidateDeviceWithAddressPattern", SPIFI_ERR_GEN);
                        }
                }
        }
        else {
                /* if NOT in memMode read device in using lib routines */
                maxRead = spifiDevGetInfo(pSpifi, SPIFI_INFO_MAXREADSIZE);

                buffer = test_suiteGetBuffer(TEST_TEMP_BUFFER_ID);
                if (maxRead > TEST_BUFFSIZE) {
                        maxRead = TEST_BUFFSIZE;
                }
                while (bytesToRead) {
                        if (maxRead > bytesToRead) {
                                maxRead = bytesToRead;
                        }
                        errCode = spifiDevRead(pSpifi, address, buffer, maxRead);
                        if (errCode != SPIFI_ERR_NONE) {
                                test_suiteError("test_suiteWriteBlock", errCode);
                        }
                        /* Check the buffer with data i.e addresses */
                        for (idx = 0; idx < maxRead; idx += sizeof(uint32_t)) {
                                uint32_t wordIndex = idx / sizeof(uint32_t);
                                if (buffer[wordIndex] != (address + idx)) {
                                        DEBUGOUT("\r\nError idx:%d Addr:0x%08x Val:0x%08x", wordIndex, (address + idx), buffer[wordIndex]);
                                        test_suiteError("test_suiteValidateDeviceWithAddressPattern", SPIFI_ERR_GEN);
                                }
                        }
                        address += maxRead;
                        bytesToRead -= maxRead;
                }
        }
        if (verbose) {
                DEBUGOUT("%d mSec Passed\r\n", StopWatch_TicksToMs(StopWatch_Elapsed(startTime)));
        }
}

/* Check supplied buffer against test value */
void test_suiteVerifyBufferFilledWithValue(TEST_BUFFERID_T bufId, uint32_t bytes, uint8_t testValue)
{
        uint32_t idx;
        uint8_t *buffer;

        buffer = (uint8_t *) test_suiteGetBuffer(bufId);

        for (idx = 0; idx < bytes; ++idx) {
                if (buffer[idx] != testValue) {
                        DEBUGOUT("Verify failure index:%d Byte:%02x Test:%02x\r\n", idx, buffer[idx], testValue);
                        test_suiteError("Verify failure (verifyFillBuffer)", SPIFI_ERR_GEN);
                }
        }
}

/* Write a block of data with the specified test buffer. */
void test_suiteWriteBlock(SPIFI_HANDLE_T *pSpifi, uint32_t blockNum, TEST_BUFFERID_T bufId, uint32_t bytes)
{
        uint32_t pageSize;
        uint32_t pageAddress;
        uint32_t bytesToWrite;
        SPIFI_ERR_T errCode;

        pageSize = spifiDevGetInfo(pSpifi, SPIFI_INFO_PAGESIZE);
        pageAddress = spifiGetAddrFromBlock(pSpifi, blockNum);
        bytesToWrite = spifiDevGetInfo(pSpifi, SPIFI_INFO_ERASE_BLOCKSIZE);

        /* Write the entire block using the supplied buffer */
        while (bytesToWrite) {
                if (pageSize > bytesToWrite) {
                        pageSize = bytesToWrite;
                }
                errCode = spifiDevPageProgram(pSpifi, pageAddress, test_suiteGetBuffer(bufId), pageSize);
                if (errCode != SPIFI_ERR_NONE) {
                        test_suiteError("test_suiteWriteBlock", errCode);
                }
                bytesToWrite -= pageSize;
                pageAddress += pageSize;
        }
}

void test_suiteWriteSubBlock(SPIFI_HANDLE_T *pSpifi, uint32_t subBlockNum, TEST_BUFFERID_T bufId, uint32_t bytes)
{
    uint32_t pageSize;
        uint32_t pageAddress;
        uint32_t bytesToWrite;
        SPIFI_ERR_T errCode;

        pageSize = spifiDevGetInfo(pSpifi, SPIFI_INFO_PAGESIZE);
        pageAddress = spifiGetAddrFromSubBlock(pSpifi, subBlockNum);
        bytesToWrite = spifiDevGetInfo(pSpifi, SPIFI_INFO_ERASE_BLOCKSIZE);

        /* Write the entire block using the supplied buffer */
        while (bytesToWrite) {
                if (pageSize > bytesToWrite) {
                        pageSize = bytesToWrite;
                }
                errCode = spifiDevPageProgram(pSpifi, pageAddress, test_suiteGetBuffer(bufId), pageSize);
                if (errCode != SPIFI_ERR_NONE) {
                        test_suiteError("test_suiteWriteSubBlock", errCode);
                }
                bytesToWrite -= pageSize;
                pageAddress += pageSize;
        }
}

/* verify device contains value at specified address range */
void test_suiteVerifyPattern(SPIFI_HANDLE_T *pSpifi, uint32_t beginAddr, uint32_t bytes, uint8_t verifyValue)
{
        uint32_t idx;
        uint32_t maxSingleRead;
        uint32_t readSize;
        uint32_t *buffer;
        SPIFI_ERR_T errCode;

        buffer = test_suiteGetBuffer(TEST_TEMP_BUFFER_ID);
        maxSingleRead = spifiDevGetInfo(pSpifi, SPIFI_INFO_MAXREADSIZE);
        readSize = maxSingleRead;

        /* limit to buffer size if max single read is larger */
        if (readSize > TEST_BUFFSIZE) {
                readSize = TEST_BUFFSIZE;
        }

        while (bytes) {
                if (readSize > bytes) {
                        readSize = bytes;
                }

                errCode = spifiDevRead(pSpifi, beginAddr, buffer, readSize);
                if (errCode != SPIFI_ERR_NONE) {
                        test_suiteError("test_suiteReadBlock", errCode);
                }

                for (idx = 0; idx < readSize; ++idx) {
                        if (((uint8_t *) buffer)[idx] != verifyValue) {
                                DEBUGOUT("\r\nVerify Fail Add:0x%08x Data:0x%02x\r\n",
                                                 beginAddr + idx, ((uint8_t *) buffer)[idx]);
                                test_suiteError("test_suiteVerifyBlockErased", SPIFI_ERR_GEN);
                        }
                }
                /* setup for next chunk */
                bytes -= readSize;
                beginAddr += readSize;
        }
}

/* Perform a timed read in mem mode */
uint32_t test_suiteTimedReadBlock(uint32_t baseAddr)
{
        uint32_t startTime;
        uint32_t index;
        uint32_t readAddress;
        const int loopCount = 500;
        uint8_t *readBuffer;

        readBuffer = (uint8_t *) test_suiteGetBuffer(TEST_TEMP_BUFFER_ID);
        /* Initialize the stopwatch */
        StopWatch_Init();
        startTime = StopWatch_Start();

        readAddress = baseAddr; /* begining of SPIFI address space */
        for (index = 0; index < loopCount; ++index) {
                memcpy(readBuffer, (void *) (readAddress + (index * TEST_BUFFSIZE)), TEST_BUFFSIZE);
        }

        /* Report the elapsed time. */
        return StopWatch_TicksToMs(StopWatch_Elapsed(startTime));
}

/* Wrapper around spifiEraseByAddr() errors are fatal */
void test_suiteEraseByAddr(SPIFI_HANDLE_T *pSpifi, uint32_t firstAddr, uint32_t lastAddr)
{
        SPIFI_ERR_T errCode;

        errCode = spifiEraseByAddr(pSpifi, firstAddr, lastAddr);
        if (errCode != SPIFI_ERR_NONE) {
                test_suiteError("test_suiteEraseByAddr", errCode);
        }
}

/* Wrapper around spifiErase() errors are fatal */
void test_suiteEraseBlocks(SPIFI_HANDLE_T *pSpifi, uint32_t beginBlock, uint32_t numBlocks)
{
        SPIFI_ERR_T errCode;

        errCode = spifiErase(pSpifi, beginBlock, numBlocks);
        if (errCode != SPIFI_ERR_NONE) {
                test_suiteError("test_suiteEraseBlocks", errCode);
        }
}

/* Function to erase a given number of sub-blocks */
void test_suiteEraseSubBlocks(SPIFI_HANDLE_T *pSpifi, uint32_t beginSubBlock, uint32_t numSubBlocks)
{
        uint32_t idx;
        SPIFI_ERR_T errCode;

        /* Erase using sub-blocks */
        for (idx = 0; idx < numSubBlocks; ++idx) {
                errCode = spifiDevEraseSubBlock(pSpifi, (idx + beginSubBlock));
                if (errCode != SPIFI_ERR_NONE) {
                        test_suiteError("test_suiteEraseSubBlocks", errCode);
                }
        }
}

/* Function to verify specified sub-block is erased */
void test_suiteVerifySubBlockErased(SPIFI_HANDLE_T *pSpifi, uint32_t subBlockNumber, uint8_t verbose)
{
        uint32_t idx;
        uint32_t *tempBuffer;
        uint32_t subBlockSize;
        uint32_t remainingBytes;
        uint32_t maxSingleRead;
        uint32_t readBytes;
        uint32_t address;
        SPIFI_ERR_T errCode;

        tempBuffer = test_suiteGetBuffer(TEST_TEMP_BUFFER_ID);
        subBlockSize = spifiDevGetInfo(pSpifi, SPIFI_INFO_ERASE_SUBBLOCKSIZE);
        remainingBytes = subBlockSize;
        maxSingleRead = spifiDevGetInfo(pSpifi, SPIFI_INFO_MAXREADSIZE);
        address = spifiGetAddrFromSubBlock(pSpifi, subBlockNumber);

        for (; remainingBytes != 0; remainingBytes -= readBytes) {
                readBytes = remainingBytes;
                if (readBytes > maxSingleRead) {
                        readBytes = maxSingleRead;
                }
                /* Read a chunk of data */
                errCode = spifiDevRead(pSpifi, address, tempBuffer, readBytes);
                if (errCode != SPIFI_ERR_NONE) {
                        test_suiteError("spifiDevRead", errCode);
                }
                /* Now test the bytes to see if they are erased. */
                for (idx = 0; idx < readBytes; ++idx) {
                        uint8_t data = ((uint8_t *) tempBuffer)[idx];
                        if (data != 0xff) {
                                uint32_t idy;
                if (verbose) {
                    for (idy = 0; idy  < readBytes; ++idy) {
                        switch ((idy & 0xf)) {
                        case 0:
                            DEBUGOUT("\r\n0x%08x %02x", address + idy, ((uint8_t *) tempBuffer)[idy]);
                            break;

                        case 8:
                            DEBUGOUT(" - %02x", ((uint8_t *) tempBuffer)[idy]);
                            break;

                        default:
                            DEBUGOUT(" %02x",  ((uint8_t *) tempBuffer)[idy]);
                            break;
                        }
                    }
                }

                                DEBUGOUT("\r\nVerify Fail SubBlk:%d Add:0x%08x Data:0x%08x\r\n",
                                                 subBlockNumber, address + idx, data);
                                test_suiteError("test_suiteVerifyBlockErased", SPIFI_ERR_GEN);
                        }
                }
                address += readBytes;
        }
}

/* Verify specified block is erased */
void test_suiteVerifyBlockErased(SPIFI_HANDLE_T *pSpifi, uint32_t blockNumber, uint8_t verbose)
{
        uint32_t idx;
        uint32_t eraseBlockSize;
        uint32_t maxSingleRead;
        uint32_t address;
        uint32_t readBytes;
        uint32_t remainingBytes;
        uint32_t startTime = 0;
        uint8_t *compareBuffer;
        SPIFI_ERR_T errCode;

        compareBuffer = (uint8_t *) test_suiteGetBuffer(TEST_TEMP_BUFFER_ID);

        eraseBlockSize =  spifiDevGetInfo(pSpifi, SPIFI_INFO_ERASE_BLOCKSIZE);
        remainingBytes = eraseBlockSize;
        maxSingleRead = spifiDevGetInfo(pSpifi, SPIFI_INFO_MAXREADSIZE);

        if (verbose) {
                DEBUGOUT("Verify... ");

                /* Initialize the stopwatch */
                StopWatch_Init();
                startTime = StopWatch_Start();
        }

        /* Only have a small buffer so force to buffer size */
        if (maxSingleRead > TEST_BUFFSIZE) {
                maxSingleRead = TEST_BUFFSIZE;
        }

        address = spifiGetAddrFromBlock(pSpifi, blockNumber);

        for (; remainingBytes != 0; remainingBytes -= readBytes) {
                readBytes = remainingBytes;
                if (readBytes > maxSingleRead) {
                        readBytes = maxSingleRead;
                }
                /* Read a chunk of data */
                errCode = spifiDevRead(pSpifi, address, (uint32_t *) compareBuffer, readBytes);
                if (errCode != SPIFI_ERR_NONE) {
                        test_suiteError("spifiDevRead", errCode);
                }
                /* Now test the bytes to see if they are erased. */
                for (idx = 0; idx < readBytes; ++idx) {
                        if (compareBuffer[idx] != 0xff) {
                                DEBUGOUT("\r\nVerify Fail Blk:%d Add:%0x Data:%d\r\n", blockNumber, address + idx, compareBuffer[idx]);
                                test_suiteError("test_suiteVerifyBlockErased", SPIFI_ERR_GEN);
                        }
                }
                address += readBytes;
        }

        if (verbose) {
                DEBUGOUT("%d mSec Passed\r\n", StopWatch_TicksToMs(StopWatch_Elapsed(startTime)));
        }
}

/* Verify entire device is erased */
void test_suiteVerifyDeviceErased(SPIFI_HANDLE_T *pSpifi, uint8_t verbose)
{
        uint32_t idx;
        uint32_t numBlocks;
        uint32_t startTime = 0;

        numBlocks = spifiDevGetInfo(pSpifi, SPIFI_INFO_ERASE_BLOCKS);
        if (verbose) {
                DEBUGOUT("Verify... ");

                /* Initialize the stopwatch */
                StopWatch_Init();
                startTime = StopWatch_Start();
        }

        for (idx = 0; idx < numBlocks; ++idx) {
                /* verify block  */
                test_suiteVerifyBlockErased(pSpifi, idx, false);
        }

        if (verbose) {
                DEBUGOUT("%d mSec Passed\r\n", StopWatch_TicksToMs(StopWatch_Elapsed(startTime)));
        }
}
