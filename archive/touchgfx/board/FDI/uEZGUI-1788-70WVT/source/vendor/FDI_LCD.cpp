#include "vendor/FDI_LCD.hpp"
#include "LPC1788.h"
#include <stdint.h>

#include "lpc177x_8x_gpio.h"
extern "C"
{
#include "lpc177x_8x_pinsel.h"
}

static void Backlight_Init()
{
  //*((volatile uint32_t*)0x4002C088) = 0xC00FDFFF;//51;

    LPC_SC->PCONP |= (1<<5);
    LPC_PWM0->MCR = 2;
    LPC_PWM0->MR0 = 0x5DC;
    LPC_PWM0->MR1 = 0x0;
    LPC_PWM0->PCR = 0x0;
    LPC_PWM0->LER = 0x1;

    LPC_PWM0->TCR = 9;
    LPC_PWM0->PCR = 0x200;

    LPC_PWM0->MR1 = 0x5DC;
    LPC_PWM0->LER = 2;
}

void FDI_LCD_BackLightLevel(uint8_t percentage)
{
  uint16_t val = LPC_PWM0->MR0;
  if (percentage < 100)
  {
      val = LPC_PWM0->MR0 / 100 * percentage;
  }
    LPC_PWM0->MCR &=  ~(7 << (1 * 3));
    LPC_PWM0->MCR |= (0 << (1 * 3));
    LPC_PWM0->MR1 = val;
    LPC_PWM0->LER = (1<<1);
}

void FDI_LCD_Init()
{
  LPC_SC->PCONP |= 1;
  LPC_SC->PCONP |= (1<<15);
  LPC_SC->PCONP |= (1<<29);

//  Backlight_Init();

//  PINSEL_ConfigPin(2, 0, 0);

  PINSEL_ConfigPin(2, 2, 7);
  PINSEL_ConfigPin(2, 4, 7);
  PINSEL_ConfigPin(2, 3, 7);
  PINSEL_ConfigPin(2, 5, 7);

  PINSEL_ConfigPin(4, 28, 7);
  PINSEL_ConfigPin(4, 29, 7);
  PINSEL_ConfigPin(2, 6, 7);
  PINSEL_ConfigPin(2, 7, 7);
  PINSEL_ConfigPin(2, 8, 7);
  PINSEL_ConfigPin(2, 9, 7);

  PINSEL_ConfigPin(1, 20, 7);
  PINSEL_ConfigPin(1, 21, 7);
  PINSEL_ConfigPin(1, 22, 7);
  PINSEL_ConfigPin(1, 23, 7);
  PINSEL_ConfigPin(1, 24, 7);
  PINSEL_ConfigPin(1, 25, 7);

  PINSEL_ConfigPin(2, 12, 7);
  PINSEL_ConfigPin(2, 13, 7);
  PINSEL_ConfigPin(1, 26, 7);
  PINSEL_ConfigPin(1, 27, 7);
  PINSEL_ConfigPin(1, 28, 7);
  PINSEL_ConfigPin(1, 29, 7);

  LPC_LCD->TIMH = 0x300202C4;
  LPC_LCD->TIMV = 0x1F3F09DF;

  LPC_LCD->POL =  0x031F1802;
  LPC_LCD->LE = 0;
  LPC_LCD->CTRL = 0x92C;
  LPC_LCD->UPBASE = 0xA0000000;

  // Give LCD highest priority on the LPC1788 (unnamed hidden register)
  *((uint32_t *)0x400FC188) = 0x00000C09 | (1<<16);

  LPC_LCD->CTRL |= 1;
  LPC_LCD->CTRL |= (1<<11);
  LPC_LCD->CTRL |= (1<<16); //Enable WATERMARK. Not sure what this means.

  // Configure
  LPC_LCD->INTMSK = 8;
  //LPC_LCD->TIMV |= 0x0F0F0000;
  //LPC_LCD->POL &= ~0x1F;
  //LPC_LCD->POL |= 0xC;



  GPIO_SetDir(2, 1, 1);
  GPIO_ClearValue(2, 1);

  GPIO_SetDir(4, (1<<31), 1);
//  GPIO_SetValue(4, (1<<31));

  PINSEL_ConfigPin(1, 2, 3);
  Backlight_Init();
}

void FDI_LCD_BackLight(bool enable)
{
  if (enable)
  {
      LPC_LCD->CTRL |= (1<<11);

  }
  else
  {
      LPC_LCD->CTRL &= ~(1<<11);

  }

}
