#ifndef __RANGE_FINDER_CONF_H
#define __RANGE_FINDER_CONF_H

typedef struct
{
	int32_t distance;
	int32_t tof;
} TOF_READING;

#define LUT_TABLE_SIZE 9

TOF_READING lut_table[] = {
		{24, 85624},
		{36, 88751},
		{48, 92491},
		{60, 96336},
		{72, 101280},
		{84, 106361},
		{96, 112678},
		{108, 119700},
		{120, 128746},
		{132, 159310},
		{144, 168434}
};

#endif
