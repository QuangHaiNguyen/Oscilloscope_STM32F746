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
#ifndef __BOARD_H
#define __BOARD_H

#ifdef __cplusplus
extern "C" {
#endif
  
#define ADC_CH_TRIMPOT (ADC_CHANNEL_3)

typedef enum
{
  WANTED_FREQ_DEFAULT = 6,
  WANTED_FREQ_72_MHZ  = 6,
  WANTED_FREQ_180_MHZ = 15,
  WANTED_FREQ_204_MHZ = 17,
} wanted_freq_t;
void improved_cgu_init(wanted_freq_t freq);

void console_init(void);
uint32_t console_send(uint8_t *txbuf, uint32_t buflen, TRANSFER_BLOCK_Type flag);
uint32_t console_sendString(uint8_t *str);
uint32_t console_receive(uint8_t *rxbuf, uint32_t buflen, TRANSFER_BLOCK_Type flag);

void trimpot_init(void);

#ifdef __cplusplus
}
#endif

#endif /* end __BOARD_H */
/****************************************************************************
**                            End Of File
*****************************************************************************/
