/*
 * hal_spi.h
 *
 *  Created on: Dec 5, 2015
 *      Author: jack
 */

#ifndef HAL_HAL_SPI_HAL_SPI_H_
#define HAL_HAL_SPI_HAL_SPI_H_

#include <stdint.h>

namespace HAL
{
	class HAL_SPI
	{
	public:
		virtual ~HAL_SPI() {}

		virtual int32_t Initialize() = 0;

		virtual int32_t TransferData(const uint8_t* data_out, uint8_t* data_in, uint32_t length) = 0;
	};
}


#endif /* HAL_HAL_SPI_HAL_SPI_H_ */
