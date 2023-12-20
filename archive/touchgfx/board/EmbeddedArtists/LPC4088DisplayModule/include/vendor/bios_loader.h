/*
 *  Copyright 2014 Embedded Artists AB
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#ifndef BIOSLOADER_H
#define BIOSLOADER_H

#include "bios.h"

#define SDRAM_START   0xA0000000
#define SDRAM_SIZE    0x10000000
#define SDRAM_END    (SDRAM_START+SDRAM_SIZE)

// ------------------ MEMORY RESERVE FOR EXTERNAL SDRAM -------------------
#define SDRAM_SZ_BIOS1  (0x100000)
#define SDRAM_SZ_BIOS2  (  0x1000)
#define SDRAM_SZ_FRAMEB (0x100000)  /* enough for 800x480 at 16bpp */

#define SDRAM_ADDR_BIOS1  (SDRAM_START                      )
#define SDRAM_ADDR_BIOS2  (SDRAM_ADDR_BIOS1 + SDRAM_SZ_BIOS1)
#define SDRAM_ADDR_FB1    (SDRAM_ADDR_BIOS2 + SDRAM_SZ_BIOS2)
#define SDRAM_ADDR_FB2    (SDRAM_ADDR_FB1 + SDRAM_SZ_FRAMEB )
    
// The SDRAM_ADDR_FREE is the first free to use area in
// the external sdram
#define SDRAM_ADDR_FREE   (SDRAM_ADDR_FB1 + SDRAM_SZ_FB1)
  
// ------------------ MEMORY RESERVE FOR EXTERNAL SDRAM -------------------

typedef enum {
    BiosLoaderError_Ok            = BiosError_Ok,
    BiosLoaderError_ConfigError   = BiosError_ConfigError,
    BiosLoaderError_WrongBPP      = BiosError_WrongBPP,
    BiosLoaderError_InvalidParam  = BiosError_InvalidParam,
    BiosLoaderError_NoInit        = BiosError_NoInit,
    BiosLoaderError_Calibration   = BiosError_Calibration,
    BiosLoaderError_Timeout       = BiosError_Timeout,
    BiosLoaderError_CommError     = BiosError_CommError,
    BiosLoaderError_NotSupported  = BiosError_NotSupported,

    BiosLoaderError_StorageError,
    BiosLoaderError_BiosInvalid,
    BiosLoaderError_BiosVersion,
    BiosLoaderError_Memory,
    BiosLoaderError_RTOS,
} BiosLoaderError_t;

BiosLoaderError_t bios_init(void);
void bios_getMacAddress(char mac[6]);
bool bios_isKnownSPIFIMemory(uint8_t mfgr, uint8_t devType, uint8_t devID, uint32_t memSize, uint32_t* eraseBlockSize);
void bios_getStats(uint8_t* type, uint8_t* major, uint8_t* minor, uint8_t* rev);

#endif /* BIOSLOADER_H */
