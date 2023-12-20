/*
 * @brief LPCSPIFILIB FLASH library test suite functions
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

#ifndef __SPIFILIB_TEST_SUITE_H_
#define __SPIFILIB_TEST_SUITE_H_

#include "spifilib_dev.h"

#ifdef __cplusplus
extern "C" {
#endif
/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/
#define TEST_BUFFSIZE (4096)

typedef enum {
        TEST_RX_BUFFER_ID = 0,
        TEST_TX_BUFFER_ID,
        TEST_TEMP_BUFFER_ID,
        TEST_BUFFER_COUNT
} TEST_BUFFERID_T;

/*****************************************************************************
 * Public functions
 ****************************************************************************/
void test_suiteLibHelperBattery(SPIFI_HANDLE_T *pSpifi);

void test_suiteLibEraseBattery(SPIFI_HANDLE_T *sSpifi, uint8_t useAddrMode);

void test_suiteDataBattery(SPIFI_HANDLE_T *pSpifi, uint8_t enableQuadRead, uint8_t enableQuadWrite);

void test_suiteMemModeTestBattery(SPIFI_HANDLE_T *pSpifi, uint32_t baseAddr, uint8_t enableQuadRead, uint8_t enableQuadWrite);

uint32_t *test_suiteGetBuffer(TEST_BUFFERID_T bufId);

void test_suiteFillBuffer(uint32_t *pBuffer, uint32_t value, uint32_t bytes);

void test_suiteError(char *str, SPIFI_ERR_T errNum);

void test_suiteSetErrorBlinkRate(uint32_t newValue);

uint32_t test_suiteGetErrorBlinkRate(void);

void test_suiteWriteBlock(SPIFI_HANDLE_T *pSpifi, uint32_t blockNum, TEST_BUFFERID_T bufId, uint32_t bytes);

void test_suiteWriteSubBlock(SPIFI_HANDLE_T *pSpifi, uint32_t subBlockNum, TEST_BUFFERID_T bufId, uint32_t bytes);

void test_suiteVerifyPattern(SPIFI_HANDLE_T *pSpifi, uint32_t beginAddr, uint32_t bytes, uint8_t verifyValue);

void test_suiteFillDeviceWithAddressPattern(SPIFI_HANDLE_T *pSpifi, uint8_t verbose);

void test_suiteValidateDeviceWithAddressPattern(SPIFI_HANDLE_T *pSpifi, uint8_t verbose);

void test_suiteCompareTestBuffers(TEST_BUFFERID_T buf1Id, TEST_BUFFERID_T buf2Id, uint32_t bytes);

void test_suiteVerifyBufferFilledWithValue(TEST_BUFFERID_T bufId, uint32_t bytes, uint8_t testValue);

uint32_t test_suiteTimedReadBlock(uint32_t baseAddr);

void test_suiteEraseByAddr(SPIFI_HANDLE_T *pSpifi, uint32_t firstAddr, uint32_t lastAddr);

void test_suiteEraseBlocks(SPIFI_HANDLE_T *pSpifi, uint32_t beginBlock, uint32_t numBlocks);

void test_suiteEraseSubBlocks(SPIFI_HANDLE_T *pSpifi, uint32_t beginSubBlock, uint32_t numSubBlocks);

void test_suiteVerifySubBlockErased(SPIFI_HANDLE_T *pSpifi, uint32_t subBlockNumber, uint8_t verbose);

void test_suiteVerifyBlockErased(SPIFI_HANDLE_T *pSpifi, uint32_t blockNumber, uint8_t verbose);

void test_suiteVerifyDeviceErased(SPIFI_HANDLE_T *pSpifi, uint8_t verbose);

#ifdef __cplusplus
}
#endif

#endif /* __SPIFILIB_TEST_SUITE_H_ */
