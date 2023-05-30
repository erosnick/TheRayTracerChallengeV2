#pragma once

#include "shape.h"

class Plane : public Shape
{

public:
	std::vector<Intersection> localIntersect(const Ray& transformedRay) override
	{
		if (std::fabsf(transformedRay.direction.y) < EPSILON)
		{
			return {};
		}

		auto t = -transformedRay.origin.y / transformedRay.direction.y;

		auto shape = std::dynamic_pointer_cast<Plane>(shared_from_this());
		
		return { { t, shape }};
	}

	tuple localNormalAt(const tuple& localPosition) override
	{
		return vector(0.0f, 1.0f, 0.0f);
	}
};

inline static std::shared_ptr<Shape> createPlane()
{
	return std::make_shared<Plane>();
}