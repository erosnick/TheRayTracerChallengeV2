#pragma once

#include "shape.h"

class Triangle : public Shape
{
public:
	Triangle() = default;

	Triangle(const tuple& inP0, const tuple& inP1, const tuple& inP2)
	: p0(inP0), p1(inP1), p2(inP2)
	{
		e0 = p1 - p0;
		e1 = p2 - p0;

		// Left-Hand
		normal = normalize(cross(e1, e0));

		n0 = n1 = n2 = normal;
	}

	Triangle(const tuple& inP0, const tuple& inP1, const tuple& inP2,
		     const tuple& inN0, const tuple& inN1, const tuple& inN2)
	: Triangle(inP0, inP1, inP2)
	{
		n0 = inN0;
		n1 = inN1;
		n2 = inN2;
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

		return { intersectionWithUV(t, shared_from_this(), u, v) };
	}

	virtual tuple localNormalAt(const tuple& localPosition, const Intersection intersection) const override
	{
		return n1 * intersection.u + n2 * intersection.v + n0 * (1.0f - intersection.u - intersection.v);
	}

	virtual bool boundingBox(BoundingBox& outputBox) override
	{
		auto minX = min(p0.x, p1.x, p2.x);
		auto minY = min(p0.x, p1.x, p2.x);
		auto minZ = min(p0.x, p1.x, p2.x);

		auto maxX = max(p0.x, p1.x, p2.x);
		auto maxY = max(p0.x, p1.x, p2.x);
		auto maxZ = max(p0.x, p1.x, p2.x);

		tuple min = point(minX,  minY, minZ);
		tuple max = point(maxX,  maxY, maxZ);

		outputBox = BoundingBox(min, max);

		return true;
	}

	tuple p0;
	tuple p1;
	tuple p2;
	tuple n0;
	tuple n1;
	tuple n2;
	tuple e0;
	tuple e1;
	tuple normal;
};

inline static bool operator==(const std::shared_ptr<Triangle>& a, const std::shared_ptr<Triangle>& b)
{
	return (a->p0 == b->p0 && 
		    a->p1 == b->p1 &&
			a->p2 == b->p2);
}

inline static std::shared_ptr<Shape> createTriangle(const tuple& p0, const tuple& p1, const tuple& p2)
{
	return std::make_shared<Triangle>(p0, p1, p2);
}

inline static std::shared_ptr<Shape> createSmoothTriangle(const tuple& p0, const tuple& p1, const tuple& p2,
									   const tuple& n0, const tuple& n1, const tuple& n2)
{
	return std::make_shared<Triangle>(p0, p1, p2, n0, n1, n2);
}