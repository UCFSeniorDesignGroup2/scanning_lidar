// tested and works correctly.
// jack ritchey

#ifndef __ANALOG_OUTPUT1_H
#define __ANALOG_OUTPUT1_H

#include <hal/analog_output/analog_output.h>

namespace HAL
{

	// implementation of Analog output
	// this analog output is for setting the threshold voltage of the
	// LTC6752 comparator used to detect received laser light
	class AnalogOutput1 : public AnalogOutput
	{
	public:
		AnalogOutput1();

		// initialize the device
		virtual int32_t Initialize();

		// set the threshold of the comparator
		virtual int32_t SetVoltage(float voltage);

		// get an instance of the analog output
		static AnalogOutput* GetInstance();

	protected:

		// instance of analog output.
		static AnalogOutput1 mInstance;

		bool mInitialized;
	};
}

#endif
