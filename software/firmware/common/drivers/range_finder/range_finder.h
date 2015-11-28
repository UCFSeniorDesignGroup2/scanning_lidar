/*
 * range_finder.h
 *
 *  Created on: Nov 19, 2015
 *      Author: jack
 */

#ifndef DRIVERS_RANGE_FINDER_RANGE_FINDER_H_
#define DRIVERS_RANGE_FINDER_RANGE_FINDER_H_

#include <stdint.h>
#include <hal/i2c_master/i2c_master.h>

namespace Drivers
{
	class RangeFinder
	{
	public:
		RangeFinder();
		void SetI2CInterface(HAL::I2C_MASTER* i2c);
		int32_t GetRangeInMiliMeters();

		int32_t WriteRegister(uint8_t start_reg_addr, const uint8_t* data, uint32_t length);
		int32_t ReadRegister(uint8_t start_reg_addr, uint8_t* data, uint32_t length);
	protected:



		HAL::I2C_MASTER* mI2CInterface;
	};

}

#endif /* DRIVERS_RANGE_FINDER_RANGE_FINDER_H_ */
