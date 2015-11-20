/*
 * slip_interface.h
 *
 *  Created on: Nov 19, 2015
 *      Author: jack
 */

#ifndef DRIVERS_SLIP_INTERFACE_SLIP_INTERFACE_H_
#define DRIVERS_SLIP_INTERFACE_SLIP_INTERFACE_H_

#include <stdint.h>

namespace Drivers
{
	class DataPipe
	{
	public:

		virtual ~DataPipe() {}

		// called when data is to be sent out.
		// called synchronously by slip interface
		virtual int WriteData(const uint8_t* data, uint32_t length) = 0;

	};

	class SlipInterface
	{
	public:

		typedef enum
		{
			SLIP_END = 0xC0,
			SLIP_ESC = 0xDB,
			SLIP_ESC_END = 0xDC,
			SLIP_ESC_ESC = 0xDD
		} SLIP_CHARS;

		SlipInterface();

		void SetDataPipe(DataPipe* pipe);

		int SendSlipData(const uint8_t* data, uint32_t length);

		void OnDataRecieved(const uint8_t* data, uint32_t length);

	protected:


		DataPipe* mDataPipe;
	};

}



#endif /* DRIVERS_SLIP_INTERFACE_SLIP_INTERFACE_H_ */
