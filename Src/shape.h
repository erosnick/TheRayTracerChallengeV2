#pragma once

#include "ray.h"
#include "matrix.h"
#include "material.h"
#include "intersection.h"
#include "aabb.h"

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

	void setScale(const tuple& inScale)
	{
		scale = inScale;
	}

	void setScale(float x, float y, float z)
	{
		setScale(point(x, y, z));
	}

	void setRotation(float pitch, float yaw, float roll)
	{
		setRotation(point(pitch, yaw, roll));
	}

	void setRotation(const tuple& inRotation)
	{
		rotation = inRotation;
	}

	void setTranslation(float x, float y, float z)
	{
		setRotation(point(x, y, z));
	}

	void setTranslation(const tuple& inTranslation)
	{
		translation = inTranslation;
	}

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

	virtual tuple localNormalAt(const tuple& localPosition) const { return {}; }

	virtual bool boundingBox(float time0, float time1, AABB& outputBox) = 0;

	matrix4 transform;
	tuple scale{ 1.0f, 1.0f, 1.0f, 1.0f };
	tuple rotation{ 0.0f, 0.0f, 0.0f, 1.0f };
	tuple translation{ 0.0f, 0.0f, 0.0f, 1.0f };
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

	virtual tuple localNormalAt(const tuple& localPosition) const override 
	{ 
		return localPosition; 
	}

	virtual bool boundingBox(float time0, float time1, AABB& outputBox) override
	{
		return true;
	}

	Ray savedRay;
};

inline static std::shared_ptr<TestShape> testShape()
{
	return std::make_shared<TestShape>();
}

inline static AABB surroundingBox(const AABB& box0, const AABB& box1)
{
	auto minX = std::fminf(box0.min().x, box1.min().x);
	auto minY = std::fminf(box0.min().y, box1.min().y);
	auto minZ = std::fminf(box0.min().z, box1.min().z);

	tuple min = point(minX, minY, minZ);

	auto maxX = std::fmaxf(box0.max().x, box1.max().x);
	auto maxY = std::fmaxf(box0.max().y, box1.max().y);
	auto maxZ = std::fmaxf(box0.max().z, box1.max().z);

	tuple max = point(maxX, maxY, maxZ);

	return AABB(min, max);
}

inline static bool boundingBox(float time0, float time1, const std::vector<std::shared_ptr<Shape>>& objects, AABB& outputBox)
{
	if (objects.empty())
	{
		return false;
	}

	AABB tempBox;
	bool firstBox = true;

	for (const auto& object : objects)
	{
		if (!object->boundingBox(time0, time1, tempBox))
		{
			return false;
		}
		outputBox = firstBox ? tempBox : surroundingBox(outputBox, tempBox);
		firstBox = false;
	}

	return true;
}