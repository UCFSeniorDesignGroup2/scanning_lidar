#ifndef __ANALOG_OUTPUT
#define __ANALOG_OUTPUT

#include <stdint.h>

namespace HAL
{
	// abstract class for interfacing with a dac
	class AnalogOutput
	{
	public:

		virtual ~AnalogOutput() {}

		// initialize the peripheral so it is in a state that can output voltages
		virtual int32_t Initialize() = 0;
		// set the voltage to be output from the device
		virtual int32_t SetVoltage(float voltage) = 0;

	};
}

#endif
