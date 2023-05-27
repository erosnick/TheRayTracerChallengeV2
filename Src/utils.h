#pragma once

#include <cmath>

#define ToString(a) #a

constexpr float EPSILON = 0.00001f;

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