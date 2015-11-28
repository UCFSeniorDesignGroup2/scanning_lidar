#ifndef __HAL_I2C_h
#define __HAL_I2C_h

#include <stdint.h>
#include <oal_mutex.h>

namespace HAL
{
	class I2C_MASTER
	{
	public:
		virtual ~I2C_MASTER();

		virtual int32_t Initialize() = 0;

		virtual int32_t WriteData(uint8_t addr, const uint8_t* data, uint32_t length) = 0;
		virtual int32_t ReadData(uint8_t addr, uint8_t* data, uint32_t length) = 0;

		void TakeControl();
		void ReleaseControl();

	protected:
		OAL::Mutex mMutex;
	};
}
#endif
