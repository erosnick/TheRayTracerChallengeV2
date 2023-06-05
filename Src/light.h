#pragma once

#include "tuple.h"

struct PointLight
{
	tuple position;
	tuple intensity;
	float constant = 1.0f;
	float linear = 0.022f;
	float quadratic = 0.0019f;
};

inline static bool operator==(const PointLight& a, const PointLight& b)
{
	return (a.position == b.position) && 
		   (a.intensity == b.intensity);
}

inline static PointLight pointLight(const tuple& position, const tuple& intensity)
{
	return { position, intensity };
}