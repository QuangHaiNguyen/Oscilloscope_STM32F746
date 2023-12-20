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
#include <touchgfx/hal/GPIO.hpp>
#include <touchgfx/hal/OSWrappers.hpp>
#include <bsp/ResistiveTouchController.hpp>
#include <platform/core/arm/cortex-m/CortexMMCUInstrumentation.hpp>
#include <platform/driver/lcd/LCD16bpp.hpp>
#include <platform/driver/lcd/LCD24bpp.hpp>

extern "C" {
#include "stm32f4xx.h"
#include "stm32f429i_discovery.h"
#include "stm32f429i_discovery_lcd.h"
}


/***********************************************************
 ******   Single buffer in internal RAM              *******
 ***********************************************************
 * On this platform, TouchGFX is able to run using a single
 * frame buffer in internal SRAM, thereby avoiding the need
 * for external SDRAM.
 * This feature was introduced in TouchGFX 4.7.0. To enable it,
 * uncomment the define below. The function touchgfx_init()
 * later in this file will check for this define and configure
 * TouchGFX accordingly.
 * For details on the single buffer strategy, please refer to
 * the knowledge base article "Single vs double buffering in TouchGFX"
 * on our support site.
 */
//#define SINGLE_FRAME_BUFFER_INTERNAL

/***********************************************************
 ******         24 Bits Per Pixel Support            *******
 ***********************************************************
 *
 * The default bit depth of the framebuffer is 16bpp. If you want 24bpp support, define the symbol "USE_BPP" with a value
 * of "24", e.g. "USE_BPP=24". This symbol affects the following:
 *
 * 1. Type of TouchGFX LCD (16bpp vs 24bpp)
 * 2. Bit depth of the framebuffer(s)
 * 3. TFT controller configuration.
 *
 * WARNING: Remember to modify your image formats accordingly in app/config/. Please see the following knowledgebase article
 * for further details on how to choose and configure the appropriate image formats for your application:
 *
 * https://touchgfx.zendesk.com/hc/en-us/articles/206725849
 */

#ifdef SINGLE_FRAME_BUFFER_INTERNAL
uint16_t frameBuffer[240 * 320];
static uint32_t frameBuf0 = (uint32_t)& frameBuffer[0];
#else
//Use start of SDRAM
static uint32_t frameBuf0 = (uint32_t)(0xd0000000);
#endif
static void LCD_Config(void);
static void LCD_AF_GPIOConfig(void);

/**
  * @brief LCD configuration.
  * @note  This function Configure tha LTDC peripheral :
  *        1) Configure the Pixel Clock for the LCD
  *        2) Configure the LTDC Timing and Polarity
  *        3) Configure the LTDC Layer 1 :
  *           - The frame buffer is located at FLASH memory
  *           - The Layer size configuration : 240x160
  *        4) Configure the LTDC Layer 2.
  *           - The frame buffer is located at FLASH memory
  *           - The Layer size configuration : 240x160
  * @retval
  *  None
  */
static void LCD_Config(void)
{
    LTDC_InitTypeDef               LTDC_InitStruct;
    LTDC_Layer_InitTypeDef         LTDC_Layer_InitStruct;
    GPIO_InitTypeDef               GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

    /* Configure NCS in Output Push-Pull mode */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    /* Configure the LCD Control pins ------------------------------------------*/
    LCD_CtrlLinesConfig();
    LCD_ChipSelect(DISABLE);
    LCD_ChipSelect(ENABLE);

    /* Configure the LCD_SPI interface -----------------------------------------*/
    LCD_SPIConfig();

    /* Power on the LCD --------------------------------------------------------*/
    LCD_PowerOn();

    /* Enable the LTDC Clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_LTDC, ENABLE);

    /* Enable the DMA2D Clock */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2D, ENABLE);

    /* Configure the LCD Control pins */
    LCD_AF_GPIOConfig();

