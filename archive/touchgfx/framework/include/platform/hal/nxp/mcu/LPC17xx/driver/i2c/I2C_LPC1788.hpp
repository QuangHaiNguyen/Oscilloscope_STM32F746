/******************************************************************************
 *
 * @brief     This file is part of the TouchGFX 4.7.0 evaluation distribution.
 *
 * @author    Draupner Graphics A/S <http://www.touchgfx.com>
 *
 ******************************************************************************
 *
 * @section Copyright
 *
 * Copyright (C) 2014-2016 Draupner Graphics A/S <http://www.touchgfx.com>.
 * All rights reserved.
 *
 * TouchGFX is protected by international copyright laws and the knowledge of
 * this source code may not be used to write a similar product. This file may
 * only be used in accordance with a license and should not be re-
 * distributed in any way without the prior permission of Draupner Graphics.
 *
 * This is licensed software for evaluation use, any use must strictly comply
 * with the evaluation license agreement provided with delivery of the
 * TouchGFX software.
 *
 * The evaluation license agreement can be seen on www.touchgfx.com
 *
 * @section Disclaimer
 *
 * DISCLAIMER OF WARRANTY/LIMITATION OF REMEDIES: Draupner Graphics A/S has
 * no obligation to support this software. Draupner Graphics A/S is providing
 * the software "AS IS", with no express or implied warranties of any kind,
 * including, but not limited to, any implied warranties of merchantability
 * or fitness for any particular purpose or warranties against infringement
 * of any proprietary rights of a third party.
 *
 * Draupner Graphics A/S can not be held liable for any consequential,
 * incidental, or special damages, or any other relief, or for any claim by
 * any third party, arising from your use of this software.
 *
 *****************************************************************************/
#ifndef I2C_LPC1788_HPP
#define I2C_LPC1788_HPP

#include <platform/driver/i2c/i2c.hpp>
#include <LPC1788.h>

#define I2C_TIMEOUT       212500
#define I2C_CLOCKRATE     400000

#define WRITE      0                    // Master write
#define READ       1                    // Master read

/*
CONCLR REGISTER
*/

#define I2C_CONCLR_AAC    0x04         // Clear Assert ACK
#define I2C_CONCLR_SIC    0x08         // Clear I2C Interrupt
#define I2C_CONCLR_STAC   0x20         // Clear START */
#define I2C_CONCLR_I2ENC  0x40         // Clear I2C Enable (=Disable)
#define WIPE_I2C_CONCLR  (I2C_CONCLR_I2ENC|I2C_CONCLR_STAC|I2C_CONCLR_SIC|I2C_CONCLR_AAC)

/*
CONSET REGISTER
*/

#define I2C_CONSET_AA     0x04     // Set Assert ACK
#define I2C_CONSET_SI     0x08     // Set I2C interrupt
#define I2C_CONSET_STO    0x10     // Set STOP
#define I2C_CONSET_STA    0x20     // Set START
#define I2C_CONSET_I2EN   0x40     // Set I2C ENABLE

/*
STAT REGISTER
*/

#define I2C_STAT_ST       0x08          // START transmitted
#define I2C_STAT_RPTST    0x10          // Repeated START transmitted
#define I2C_STAT_SLAWA    0x18          // SLA+W transmitted + ACKed
#define I2C_STAT_SLAWNA   0x20          // SLA+W transmitted - ACK
#define I2C_STAT_DTA      0x28          // Data transmitted, ACK received
#define I2C_STAT_DTNA     0x30          // Data transmitted, no ACK
#define I2C_STAT_ALOST    0x38          // Arbitration lost
#define I2C_STAT_SLARA    0x40          // STAT:SLA+R transmitted and ACKed
#define I2C_STAT_SLARNA   0x48          // STAT:SLA+R transmitted, no ACK
#define I2C_STAT_DRA      0x50          // Data received, ack returned
#define I2C_STAT_DRNA     0x58          // Data received, not ack returned

/*
STATES
*/

