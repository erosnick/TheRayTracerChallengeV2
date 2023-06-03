#pragma once

#include <tuple>

#include "shape.h"

class Cube : public Shape
{
public:
	virtual std::vector<Intersection> localIntersect(const Ray& transformedRay) override
	{
		auto [xTMin, xTMax] = checkAxis(transformedRay.origin.x, transformedRay.direction.x);
		auto [yTMin, yTMax] = checkAxis(transformedRay.origin.y, transformedRay.direction.y);
		auto [zTMin, zTMax] = checkAxis(transformedRay.origin.z, transformedRay.direction.z);

		auto tMin = std::max(std::max(xTMin, yTMin), zTMin);
		auto tMax = std::min(std::min(xTMax, yTMax), zTMax);

		if (tMin > tMax)
		{
			return {};
		}

		return { { tMin, shared_from_this() }, { tMax, shared_from_this() } };
	}

	virtual tuple localNormalAt(const tuple& localPosition) override
	{
		auto absX = std::fabsf(localPosition.x);
		auto absY = std::fabsf(localPosition.y);
		auto absZ = std::fabsf(localPosition.z);
		auto maxComponent = std::max(std::max(absX, absY), absZ);

		if (equal(maxComponent, absX))
		{
			return vector(localPosition.x, 0.0f, 0.0f);
		}
		else if (equal(maxComponent, absY))
		{
			return vector(0.0f, localPosition.y, 0.0f);
		}

		return vector(0.0f, 0.0f, localPosition.z);
	}

private:
	std::tuple<float, float> checkAxis(float origin, float direction)
	{
		auto tMinNumerator = (-1.0f - origin);
		auto tMaxNumerator = (1.0f - origin);
		auto tMin = INFINITY;
		auto tMax = INFINITY;

		if (std::fabsf(direction) >= EPSILON)
		{
			tMin = tMinNumerator / direction;
			tMax = tMaxNumerator / direction;
		}
		else
		{
			// https://pragprog.com/cms/errata/jbtracer-errata/
			tMin = tMinNumerator * INFINITY;
			tMax = tMaxNumerator * INFINITY;
		}

		if (tMin > tMax)
		{
			std::swap(tMin, tMax);
		}

		return { tMin, tMax };
	}
};

std::shared_ptr<Shape> createCube()
{
	return std::make_shared<Cube>();
}