#ifndef SINGLE_FRAME_BUFFER_INTERNAL
    /* Configure the FMC Parallel interface : SDRAM is used as Frame Buffer for
       LCD */
    SDRAM_Init();
#endif

    /* Enable Pixel Clock ------------------------------------------------------*/

    /* Configure PLLSAI prescalers for LCD */
    /* PLLSAI_VCO Input = HSE_VALUE/PLL_M = 1 Mhz */
    /* PLLSAI_VCO Output = PLLSAI_VCO Input * PLLSAI_N = 192 Mhz */
    /* PLLLCDCLK = PLLSAI_VCO Output/PLLSAI_R = 192/3 = 64 Mhz */
    /* LTDC clock frequency = PLLLCDCLK / RCC_PLLSAIDivR = 64/8 = 8 Mhz */
    RCC_PLLSAIConfig(192, 7, 7);
    RCC_LTDCCLKDivConfig(RCC_PLLSAIDivR_Div4);

    /* Enable PLLSAI Clock */
    RCC_PLLSAICmd(ENABLE);
    /* Wait for PLLSAI activation */
    while (RCC_GetFlagStatus(RCC_FLAG_PLLSAIRDY) == RESET)
    {
    }

    /* LTDC Initialization -----------------------------------------------------*/

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
    LTDC_InitStruct.LTDC_HorizontalSync = 9;
    /* Configure vertical synchronization height */
    LTDC_InitStruct.LTDC_VerticalSync = 1;
    /* Configure accumulated horizontal back porch */
    LTDC_InitStruct.LTDC_AccumulatedHBP = 29;
    /* Configure accumulated vertical back porch */
    LTDC_InitStruct.LTDC_AccumulatedVBP = 3;
    /* Configure accumulated active width */
    LTDC_InitStruct.LTDC_AccumulatedActiveW = 269;
    /* Configure accumulated active height */
    LTDC_InitStruct.LTDC_AccumulatedActiveH = 323;
    /* Configure total width */
    LTDC_InitStruct.LTDC_TotalWidth = 279;
    /* Configure total height */
    LTDC_InitStruct.LTDC_TotalHeigh = 327;

    /* Configure R,G,B component values for LCD background color */
    LTDC_InitStruct.LTDC_BackgroundRedValue = 0;
    LTDC_InitStruct.LTDC_BackgroundGreenValue = 0;
    LTDC_InitStruct.LTDC_BackgroundBlueValue = 0;

    LTDC_Init(&LTDC_InitStruct);

    /* LTDC initialization end -------------------------------------------------*/


    /* Layer1 Configuration ----------------------------------------------------*/

    /* Windowing configuration */
    /* In this case all the active display area is used to display a picture then:
    Horizontal start = horizontal synchronization + Horizontal back porch = 30
    Horizontal stop = Horizontal start + window width -1 = 30 + 240 -1
    Vertical start   = vertical synchronization + vertical back porch     = 4
    Vertical stop   = Vertical start + window height -1  = 4 + 160 -1      */
    LTDC_Layer_InitStruct.LTDC_HorizontalStart = 30;
    LTDC_Layer_InitStruct.LTDC_HorizontalStop = (240 + 30 - 1);
    LTDC_Layer_InitStruct.LTDC_VerticalStart = 4;
    LTDC_Layer_InitStruct.LTDC_VerticalStop = 320 + 4 - 1;

    /* Alpha constant (255 totally opaque) */
    LTDC_Layer_InitStruct.LTDC_ConstantAlpha = 255;

    /* Default Color configuration (configure A,R,G,B component values) */
    LTDC_Layer_InitStruct.LTDC_DefaultColorBlue = 0;
    LTDC_Layer_InitStruct.LTDC_DefaultColorGreen = 0;
    LTDC_Layer_InitStruct.LTDC_DefaultColorRed = 0;
    LTDC_Layer_InitStruct.LTDC_DefaultColorAlpha = 0;

    /* Input Address configuration */
    LTDC_Layer_InitStruct.LTDC_CFBStartAdress = frameBuf0;

    /* configure the number of lines */
    LTDC_Layer_InitStruct.LTDC_CFBLineNumber = 320;

