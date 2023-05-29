#pragma once

#include <memory>
#include <algorithm>

#include "sphere.h"
#include "world.h"

struct Intersection
{
	float t = 0.0f;
	std::shared_ptr<Sphere> object;
};

struct HitResult
{
	float t = 0.0f;
	std::shared_ptr<Sphere> object;
	tuple position;
	tuple viewDirection;
	tuple normal;
	bool inside = false;
};

bool operator==(const Intersection& a, const Intersection& b)
{
	return equal(a.t, b.t) && (a.object == b.object);
}

inline std::vector<Intersection> intersect(const std::shared_ptr<Sphere>& sphere, const Ray& ray)
{
	// The vector from the sphere's center, to the ray origin
	// Remember: the sphere is centered at the world origin
	auto transformedRay = transformRay(ray, inverse(sphere->transform));

	auto sphereToRay = transformedRay.origin - sphere->center;

	auto a = dot(transformedRay.direction, transformedRay.direction);
	auto b = 2.0f * dot(transformedRay.direction, sphereToRay);
	auto c = dot(sphereToRay, sphereToRay) - sphere->radius;

	auto discriminant = b * b - 4.0f * a * c;

	if (discriminant < 0.0f)
	{
		return {};
	}

	auto t1 = (-b - std::sqrtf(discriminant)) / (2.0f * a);
	auto t2 = (-b + std::sqrtf(discriminant)) / (2.0f * a);

	return { { t1, sphere }, { t2, sphere } };
}

inline tuple normalAt(const std::shared_ptr<Sphere>& sphere, const tuple& worldPoint)
{
	auto objectPoint = inverse(sphere->transform) * worldPoint;
	auto objectNormal = objectPoint - sphere->center;

	auto worldNormal = transpose(inverse(sphere->transform)) * objectNormal;
	worldNormal.w = 0.0f;

	return  normalize(worldNormal);
}

bool compare(const Intersection& a, const Intersection& b)
{
	return a.t < b.t;
}

inline auto intersections(const std::initializer_list<Intersection>& intersectionList)
{
	std::vector<Intersection> result;

	std::for_each(intersectionList.begin(), intersectionList.end(), [&](const Intersection& intersection)
		{
			result.emplace_back(intersection);
		});

	std::sort(result.begin(), result.end(), compare);

	return result;
}

inline auto intersections(const std::vector<Intersection>& intersectionList)
{
	std::vector<Intersection> result;

	std::for_each(intersectionList.begin(), intersectionList.end(), [&](const Intersection& intersection)
		{
			result.emplace_back(intersection);
		});

	std::sort(result.begin(), result.end(), compare);

	return result;
}

inline Intersection hit(const std::vector<Intersection>& intersections)
{
	for (size_t i = 0; i < intersections.size(); i++)
	{
		if (intersections[i].t > 0.0f)
		{
			return intersections[i];
		}
	}

	return {};
}

inline auto intersectWorld(const World& world, const Ray& ray)
{
	std::vector<Intersection> result;

	for (const auto& object : world.getObjects())
	{
		auto intersection = intersect(object, ray);

		result.insert(result.end(), intersection.begin(), intersection.end());
	}

	return intersections(result);
}

HitResult prepareComputations(const Intersection& intersection, const Ray& ray)
{
	// Instantiate a data structure for storing some precomputed values
	HitResult hitResult;

	// Copy the intersection's properties, for convenience
	hitResult.t = intersection.t;
	hitResult.object = intersection.object;

	// Precompute some useful values
	hitResult.position = ray.at(hitResult.t);
	hitResult.viewDirection = -ray.direction;
	hitResult.normal = normalAt(hitResult.object, hitResult.position);

	if (dot(hitResult.normal, hitResult.viewDirection) < 0.0f)
	{
		hitResult.inside = true;
		hitResult.normal = -hitResult.normal;
	}
	else
	{
		hitResult.inside = false;
	}

	return hitResult;
}
