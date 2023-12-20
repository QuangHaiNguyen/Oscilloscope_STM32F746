#ifndef FDI_TOUCH_HPP
#define FDI_TOUCH_HPP

#include <stdint.h>

typedef enum
{
  TOUCH_RELEASED,
  TOUCH_PRESSED
} FDI_TOUCH_STATE;

bool FDI_Touch_Changed(FDI_TOUCH_STATE& currentState);
void FDI_Touch_ReadXY(int32_t& x, int32_t& y);
void FDI_Touch_Detect();
bool FDI_Touch_HasTouch(bool wasPressed);
void FDI_Touch_Init();
bool FDI_Touch_TrustValue();

#endif /* FDI_TOUCH_HPP */
