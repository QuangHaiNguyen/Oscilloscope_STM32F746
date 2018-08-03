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

#ifndef SPIFI_H
#define SPIFI_H

typedef enum {
    SpifiError_Ok            =       0,
    SpifiError_Uninitialized =       1,
    SpifiError_Verification  =       2,
    SpifiError_SameAddress   =       3,
    SpifiError_UnknownError  =       4,
    SpifiError_InternalError = 0x20002,
    SpifiError_Timeout       = 0x20003,
    SpifiError_OperandError  = 0x20004,
    SpifiError_Status        = 0x20005,
    SpifiError_ExtDeviceId   = 0x20006,
    SpifiError_DeviceId      = 0x20007,
    SpifiError_DeviceType    = 0x20008,
    SpifiError_Manufacturer  = 0x20009,
    SpifiError_InvalidJDECId = 0x2000A,
    SpifiError_EraseConflict = 0x2000B,
} SpifiError_t;

/** Initializes the SPIFI ROM driver making the content of the external serial flash available
 *
 *  @returns
 *       Ok on success
 *       An error code on failure
 */
SpifiError_t spifi_init(void);

#endif
