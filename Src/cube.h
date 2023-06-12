#pragma once

#include <tuple>

#include "shape.h"

class Cube : public Shape
{
public:
	Cube(float inExtent = 1.0f)
	: extent(inExtent)
	{
	}

	virtual std::vector<Intersection> localIntersect(const Ray& transformedRay) override
	{
		auto [xTMin, xTMax] = checkAxis(transformedRay.origin.x, transformedRay.direction.x);
		auto [yTMin, yTMax] = checkAxis(transformedRay.origin.y, transformedRay.direction.y);
		auto [zTMin, zTMax] = checkAxis(transformedRay.origin.z, transformedRay.direction.z);

		auto tMin = Math::max(xTMin, yTMin, zTMin);
		auto tMax = Math::min(xTMax, yTMax, zTMax);

		if (tMin > tMax)
		{
			return {};
		}

		return { { tMin, shared_from_this() }, { tMax, shared_from_this() } };
	}

	virtual tuple localNormalAt(const tuple& localPosition, const Intersection intersection = {}) const override
	{
		auto absX = std::fabsf(localPosition.x);
		auto absY = std::fabsf(localPosition.y);
		auto absZ = std::fabsf(localPosition.z);
		auto maxComponent = Math::max(absX, absY, absZ);

		if (Math::equal(maxComponent, absX))
		{
			return vector(localPosition.x, 0.0f, 0.0f);
		}
		else if (Math::equal(maxComponent, absY))
		{
			return vector(0.0f, localPosition.y, 0.0f);
		}

		return vector(0.0f, 0.0f, localPosition.z);
	}

	virtual bool boundingBox(BoundingBox& outputBox) override
	{
		return true;
	}

private:
	std::tuple<float, float> checkAxis(float origin, float direction)
	{
		auto tMinNumerator = (-extent - origin);
		auto tMaxNumerator = ( extent - origin);
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

	float extent = 1.0f;
};

std::shared_ptr<Shape> createCube(float extent = 1.0f)
{
	return std::make_shared<Cube>(extent);
}