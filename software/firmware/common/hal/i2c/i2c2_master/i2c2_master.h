#ifndef __I2C2_MASTER
#define __I2C2_MASTER

#include <i2c/hal_i2c_master.h>

namespace HAL
{
	class I2C2_MASTER : public I2C_MASTER
	{
	public:

		virtual int32_t Initialize();

		virtual int32_t WriteData(uint8_t addr, const uint8_t* data, uint32_t length);

		virtual int32_t ReadData(uint8_t addr, uint8_t* data, uint32_t length);

		static I2C_MASTER* GetInstance();

	protected:

		static I2C2_MASTER mInstance;
	};
}

#endif
