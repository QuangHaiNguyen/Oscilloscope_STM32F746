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
# Defines the assembler binary and options. These are optional and only
# of relevance if the component includes source files with an
# extension of .asm.

#no_libs := -nostdlib -nodefaultlibs For now, include everything.

assembler         := arm-none-eabi-gcc 
assembler_options += -g  \
                    -nostartfiles -fno-exceptions\
                    $(no_libs) -mthumb -mno-thumb-interwork  \
                     -Wall
assembler_options += $(float_options)

c_compiler         := arm-none-eabi-gcc
c_compiler_options += -g \
                    -nostartfiles -mthumb -fno-exceptions \
                    -mno-thumb-interwork -std=c99 \
					$(no_libs) \
                    -Os -fno-strict-aliasing -fdata-sections -ffunction-sections
#TODO removed -Wall

c_compiler_options += $(float_options)

cpp_compiler         := arm-none-eabi-g++
cpp_compiler_options += -g -mthumb \
                    -nostartfiles $(no_libs) \
                    -mno-thumb-interwork -fno-rtti -fno-exceptions  \
                    -Os -fno-strict-aliasing -fdata-sections -ffunction-sections

#TODO removed -Wall

cpp_compiler_options += $(float_options)

linker         := arm-none-eabi-g++
linker_options += -g -Wl,-static -nostartfiles  -mthumb $(no_libs) -mno-thumb-interwork \
                  -fno-exceptions -fno-rtti \
                  -Os -fno-strict-aliasing -Wl,--gc-sections

objcopy := arm-none-eabi-objcopy

archiver := arm-none-eabi-ar

strip := arm-none-eabi-strip
