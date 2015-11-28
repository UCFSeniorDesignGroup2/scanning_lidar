#ifndef __I2C_HAL_CONF_H
#define __I2C_HAL_CONF_H

#include <stdint.h>

namespace HAL
{

	int32_t I2C1_Initialize_Hardware();

	int32_t I2C2_Initialize_Hardware();

	int32_t I2C1_WriteData(uint8_t addr, const uint8_t* data, uint32_t length);

	int32_t I2C2_WriteData(uint8_t addr, const uint8_t* data, uint32_t length);

	int32_t I2C1_ReadData(uint8_t addr, uint8_t* data, uint32_t length);

	int32_t I2C2_ReadData(uint8_t addr, uint8_t* data, uint32_t length);


}

#endif
