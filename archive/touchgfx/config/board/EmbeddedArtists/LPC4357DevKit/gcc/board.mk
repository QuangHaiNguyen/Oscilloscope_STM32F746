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
	$(touchgfx_path)/board/EmbeddedArtists/LPC4357DevKit/source/bsp/LEDs_init.c \
	$(touchgfx_path)/board/EmbeddedArtists/LPC4357DevKit/source/vendor/board.c \
	$(touchgfx_path)/board/EmbeddedArtists/LPC4357DevKit/source/vendor/calibrate.c \
	$(touchgfx_path)/board/EmbeddedArtists/LPC4357DevKit/source/vendor/ea_lcd_board.c \
	$(touchgfx_path)/board/EmbeddedArtists/LPC4357DevKit/source/vendor/lcd_driver.c \
	$(touchgfx_path)/board/EmbeddedArtists/LPC4357DevKit/source/vendor/lcdb_eeprom.c \
	$(touchgfx_path)/board/EmbeddedArtists/LPC4357DevKit/source/vendor/norflash.c \
	$(touchgfx_path)/board/EmbeddedArtists/LPC4357DevKit/source/vendor/pca9532.c \
	$(touchgfx_path)/board/EmbeddedArtists/LPC4357DevKit/source/vendor/fpu_init.c \
	$(touchgfx_path)/board/EmbeddedArtists/LPC4357DevKit/source/vendor/sdram.c \
	$(touchgfx_path)/board/EmbeddedArtists/LPC4357DevKit/source/vendor/system_LPC43xx.c \
	$(touchgfx_path)/board/EmbeddedArtists/LPC4357DevKit/source/vendor/truly_g240320ltsw.c \
	$(touchgfx_path)/board/EmbeddedArtists/LPC4357DevKit/source/vendor/tsc2046_touch.c \
	$(touchgfx_path)/framework/source/platform/hal/nxp/mcu/LPC43xx/vendor/lpc43xx_adc.c \
	$(touchgfx_path)/framework/source/platform/hal/nxp/mcu/LPC43xx/vendor/lpc43xx_cgu.c \
	$(touchgfx_path)/framework/source/platform/hal/nxp/mcu/LPC43xx/vendor/lpc43xx_emc.c \
	$(touchgfx_path)/framework/source/platform/hal/nxp/mcu/LPC43xx/vendor/lpc43xx_gpdma.c \
	$(touchgfx_path)/framework/source/platform/hal/nxp/mcu/LPC43xx/vendor/lpc43xx_gpio.c \
	$(touchgfx_path)/framework/source/platform/hal/nxp/mcu/LPC43xx/vendor/lpc43xx_i2c.c \
	$(touchgfx_path)/framework/source/platform/hal/nxp/mcu/LPC43xx/vendor/lpc43xx_lcd.c \
	$(touchgfx_path)/framework/source/platform/hal/nxp/mcu/LPC43xx/vendor/lpc43xx_rgu.c \
	$(touchgfx_path)/framework/source/platform/hal/nxp/mcu/LPC43xx/vendor/lpc43xx_scu.c \
	$(touchgfx_path)/framework/source/platform/hal/nxp/mcu/LPC43xx/vendor/lpc43xx_ssp.c \
	$(touchgfx_path)/framework/source/platform/hal/nxp/mcu/LPC43xx/vendor/lpc43xx_timer.c \
	$(touchgfx_path)/framework/source/platform/hal/nxp/mcu/LPC43xx/vendor/lpc43xx_uart.c \
	$(touchgfx_path)/framework/source/platform/hal/nxp/lpcspifilib/src/spifilib_dev_common.c \
	$(touchgfx_path)/framework/source/platform/hal/nxp/lpcspifilib/src/spifilib_fam_mac_2b_stat.c \
	$(touchgfx_path)/framework/source/platform/hal/nxp/lpcspifilib/src/spifilib_fam_spa_2b_pstat.c \
	$(touchgfx_path)/framework/source/platform/hal/nxp/lpcspifilib/src/spifilib_fam_spa_3b_stat.c 

board_cpp_files := \
	$(touchgfx_path)/board/EmbeddedArtists/LPC4357DevKit/source/bsp/TSC2046TouchController.cpp \
	$(touchgfx_path)/framework/source/platform/hal/nxp/mcu/LPC43xx/NXP43XXHAL.cpp \
	$(touchgfx_path)/framework/source/platform/hal/nxp/mcu/LPC43xx/NXP43XXDMA.cpp \
	$(touchgfx_path)/framework/source/platform/core/arm/cortex-m/CortexMMCUInstrumentation.cpp 

