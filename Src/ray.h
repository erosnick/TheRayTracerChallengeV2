#pragma once

#include "tuple.h"
#include "transforms.h"

struct Ray
{
	Ray() {}

	Ray(const tuple& inOrigin, const tuple& inDirection)
		: origin(inOrigin), direction(inDirection)
	{}

	tuple at(float t) const { return origin + direction * t; }
	tuple origin{ 0.0f, 0.0f, 0.0f, 1.0f };
	tuple direction;
};

inline Ray transformRay(const Ray& ray, const matrix4& m)
{
	Ray result;

	result.origin = m * ray.origin;
	result.direction = m * ray.direction;

	return result;
}