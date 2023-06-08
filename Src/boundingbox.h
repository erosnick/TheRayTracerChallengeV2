#pragma once

#include "tuple.h"

class BoundingBox
{
public:
	BoundingBox() = default;
	BoundingBox(const tuple& inMin, const tuple& inMax)
	: min(inMin), max(inMax)
	{}

	void addPoint(const tuple& point)
	{
		// Adding "point" to "box"
		if (point.x < min.x) min.x = point.x;
		if (point.y < min.y) min.y = point.y;
		if (point.z < min.z) min.z = point.z;

		if (point.x > max.x) max.x = point.x;
		if (point.y > max.y) max.y = point.y;
		if (point.z > max.z) max.z = point.z;
	}

	tuple min = point(std::numeric_limits<float>::max());
	tuple max = point(-std::numeric_limits<float>::max());
};