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
	$(touchgfx_path)/board/EmbeddedArtists/LPC4088DisplayModule/source/vendor/bios_loader.c \
	$(touchgfx_path)/board/EmbeddedArtists/LPC4088DisplayModule/source/vendor/board.c \
	$(touchgfx_path)/board/EmbeddedArtists/LPC4088DisplayModule/source/vendor/board_sysinit.c \
	$(touchgfx_path)/board/EmbeddedArtists/LPC4088DisplayModule/source/vendor/spifi.c \
	$(touchgfx_path)/framework/source/platform/hal/nxp/mcu/LPC40xx/vendor/adc_17xx_40xx.c \
	$(touchgfx_path)/framework/source/platform/hal/nxp/mcu/LPC40xx/vendor/can_17xx_40xx.c \
	$(touchgfx_path)/framework/source/platform/hal/nxp/mcu/LPC40xx/vendor/chip_17xx_40xx.c \
	$(touchgfx_path)/framework/source/platform/hal/nxp/mcu/LPC40xx/vendor/clock_17xx_40xx.c \
	$(touchgfx_path)/framework/source/platform/hal/nxp/mcu/LPC40xx/vendor/cmp_17xx_40xx.c \
	$(touchgfx_path)/framework/source/platform/hal/nxp/mcu/LPC40xx/vendor/crc_17xx_40xx.c \
	$(touchgfx_path)/framework/source/platform/hal/nxp/mcu/LPC40xx/vendor/dac_17xx_40xx.c \
	$(touchgfx_path)/framework/source/platform/hal/nxp/mcu/LPC40xx/vendor/eeprom_17xx_40xx.c \
	$(touchgfx_path)/framework/source/platform/hal/nxp/mcu/LPC40xx/vendor/emc_17xx_40xx.c \
	$(touchgfx_path)/framework/source/platform/hal/nxp/mcu/LPC40xx/vendor/enet_17xx_40xx.c \
	$(touchgfx_path)/framework/source/platform/hal/nxp/mcu/LPC40xx/vendor/fpu_init.c \
	$(touchgfx_path)/framework/source/platform/hal/nxp/mcu/LPC40xx/vendor/gpdma_17xx_40xx.c \
	$(touchgfx_path)/framework/source/platform/hal/nxp/mcu/LPC40xx/vendor/gpio_17xx_40xx.c \
	$(touchgfx_path)/framework/source/platform/hal/nxp/mcu/LPC40xx/vendor/gpioint_17xx_40xx.c \
	$(touchgfx_path)/framework/source/platform/hal/nxp/mcu/LPC40xx/vendor/i2c_17xx_40xx.c \
	$(touchgfx_path)/framework/source/platform/hal/nxp/mcu/LPC40xx/vendor/i2s_17xx_40xx.c \
	$(touchgfx_path)/framework/source/platform/hal/nxp/mcu/LPC40xx/vendor/iap.c \
	$(touchgfx_path)/framework/source/platform/hal/nxp/mcu/LPC40xx/vendor/iocon_17xx_40xx.c \
	$(touchgfx_path)/framework/source/platform/hal/nxp/mcu/LPC40xx/vendor/lcd_17xx_40xx.c \
	$(touchgfx_path)/framework/source/platform/hal/nxp/mcu/LPC40xx/vendor/pmu_17xx_40xx.c \
	$(touchgfx_path)/framework/source/platform/hal/nxp/mcu/LPC40xx/vendor/ring_buffer.c \
	$(touchgfx_path)/framework/source/platform/hal/nxp/mcu/LPC40xx/vendor/ritimer_17xx_40xx.c \
	$(touchgfx_path)/framework/source/platform/hal/nxp/mcu/LPC40xx/vendor/rtc_17xx_40xx.c \
	$(touchgfx_path)/framework/source/platform/hal/nxp/mcu/LPC40xx/vendor/sdc_17xx_40xx.c \
	$(touchgfx_path)/framework/source/platform/hal/nxp/mcu/LPC40xx/vendor/sdmmc_17xx_40xx.c \
	$(touchgfx_path)/framework/source/platform/hal/nxp/mcu/LPC40xx/vendor/spi_17xx_40xx.c \
	$(touchgfx_path)/framework/source/platform/hal/nxp/mcu/LPC40xx/vendor/ssp_17xx_40xx.c \
	$(touchgfx_path)/framework/source/platform/hal/nxp/mcu/LPC40xx/vendor/stopwatch_17xx_40xx.c \
	$(touchgfx_path)/framework/source/platform/hal/nxp/mcu/LPC40xx/vendor/sysctl_17xx_40xx.c \
	$(touchgfx_path)/framework/source/platform/hal/nxp/mcu/LPC40xx/vendor/sysinit.c \
	$(touchgfx_path)/framework/source/platform/hal/nxp/mcu/LPC40xx/vendor/sysinit_17xx_40xx.c \
	$(touchgfx_path)/framework/source/platform/hal/nxp/mcu/LPC40xx/vendor/timer_17xx_40xx.c \
	$(touchgfx_path)/framework/source/platform/hal/nxp/mcu/LPC40xx/vendor/uart_17xx_40xx.c \
	$(touchgfx_path)/framework/source/platform/hal/nxp/mcu/LPC40xx/vendor/wwdt_17xx_40xx.c

board_cpp_files := \
	$(touchgfx_path)/board/EmbeddedArtists/LPC4088DisplayModule/source/bsp/DMTouchController.cpp \
	$(touchgfx_path)/framework/source/platform/hal/nxp/mcu/LPC40xx/NXP4088HAL.cpp \
	$(touchgfx_path)/framework/source/platform/hal/nxp/mcu/LPC40xx/NXP4088DMA.cpp \
	$(touchgfx_path)/framework/source/platform/core/arm/cortex-m/CortexMMCUInstrumentation.cpp
