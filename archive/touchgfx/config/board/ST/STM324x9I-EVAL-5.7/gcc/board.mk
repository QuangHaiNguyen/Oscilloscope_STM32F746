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
	$(touchgfx_path)/board/ST/STM324x9I-EVAL-5.7/source/vendor/stm324x9i_eval.c \
	$(touchgfx_path)/board/ST/STM324x9I-EVAL-5.7/source/vendor/stm324x9i_eval_fmc_nor.c \
	$(touchgfx_path)/board/ST/STM324x9I-EVAL-5.7/source/vendor/stm324x9i_eval_fmc_sdram.c \
	$(touchgfx_path)/board/ST/STM324x9I-EVAL-5.7/source/vendor/stm324x9i_eval_fmc_sram.c \
	$(touchgfx_path)/board/ST/STM324x9I-EVAL-5.7/source/vendor/stm324x9i_eval_ioe8.c \
	$(touchgfx_path)/board/ST/STM324x9I-EVAL-5.7/source/vendor/stm324x9i_eval_ioe16.c \
	$(touchgfx_path)/board/ST/STM324x9I-EVAL-5.7/source/vendor/stm324x9i_eval_lcd.c \
	$(touchgfx_path)/board/ST/STM324x9I-EVAL-5.7/source/vendor/stm324x9i_eval_sdio_sd.c 

board_c_files += \
	$(touchgfx_path)/framework/source/platform/hal/ST/mcu/stm32f4x9/vendor/misc.c \
	$(touchgfx_path)/framework/source/platform/hal/ST/mcu/stm32f4x9/vendor/stm32f4xx_adc.c \
	$(touchgfx_path)/framework/source/platform/hal/ST/mcu/stm32f4x9/vendor/stm32f4xx_can.c \
	$(touchgfx_path)/framework/source/platform/hal/ST/mcu/stm32f4x9/vendor/stm32f4xx_crc.c \
	$(touchgfx_path)/framework/source/platform/hal/ST/mcu/stm32f4x9/vendor/stm32f4xx_cryp.c \
	$(touchgfx_path)/framework/source/platform/hal/ST/mcu/stm32f4x9/vendor/stm32f4xx_cryp_aes.c \
	$(touchgfx_path)/framework/source/platform/hal/ST/mcu/stm32f4x9/vendor/stm32f4xx_cryp_des.c \
	$(touchgfx_path)/framework/source/platform/hal/ST/mcu/stm32f4x9/vendor/stm32f4xx_cryp_tdes.c \
	$(touchgfx_path)/framework/source/platform/hal/ST/mcu/stm32f4x9/vendor/stm32f4xx_dac.c \
	$(touchgfx_path)/framework/source/platform/hal/ST/mcu/stm32f4x9/vendor/stm32f4xx_dbgmcu.c \
	$(touchgfx_path)/framework/source/platform/hal/ST/mcu/stm32f4x9/vendor/stm32f4xx_dcmi.c \
	$(touchgfx_path)/framework/source/platform/hal/ST/mcu/stm32f4x9/vendor/stm32f4xx_dma.c \
	$(touchgfx_path)/framework/source/platform/hal/ST/mcu/stm32f4x9/vendor/stm32f4xx_dma2d.c \
	$(touchgfx_path)/framework/source/platform/hal/ST/mcu/stm32f4x9/vendor/stm32f4xx_exti.c \
	$(touchgfx_path)/framework/source/platform/hal/ST/mcu/stm32f4x9/vendor/stm32f4xx_flash.c \
	$(touchgfx_path)/framework/source/platform/hal/ST/mcu/stm32f4x9/vendor/stm32f4xx_fmc.c \
	$(touchgfx_path)/framework/source/platform/hal/ST/mcu/stm32f4x9/vendor/stm32f4xx_gpio.c \
	$(touchgfx_path)/framework/source/platform/hal/ST/mcu/stm32f4x9/vendor/stm32f4xx_hash.c \
	$(touchgfx_path)/framework/source/platform/hal/ST/mcu/stm32f4x9/vendor/stm32f4xx_hash_md5.c \
	$(touchgfx_path)/framework/source/platform/hal/ST/mcu/stm32f4x9/vendor/stm32f4xx_hash_sha1.c \
	$(touchgfx_path)/framework/source/platform/hal/ST/mcu/stm32f4x9/vendor/stm32f4xx_i2c.c \
	$(touchgfx_path)/framework/source/platform/hal/ST/mcu/stm32f4x9/vendor/stm32f4xx_iwdg.c \
	$(touchgfx_path)/framework/source/platform/hal/ST/mcu/stm32f4x9/vendor/stm32f4xx_ltdc.c \
	$(touchgfx_path)/framework/source/platform/hal/ST/mcu/stm32f4x9/vendor/stm32f4xx_pwr.c \
	$(touchgfx_path)/framework/source/platform/hal/ST/mcu/stm32f4x9/vendor/stm32f4xx_rcc.c \
	$(touchgfx_path)/framework/source/platform/hal/ST/mcu/stm32f4x9/vendor/stm32f4xx_rng.c \
	$(touchgfx_path)/framework/source/platform/hal/ST/mcu/stm32f4x9/vendor/stm32f4xx_rtc.c \
	$(touchgfx_path)/framework/source/platform/hal/ST/mcu/stm32f4x9/vendor/stm32f4xx_sai.c \
	$(touchgfx_path)/framework/source/platform/hal/ST/mcu/stm32f4x9/vendor/stm32f4xx_sdio.c \
	$(touchgfx_path)/framework/source/platform/hal/ST/mcu/stm32f4x9/vendor/stm32f4xx_spi.c \
	$(touchgfx_path)/framework/source/platform/hal/ST/mcu/stm32f4x9/vendor/stm32f4xx_syscfg.c \
	$(touchgfx_path)/framework/source/platform/hal/ST/mcu/stm32f4x9/vendor/stm32f4xx_tim.c \
	$(touchgfx_path)/framework/source/platform/hal/ST/mcu/stm32f4x9/vendor/stm32f4xx_usart.c \
	$(touchgfx_path)/framework/source/platform/hal/ST/mcu/stm32f4x9/vendor/stm32f4xx_wwdg.c \
	$(touchgfx_path)/framework/source/platform/hal/ST/mcu/stm32f4x9/vendor/system_stm32f4xx.c 

board_cpp_files := \
	$(touchgfx_path)/board/ST/STM324x9I-EVAL-5.7/source/bsp/TS3510TouchController.cpp 

board_cpp_files += \
	$(touchgfx_path)/framework/source/platform/hal/ST/mcu/stm32f4x9/STM32F4DMA.cpp \
	$(touchgfx_path)/framework/source/platform/hal/ST/mcu/stm32f4x9/STM32F4HAL.cpp \
	$(touchgfx_path)/framework/source/platform/core/arm/cortex-m/CortexMMCUInstrumentation.cpp 
