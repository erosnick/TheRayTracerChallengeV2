#pragma once

#include "tuple.h"
#include "transforms.h"

struct Ray
{
	Ray(const tuple& inOrigin, const tuple& inDirection)
		: origin(inOrigin), direction(inDirection)
	{}

	tuple at(float t) { return origin + direction * t; }
	tuple origin{ 0.0f, 0.0f, 0.0f, 1.0f };
	tuple direction;
};

inline Ray transform(const Ray& ray, const matrix4& m)
{
	Ray result = ray;

	result.origin.x *= m(0, 0);
	result.origin.y *= m(1, 1);
	result.origin.z *= m(2, 2);

	result.origin.x += m.column(3)[0];
	result.origin.y += m.column(3)[1];
	result.origin.z += m.column(3)[2];

	result.direction = m * result.direction;

	return result;
}