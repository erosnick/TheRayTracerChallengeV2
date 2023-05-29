#pragma once

#include <cmath>

#define ToString(a) #a

#include "constants.h"

inline bool equal(float a, float b)
{
	return std::fabsf(a - b) < EPSILON;
}

inline float clamp(float value, float min, float max)
{
	float result = value;

	if (result < min)
	{
		result = min;
	}

	if (result > max)
	{
		result = max;
	}

	return result;
}

inline float radians(float angle)
{
	return (PI / 180.0f) * angle;
}