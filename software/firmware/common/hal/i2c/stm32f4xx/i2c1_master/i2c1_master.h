#ifndef __I2C1_H
#define __I2C1_H

#include <i2c/hal_i2c_master.h>

#include <oal_semaphore.h>

namespace HAL
{
	class I2C1_MASTER : public I2C_MASTER
	{
	public:

		I2C1_MASTER();

		virtual int32_t Initialize();

		virtual int32_t WriteData(uint8_t addr, const uint8_t* data, uint32_t length);
		virtual int32_t ReadData(uint8_t addr, uint8_t* data, uint32_t length);

		static I2C1_MASTER* GetInstance();

		void BlockTxThread();
		void UnblockTxThread();

		void BlockRxThread();
		void UnblockRxThread();

	protected:

		OAL::Semaphore mTxSemaphore;
		OAL::Semaphore mRxSemaphore;

		static I2C1_MASTER mInstance;
	};
}

#endif
