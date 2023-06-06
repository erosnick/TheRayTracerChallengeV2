#pragma once

#include "ray.h"

class AABB
{
public:
	AABB() = default;
	AABB(const tuple& a, const tuple& b)
	{
		minimum = a;
		maximum = b;
	}

	tuple min() const { return minimum; }
	tuple max() const { return maximum; }

	//bool hit(const Ray& ray, float inTMin, float inTMax) const 
	//{
	//	auto localRay = transformRay(ray, inverse(transform));

	//	for (int a = 0; a < 3; a++)
	//	{
	//		auto t0 = std::fminf((minimum[a] - ray.origin[a]) / ray.direction[a],
	//								   (maximum[a] - ray.origin[a]) / ray.direction[a]);

	//		auto t1 = std::fmaxf((minimum[a] - ray.origin[a]) / ray.direction[a],
	//								   (maximum[a] - ray.origin[a]) / ray.direction[a]);

	//		auto tMin = std::fmaxf(t0, inTMin);
	//		auto tMax = std::fminf(t1, inTMax);

	//		if (tMax <= tMin)
	//		{
	//			return false;
	//		}
	//	}

	//	return true;
	//}

	inline bool hit(const Ray& ray, float inTMin, float inTMax) const
	{
		for (int a = 0; a < 3; a++)
		{
			auto invD = 1.0f / ray.direction[a];
			auto t0 = (min()[a] - ray.origin[a]) * invD;
			auto t1 = (max()[a] - ray.origin[a]) * invD;

			if (invD < 0.0f)
			{
				std::swap(t0, t1);
			}

			auto tMin = t0 > inTMin ? t0 : inTMin;
			auto tMax = t1 > inTMax ? t1 : inTMax;

			if (tMax <= tMin)
			{
				return false;
			}
		}
		return true;
	}

	matrix4 transform;
	
	tuple minimum;
	tuple maximum;
};