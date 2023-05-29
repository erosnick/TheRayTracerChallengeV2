#pragma once

#include <memory>
#include <algorithm>

#include "sphere.h"

struct Intersection
{
	float t = 0.0f;
	std::shared_ptr<Sphere> object;
};

bool operator==(const Intersection& a, const Intersection& b)
{
	return equal(a.t, b.t) && (a.object == b.object);
}

inline std::vector<Intersection> intersect(const std::shared_ptr<Sphere>& sphere, const Ray& ray)
{
	// The vector from the sphere's center, to the ray origin
	// Remember: the sphere is centered at the world origin
	auto transformedRay = transform(ray, inverse(sphere->transform));

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

tuple normalAt(const std::shared_ptr<Sphere>& sphere, const tuple& worldPoint)
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

inline std::vector<Intersection> intersections(const std::initializer_list<Intersection>& intersectionList)
{
	std::vector<Intersection> result;

	std::for_each(intersectionList.begin(), intersectionList.end(), [&](const Intersection& intersection)
		{
			result.emplace_back(intersection);
		});

	std::sort(result.begin(), result.end(), compare);

	return result;
}

Intersection hit(const std::vector<Intersection>& intersections)
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