/*-------------------------------------------------------------------------*
 * File:  Config.h
 *-------------------------------------------------------------------------*
 * Description:
 *     Configuration of all accepted modes of operation
 *-------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------
 * uEZ(R) - Copyright (C) 2007-2010 Future Designs, Inc.
 *--------------------------------------------------------------------------
 * This file is part of the uEZ(R) distribution.  See the included
 * uEZLicense.txt or visit http://www.teamfdi.com/uez for details.
 *
 *    *===============================================================*
 *    |  Future Designs, Inc. can port uEZ(tm) to your own hardware!  |
 *    |             We can get you up and running fast!               |
 *    |      See http://www.teamfdi.com/uez for more details.         |
 *    *===============================================================*
 *
 *-------------------------------------------------------------------------*/

#ifndef __CONFIG_H
#define __CONFIG_H

#define UEZ_VERSION_MAJOR           2
#define UEZ_VERSION_MINOR           00
#define UEZ_VERSION_STRING          "2.00"
#define UEZ_VERSION_DATE            "3/8/2012" // MM/DD/YYYY

/*-------------------------------------------------------------------------*
 * Option: Compiler
 *-------------------------------------------------------------------------*/
// List of Compilers
#define RowleyARM                   1
#define IAR                         2
#define CodeRed                     3
#define HEW                                                     4
#define Keil4                                           5
#define RenesasRX                   6
#define VisualC                     7

// List of RTOS's
#define FreeRTOS                    1
#define SafeRTOS                    2

// Include any specific compile options for this build
#include <Config_Build.h>

/*-------------------------------------------------------------------------*
 * Option: RTOS
 *-------------------------------------------------------------------------*/
// Select RTOS:
#ifndef RTOS
#define RTOS                        FreeRTOS
#endif

/* The RTOS definition is needed in Config_Build and Config_App and hence must
be declared here. */
#ifndef RTOS
#error "An RTOS must be defined."
#endif

#ifdef __CODE_RED
#define COMPILER_TYPE CodeRed
#endif

// Select compiler
#ifndef COMPILER_TYPE
#define COMPILER_TYPE               RowleyARM
#endif

// Get the application's configuration and requirements for the
// platform and processor.
#ifndef UEZ_LIBRARY
#include <Config_App.h>
#endif

#ifndef TAG_BUILD_DATE
//#define TAG_BUILD_DATE          __DATE__
#define TAG_BUILD_DATE          UEZ_VERSION_DATE
#endif
#ifndef RELEASE_DATE
//#define RELEASE_DATE            __DATE__
#define RELEASE_DATE            UEZ_VERSION_DATE
#endif

/*-------------------------------------------------------------------------*
 * Option: Processor
 *-------------------------------------------------------------------------*/
// List of processor types
#define NXP_LPC2478                             1
#define NXP_LPC1768                             2
#define RENESAS_H8SX_1668RF                         3
#define NXP_LPC1788                             4
#define RENESAS_RX62N                           5
#define STMICRO_STM32F105_7                     6

// Select processor:
//#ifndef UEZ_PROCESSOR
//#define UEZ_PROCESSOR                       NXP_LPC2478
//#endif

/*-------------------------------------------------------------------------*
 * Option: uEZ settings
 *-------------------------------------------------------------------------*/
#ifndef UEZ_NUM_HANDLES
#define UEZ_NUM_HANDLES           150
#endif

#endif // __CONFIG_H
/*-------------------------------------------------------------------------*
 * End of File:  Config.h
 *-------------------------------------------------------------------------*/
