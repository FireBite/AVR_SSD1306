/*
 * i2c.h
 *
 * Created: 08/07/2023 22:50:18
 *  Author: milos
 */ 

#ifndef I2C_H_
#define I2C_H_

#include <util/twi.h>

typedef enum {
	I2C_OK = 0,
	I2C_NACK,
	I2C_Error = 255
} I2C_Status;

void I2C_Init();
I2C_Status I2C_WriteRegister(uint8_t addr, uint8_t data);
I2C_Status I2C_ReadRegister(uint8_t addr, uint8_t* data);
I2C_Status I2C_WriteMultiple(uint8_t addr, uint8_t* data, uint16_t len);
I2C_Status I2C_WriteCommandMultiple(uint8_t addr, uint8_t cmd, uint8_t* data, uint16_t len);
I2C_Status I2C_ReadMultiple(uint8_t addr, uint8_t* data, uint16_t len);

#endif /* I2C_H_ */