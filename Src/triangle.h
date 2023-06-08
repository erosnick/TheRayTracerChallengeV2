#pragma once

#include "shape.h"

class Triangle : public Shape
{
public:
	Triangle(const tuple& inP0, const tuple& inP1, const tuple& inP2)
	: p0(inP0), p1(inP1), p2(inP2)
	{
		e0 = p1 - p0;
		e1 = p2 - p0;

		// Left-Hand
		normal = normalize(cross(e1, e0));
	}

	virtual std::vector<Intersection> localIntersect(const Ray& transformedRay) override
	{
		auto dirCrossE1 = cross(transformedRay.direction, e1);
		auto determinant = dot(e0, dirCrossE1);

		if (std::fabsf(determinant) < EPSILON)
		{
			return {};
		}

		auto f = 1.0f / determinant;

		auto p0ToOrigin = transformedRay.origin - p0;
		auto u = f * dot(p0ToOrigin, dirCrossE1);

		if (u < 0.0f || u > 1.0f)
		{
			return {};
		}

		auto originCrossE0 = cross(p0ToOrigin, e0);
		auto v = f * dot(transformedRay.direction, originCrossE0);

		if (v < 0.0f || (u + v) > 1.0f)
		{
			return {};
		}

		auto t = f * dot(e1, originCrossE0);

		return { { t, shared_from_this()}};
	}

	virtual tuple localNormalAt(const tuple& localPosition) const override
	{
		return normal;
	}

	virtual bool boundingBox(float time0, float time1, AABB& outputBox) override
	{
		return true;
	}

	tuple p0;
	tuple p1;
	tuple p2;
	tuple e0;
	tuple e1;
	tuple normal;
};

std::shared_ptr<Shape> createTriangle(const tuple& p0, const tuple& p1, const tuple& p2)
{
	return std::make_shared<Triangle>(p0, p1, p2);
}