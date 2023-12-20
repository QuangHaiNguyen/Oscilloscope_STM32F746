/*****************************************************************************
 *
 *   Copyright(C) 2011, Embedded Artists AB
 *   All rights reserved.
 *
 ******************************************************************************
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * products. This software is supplied "AS IS" without any warranties.
 * Embedded Artists AB assumes no responsibility or liability for the
 * use of the software, conveys no license or title under any patent,
 * copyright, or mask work right to the product. Embedded Artists AB
 * reserves the right to make changes in the software without
 * notification. Embedded Artists AB also make no representation or
 * warranty that such application will be suitable for the specified
 * use without further testing or modification.
 *****************************************************************************/
#ifndef __MEMREG_H
#define __MEMREG_H

#ifdef __cplusplus
extern "C" {
#endif

#define MEMREG_BASE   0x1e000000


extern uint32_t memreg_init (void);

#ifdef __cplusplus
}
#endif

#endif /* end __MEMREG_H */
/****************************************************************************
**                            End Of File
*****************************************************************************/
