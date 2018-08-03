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
# Will define the necessary compiler options for this specific board.
# Should expose the following vars:
# board_include_paths : Include paths relevant for this board (both C and C++)

board_include_paths := \
	$(touchgfx_path)/framework/include/platform/core/arm/cortex-m/CMSIS \
	$(touchgfx_path)/framework/include/platform/hal/ST/mcu/stm32f4cube \
	$(touchgfx_path)/framework/include/platform/hal/ST/mcu/stm32f4cube/STM32F4xx_HAL_Driver \
	$(touchgfx_path)/framework/include/platform/hal/ST/mcu/stm32f4cube/CMSIS \
	$(touchgfx_path)/board/ST/STM32469I-EVAL/include \
	$(touchgfx_path)/board/ST/STM32469I-EVAL/include/bsp \
	$(touchgfx_path)/board/ST/STM32469I-EVAL/source/vendor/STM32469I_EVAL

c_compiler_options += -DST -DSTM32F469xx -DUSE_OS_SYSTICK -DUSE_FLOATING_POINT -DUSE_STM32469I_EVAL -DUSE_IOEXPANDER
cpp_compiler_options +=  -DST -DSTM32F469xx -DUSE_OS_SYSTICK -DUSE_FLOATING_POINT -DUSE_STM32469I_EVAL -DUSE_IOEXPANDER

