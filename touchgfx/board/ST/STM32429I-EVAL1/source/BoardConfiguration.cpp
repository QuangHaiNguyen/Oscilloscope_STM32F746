/******************************************************************************
 *
 * @brief     This file is part of the TouchGFX 4.7.0 evaluation distribution.
 *
 * @author    Draupner Graphics A/S <http://www.touchgfx.com>
 *
 ******************************************************************************
 *
 * @section Copyright
 *
 * Copyright (C) 2014-2016 Draupner Graphics A/S <http://www.touchgfx.com>.
 * All rights reserved.
 *
 * TouchGFX is protected by international copyright laws and the knowledge of
 * this source code may not be used to write a similar product. This file may
 * only be used in accordance with a license and should not be re-
 * distributed in any way without the prior permission of Draupner Graphics.
 *
 * This is licensed software for evaluation use, any use must strictly comply
 * with the evaluation license agreement provided with delivery of the
 * TouchGFX software.
 *
 * The evaluation license agreement can be seen on www.touchgfx.com
 *
 * @section Disclaimer
 *
 * DISCLAIMER OF WARRANTY/LIMITATION OF REMEDIES: Draupner Graphics A/S has
 * no obligation to support this software. Draupner Graphics A/S is providing
 * the software "AS IS", with no express or implied warranties of any kind,
 * including, but not limited to, any implied warranties of merchantability
 * or fitness for any particular purpose or warranties against infringement
 * of any proprietary rights of a third party.
 *
 * Draupner Graphics A/S can not be held liable for any consequential,
 * incidental, or special damages, or any other relief, or for any claim by
 * any third party, arising from your use of this software.
 *
 *****************************************************************************/
#include <common/TouchGFXInit.hpp>
#include <touchgfx/hal/BoardConfiguration.hpp>
#include <platform/hal/ST/mcu/stm32f4x9/STM32F4HAL.hpp>
#include <platform/hal/ST/mcu/stm32f4x9/STM32F4DMA.hpp>
#include <bsp/ResistiveTouchController.hpp>
#include <touchgfx/hal/GPIO.hpp>
#include <platform/core/arm/cortex-m/CortexMMCUInstrumentation.hpp>
#include <platform/driver/lcd/LCD16bpp.hpp>

extern "C"
{
#include "stm324x9i_eval_fmc_sram.h"
#include "stm324x9i_eval_fmc_sdram.h"
#include "stm324x9i_eval_fmc_nor.h"
#include "stm324x9i_eval.h"
#include "stm324x9i_eval_lcd.h"
}

static void LCD_Config(void);
static void LCD_AF_GPIOConfig(void);

static uint32_t frameBuf0 = (uint32_t)(0x64000000);

