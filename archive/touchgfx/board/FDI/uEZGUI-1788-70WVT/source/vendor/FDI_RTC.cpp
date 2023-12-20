#include "vendor/FDI_RTC.hpp"

#include "lpc177x_8x_rtc.h"

void FDI_RTC_Init()
{
  RTC_Init(LPC_RTC);
  RTC_Cmd(LPC_RTC, ENABLE);
//  RTC_CntIncrIntConfig(LPC_RTC, RTC_TIMETYPE_MINUTE, ENABLE);
}

void FDI_RTC_SetTime(uint16_t year, uint16_t month, uint16_t day, uint8_t hour, uint8_t minute)
{
  RTC_SetTime(LPC_RTC, RTC_TIMETYPE_SECOND, 0);
  RTC_SetTime(LPC_RTC, RTC_TIMETYPE_YEAR, year);
  RTC_SetTime(LPC_RTC, RTC_TIMETYPE_MONTH, month);
  RTC_SetTime(LPC_RTC, RTC_TIMETYPE_DAYOFMONTH, day);
  RTC_SetTime(LPC_RTC, RTC_TIMETYPE_HOUR, hour);
  RTC_SetTime(LPC_RTC, RTC_TIMETYPE_MINUTE, minute);
}
/*
bool FDI_RTC_UpdateTime(RTCValue& rtc)
{
  if (RTC_GetTime(LPC_RTC, RTC_TIMETYPE_MINUTE) != rtc.minute)
  {
      rtc.year = RTC_GetTime(LPC_RTC, RTC_TIMETYPE_YEAR);
      rtc.month = RTC_GetTime(LPC_RTC, RTC_TIMETYPE_MONTH);
      rtc.day = RTC_GetTime(LPC_RTC, RTC_TIMETYPE_DAYOFMONTH);
      rtc.hour = RTC_GetTime(LPC_RTC, RTC_TIMETYPE_HOUR);
      rtc.minute = RTC_GetTime(LPC_RTC, RTC_TIMETYPE_MINUTE);
      return true;
  }
  return false;
}
*/
