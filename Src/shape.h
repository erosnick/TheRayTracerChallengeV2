#pragma once

#include "ray.h"
#include "matrix.h"
#include "material.h"
#include "intersection.h"

enum class ShapeType : uint8_t
{
	Sphere,
	Plane,
	Cube
};

class Shape : public std::enable_shared_from_this<Shape>
{
public:
	virtual ~Shape() = default;

	void setTransform(const matrix4& inTransform)
	{
		transform = inTransform;
	}

	std::vector<Intersection> intersect(const Ray& ray)
	{
		auto localRay = transformRay(ray, inverse(transform));
		return localIntersect(localRay); 
	}

	virtual std::vector<Intersection> localIntersect(const Ray& transformedRay) { return {}; }

	tuple normalAt(const tuple& worldPosition)
	{ 
		auto localPosition = inverse(transform) * worldPosition;
		auto localNormal = localNormalAt(localPosition);
		auto worldNormal = transpose(inverse(transform)) * localNormal;
		worldNormal.w = 0.0f;

		return normalize(worldNormal);
	}

	virtual tuple localNormalAt(const tuple& localPosition) { return {}; }

	bool transparent() { return material.transparency > 0.0f; }

	matrix4 transform;
	Material material;
};

class TestShape : public Shape
{
public:
	virtual std::vector<Intersection> localIntersect(const Ray& transformedRay) override 
	{
		savedRay = transformedRay;
		return {}; 
	}

	virtual tuple localNormalAt(const tuple& localPosition) override 
	{ 
		return localPosition; 
	}

	Ray savedRay;
};

inline static std::shared_ptr<TestShape> testShape()
{
	return std::make_shared<TestShape>();
}