#if !defined(USE_BPP) || USE_BPP==16
    /* Pixel Format configuration*/
    LTDC_Layer_InitStruct.LTDC_PixelFormat = LTDC_Pixelformat_RGB565;

    /* Configure blending factors */
    LTDC_Layer_InitStruct.LTDC_BlendingFactor_1 = LTDC_BlendingFactor1_CA;
    LTDC_Layer_InitStruct.LTDC_BlendingFactor_2 = LTDC_BlendingFactor2_CA;

    /*  the pitch is the increment from the start of one line of pixels to the
    start of the next line in bytes, then :
    Pitch = Active high width x number of bytes per pixel
    */
    LTDC_Layer_InitStruct.LTDC_CFBPitch = (240 * 2);

    /* the length of one line of pixels in bytes + 3 then :
    Line Lenth = Active high width x number of bytes per pixel + 3
    Active high width         = 240
    number of bytes per pixel = 2    (pixel_format : RGB565)
    */
    LTDC_Layer_InitStruct.LTDC_CFBLineLength = ((240 * 2) + 3);
#elif USE_BPP==24
    /* Pixel Format configuration*/
    LTDC_Layer_InitStruct.LTDC_PixelFormat = LTDC_Pixelformat_RGB888;

    /* Configure blending factors */
    LTDC_Layer_InitStruct.LTDC_BlendingFactor_1 = LTDC_BlendingFactor1_PAxCA;
    LTDC_Layer_InitStruct.LTDC_BlendingFactor_2 = LTDC_BlendingFactor2_PAxCA;

    /*  Pitch = Active high width x 3 bytes per pixel */
    LTDC_Layer_InitStruct.LTDC_CFBPitch = (240 * 3);

    /* the length of one line of pixels in bytes + 3 then :
    Line Lenth = Active high width x number of bytes per pixel + 3
    Active high width         = 240
    number of bytes per pixel = 3    (pixel_format : RGB888)
    */
    LTDC_Layer_InitStruct.LTDC_CFBLineLength = ((240 * 3) + 3);
#else
#error Unknown USE_BPP
#endif


    LTDC_LayerInit(LTDC_Layer1, &LTDC_Layer_InitStruct);

    /* Layer1 Configuration end ------------------------------------------------*/

    LTDC_DitherCmd(ENABLE);
}

/**
  * @brief GPIO config for LTDC.
  * @retval
  *  None
  */
