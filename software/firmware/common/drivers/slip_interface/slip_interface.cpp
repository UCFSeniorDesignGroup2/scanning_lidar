/*
 * slip_interface.cpp
 *
 *  Created on: Nov 19, 2015
 *      Author: jack
 */


#include "slip_interface.h"

using namespace Drivers;

SlipInterface::SlipInterface()
{
	mDataPipe = 0;
}

void SlipInterface::SetDataPipe(DataPipe* pipe)
{
	mDataPipe = pipe;
}

int SlipInterface::SendSlipData(const uint8_t* data, uint32_t length)
{
	uint8_t tx_data_buffer[(const uint32_t)length*2+1];

	uint32_t index = 0;
	tx_data_buffer[index++] = SLIP_END;
	for(uint32_t i = 0; i < length; i++)
	{
		if(data[i] == SLIP_END)
		{
			tx_data_buffer[index++] = SLIP_ESC;
			tx_data_buffer[index++] = SLIP_ESC_END;
		}
		else if(data[i] == SLIP_ESC)
		{
			tx_data_buffer[index++] = SLIP_ESC;
			tx_data_buffer[index++] = SLIP_ESC_ESC;
		}
		else
		{
			tx_data_buffer[index++] = data[i];
		}
	}

	tx_data_buffer[index++] = SLIP_END;

	if(mDataPipe != 0)
	{
		return mDataPipe->WriteData(tx_data_buffer, index);
	}


	return -1;
}

void SlipInterface::OnDataRecieved(const uint8_t* data, uint32_t length)
{
	/*
	 * todo: handle slip recieve packet
	 */
}
