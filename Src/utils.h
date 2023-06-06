#pragma once

#include <cmath>
#include <random>

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

inline double randomDouble() 
{
	static std::uniform_real_distribution<double> distribution(0.0, 1.0);
	static std::mt19937_64 generator;
	return distribution(generator);
}

inline float randomFloat()
{
	static std::uniform_real_distribution<float> distribution(0.0f, 1.0f);
	static std::mt19937_64 generator;
	return distribution(generator);
}

inline int32_t randomInt(int32_t min, int32_t max)
{
	static std::uniform_int_distribution<int32_t> distribution(min, max);
	static std::mt19937_64 generator;
	return distribution(generator);
}