static void LCD_Config(void)
{
    LTDC_InitTypeDef               LTDC_InitStruct;
    LTDC_Layer_InitTypeDef         LTDC_Layer_InitStruct;

    /* Enable Pixel Clock --------------------------------------------------------*/

    /* Configure PLLSAI prescalers for LCD */
    /* PLLSAI_VCO Input = HSE_VALUE/PLL_M = 1 Mhz */
    /* PLLSAI_VCO Output = PLLSAI_VCO Input * PLLSAI_N = 192 Mhz */
    /* PLLLCDCLK = PLLSAI_VCO Output/PLLSAI_R = 192/7 = 27.4 Mhz */
    /* LTDC clock frequency = PLLLCDCLK / RCC_PLLSAIDivR = 27.4/2 = 13.7 Mhz */
    RCC_PLLSAIConfig(192, 7, 7);
    RCC_LTDCCLKDivConfig(RCC_PLLSAIDivR_Div2);

    /* Enable PLLSAI Clock */
    RCC_PLLSAICmd(ENABLE);
    /* Wait for PLLSAI activation */
    while (RCC_GetFlagStatus(RCC_FLAG_PLLSAIRDY) == RESET)
    {
    }

    /* Enable the LTDC Clock -----------------------------------------------------*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_LTDC, ENABLE);

    /* Configure the LCD Control pins --------------------------------------------*/
    LCD_AF_GPIOConfig();

    /* Sets LCD backlight --------------------------------------------------------*/
    GPIO_SetBits(GPIOA, GPIO_Pin_8);
    GPIO_SetBits(GPIOC, GPIO_Pin_6);

    /* LTDC Initialization -------------------------------------------------------*/

    /* Initialize the horizontal synchronization polarity as active low*/
    LTDC_InitStruct.LTDC_HSPolarity = LTDC_HSPolarity_AL;
    /* Initialize the vertical synchronization polarity as active low */
    LTDC_InitStruct.LTDC_VSPolarity = LTDC_VSPolarity_AL;
    /* Initialize the data enable polarity as active low */
    LTDC_InitStruct.LTDC_DEPolarity = LTDC_DEPolarity_AL;
    /* Initialize the pixel clock polarity as input pixel clock */
    LTDC_InitStruct.LTDC_PCPolarity = LTDC_PCPolarity_IPC;

    /* Timing configuration */
    /* Configure horizontal synchronization width */
    LTDC_InitStruct.LTDC_HorizontalSync = 40;
    /* Configure vertical synchronization height */
    LTDC_InitStruct.LTDC_VerticalSync = 9;
    /* Configure accumulated horizontal back porch */
    LTDC_InitStruct.LTDC_AccumulatedHBP = 42;
    /* Configure accumulated vertical back porch */
    LTDC_InitStruct.LTDC_AccumulatedVBP = 11;
    /* Configure accumulated active width */
    LTDC_InitStruct.LTDC_AccumulatedActiveW = 522;
    /* Configure accumulated active height */
    LTDC_InitStruct.LTDC_AccumulatedActiveH = 283;
    /* Configure total width */
    LTDC_InitStruct.LTDC_TotalWidth = 524;
    /* Configure total height */
    LTDC_InitStruct.LTDC_TotalHeigh = 471;

    /* Configure R,G,B component values for LCD background color */
    LTDC_InitStruct.LTDC_BackgroundRedValue = 0;
    LTDC_InitStruct.LTDC_BackgroundGreenValue = 0;
    LTDC_InitStruct.LTDC_BackgroundBlueValue = 0;

    LTDC_Init(&LTDC_InitStruct);

    /* LTDC initialization end ---------------------------------------------------*/

    /* Layer1 Configuration ------------------------------------------------------*/

    /* Windowing configuration */
    /* In this case all the active display area is used to display a picture then :
       Horizontal start = horizontal synchronization + Horizontal back porch = 43
       Vertical start   = vertical synchronization + vertical back porch     = 12
       Horizontal stop = Horizontal start + window width -1 = 43 + 480 -1
       Vertical stop   = Vertical start + window height -1  = 12 + 272 -1      */
    LTDC_Layer_InitStruct.LTDC_HorizontalStart = 43;
    LTDC_Layer_InitStruct.LTDC_HorizontalStop = (480 + 43 - 1);
    LTDC_Layer_InitStruct.LTDC_VerticalStart = 12;
    LTDC_Layer_InitStruct.LTDC_VerticalStop = (272 + 12 - 1);

    /* Pixel Format configuration*/
    //LTDC_Layer_InitStruct.LTDC_PixelFormat = LTDC_Pixelformat_ARGB4444;
    LTDC_Layer_InitStruct.LTDC_PixelFormat = LTDC_Pixelformat_RGB565;


    /* Alpha constant (255 totally opaque) */
    LTDC_Layer_InitStruct.LTDC_ConstantAlpha = 255;

    /* Configure blending factors */
    LTDC_Layer_InitStruct.LTDC_BlendingFactor_1 = LTDC_BlendingFactor1_CA;
    LTDC_Layer_InitStruct.LTDC_BlendingFactor_2 = LTDC_BlendingFactor2_CA;

    /* Default Color configuration (configure A,R,G,B component values) */
    LTDC_Layer_InitStruct.LTDC_DefaultColorBlue = 0;
    LTDC_Layer_InitStruct.LTDC_DefaultColorGreen = 0;
    LTDC_Layer_InitStruct.LTDC_DefaultColorRed = 0;
    LTDC_Layer_InitStruct.LTDC_DefaultColorAlpha = 0;

    /* Input Address configuration */
    //LTDC_Layer_InitStruct.LTDC_CFBStartAdress = (uint32_t)&ARGB4444_480x272;
    LTDC_Layer_InitStruct.LTDC_CFBStartAdress = frameBuf0;

    /* the length of one line of pixels in bytes + 3 then :
       Line Lenth = Active high width x number of bytes per pixel + 3
       Active high width         = 480
       number of bytes per pixel = 2    (pixel_format : ARGB4444)
    */
    LTDC_Layer_InitStruct.LTDC_CFBLineLength = ((480 * 2) + 3);

    /*  the pitch is the increment from the start of one line of pixels to the
        start of the next line in bytes, then :
        Pitch = Active high width x number of bytes per pixel
    */
    LTDC_Layer_InitStruct.LTDC_CFBPitch = (480 * 2);

    /* configure the number of lines */
    LTDC_Layer_InitStruct.LTDC_CFBLineNumber = 272;

    LTDC_LayerInit(LTDC_Layer1, &LTDC_Layer_InitStruct);

    /* Layer1 Configuration end --------------------------------------------------*/
}

/**
  * @brief GPIO configuration for LTDC.
  * @retval
  *  None
  */
