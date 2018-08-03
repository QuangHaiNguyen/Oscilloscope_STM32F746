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
	$(touchgfx_path)/board/ST/STM32469I-DISCO/source/vendor/STM32469I-Discovery/stm32469i_discovery.c \
	$(touchgfx_path)/board/ST/STM32469I-DISCO/source/vendor/STM32469I-Discovery/stm32469i_discovery_lcd.c \
	$(touchgfx_path)/board/ST/STM32469I-DISCO/source/vendor/STM32469I-Discovery/stm32469i_discovery_qspi.c \
	$(touchgfx_path)/board/ST/STM32469I-DISCO/source/vendor/STM32469I-Discovery/stm32469i_discovery_sdram.c \
	$(touchgfx_path)/board/ST/STM32469I-DISCO/source/vendor/STM32469I-Discovery/stm32469i_discovery_ts.c \
	$(touchgfx_path)/board/ST/STM32469I-DISCO/source/vendor/Components/ft6x06/ft6x06.c \
	$(touchgfx_path)/board/ST/STM32469I-DISCO/source/vendor/Components/otm8009a/otm8009a.c

board_c_files += \
	$(touchgfx_path)/framework/source/platform/hal/ST/mcu/stm32f4cube/STM32F4xx_HAL_Driver/stm32f4xx_hal.c \
	$(touchgfx_path)/framework/source/platform/hal/ST/mcu/stm32f4cube/STM32F4xx_HAL_Driver/stm32f4xx_hal_cortex.c \
	$(touchgfx_path)/framework/source/platform/hal/ST/mcu/stm32f4cube/STM32F4xx_HAL_Driver/stm32f4xx_hal_dma.c \
	$(touchgfx_path)/framework/source/platform/hal/ST/mcu/stm32f4cube/STM32F4xx_HAL_Driver/stm32f4xx_hal_dma2d.c \
	$(touchgfx_path)/framework/source/platform/hal/ST/mcu/stm32f4cube/STM32F4xx_HAL_Driver/stm32f4xx_hal_dma_ex.c \
	$(touchgfx_path)/framework/source/platform/hal/ST/mcu/stm32f4cube/STM32F4xx_HAL_Driver/stm32f4xx_hal_gpio.c \
	$(touchgfx_path)/framework/source/platform/hal/ST/mcu/stm32f4cube/STM32F4xx_HAL_Driver/stm32f4xx_hal_i2c.c \
	$(touchgfx_path)/framework/source/platform/hal/ST/mcu/stm32f4cube/STM32F4xx_HAL_Driver/stm32f4xx_hal_ltdc.c \
	$(touchgfx_path)/framework/source/platform/hal/ST/mcu/stm32f4cube/STM32F4xx_HAL_Driver/stm32f4xx_hal_ltdc_ex.c \
	$(touchgfx_path)/framework/source/platform/hal/ST/mcu/stm32f4cube/STM32F4xx_HAL_Driver/stm32f4xx_hal_qspi.c \
	$(touchgfx_path)/framework/source/platform/hal/ST/mcu/stm32f4cube/STM32F4xx_HAL_Driver/stm32f4xx_hal_dsi.c \
	$(touchgfx_path)/framework/source/platform/hal/ST/mcu/stm32f4cube/STM32F4xx_HAL_Driver/stm32f4xx_hal_pwr_ex.c \
	$(touchgfx_path)/framework/source/platform/hal/ST/mcu/stm32f4cube/STM32F4xx_HAL_Driver/stm32f4xx_hal_rcc.c \
	$(touchgfx_path)/framework/source/platform/hal/ST/mcu/stm32f4cube/STM32F4xx_HAL_Driver/stm32f4xx_hal_rcc_ex.c \
	$(touchgfx_path)/framework/source/platform/hal/ST/mcu/stm32f4cube/STM32F4xx_HAL_Driver/stm32f4xx_hal_sdram.c \
	$(touchgfx_path)/framework/source/platform/hal/ST/mcu/stm32f4cube/STM32F4xx_HAL_Driver/stm32f4xx_ll_fmc.c \
	$(touchgfx_path)/framework/source/platform/hal/ST/mcu/stm32f4cube/STM32F4xx_HAL_Driver/stm32f4xx_ll_fsmc.c \
	$(touchgfx_path)/framework/source/platform/hal/ST/mcu/stm32f4cube/CMSIS/system_stm32f4xx.c

board_cpp_files := \
	$(touchgfx_path)/board/ST/STM32469I-DISCO/source/bsp/OTM8009TouchController.cpp 


board_cpp_files += \
	$(touchgfx_path)/framework/source/platform/hal/ST/mcu/stm32f4cube/STM32F4DMA.cpp \
	$(touchgfx_path)/framework/source/platform/hal/ST/mcu/stm32f4cube/STM32F4HAL_DSI.cpp \
	$(touchgfx_path)/framework/source/platform/core/arm/cortex-m/CortexMMCUInstrumentation.cpp 
