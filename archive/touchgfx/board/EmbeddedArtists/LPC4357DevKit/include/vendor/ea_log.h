/*****************************************************************************
 *
 *   Copyright(C) 2011, Embedded Artists AB
 *   All rights reserved.
 *
 ******************************************************************************
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * products. This software is supplied "AS IS" without any warranties.
 * Embedded Artists AB assumes no responsibility or liability for the
 * use of the software, conveys no license or title under any patent,
 * copyright, or mask work right to the product. Embedded Artists AB
 * reserves the right to make changes in the software without
 * notification. Embedded Artists AB also make no representation or
 * warranty that such application will be suitable for the specified
 * use without further testing or modification.
 *****************************************************************************/
#ifndef __EA_LOG_H
#define __EA_LOG_H

#include <stdio.h>
#include "lpc_types.h"
#include "board.h"

#ifdef __cplusplus
extern "C" {
#endif

//#define USE_EA_LOG

  #define force_log_s(__str)  console_sendString((uint8_t*)(__str))
  //#define force_log_s(__str)  Board_UARTPutSTR((char*)(__str))

  void ea_log_uint32(uint32_t val);
  void ea_log_hex(uint32_t val, int bytes);
  void ea_log_reg(uint32_t regStart, uint32_t regEnd);

  #define force_log_uint32(__num) ea_log_uint32(__num)
  #define force_log_hex32(__num)  ea_log_hex(__num, 4)
  #define force_log_hex16(__num)  ea_log_hex(__num, 2)
  #define force_log_hex8(__num)   ea_log_hex(__num, 1)

  #define force_log_array(__arr, __len) do {\
      force_log_s("{ "); \
      int i; \
      for (i = 0; i < (__len); i++) { \
        force_log_hex8((__arr)[i]); \
        force_log_s(", "); \
      } \
      force_log_s("}"); \
    } while(0)

  #define force_log_dump_regs() do { \
      force_log_s("\r\n---\r\nDumping Registers:\r\n"); \
      force_log_s("CGU:\r\n"); \
      ea_log_reg(0x40050014, 0x400500CC); \
      force_log_s("CCU1:\r\n"); \
      ea_log_reg(0x40051000, 0x40051008); \
      ea_log_reg(0x40051100, 0x40051130); \
      ea_log_reg(0x40051200, 0x40051220); \
      ea_log_reg(0x40051300, 0x40051308); \
      ea_log_reg(0x40051400, 0x40051450); \
      ea_log_reg(0x40051468, 0x400514A8); \
      ea_log_reg(0x40051500, 0x40051540); \
      ea_log_reg(0x40051600, 0x40051638); \
      ea_log_reg(0x40051700, 0x40051708); \
      ea_log_reg(0x40051710, 0x40051720); \
      /*USB0: ea_log_reg(0x40051800, 0x40051808);*/ \
      /*USB1: ea_log_reg(0x40051900, 0x40051908);*/ \
      ea_log_reg(0x40051A00, 0x40051A08); \
      /*ADCHS: ea_log_reg(0x40051B00, 0x40051B08);*/ \
      force_log_s("CCU2:\r\n"); \
      ea_log_reg(0x40052000, 0x40052008); \
      ea_log_reg(0x40052100, 0x40052108); \
      ea_log_reg(0x40052200, 0x40052208); \
      ea_log_reg(0x40052300, 0x40052308); \
      ea_log_reg(0x40052400, 0x40052408); \
      force_log_s("EMC:\r\n"); \
      ea_log_reg(0x40005000, 0x4000500C); \
      ea_log_reg(0x40005020, 0x4000502C); \
      ea_log_reg(0x40005030, 0x4000505C); \
      ea_log_reg(0x40005080, 0x40005084); \
      ea_log_reg(0x40005100, 0x40005108); \
      ea_log_reg(0x40005120, 0x40005128); \
      ea_log_reg(0x40005140, 0x40005148); \
      ea_log_reg(0x40005160, 0x40005168); \
      ea_log_reg(0x40005200, 0x4000521C); \
      ea_log_reg(0x40005220, 0x4000523C); \
      ea_log_reg(0x40005240, 0x4000525C); \
      ea_log_reg(0x40005260, 0x4000527C); \
      force_log_s("\r\nDone\r\n---\r\n"); \
    } while(0)

#ifdef USE_EA_LOG

  #define log_s(__str)            force_log_s(__str)
  #define log_uint32(__num)       force_log_uint32(__num)
  #define log_hex32(__num)        force_log_hex32(__num)
  #define log_hex16(__num)        force_log_hex16(__num)
  #define log_hex8(__num)         force_log_hex8(__num)
  #define log_array(__arr, __len) force_log_array(__arr, __len)
  #define log_dump_regs()         force_log_dump_regs()
  #define log_hex(__num)          force_log_hex32(__num)
  #define log_init()              do {} while(0)

#else //ifdef USE_EA_LOG

  #define log_s(__str)            do {} while(0)
  #define log_uint32(__num)       do {} while(0)
  #define log_hex32(__num)        do {} while(0)
  #define log_hex16(__num)        do {} while(0)
  #define log_hex8(__num)         do {} while(0)
  #define log_array(__arr, __len) do {} while(0)
  #define log_dump_regs()         do {} while(0)
  #define log_hex(__num)          do {} while(0)
  #define log_init()              do {} while(0)

#endif

#ifdef __cplusplus
}
#endif

#ifdef EA_LOG_WANT_CODE
  //#ifdef USE_EA_LOG
    #define LOG_BUF_SIZE 512
    static const char HEXDIGITS[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
    static uint8_t logbuf[LOG_BUF_SIZE];

    void ea_log_uint32(uint32_t x) {
      logbuf[10] = '\0';
      int i;
      for (i = 9; i >= 0; i--) {
        logbuf[i] = '0' + (x % 10);
        x = (x/10);
        if (x == 0) {
          break;
        }
      }
      for (--i; i >= 0; i--) {
        logbuf[i] = ' ';
      }
      force_log_s(logbuf);
    }
    void ea_log_hex(uint32_t x, int bytes) {
      logbuf[2+(bytes*2)] = '\0';
      logbuf[0] = '0';
      logbuf[1] = 'x';
      int i;
      for (i = (1 + bytes*2); i >= 2; i--) {
        logbuf[i] = HEXDIGITS[(x &0xF)];
        x = (x >> 4);
      }
      force_log_s(logbuf);
    }
    void ea_log_reg(uint32_t regStart, uint32_t regEnd) {
      uint32_t* reg = (uint32_t*)regStart;
      while (((uint32_t)reg) < regEnd) {
        force_log_hex32(((uint32_t)reg)); force_log_s("   "); force_log_hex32(*reg); force_log_s("\r\n");
        reg++;
      }
    }
  //#endif // USE_EA_LOG
#endif // EA_LOG_WANT_CODE

#endif /* end __EA_LOG_H */
/****************************************************************************
**                            End Of File
*****************************************************************************/
