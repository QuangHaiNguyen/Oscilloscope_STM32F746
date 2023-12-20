#include "vendor/FDI_Touch.hpp"
#include "LPC1788.h"
#include <stdint.h>
#include "lpc177x_8x_gpio.h"
extern "C"
{
#include "lpc177x_8x_pinsel.h"
#include "lpc177x_8x_adc.h"
}

#define nop()     __NOP()
#define nops5()    nop();nop();nop();nop();nop()
#define nops10()   nops5();nops5()
#define nops50()   nops10();nops10();nops10();nops10();nops10()

static void UEZBSPDelay1US(void)
{
    nops50();
    nops50();
    nop();
    nop();
    nop();
    nop();
}

volatile int adcres = 0;

static int32_t readADC(uint8_t channel)
{
  for(int i = 0; i < 10; i++)
    UEZBSPDelay1US();
  ADC_ChannelCmd(LPC_ADC, channel, ENABLE);

  ADC_StartCmd(LPC_ADC, ADC_START_NOW);

  while(ADC_ChannelGetStatus(LPC_ADC, channel, 1) == RESET);
  for(int i = 0; i < 10; i++)
    UEZBSPDelay1US();

  int32_t res = ADC_ChannelGetData(LPC_ADC, channel);
  ADC_ChannelCmd(LPC_ADC, channel, DISABLE);
  return res;
}

volatile int adc_samples = 7;

static int32_t getFilteredADCSample(uint8_t channel)
{
  int32_t value, min, max, acc, avg;
  acc = 0;
  max = 0;
  min = 4096;

  for ( int i=0; i<adc_samples+2; i++ )
  {
    value = readADC(channel);
    acc += value;
    min = MIN( min, value );
    max = MAX( max, value );
  }
  /* Throw away largest and smallest sample */
  acc = acc - min - max;
  /* Average */
  avg = acc / adc_samples;
  adcres = avg;
  return avg;
}

static int32_t readX()
{
  // X+ output high
  PINSEL_ConfigPin(0, 23, 0);
  PINSEL_SetPinMode(0, 23, PINSEL_BASICMODE_PLAINOUT);
  GPIO_SetDir(0, (1<<23), 1);
  GPIO_SetValue(0, (1<<23));

  // X- output low
  PINSEL_ConfigPin(2, 19, 0);
  PINSEL_SetPinMode(2, 19, PINSEL_BASICMODE_PLAINOUT);
  GPIO_SetDir(2, (1<<19), 1);
  GPIO_ClearValue(2, (1<<19));

  // Y+ ADC
  *((volatile uint32_t*)0x4002C060) = 0x121;

  // Y- input float no pullup
  PINSEL_ConfigPin(1, 18, 0);
  PINSEL_SetPinMode(1, 18, PINSEL_BASICMODE_PLAINOUT);
  GPIO_SetDir(1, (1<<18), 0);

  //Dly 10us
  for(int i = 0; i < 10; i++)
    UEZBSPDelay1US();

  return getFilteredADCSample(1);
}

static int32_t readY()
{
  // X+ ADC
  *((volatile uint32_t*)0x4002C05C) = 0x121;

  // X- input float no pullup
  PINSEL_ConfigPin(2, 19, 0);
  PINSEL_SetPinMode(2, 19, PINSEL_BASICMODE_PLAINOUT);
  GPIO_SetDir(2, (1<<19), 0);

  // Y+ output high
  PINSEL_ConfigPin(0, 24, 0);
  PINSEL_SetPinMode(0, 24, PINSEL_BASICMODE_PLAINOUT);
  GPIO_SetDir(0, (1<<24), 1);
  GPIO_SetValue(0, (1<<24));

  // Y- output low
  PINSEL_ConfigPin(1, 18, 0);
  PINSEL_SetPinMode(1, 18, PINSEL_BASICMODE_PLAINOUT);
  GPIO_SetDir(1, (1<<18), 1);
  GPIO_ClearValue(2, (1<<18));

  //Dly 10us
  for(int i = 0; i < 10; i++)
    UEZBSPDelay1US();

  return getFilteredADCSample(0);
}

void FDI_Touch_ReadXY(int32_t& x, int32_t& y)
{
    x = readX();
    y = readY();
}