/*
#define SEND_INITIAL_START_CONDITION    0
#define SET_DEVICE_ADDRESS_WRITE        1
#define WRITE_SLAVE_ADDRESS             2
#define SEND_START_CONDITION            3
#define SET_DEVICE_ADDRESS_READ         4
#define READ_DATA                       5
#define SEND_STOP_CONDITION             6
#define FINISHED                        7
*/

typedef enum
{
    SEND_INITIAL_START_CONDITION = 0,
    SET_DEVICE_ADDRESS_WRITE,
    WRITE_SLAVE_ADDRESS,
    SEND_START_CONDITION,
    SET_DEVICE_ADDRESS_READ,
    READ_DATA,
    WRITE_DATA,
    SEND_STOP_CONDITION,
    FINISHED
} STATES;


namespace touchgfx
{
/**
 * NXP LPC1788 MCU specific I2C driver.
 */
class I2C_LPC1788 : public I2C
{
public:
    /**
     * Constructor. Calls super constructor with channel parameter.
     * @param ch I2C channel
     */
    I2C_LPC1788(uint8_t ch) : I2C(ch)
    {}

    /**
     * Initializes the specified channel of the I2C bus Sets up SDA and SDL and intialize I2C peripheral.
     */
    void init();

    /**
     * Reads the specificed register on the device with the specified address.
     * @param addr The 7-bit I2C device address
     * @param reg The register
     * @param data Pointer to buffer in which to place the result
     * @param cnt Size of buffer in bytes
     * @return true on success, false otherwise.
     */
    bool readRegister(uint8_t addr, uint8_t reg, uint8_t* data, uint32_t cnt);

    /**
     * Writes the specified value in a register.
     * @param addr The I2C device address
     * @param reg The register
     * @param val The new value
     * @return true on success, false otherwise.
     */
    bool writeRegister(uint8_t addr, uint8_t reg, uint8_t val);

private:
    /**
     * Attempts to reset the bus by setting a STOP condition and calling (init)
     * @return true if reset succeeded, false otherwise
     */
    bool resetBus();

    /**
     * Sets a START condition on the I2C bus
     * @return True if succeeded, False otherwise
     */
    uint32_t sendStartCondition();

    /**
     * Sets a STOP condition on the I2C bus
     * @return True if succeeded, False otherwise
     */
    uint32_t sendStopCondition();

    /**
     * Sets an address in the I2C databuffer
     * The left seven bits of a control byte indicate the destination address (devices).
     * The rightmost bits of the control byte indicates the data transfer is write (sent) or read (receive).
     * @param address Device addresss
     * @param direction Direction bit
     * @return 0 if succeeded, 1 otherwise
    */
    uint32_t setAddress(uint8_t address, uint8_t direction);

    /**
     * Writes a single byte to the databuffer and waits for transmission + ACK
     * @param byte byte to write
     * @return 0 if succeeded, 1 otherwise
     */
    uint32_t writeByte(uint8_t byte);

    /**
    * Reads a byte from the databuffer.
    * @param byte Pointer to start of data buffer (ReadByte might get called multiple times).
    * @return 0 if succeeded, 1 otherwise
    */
    uint32_t readByte(uint32_t ack, uint8_t* byte);

    /**
     * Waits for a specified amount of time for a status code to become valid
     * @return 0 if statusCode was detected, 1 otherwise
     */
    uint32_t statusTimeout(uint32_t statusCode);

    /**
     * Waits for a specified amount of time for a START or repeated START condition to become valid. In a combined message, each read or write begins with a START and the slave address.
       After the first START in a combined message these are also called repeated START bits. Repeated START bits are not preceded by STOP bits, which is how slaves know the next transfer is part of the same message.
     * @return 0 if either START condition was detected, 1 otherwise
     */
    uint32_t startConditionTimeout();

    bool processStates(uint8_t deviceAddress, uint8_t slaveAddress, uint8_t* buf, uint32_t cnt, uint8_t* states);

    /**
     * LPC1788 I2C channel handle definition
     */
    LPC_I2C_TypeDef* chHndl;
};
}

#endif // I2C_LPC1788_HPP
