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
assembler               := as
assembler_options       +=

# Defines the C compiler binary and options. These variables shall
# be defined even if the component does not include .c files. The
# variables are potentially used by plug-ins
c_compiler              := g++
c_compiler_options      += -DSIMULATOR='' -g

# Defines the C++ compiler binary and options. These variables shall
# be defined even if the component does not include .cpp files. The
# variables are potentially used by plug-ins
cpp_compiler            := g++
cpp_compiler_options    += -g -DSIMULATOR='' -DENABLE_LOG

# Defines the linker binary and options. These variables shall
# be defined even if the component does not include a main.c or
# main.cpp file. The variables are potentially used by plug-ins.
linker                  := g++
linker_options          += -static-libgcc -static-libstdc++

# Defines the archiver binary. The archiver creates a static library
# containing the component object files, excluding the optional
# main.c(pp). This variable shall always be defined.
archiver                := ar

# Defined the strip binary used for removing debug information from
# object files, libraries and executables
strip                   := strip
