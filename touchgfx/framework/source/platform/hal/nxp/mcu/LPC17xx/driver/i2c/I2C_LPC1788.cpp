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
#include "platform/hal/nxp/mcu/LPC17xx/driver/i2c/I2C_LPC1788.hpp"

extern "C" {
#include "lpc177x_8x_clkpwr.h"
#include "lpc177x_8x_i2c.h"
#include "lpc177x_8x_pinsel.h"
}

#ifndef NO_USING_NAMESPACE_TOUCHGFX
using namespace touchgfx;
#endif

uint32_t I2C_LPC1788::startConditionTimeout()
{
    uint32_t statRegister, tick = 0;

    //Within the timeout limit, check if Start condition has been received: If START or Repeated START has been transmitted.
    while (tick < I2C_TIMEOUT)
    {
        statRegister = chHndl->STAT;
        if (statRegister == I2C_STAT_ST || statRegister == I2C_STAT_RPTST)
        {
            return 0;
        }
        tick++;
    }
    return 1;
}

uint32_t I2C_LPC1788::statusTimeout(uint32_t statusCode)
{
    uint32_t tick = 0;
    while (tick < I2C_TIMEOUT)
    {
        if (chHndl->STAT == statusCode)
        {
            return 0;
        }
        tick++;
    }
    return 1;
}

void I2C_LPC1788::init()
{

    //Setup port 2, pin 14, I2C1_SDA (data i/o)
    PINSEL_ConfigPin(2, 14, 2);
    //Setup port 2, pin 15, I2C1_SCL (clock i/o)
    PINSEL_ConfigPin(2, 15, 2);
    //Save channel for bus recovery

    //Setup I2C peripheral and target clockrate - I2c_1 needs to come from outside according to interface. I2C_1
    I2C_Init(channel, I2C_CLOCKRATE);

    //Set I2C Channel handle
    switch (channel)
    {
    case I2C_0:
        chHndl = ((LPC_I2C_TypeDef*) LPC_I2C0_BASE);
        break;
    case I2C_1:
        chHndl = ((LPC_I2C_TypeDef*) LPC_I2C1_BASE);
        break;
    case I2C_2:
        chHndl = ((LPC_I2C_TypeDef*) LPC_I2C2_BASE);
        break;
    }

    //CLEAR everything
    chHndl->CONCLR = WIPE_I2C_CONCLR;
    //Enable I2C
    chHndl->CONSET = I2C_CONSET_I2EN;
}

bool I2C_LPC1788::resetBus()
{
    chHndl->CONSET = I2C_CONSET_STO;
    chHndl->CONCLR = I2C_CONCLR_STAC | I2C_CONSET_SI;
    init();
    return true;
}

uint32_t I2C_LPC1788::sendStartCondition(void)
{
    chHndl->CONCLR = I2C_CONCLR_SIC;
    chHndl->CONSET = I2C_CONSET_STA;
    return startConditionTimeout();
}

uint32_t I2C_LPC1788::sendStopCondition(void)
{
    chHndl->CONCLR = I2C_CONCLR_STAC;
    chHndl->CONSET = I2C_CONSET_STO;
    return 0;
}

uint32_t I2C_LPC1788::setAddress(uint8_t address, uint8_t rw_direction)
{
    //Insert 7-bit Slave Address + Direction bit into Databuffer register.
    chHndl->DAT = (address << 1) | rw_direction;
    chHndl->CONCLR = I2C_CONCLR_SIC;
    uint32_t stat = rw_direction ? I2C_STAT_SLARA : I2C_STAT_SLAWA;
    //Wait for ACK on Slave Address Read/Write (dir)
    return statusTimeout(stat);
}

uint32_t I2C_LPC1788::writeByte(uint8_t byte)
{
    //Load data into databuffer
    chHndl->DAT = byte;
    //Clear interrupt and start
    chHndl->CONCLR = I2C_CONCLR_SIC | I2C_CONCLR_STAC;
    //Data transmitted, ACK received
    return statusTimeout(I2C_STAT_DTA);
}