static void LCD_AF_GPIOConfig(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    /* Enable GPIOI, GPIOJ, GPIOG, GPIOF, GPIOH AHB Clocks */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | \
                           RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD | \
                           RCC_AHB1Periph_GPIOF | RCC_AHB1Periph_GPIOG, ENABLE);

    /* GPIOs Configuration */
    /*
    +------------------------+-----------------------+----------------------------+
    +                       LCD pins assignment                                   +
    +------------------------+-----------------------+----------------------------+
    |  LCD_TFT R2 <-> PC.12  |  LCD_TFT G2 <-> PA.06 |  LCD_TFT B2 <-> PD.06      |
    |  LCD_TFT R3 <-> PB.00  |  LCD_TFT G3 <-> PG.10 |  LCD_TFT B3 <-> PG.11      |
    |  LCD_TFT R4 <-> PA.11  |  LCD_TFT G4 <-> PB.10 |  LCD_TFT B4 <-> PG.12      |
    |  LCD_TFT R5 <-> PA.12  |  LCD_TFT G5 <-> PB.11 |  LCD_TFT B5 <-> PA.03      |
    |  LCD_TFT R6 <-> PB.01  |  LCD_TFT G6 <-> PC.07 |  LCD_TFT B6 <-> PB.08      |
    |  LCD_TFT R7 <-> PG.06  |  LCD_TFT G7 <-> PD.03 |  LCD_TFT B7 <-> PB.09      |
    -------------------------------------------------------------------------------
    |  LCD_TFT HSYNC <-> PC.06  | LCDTFT VSYNC <->  PA.04 |
    |  LCD_TFT CLK   <-> PG.07  | LCD_TFT DE   <->  PF.10 |
    -----------------------------------------------------

    */

    /* GPIOA configuration */
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource4, GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource12, GPIO_AF_LTDC);

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_6 | \
                               GPIO_Pin_11 | GPIO_Pin_12;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* GPIOB configuration */
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource0,  0x09);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource1, 0x09);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_LTDC);

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_8 | \
                               GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11;

    GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* GPIOC configuration */
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_LTDC);

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_10;

    GPIO_Init(GPIOC, &GPIO_InitStruct);

    /* GPIOD configuration */
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource3, GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource6, GPIO_AF_LTDC);

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_6;

    GPIO_Init(GPIOD, &GPIO_InitStruct);

    /* GPIOF configuration */
    GPIO_PinAFConfig(GPIOF, GPIO_PinSource10, GPIO_AF_LTDC);

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;

    GPIO_Init(GPIOF, &GPIO_InitStruct);

    /* GPIOG configuration */
    GPIO_PinAFConfig(GPIOG, GPIO_PinSource6, GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOG, GPIO_PinSource7, GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOG, GPIO_PinSource10, 0x09);
    GPIO_PinAFConfig(GPIOG, GPIO_PinSource11, GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOG, GPIO_PinSource12, 0x09);

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_10 | \
                               GPIO_Pin_11 | GPIO_Pin_12;

    GPIO_Init(GPIOG, &GPIO_InitStruct);
}

namespace touchgfx
{
STM32F4DMA dma;
ResistiveTouchController tc;
CortexMMCUInstrumentation mcuInstr;

#if !defined(USE_BPP) || USE_BPP==16
LCD16bpp display;
#elif USE_BPP==24
LCD24bpp display;
#else
#error Unknown USE_BPP
#endif

void touchgfx_init()
{
    HAL& hal = touchgfx_generic_init<STM32F4HAL>(dma, display, tc, 240, 320, 0, 0);
#if !defined(USE_BPP) || USE_BPP==16
#ifdef SINGLE_FRAME_BUFFER_INTERNAL
    //setup for single buffering
    hal.setFrameBufferStartAddress((uint16_t*)frameBuf0, 16, false, false);

    // The optimized strategy for single buffering requires the presence of a
    // task delay function.
    hal.registerTaskDelayFunction(&OSWrappers::taskDelay);

    // Enable strategy.
    hal.setFrameRefreshStrategy(HAL::REFRESH_STRATEGY_OPTIM_SINGLE_BUFFER_TFT_CTRL);
#else
    //setup for double buffering.
    hal.setFrameBufferStartAddress((uint16_t*)frameBuf0);
#endif
#elif USE_BPP==24
#ifdef  SINGLE_FRAME_BUFFER_INTERNAL
#error Single frame buffer in internal is only possible in 16bpp due to memory constraints.
#endif
    hal.setFrameBufferStartAddress((uint16_t*)frameBuf0, 24);
#else
#error Unknown USE_BPP
#endif
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
    /* Configure LCD */
    LCD_Config();

    /* Enable Layer 1 */
    LTDC_LayerCmd(LTDC_Layer1, ENABLE);

    /* Reload LTDC configuration  */
    LTDC_ReloadConfig(LTDC_IMReload);

    /* Enable The LCD */
    LTDC_Cmd(ENABLE);

    /* Set LCD foreground layer */
    LCD_SetLayer(LCD_FOREGROUND_LAYER);

    GPIO::init();
}
}

