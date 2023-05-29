#pragma once

#include "tuple.h"

struct PointLight
{
	tuple position;
	tuple intensity;
};

inline PointLight pointLight(const tuple& position, const tuple& intensity)
{
	return { position, intensity };
}