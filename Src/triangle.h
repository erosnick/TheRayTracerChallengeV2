#pragma once

#include "shape.h"

class Triangle : public Shape
{
public:
	Triangle() = default;

	Triangle(const tuple& inP0, const tuple& inP1, const tuple& inP2, bool inUseParentMaterial = true)
		: p0(inP0), p1(inP1), p2(inP2)
	{
		e0 = p1 - p0;
		e1 = p2 - p0;

		// Left-Hand
		normal = normalize(cross(e1, e0));

		n0 = n1 = n2 = normal;

		useParentMaterial = inUseParentMaterial;
	}

	Triangle(const tuple& inP0, const tuple& inP1, const tuple& inP2,
		     const tuple& inN0, const tuple& inN1, const tuple& inN2, 
			 const tuple& inT0 = {}, const tuple& inT1 = {}, const tuple& inT2 = {}, bool inUseParentMaterial = true)
	: Triangle(inP0, inP1, inP2, inUseParentMaterial)
	{
		n0 = inN0;
		n1 = inN1;
		n2 = inN2;
		t0 = inT0;
		t1 = inT1;
		t2 = inT2;
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

		a = u;
		b = v;

		auto c = 1.0f - a - b;

		auto texcoordU = (a * t1.x + b * t2.x + c * t0.x);
		auto texcoordV = (a * t1.y + b * t2.y + c * t0.y);

		return { intersectionWithUV(t, shared_from_this(), a, b, texcoordU, texcoordV) };
	}

	virtual tuple localNormalAt(const tuple& localPosition, const Intersection intersection) const override
	{
		return n1 * intersection.a + n2 * intersection.b + n0 * (1.0f - intersection.a - intersection.b);
	}

	virtual bool boundingBox(BoundingBox& outputBox) override
	{
		auto minX = Math::min(p0.x, p1.x, p2.x);
		auto minY = Math::min(p0.y, p1.y, p2.y);
		auto minZ = Math::min(p0.z, p1.z, p2.z);

		auto maxX = Math::max(p0.x, p1.x, p2.x);
		auto maxY = Math::max(p0.y, p1.y, p2.y);
		auto maxZ = Math::max(p0.z, p1.z, p2.z);

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
	tuple t0;
	tuple t1;
	tuple t2;
	tuple e0;
	tuple e1;
	tuple normal;
	float a = 0.0f;
	float b = 0.0f;
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
									   const tuple& n0, const tuple& n1, const tuple& n2, const tuple& t0, const tuple& t1, const tuple& t2)
{
	return std::make_shared<Triangle>(p0, p1, p2, n0, n1, n2, t0, t1, t2);
}