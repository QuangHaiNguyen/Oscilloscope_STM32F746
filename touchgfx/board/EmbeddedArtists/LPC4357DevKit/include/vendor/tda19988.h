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
#ifndef __TDA19988_H
#define __TDA19988_H

#include "lpc_types.h"

typedef enum
{
  RES_VGA = 0,
  RES_480P60 = 1,
  RES_576P50 = 7,  
  RES_SVGA = 0xfe,
  RES_OTHER = 0xff,
} tda19988_res;

Status tda19988_init(void);
Status tda19988_setRes(tda19988_res res);

#endif /* end __TDA19988_H */
/****************************************************************************
**                            End Of File
*****************************************************************************/
