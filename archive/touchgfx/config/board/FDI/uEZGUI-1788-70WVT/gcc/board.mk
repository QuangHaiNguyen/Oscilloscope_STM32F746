###############################################################################
#
# @brief     This file is part of the TouchGFX 4.7.0 evaluation distribution.
#
# @author    Draupner Graphics A/S <http://www.touchgfx.com>
#
###############################################################################
#
# @section Copyright
#
# Copyright (C) 2014-2016 Draupner Graphics A/S <http://www.touchgfx.com>.
# All rights reserved.
#
# TouchGFX is protected by international copyright laws and the knowledge of
# this source code may not be used to write a similar product. This file may
# only be used in accordance with a license and should not be re-
# distributed in any way without the prior permission of Draupner Graphics.
#
# This is licensed software for evaluation use, any use must strictly comply
# with the evaluation license agreement provided with delivery of the
# TouchGFX software.
#
# The evaluation license agreement can be seen on www.touchgfx.com
#
# @section Disclaimer
#
# DISCLAIMER OF WARRANTY/LIMITATION OF REMEDIES: Draupner Graphics A/S has
# no obligation to support this software. Draupner Graphics A/S is providing
# the software "AS IS", with no express or implied warranties of any kind,
# including, but not limited to, any implied warranties of merchantability
# or fitness for any particular purpose or warranties against infringement
# of any proprietary rights of a third party.
#
# Draupner Graphics A/S can not be held liable for any consequential,
# incidental, or special damages, or any other relief, or for any claim by
# any third party, arising from your use of this software.
#
###############################################################################
# Makefile snippet to be included from application Makefile.
# Will define the necessary files to compile for this specific board.
# Should expose the following vars:
# board_c_files       : C source files to include in compilation
# board_cpp_files     : C+ source files to include in compilation

board_c_files := \
	$(touchgfx_path)/framework/source/platform/hal/nxp/mcu/LPC17xx/vendor/debug_frmwrk.c \
	$(touchgfx_path)/framework/source/platform/hal/nxp/mcu/LPC17xx/vendor/lpc177x_8x_adc.c \
	$(touchgfx_path)/framework/source/platform/hal/nxp/mcu/LPC17xx/vendor/lpc177x_8x_can.c \
	$(touchgfx_path)/framework/source/platform/hal/nxp/mcu/LPC17xx/vendor/lpc177x_8x_clkpwr.c \
	$(touchgfx_path)/framework/source/platform/hal/nxp/mcu/LPC17xx/vendor/lpc177x_8x_crc.c \
	$(touchgfx_path)/framework/source/platform/hal/nxp/mcu/LPC17xx/vendor/lpc177x_8x_dac.c \
	$(touchgfx_path)/framework/source/platform/hal/nxp/mcu/LPC17xx/vendor/lpc177x_8x_eeprom.c \
	$(touchgfx_path)/framework/source/platform/hal/nxp/mcu/LPC17xx/vendor/lpc177x_8x_emc.c \
	$(touchgfx_path)/framework/source/platform/hal/nxp/mcu/LPC17xx/vendor/lpc177x_8x_exti.c \
	$(touchgfx_path)/framework/source/platform/hal/nxp/mcu/LPC17xx/vendor/lpc177x_8x_gpdma.c \
	$(touchgfx_path)/framework/source/platform/hal/nxp/mcu/LPC17xx/vendor/lpc177x_8x_gpio.c \
	$(touchgfx_path)/framework/source/platform/hal/nxp/mcu/LPC17xx/vendor/lpc177x_8x_i2c.c \
	$(touchgfx_path)/framework/source/platform/hal/nxp/mcu/LPC17xx/vendor/lpc177x_8x_i2s.c \
	$(touchgfx_path)/framework/source/platform/hal/nxp/mcu/LPC17xx/vendor/lpc177x_8x_lcd.c \
	$(touchgfx_path)/framework/source/platform/hal/nxp/mcu/LPC17xx/vendor/lpc177x_8x_mci.c \
	$(touchgfx_path)/framework/source/platform/hal/nxp/mcu/LPC17xx/vendor/lpc177x_8x_mcpwm.c \
	$(touchgfx_path)/framework/source/platform/hal/nxp/mcu/LPC17xx/vendor/lpc177x_8x_nvic.c \
	$(touchgfx_path)/framework/source/platform/hal/nxp/mcu/LPC17xx/vendor/lpc177x_8x_pinsel.c \
	$(touchgfx_path)/framework/source/platform/hal/nxp/mcu/LPC17xx/vendor/lpc177x_8x_pwm.c \
	$(touchgfx_path)/framework/source/platform/hal/nxp/mcu/LPC17xx/vendor/lpc177x_8x_qei.c \
	$(touchgfx_path)/framework/source/platform/hal/nxp/mcu/LPC17xx/vendor/lpc177x_8x_rtc.c \
	$(touchgfx_path)/framework/source/platform/hal/nxp/mcu/LPC17xx/vendor/lpc177x_8x_ssp.c \
	$(touchgfx_path)/framework/source/platform/hal/nxp/mcu/LPC17xx/vendor/lpc177x_8x_systick.c \
	$(touchgfx_path)/framework/source/platform/hal/nxp/mcu/LPC17xx/vendor/lpc177x_8x_timer.c \
	$(touchgfx_path)/framework/source/platform/hal/nxp/mcu/LPC17xx/vendor/lpc177x_8x_uart.c \
	$(touchgfx_path)/framework/source/platform/hal/nxp/mcu/LPC17xx/vendor/lpc177x_8x_usbhost.c \
	$(touchgfx_path)/framework/source/platform/hal/nxp/mcu/LPC17xx/vendor/lpc177x_8x_wwdt.c \
	$(touchgfx_path)/framework/source/platform/hal/nxp/mcu/LPC17xx/vendor/system_LPC177x_8x.c


board_cpp_files := \
	$(touchgfx_path)/board/FDI/uEZGUI-1788-70WVT/source/bsp/ResistiveTouchController.cpp \
	$(touchgfx_path)/board/FDI/uEZGUI-1788-70WVT/source/vendor/FDI_LCD.cpp \
	$(touchgfx_path)/board/FDI/uEZGUI-1788-70WVT/source/vendor/FDI_Memory.cpp \
	$(touchgfx_path)/board/FDI/uEZGUI-1788-70WVT/source/vendor/FDI_RTC.cpp \
	$(touchgfx_path)/board/FDI/uEZGUI-1788-70WVT/source/vendor/FDI_Touch.cpp \
	$(touchgfx_path)/framework/source/platform/hal/nxp/mcu/LPC17xx/NXP1788DMA.cpp \
	$(touchgfx_path)/framework/source/platform/hal/nxp/mcu/LPC17xx/NXP1788HAL.cpp
