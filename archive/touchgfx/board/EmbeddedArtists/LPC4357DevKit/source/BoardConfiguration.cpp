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
#include <bsp/TSC2046.hpp>

#include <platform/hal/nxp/mcu/LPC43xx/NXP43XXDMA.hpp>
#include <platform/hal/nxp/mcu/LPC43xx/NXP43XXHAL.hpp>
#include <platform/driver/lcd/LCD16bpp.hpp>

#include "bsp/LEDs_init.hpp"

#include "lpc_types.h"
#include "lpc43xx.h"
#include "lpc43xx_cgu.h"
#include "lpc43xx_scu.h"
#include "lpc43xx_ssp.h"
#include "lpc43xx_timer.h"
#include "lcd_driver.h"
#include "ea_lcd_board.h"
#include "sdram.h"
#include "pca9532.h"
#include "lcdb_eeprom.h"
#include "board.h"

#define I2C_PORT (LPC_I2C0)

static uint32_t frameBuf0 = (int32_t)(SDRAM_BASE);

static const uint16_t DISPLAY_NATIVE_WIDTH  = 480;
static const uint16_t DISPLAY_NATIVE_HEIGHT = 272;

extern "C"
{
    void initSPIFI();
}

static void fadeOutInDisplay(uint32_t value)
{
    ea_lcdb_ctrl_backlightContrast(value);
}

static bool initLCD()
{
    int32_t dev_lcd = 0;
    LCD_PARAM_T lp;

    if (ea_lcdb_open((LCD_PARAM_T*)NULL, (char*)NULL, &dev_lcd) == LCDB_RESULT_OK)
    {
        if (ea_lcdb_getLcdParams(&lp) != LCDB_RESULT_OK)
        {
            return false;
        }

        if (ea_lcdb_enable(dev_lcd, (signed int)frameBuf0) != LCDB_RESULT_OK)
        {
            return false;
        }
    }
    else
    {
        return false;
    }

    fadeOutInDisplay(100);

    return true;
}


//Linked list memory for scatter-gather DMA. Must contain same number of elements as the height of the display.
GPDMA_LLI_Type lliMemory[DISPLAY_NATIVE_HEIGHT];

NXP43XXDMA dma(lliMemory);
TSC2046 tc;
LCD16bpp display;

namespace touchgfx
{
void touchgfx_init()
{
    HAL& hal = touchgfx_generic_init<NXP43XXHAL>(dma, display, tc, DISPLAY_NATIVE_WIDTH, DISPLAY_NATIVE_HEIGHT, 0, 0);
    hal.setFrameBufferStartAddress((uint16_t*)SDRAM_BASE);
    hal.setDragThreshold(3); //"Unstable" touch, so filter input before reporting drag events.
    hal.setFingerSize(1);

    // By default frame rate compensation is off.
    // Enable frame rate compensation to smooth out animations in case there is periodic slow frame rates.
    hal.setFrameRateCompensation(false);

    HAL::getInstance()->lockDMAToFrontPorch(false);
}

void hw_init()
{
    improved_cgu_init(WANTED_FREQ_204_MHZ);

    RGU_SoftReset(RGU_SIG_LCD);
    RGU_SoftReset(RGU_SIG_DMA);
    RGU_SoftReset(RGU_SIG_GPIO);
    RGU_SoftReset(RGU_SIG_TIMER0);
    RGU_SoftReset(RGU_SIG_I2C0);
    RGU_SoftReset(RGU_SIG_SSP0);

    // Setup the 16 LEDs
    LEDs_init();
    setLEDs(0x0000);

    // initialize timer
    TIM_TIMERCFG_Type timerCfg;
    TIM_ConfigStructInit(TIM_TIMER_MODE, &timerCfg);
    TIM_Init(LPC_TIMER0, TIM_TIMER_MODE, &timerCfg);

    // Initialize I2C
    I2C_Init(I2C_PORT, 100000);
    I2C_Cmd(I2C_PORT, ENABLE);

    // SDRAM initialize
    sdram_init();

    //Initialize SPIFI
    initSPIFI();

    // Setup the 16 LEDs
    LEDs_init();
    setLEDs(0x0000);
    setLEDs(SystemCoreClock / 1000000); // Indicate the clock in MHz

    //Init LCD
    initLCD();
}
}