static void LCD_AF_GPIOConfig(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    /* Enable GPIOI, GPIOJ, GPIOG, GPIOF, GPIOH AHB Clocks */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOI | RCC_AHB1Periph_GPIOJ | \
                           RCC_AHB1Periph_GPIOK | RCC_AHB1Periph_GPIOC | \
                           RCC_AHB1Periph_GPIOA,  ENABLE);

    /* GPIOs Configuration */
    /*
     +------------------------+-----------------------+----------------------------+
     +                       LCD pins assignment                                   +
     +------------------------+-----------------------+----------------------------+
     |  LCD_TFT R0 <-> PI.15  |  LCD_TFT G0 <-> PJ.07 |  LCD_TFT B0 <-> PJ.12      |
     |  LCD_TFT R1 <-> PJ.00  |  LCD_TFT G1 <-> PJ.08 |  LCD_TFT B1 <-> PJ.13      |
     |  LCD_TFT R2 <-> PJ.01  |  LCD_TFT G2 <-> PJ.09 |  LCD_TFT B2 <-> PJ.14      |
     |  LCD_TFT R3 <-> PJ.02  |  LCD_TFT G3 <-> PJ.10 |  LCD_TFT B3 <-> PJ.15      |
     |  LCD_TFT R4 <-> PJ.03  |  LCD_TFT G4 <-> PJ.11 |  LCD_TFT B4 <-> PK.03      |
     |  LCD_TFT R5 <-> PJ.04  |  LCD_TFT G5 <-> PK.00 |  LCD_TFT B5 <-> PK.04      |
     |  LCD_TFT R6 <-> PJ.05  |  LCD_TFT G6 <-> PK.01 |  LCD_TFT B6 <-> PK.05      |
     |  LCD_TFT R7 <-> PJ.06  |  LCD_TFT G7 <-> PK.02 |  LCD_TFT B7 <-> PK.06      |
     -------------------------------------------------------------------------------
              |  LCD_TFT HSYNC <-> PI.12  | LCDTFT VSYNC <->  PI.13 |
              |  LCD_TFT CLK   <-> PI.14  | LCD_TFT DE   <->  PK.07 |
               -----------------------------------------------------
              |LCD_TFT backlight <-> PA.08| LCD_ENABLE <-> PC.06    |
               -----------------------------------------------------
    */

    /* GPIOI configuration */
    GPIO_PinAFConfig(GPIOI, GPIO_PinSource12, GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOI, GPIO_PinSource13, GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOI, GPIO_PinSource14, GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOI, GPIO_PinSource15, GPIO_AF_LTDC);

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;

    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOI, &GPIO_InitStruct);

    /* GPIOJ configuration */
    GPIO_PinAFConfig(GPIOJ, GPIO_PinSource0, GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOJ, GPIO_PinSource1, GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOJ, GPIO_PinSource2, GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOJ, GPIO_PinSource3, GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOJ, GPIO_PinSource4, GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOJ, GPIO_PinSource5, GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOJ, GPIO_PinSource6, GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOJ, GPIO_PinSource7, GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOJ, GPIO_PinSource8, GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOJ, GPIO_PinSource9, GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOJ, GPIO_PinSource10, GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOJ, GPIO_PinSource11, GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOJ, GPIO_PinSource12, GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOJ, GPIO_PinSource13, GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOJ, GPIO_PinSource14, GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOJ, GPIO_PinSource15, GPIO_AF_LTDC);

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | \
                               GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | \
                               GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | \
                               GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;

    GPIO_Init(GPIOJ, &GPIO_InitStruct);

    /* GPIOI configuration */
    GPIO_PinAFConfig(GPIOK, GPIO_PinSource0, GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOK, GPIO_PinSource1, GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOK, GPIO_PinSource2, GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOK, GPIO_PinSource3, GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOK, GPIO_PinSource4, GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOK, GPIO_PinSource5, GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOK, GPIO_PinSource6, GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOK, GPIO_PinSource7, GPIO_AF_LTDC);

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | \
                               GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;

    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOK, &GPIO_InitStruct);

    /* GPIOC configuration */
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;

    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOC, &GPIO_InitStruct);

    /* GPIOA configuration */
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;

    GPIO_Init(GPIOA, &GPIO_InitStruct);
}

namespace touchgfx
{
STM32F4DMA dma;
ResistiveTouchController tc;
CortexMMCUInstrumentation mcuInstr;
LCD16bpp display;

void touchgfx_init()
{
    HAL& hal = touchgfx_generic_init<STM32F4HAL>(dma, display, tc, 480, 272, 0, 0);
    hal.setFrameBufferStartAddress((uint16_t*)frameBuf0);
    hal.setTouchSampleRate(2);
    hal.setFingerSize(1);

    // By default frame rate compensation is off.
    // Enable frame rate compensation to smooth out animations in case there is periodic slow frame rates.
    hal.setFrameRateCompensation(false);

    // This platform can handle simultaneous DMA and TFT accesses to SDRAM, so disable lock to increase performance.
    hal.lockDMAToFrontPorch(false);

    mcuInstr.init();

    //Set MCU instrumentation and Load calculation
    hal.setMCUInstrumentation(&mcuInstr);
    hal.enableMCULoadCalculation(true);
}

void hw_init()
{
    NOR_Init();
    SRAM_Init();

    /* LCD Configuration */
    LCD_Config();

    /* Enable Layer1 */
    LTDC_LayerCmd(LTDC_Layer1, ENABLE);

    /* Reload configuration of Layer1 */
    LTDC_ReloadConfig(LTDC_IMReload);

    /* Enable The LCD */
    LTDC_Cmd(ENABLE);

    GPIO::init();

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2D, ENABLE);
}
}

