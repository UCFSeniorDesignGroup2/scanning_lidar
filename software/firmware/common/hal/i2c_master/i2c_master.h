#ifndef __I2C_MASTER_H
#define __I2C_MASTER_H

#include <stdint.h>
#include <oal/oal_common.h>

namespace HAL
{
	class I2C_MASTER
	{
	public:

		I2C_MASTER();
		virtual ~I2C_MASTER() {}

		virtual int32_t Initialize() = 0;
		virtual int32_t WriteData(uint8_t addr, const uint8_t* data, uint32_t length) = 0;
		virtual int32_t ReadData(uint8_t addr, uint8_t* data, uint32_t length) = 0;

		void BlockCallingThread();
		void UnblockCallingThread();

	protected:

		OAL::Semaphore mSemaphore;
	};
}

#endif
