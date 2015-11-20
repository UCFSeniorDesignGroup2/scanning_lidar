/*
 * range_finder.h
 *
 *  Created on: Nov 19, 2015
 *      Author: jack
 */

#ifndef DRIVERS_RANGE_FINDER_RANGE_FINDER_H_
#define DRIVERS_RANGE_FINDER_RANGE_FINDER_H_

#include <stdint.h>

namespace Drivers
{
	class RangeFinder
	{
	public:

		int32_t GetRangeInMiliMeters();
	};

}

#endif /* DRIVERS_RANGE_FINDER_RANGE_FINDER_H_ */