void FDI_Touch_Detect()
{
  // X+ output low
  PINSEL_ConfigPin(0, 23, 0);
  PINSEL_SetPinMode(0, 23, PINSEL_BASICMODE_PLAINOUT);
  GPIO_SetDir(0, (1<<23), 1);
  GPIO_ClearValue(0, (1<<23));

  // X- input float no pullup
  PINSEL_ConfigPin(2, 19, 0);
  PINSEL_SetPinMode(2, 19, PINSEL_BASICMODE_PLAINOUT);
  GPIO_SetDir(2, (1<<19), 0);

  // Y+ input pullup
  PINSEL_ConfigPin(0, 24, 0);
  PINSEL_SetPinMode(0, 24, PINSEL_BASICMODE_PULLUP);
  GPIO_SetDir(0, (1<<24), 0);

  // Y- input float no pullup
  PINSEL_ConfigPin(1, 18, 0);
  PINSEL_SetPinMode(1, 18, PINSEL_BASICMODE_PLAINOUT);
  GPIO_SetDir(1, (1<<18), 0);
}
volatile unsigned int touch_val = 0;
volatile int hys_ceil = 1000;
volatile int hys_floor = 250;

static uint8_t forceReset = 0;
volatile uint8_t forceResetMax = 4;

bool FDI_Touch_TrustValue()
{
  FDI_Touch_Detect();
  for(int i = 0; i < 20; i++)
    UEZBSPDelay1US();

  unsigned int touch_val = getFilteredADCSample(1);
  return ((touch_val > 4000) || touch_val < 1000);
}

bool FDI_Touch_HasTouch(bool wasTouched)
{
  FDI_Touch_Detect();
  for(int i = 0; i < 20; i++)
    UEZBSPDelay1US();

  touch_val = getFilteredADCSample(1);
  if (wasTouched)
  {
    if (touch_val > hys_ceil)
    {
      forceReset = forceResetMax;
      return false;
    }
    return true;
  }
  else
  {
    if (forceReset)
    {
      if ((GPIO_ReadValue(1) & (1 << 18)) && touch_val > 4000)
        forceReset--;
    }
    else if (touch_val < hys_floor)
      return true;

    return false;
  }

 // Old method, too simple. return !(GPIO_ReadValue(1) & (1 << 18));
}


bool FDI_Touch_Changed(FDI_TOUCH_STATE& currentState)
{
    if (FDI_Touch_HasTouch(currentState == TOUCH_PRESSED))
    {
      if (currentState == TOUCH_PRESSED)
        return false;
      currentState = TOUCH_PRESSED;
      return true;
    }
    else
    {
      if (currentState == TOUCH_RELEASED)
        return false;
      currentState = TOUCH_RELEASED;
      return true;
    }
}

void FDI_Touch_Init()
{
  ADC_Init(LPC_ADC, 200000);

// GPIO pins
  PINSEL_ConfigPin(2, 19, 0); //P2_19 (Xminus GPIO)
  PINSEL_ConfigPin(1, 18, 0);  //P1_18 (Yminus GPIO)

  PINSEL_SetPinMode(2, 19, PINSEL_BASICMODE_PLAINOUT);
  PINSEL_SetPinMode(1, 18, PINSEL_BASICMODE_PLAINOUT);

  GPIO_SetDir(2, (1<<19), 0);
  GPIO_SetDir(1, (1<<18), 0);

  PINSEL_SetFilter(2, 19, 1);
  PINSEL_SetFilter(1, 18, 1);

// ADC pins
  /*
  PINSEL_ConfigPin(0, 23, 1); //XPlus ADC
  PINSEL_ConfigPin(0, 24, 1); //YPlus ADC

  PINSEL_SetPinMode(0, 23, PINSEL_BASICMODE_PULLUP);
  PINSEL_SetPinMode(0, 24, PINSEL_BASICMODE_PULLUP);

  GPIO_SetDir(0, (1<<23), 0);
  GPIO_SetDir(0, (1<<24), 0);

  PINSEL_SetFilter(0, 23, 1);
  PINSEL_SetFilter(0, 24, 1);
  */
  *((volatile uint32_t*)0x4002C05C) = 433;
  *((volatile uint32_t*)0x4002C060) = 433;

}
