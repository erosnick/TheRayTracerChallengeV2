#pragma once

#include <cmath>
#include <random>

#define ToString(a) #a

#include "constants.h"

inline static bool equal(float a, float b)
{
	return std::fabsf(a - b) < EPSILON;
}

inline static bool isZero(float a)
{
	return std::fabsf(a) <= EPSILON;
}

inline static bool between(float a, float left, float right)
{
	return a > left && a < right;
}

inline static float clamp(float value, float min, float max)
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

inline static float radians(float angle)
{
	return (PI / 180.0f) * angle;
}

inline static double randomDouble()
{
	static std::uniform_real_distribution<double> distribution(0.0, 1.0);
	static std::mt19937_64 generator;
	return distribution(generator);
}

inline static float randomFloat()
{
	static std::uniform_real_distribution<float> distribution(0.0f, 1.0f);
	static std::mt19937_64 generator;
	return distribution(generator);
}

inline static int32_t randomInt(int32_t min, int32_t max)
{
	static std::uniform_int_distribution<int32_t> distribution(min, max);
	static std::mt19937_64 generator;
	return distribution(generator);
}

inline static float max(float a, float b, float c)
{
	return std::max(std::max(a, b), c);
}

inline static float min(float a, float b, float c)
{
	return std::min(std::min(a, b), c);
}