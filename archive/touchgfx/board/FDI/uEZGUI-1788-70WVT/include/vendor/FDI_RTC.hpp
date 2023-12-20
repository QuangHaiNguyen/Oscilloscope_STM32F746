#ifndef FDI_RTC_HPP
#define FDI_RTC_HPP

#include <touchgfx/hal/Types.hpp>
//#include <linak_common/RTCValue.hpp>

void FDI_RTC_Init();
void FDI_RTC_SetTime(uint16_t year, uint16_t month, uint16_t day, uint8_t hour, uint8_t minute);
//bool FDI_RTC_UpdateTime(RTCValue& rtc);
#endif
