#pragma once

#include "shape.h"

class Cone : public Shape
{
public:
	Cone(float inMinimum = -std::numeric_limits<float>::max(),
		 float inMaximum = std::numeric_limits<float>::max(), bool inClosed = false)
		: minimum(inMinimum), maximum(inMaximum), closed(inClosed)
	{}

	virtual std::vector<Intersection> localIntersect(const Ray& transformedRay) override
	{
		// https://forum.raytracerchallenge.com/thread/166/chapter-cones-all-tests-pass
		auto origin = transformedRay.origin;
		auto direction = transformedRay.direction;

		auto a = direction.x * direction.x -
					  direction.y * direction.y +
					  direction.z * direction.z;

		auto b = 2.0f * origin.x * direction.x - 
				      2.0f * origin.y * direction.y +
				      2.0f * origin.z * direction.z;

		auto c = origin.x * origin.x -
					  origin.y * origin.y +
					  origin.z * origin.z;

		std::vector<Intersection> result;

		if (isZeroHighPrecision(a) && !isZeroHighPrecision(b))
		{
			auto t = -c / (2.0f * b);

			result.push_back({ t, shared_from_this() });
		}

		if (a > EPSILON_HIGH_PRECISION)
		{
			auto discriminant = b * b - 4 * a * c;

			const auto realEpsilon = EPSILON_HIGH_PRECISION * max(std::fabsf(a), std::fabsf(b), std::fabsf(c));

			if (discriminant >= -realEpsilon)
			{
				const auto sqrtDiscriminant = std::sqrtf(std::max(discriminant, 0.0f)); //round to 0 if need be.

				auto t0 = (-b - sqrtDiscriminant) / (2.0f * a);
				auto t1 = (-b + sqrtDiscriminant) / (2.0f * a);

				if (t0 > t1)
				{
					std::swap(t0, t1);
				}

				auto y0 = origin.y + t0 * direction.y;
				if (between(y0, minimum, maximum))
				{
					result.push_back({ t0, shared_from_this() });
				}

				auto y1 = origin.y + t1 * direction.y;
				if (between(y1, minimum, maximum))
				{
					result.push_back({ t1, shared_from_this() });
				}
			}
		}

		intersectCaps(transformedRay, result);

		return result;

		//// When a is zero, it means the ray is parallel to one of the cone¡¯s halves
		//// this still means the ray might intersect the other half of the cone.

		//if (equal(a, 0.0f))
		//{  
		//	// In this case the ray will miss when both a and b are zero.
		//	if (!equal(b, 0.0f))
		//	{
		//		// If a is zero but b isn¡¯t, calc the single point of intersection:
		//		const auto t = -c / (2.0f * b);

		//		result.push_back({ t, shared_from_this() });
		//	}
		//}
		//else 
		//{
		//	// a is non-zero
		//	// In general, the discriminant of the quadratic equation should be non-negative if there are real solutions,
		//	// and negative if there are complex solutions. However, due to floating-point roundoff errors, the discriminant
		//	// may sometimes become slightly negative even when there are real solutions, leading to incorrect results.
		//	// Here, rel_eps is a small relative epsilon based on the magnitudes of the coefficients, making it more robust to different combinations of coefficients
		//	const auto realEpsilon = 0.000001f * max(std::fabsf(a), std::fabsf(b), std::fabsf(c));
		//	const auto discriminant = b * b - 4.0f * a * c;

		//	if (discriminant >= -realEpsilon)
		//	{ 
		//		//The discriminant is considered to be non-negative if it is greater than or equal to -rel_eps            
		//		const auto sqrtDiscriminant = std::sqrtf(std::max(discriminant, 0.0f)); //round to 0 if need be.

		//		const auto t0 = (-b - sqrtDiscriminant) / (2.0f * a);

		//		const auto t1 = (-b + sqrtDiscriminant) / (2.0f * a);

		//		const auto y0 = origin.y + t0 * direction.y;

		//		if (between(y0, minimum, maximum)) 
		//		{
		//			result.push_back({ t0, shared_from_this() });
		//		}

		//		const auto y1 = origin.y + t1 * direction.y;

		//		if (between(y1, minimum, maximum))
		//		{
		//			result.push_back({ t1, shared_from_this() });
		//		}
		//	}
		//}

		//intersectCaps(transformedRay, result);

		//return result;
	}

	virtual tuple localNormalAt(const tuple& localPosition) const override
	{
		// compute the square of the distance from the y axis
		auto distance = std::powf(localPosition.x, 2.0f) + std::powf(localPosition.z, 2.0f);
		if (distance < 1.0f && localPosition.y >= maximum - EPSILON)
		{
			return vector(0.0f, 1.0f, 0.0f);
		}

		if (distance < 1.0f && localPosition.y <= minimum + EPSILON)
		{
			return vector(0.0f, -1.0f, 0.0f);
		}

		auto y = std::sqrtf(distance);
		if (localPosition.y > 0.0f)
		{
			y *= -1.0f;
		}

		return vector(localPosition.x, y, localPosition.z);
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
	bool checkCap(const Ray& ray, float t, float y)
	{
		auto x = ray.origin.x + t * ray.direction.x;
		auto z = ray.origin.z + t * ray.direction.z;
		return (x * x + z * z) <= y * y;
	}

	void intersectCaps(const Ray& ray, std::vector<Intersection>& result)
	{
		// caps only matter if the cone is closed, and might possibly be intersected by the ray.
		if (!closed || std::fabsf(ray.direction.y) <= EPSILON)
		{
			return;
		}

		// check for an intersection with the lower end cap by intersecting
		// the ray with the plane at y=cone.minimum
		auto t = (minimum - ray.origin.y) / ray.direction.y;
		if (checkCap(ray, t, minimum))
		{
			result.push_back({ t, shared_from_this() });
		}

		// check for an intersection with the upper end cap by intersecting
		// the ray with the plane at y=cone.maximum
		t = (maximum - ray.origin.y) / ray.direction.y;
		if (checkCap(ray, t, maximum))
		{
			result.push_back({ t, shared_from_this() });
		}
	}
};

inline static std::shared_ptr<Shape> createCone(float minimum = -std::numeric_limits<float>::max(),
												 float maximum = std::numeric_limits<float>::max(), bool closed = false)
{
	return std::make_shared<Cone>(minimum, maximum, closed);
}