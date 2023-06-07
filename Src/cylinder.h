#pragma once

#include "shape.h"

class Cylinder : public Shape
{
public:
	Cylinder(float inMinimum = -std::numeric_limits<float>::max(), 
			 float inMaximum = std::numeric_limits<float>::max(), bool inClosed = false)
	: minimum(inMinimum), maximum(inMaximum), closed(inClosed)
	{}

	virtual std::vector<Intersection> localIntersect(const Ray& transformedRay)
	{
		auto origin = transformedRay.origin;
		auto direction = transformedRay.direction;

		auto a = std::powf(direction.x, 2.0f) + 
					  std::powf(direction.z, 2.0f);

		// Ray is parallel to the y axis
		if (equal(a, 0.0f))
		{
			std::vector<Intersection> result;
			intersectCaps(transformedRay, result);
			return result;
		}

		auto b = 2.0f * origin.x * direction.x +
					  2.0f * origin.z * direction.z;

		auto c = std::powf(origin.x, 2.0f) +
					  std::powf(origin.z, 2.0f) - 1.0f;

		auto discriminant = b * b - 4 * a * c;

		// Ray does not intersect the cylinder
		if (discriminant < 0.0f)
		{
			return {};
		}

		auto t0 = (-b - std::sqrtf(discriminant)) / (2.0f * a);
		auto t1 = (-b + std::sqrtf(discriminant)) / (2.0f * a);

		if (t0 > t1)
		{
			std::swap(t0, t1);
		}

		std::vector<Intersection> result;

		auto y0 = origin.y + t0 * direction.y;

		if (y0 > minimum && y0 < maximum)
		{
			result.push_back({ t0, shared_from_this() });
		}

		auto y1 = origin.y + t1 * direction.y;

		if (y1 > minimum && y1 < maximum)
		{
			result.push_back({ t1, shared_from_this() });
		}

		intersectCaps(transformedRay, result);

		// This is just a placeholder, to ensure the tests
		// pass that expect the ray to miss
		return result;
	}

	virtual tuple localNormalAt(const tuple& localPosition) const
	{
		// Compute the square of the distance from the y axis
		auto distance = std::powf(localPosition.x, 2.0f) +
							 std::powf(localPosition.z, 2.0f);

		if ((distance < 1.0f) && localPosition.y >= maximum - EPSILON)
		{
			return vector(0.0f, 1.0f, 0.0f);
		}
		else if ((distance < 1.0f) && localPosition.y <= minimum + EPSILON)
		{
			return vector(0.0f, -1.0f, 0.0f);
		}
		return vector(localPosition.x, 0.0f, localPosition.z);
	}

	virtual bool boundingBox(float time0, float time1, AABB& outputBox) override
	{
		return true;
	}

public:
	float minimum = std::numeric_limits<float>::min();
	float maximum = std::numeric_limits<float>::max();
	bool closed = false;
private:
	// A helper function to reduce duplication.
	// checks to see if the intersection at `t` is within a radius
	// of 1 (the radius of your cylinders) from the y axis.
	bool checkCap(const Ray& ray, float t)
	{
		auto x = ray.origin.x + t * ray.direction.x;
		auto z = ray.origin.z + t * ray.direction.z;

		// Test case Example 3和Example 5的时候这里的z = 1.00000024, 导致返回false
		return (x * x + z * z) <= (1.0f + EPSILON);
	}

	void intersectCaps(const Ray& ray, std::vector<Intersection>& result)
	{
		// Caps only matter if the cylinder is closed, and might possibly be
		// intersected by the ray.
		if (!closed || equal(std::fabsf(ray.direction.y), 0.0f))
		{
			return;
		}

		// Check for an intersection with the lower end cap by intersecting
		// the ray with the plane at y = cylinder.minimum
		auto t0 = (minimum - ray.origin.y) / ray.direction.y;
		if (checkCap(ray, t0))
		{
			result.push_back({ t0, shared_from_this() });
		}

		// Check for an intersection with the upper end cap by intersecting
		// the ray with the plane at y = cylinder.maximum
		auto t1 = (maximum - ray.origin.y) / ray.direction.y;
		if (checkCap(ray, t1))
		{
			result.push_back({ t1, shared_from_this() });
		}
	}
};

inline static std::shared_ptr<Shape> createCylinder(float minimum = -std::numeric_limits<float>::max(),
													 float maximum = std::numeric_limits<float>::max(), bool closed = false)
{
	return std::make_shared<Cylinder>(minimum, maximum, closed);
}