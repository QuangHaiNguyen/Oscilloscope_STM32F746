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
#include <touchgfx/hal/OSWrappers.hpp>
#include <platform/hal/Freescale/mcu/MK70F12/FreescaleK70HAL.hpp>
#include <touchgfx/lcd/LCD.hpp>
#include <touchgfx/hal/GPIO.hpp>

extern "C"
{
#include "MK70F12.h"
#include "arm_cm4.h"
}

uint16_t* FreescaleK70HAL::getTFTFrameBuffer() const
{
    return (uint16_t*)(LCDC_LSSAR);
}

void FreescaleK70HAL::setTFTFrameBuffer(uint16_t* adr)
{
    LCDC_LSSAR = (uint32_t)adr;
}

/*
 Interrupt configuration
*/
void FreescaleK70HAL::configureInterrupts()
{
    //DMA interrupts - DMA channel 0 transfer complete interrupt
    set_irq_priority(0, 0x9);

    //LCD interrupts. VectorNr -16.
    set_irq_priority(97, 0x9);
}

void FreescaleK70HAL::disableInterrupts()
{
    disable_irq(0);
    disable_irq(97);
}

void FreescaleK70HAL::enableInterrupts()
{
    enable_irq(0); //enable interrupt on DMA channel 0 completion
    enable_irq(97); //Enable LCD interrupt
}

void FreescaleK70HAL::enableLCDControllerInterrupt()
{
    //Enable LCDC interrupts. 0x1: Beginning of frame interrupt enable.
    LCDC_LIER = 0x1;

    //Set initial LCD interrupt conditions. 0x1: LICR[INTCON] must be 1 to use the BOF interrupt.
    LCDC_LICR = 0x1;
}

static bool LCD_HasIntPorch(uint32_t reg)
{
    return ((reg & 2) > 0) && ((LCDC_LICR & 4) > 0);
}

/* Set next interrupt to be Front Porch interrupt  */
static void LCD_SetNextIntPorch()
{
    uint32_t val_licr = LCDC_LICR & 0xFFFFFFF8;
    uint32_t val_lier = LCDC_LIER & 0xFFFFFFFC;

    LCDC_LICR = val_licr | (0x4); //SYN = 1, CON=0. Interrupt flag is set on output of last data of frame to LCD panel.
    LCDC_LIER = val_lier | (0x2); //EOF_EN = 1, BOF_EN = 0
}

/* VSYNC interrupt */
static void LCD_SetNextIntVideo()
{
    /****************************************************************
    * Flag set on loading first data of frame from memory
    ****************************************************************/
    uint32_t val_licr = LCDC_LICR;
    uint32_t val_lier = LCDC_LIER;

    LCDC_LICR = (val_licr & 0xFFFFFFF8) | 0x1;
    LCDC_LIER = (val_lier & 0xFFFFFFFC) | 0x1;
}

extern "C"
__irq void LCD_IRQHandler(void)
{
    /* LISR - indicates whether or not an interrupt condition has been met.
     If a bit in this register has been set, and the corresponding bit in LIER has been set
     the LCDC interrupt pin is asserted to the interrupt controller. */

    uint32_t reg = LCDC_LISR;
    if (reg & 0x3 > 0) //If either BOF or EOF interrupt is active.
    {
        if (LCD_HasIntPorch(reg))
        {
            GPIO::clear(GPIO::VSYNC_FREQ);
            LCD_SetNextIntVideo();                             //Set next interrupt to start of vertical synchronization
            touchgfx::HAL::getInstance()->frontPorchEntered(); //Allow DMA transfer to start
        }
        else
        {
            GPIO::set(GPIO::VSYNC_FREQ);
            LCD_SetNextIntPorch();
            HAL::getInstance()->vSync();
            touchgfx::OSWrappers::signalVSync();
        }
    }
}

extern "C"
__irq void DMA_IRQHandler(void)
{
    //Interrupt at End-of-Major-loop
    if (DMA_INT & 0x1)
    {
        DMA_CINT = 0x0;
        touchgfx::HAL::getInstance()->signalDMAInterrupt();
    }
}
