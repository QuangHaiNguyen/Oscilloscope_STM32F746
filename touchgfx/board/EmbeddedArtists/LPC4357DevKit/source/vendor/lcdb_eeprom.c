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

/*
 * NOTE: I2C must have been initialized before calling any functions in this
 * file.
 */

/******************************************************************************
 * Includes
 *****************************************************************************/

#include "lpc_types.h"
#include "lpc43xx_i2c.h"
#include "lpc43xx_timer.h"
#include "string.h"
#include "stdio.h"
#include "lcdb_eeprom.h"

/******************************************************************************
 * Defines and typedefs
 *****************************************************************************/

#define I2C_PORT (LPC_I2C0)

#ifndef MIN
#define MIN(x, y) ((x) < (y) ? (x) : (y))
#endif

#define LCDB_EEPROM_I2C_ADDR  (0x56)

#define LCDB_EEPROM_PAGE_SIZE     32
#define LCDB_EEPROM_TOTAL_SIZE  8192


/******************************************************************************
 * External global variables
 *****************************************************************************/

/******************************************************************************
 * Local variables
 *****************************************************************************/


/******************************************************************************
 * Local Functions
 *****************************************************************************/

static void eepromDelay(void)
{
  volatile int i = 0;
  for (i = 0; i <0x20000; i++);
}

static Status I2CWrite(uint32_t addr, uint8_t* buf, uint32_t len) 
{
  I2C_M_SETUP_Type i2cData;

        i2cData.sl_addr7bit = addr;
        i2cData.tx_data = buf;
        i2cData.tx_length = len;
        i2cData.rx_data = (uint8_t*)NULL;
        i2cData.rx_length = 0;
        i2cData.retransmissions_max = 3;

  return I2C_MasterTransferData(I2C_PORT, &i2cData, I2C_TRANSFER_POLLING);
}

static Status I2CRead(uint32_t addr, uint8_t* buf, uint32_t len) 
{
  I2C_M_SETUP_Type i2cData;

        i2cData.sl_addr7bit = addr;
        i2cData.tx_data = (uint8_t*)NULL;
        i2cData.tx_length = 0;
        i2cData.rx_data = buf;
        i2cData.rx_length = len;
        i2cData.retransmissions_max = 3;

  return I2C_MasterTransferData(I2C_PORT, &i2cData, I2C_TRANSFER_POLLING);
}


/******************************************************************************
 * Public Functions
 *****************************************************************************/

/******************************************************************************
 *
 * Description:
 *    Initialize the EEPROM Driver
 *
 *****************************************************************************/
void lcdb_eeprom_init (void)
{
  I2C_Cmd(I2C_PORT, ENABLE);
}

/******************************************************************************
 *
 * Description:
 *    Read from the EEPROM
 *
 * Params:
 *   [in] buf - read buffer
 *   [in] offset - offset to start to read from
 *   [in] len - number of bytes to read
 *
 * Returns:
 *   number of read bytes or -1 in case of an error
 *
 *****************************************************************************/
int16_t lcdb_eeprom_read(uint8_t* buf, uint16_t offset, uint16_t len)
{
  int i = 0;
  uint8_t off[2];

  if (len > LCDB_EEPROM_TOTAL_SIZE || offset+len > LCDB_EEPROM_TOTAL_SIZE) {
    return -1;
  }

  off[0] = ((offset >> 8) & 0xff);
  off[1] = (offset & 0xff);

  // 10ms delay needed to prevent misreads from EEPROM
  TIM_Waitms(10);

  if (I2CWrite((LCDB_EEPROM_I2C_ADDR), off, 2) == SUCCESS)
  {
    for ( i = 0; i < 0x2000; i++);
    if (I2CRead((LCDB_EEPROM_I2C_ADDR), buf, len) == SUCCESS) {
      return len;
    }
  }

  return -1;
}

/******************************************************************************
 *
 * Description:
 *    Write to the EEPROM
 *
 * Params:
 *   [in] buf - data to write
 *   [in] offset - offset to start to write to
 *   [in] len - number of bytes to write
 *
 * Returns:
 *   number of written bytes or -1 in case of an error
 *
 *****************************************************************************/
int16_t lcdb_eeprom_write(uint8_t* buf, uint16_t offset, uint16_t len)
{
  int16_t written = 0;
  uint16_t wLen = 0;
  uint16_t off = offset;
  uint8_t tmp[LCDB_EEPROM_PAGE_SIZE+2];

  if (len > LCDB_EEPROM_TOTAL_SIZE || offset+len > LCDB_EEPROM_TOTAL_SIZE) {
    return -1;
  }

  wLen = LCDB_EEPROM_PAGE_SIZE - (off % LCDB_EEPROM_PAGE_SIZE);
  wLen = MIN(wLen, len);

  while (len) {
    tmp[0] = ((off >> 8) & 0xff);
    tmp[1] = (off & 0xff);
    memcpy(&tmp[2], (void*)&buf[written], wLen);
    I2CWrite((LCDB_EEPROM_I2C_ADDR), tmp, wLen+2);

    /* delay to wait for a write cycle */
    eepromDelay();

    len     -= wLen;
    written += wLen;
    off     += wLen;

    wLen = MIN(LCDB_EEPROM_PAGE_SIZE, len);
  }

  return written;
}