/* rename to readbyte */
uint32_t I2C_LPC1788::readByte(uint32_t ack, uint8_t* byte)
{
    uint32_t stat;

    //If acknowledge is set, Set ASSERT ACK
    if (ack)
    {
        stat = I2C_STAT_DRA;
        chHndl->CONSET = I2C_CONSET_AA;
    }
    // Clear ASSERT ACK otherwise
    else
    {
        stat = I2C_STAT_DRNA;
        chHndl->CONCLR = I2C_CONCLR_AAC;
    }

    //Clear Start and interrupt flags
    chHndl->CONCLR = I2C_CONCLR_STAC | I2C_CONCLR_SIC;

    //Get data from databuffer if status achieved
    if (!statusTimeout(stat))
    {
        *byte = chHndl->DAT;
        return 0;
    }
    return 1;
}

bool I2C_LPC1788::writeRegister(uint8_t deviceAddress, uint8_t slaveAddress, uint8_t buf)
{

    uint8_t writeSTATES[] =
    {
        SEND_START_CONDITION,
        SET_DEVICE_ADDRESS_WRITE,
        WRITE_SLAVE_ADDRESS,
        WRITE_DATA,
        SEND_STOP_CONDITION
    };

    return processStates(deviceAddress, slaveAddress, &buf, 1, writeSTATES);
}

bool I2C_LPC1788::processStates(uint8_t deviceAddress, uint8_t slaveAddress, uint8_t* buf, uint32_t cnt, uint8_t* states)
{
    uint8_t*  data = buf;
    uint32_t error = 0, step = 0;
    uint32_t bytesLeftToRead = cnt;

    do
    {
        switch (states[step++])
        {
        case SEND_START_CONDITION:
            error |= sendStartCondition();
            break;
        case SET_DEVICE_ADDRESS_WRITE:
            error |= setAddress(deviceAddress, WRITE);
            break;
        case WRITE_SLAVE_ADDRESS:
            error |= writeByte(slaveAddress);
            break;
        case SET_DEVICE_ADDRESS_READ:
            error |= setAddress(deviceAddress, READ);
            break;
        case READ_DATA:
            //For each byte, decide if ACK should be expected, and move the data pointer and read
            while (!error && bytesLeftToRead--)
            {
                error |= readByte((bytesLeftToRead != 0), data++);
            }
            break;
        case WRITE_DATA:
            error |= writeByte(*buf);
            break;
        case SEND_STOP_CONDITION:
            error |= sendStopCondition();
            break;
        }
        // If an error occured during the read process and we succeed in resetting the bus, try again
        if (error && resetBus())
        {
            data  = buf; //reset data pointer
            step = 0;
            error = 0;
        }
    }
    while (!error && step < sizeof(states) + 1);
    return error;
}

bool I2C_LPC1788::readRegister(uint8_t deviceAddress, uint8_t slaveAddress, uint8_t* buf, uint32_t cnt)
{
    uint8_t*  data = buf;
    uint32_t error = 0, step = 0;
    uint32_t bytesLeftToRead = cnt;

    //States that define the process of reading from an i2c slave.
    uint8_t readSTATES[] =
    {
        SEND_START_CONDITION,
        SET_DEVICE_ADDRESS_WRITE,
        WRITE_SLAVE_ADDRESS,
        SEND_START_CONDITION,
        SET_DEVICE_ADDRESS_READ,
        READ_DATA,
        SEND_STOP_CONDITION
    };

    //return processStates(deviceAddress, slaveAddress, buf, 1, readSTATES);

    do
    {
        switch (readSTATES[step++])
        {
        case SEND_START_CONDITION:
            error |= sendStartCondition();
            break;
        case SET_DEVICE_ADDRESS_WRITE:
            error |= setAddress(deviceAddress, WRITE);
            break;
        case WRITE_SLAVE_ADDRESS:
            error |= writeByte(slaveAddress);
            break;
        case SET_DEVICE_ADDRESS_READ:
            error |= setAddress(deviceAddress, READ);
            break;
        case READ_DATA:
            //For each byte, decide if ACK should be expected, and move the data pointer and read
            while (!error && bytesLeftToRead--)
            {
                error |= readByte((bytesLeftToRead != 0), data++);
            }
            break;
        case SEND_STOP_CONDITION:
            error |= sendStopCondition();
            break;
        }
        // If an error occured during the read process and we succeed in resetting the bus, try again
        if (error && resetBus())
        {
            data  = buf; //reset data pointer
            step = 0;
            error = 0;
        }
    }
    while (!error && step < sizeof(readSTATES) + 1);
    return error;
}
