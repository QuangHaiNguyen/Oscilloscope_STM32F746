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
#ifndef __OEM_EEPROM_H
#define __OEM_EEPROM_H

/* device ID for the 2 kBit EEPROM with EUI-48 */
#define OEM_EEPROM_2K (0)

/* device size is 256 bytes, but the upper 128 bytes are read-only.
 * as they contain an unique node id */
#define OEM_EEPROM0_TOTAL_SIZE  128

void oem_eeprom_init (void);
int16_t oem_eeprom_read(uint8_t dev, uint8_t* buf, uint16_t offset, uint16_t len);
int16_t oem_eeprom_write(uint8_t dev, uint8_t* buf, uint16_t offset, uint16_t len);

/* Read the write-protected globally unique node identifier (EUI-48) which is 6 bytes */
int16_t oem_eeprom_readUniqueID(uint8_t dev, uint8_t* buf);


#endif /* end __OEM_EEPROM_H */
/****************************************************************************
**                            End Of File
*****************************************************************************/
