/*
 * i2c.c
 *
 * Created: 08/07/2023 22:50:09
 *  Author: milos
 */ 

#include <avr/io.h>

#include "i2c.h"

I2C_Status _I2C_StartTransaction();
void _I2C_Write(uint8_t data);
void _I2C_SendACK();
void _I2C_SendNACK();
uint8_t _I2C_Read();
I2C_Status _I2C_CheckStatus(uint8_t status);
void _I2C_StopTransaction();

void I2C_Init() {
	DDRC &= 0b11001111; /* make SDA and SCL inputs */
	PORTC &= 0b11001111;/* disable internal pullups on SDA and SCL */
	
	// Set clock to 250kHz, no prescalers
	// freq = F_CPU / (16 + 2*TWBR.TWPS)
	// For 250kHz: TWBR = 12 TWPS = 00
	TWSR0 = (0 << TWPS1) | (0 << TWPS0);
	TWBR0 = 24;
	
	// Enable interface
	TWCR0 = (1 << TWEN);
}

I2C_Status I2C_WriteRegister(uint8_t addr, uint8_t data) {
	I2C_Status res = I2C_OK;
	
	// Send start condition
	res = _I2C_StartTransaction();
	if (res)
		return res;
	
	// Send address and write bit
	_I2C_Write((addr & 0xFE) | TW_WRITE);
	
	if (_I2C_CheckStatus(TW_MT_SLA_ACK)) {
		_I2C_StopTransaction();
		return I2C_NACK;
	}
	
	// Send data
	_I2C_Write(data);
	
	if (_I2C_CheckStatus(TW_MT_DATA_ACK))
		res = I2C_NACK;
	
	// Send stop condition
	_I2C_StopTransaction();
	
	return res;
}

I2C_Status I2C_WriteMultiple(uint8_t addr, uint8_t* data, uint16_t len) {
	I2C_Status res = I2C_OK;
	
	// Send start condition
	res = _I2C_StartTransaction();
	if (res)
		return res;
	
	// Send address and write bit
	_I2C_Write((addr & 0xFE) | TW_WRITE);
	
	if (_I2C_CheckStatus(TW_MT_SLA_ACK)) {
		_I2C_StopTransaction();
		return I2C_NACK;
	}
	
	// Send data
	for (uint16_t i = 0; i < len; i++) {
		_I2C_Write(data[i]);
	
		if (_I2C_CheckStatus(TW_MT_DATA_ACK)) {
			res = I2C_NACK;
			break;
		}
	}
	
	// Send stop condition
	_I2C_StopTransaction();
	
	return res;
}

I2C_Status I2C_WriteCommandMultiple(uint8_t addr, uint8_t cmd, uint8_t* data, uint16_t len) {
	I2C_Status res = I2C_OK;
	
	// Send start condition
	res = _I2C_StartTransaction();
	if (res)
	return res;
	
	// Send address and write bit
	_I2C_Write((addr & 0xFE) | TW_WRITE);
	
	if (_I2C_CheckStatus(TW_MT_SLA_ACK)) {
		_I2C_StopTransaction();
		return I2C_NACK;
	}
	
	// Send command
	_I2C_Write(cmd);
	
	if (_I2C_CheckStatus(TW_MT_DATA_ACK)) {
		return I2C_NACK;
	}
	
	// Send data
	for (uint16_t i = 0; i < len; i++) {
		_I2C_Write(data[i]);
		
		if (_I2C_CheckStatus(TW_MT_DATA_ACK)) {
			_I2C_StopTransaction();
			return I2C_NACK;
		}
	}
	
	// Send stop condition
	_I2C_StopTransaction();
	
	return res;
}

I2C_Status I2C_ReadRegister(uint8_t addr, uint8_t* data) {
	I2C_Status res = I2C_OK;
	
	// Send start condition
	res = _I2C_StartTransaction();
	if (res)
		return res;
	
	// Send address and write bit
	_I2C_Write(addr << 1 | TW_READ);
	
	if (_I2C_CheckStatus(TW_MR_SLA_ACK)) {
		_I2C_StopTransaction();
		return I2C_NACK;
	}
	
	// Read data and send NACK to end transaction
	(*data) = _I2C_Read();
	
	// Send stop condition
	_I2C_StopTransaction();
	
	return res;
}

I2C_Status _I2C_StartTransaction() {
	// Send start condition
	TWCR0 = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
	
	// Wait for the start condition to be set
	while (!(TWCR0 & (1 << TWINT)));
	
	// Check for errors
	return _I2C_CheckStatus(TW_START) & _I2C_CheckStatus(TW_REP_START);
}

void _I2C_Write(uint8_t data) {
	// Transmit byte
	TWDR0 = data;
	TWCR0 = (1 << TWINT) | (1 << TWEN);
	
	// Wait for data to be sent
	while (!(TWCR0 & (1 << TWINT)));
}

void _I2C_SendACK() {
	TWCR0 = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
	
	// Wait for the ACK to be sent
	while (!(TWCR0 & (1 << TWINT)));
}

void _I2C_SendNACK() {
	TWCR0 = (1 << TWINT) | (1 << TWEN);
	
	// Wait for the NACK to be sent
	while (!(TWCR0 & (1 << TWINT)));
}

uint8_t _I2C_Read() {
	return TWDR0;
}


I2C_Status _I2C_CheckStatus(uint8_t status) {
	return ((TWSR0 & 0xF8) == status) ? I2C_OK : I2C_Error;
}

void _I2C_StopTransaction() {
	// Send stop condition
	TWCR0 = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
}


