#pragma once

#include <vector>
#include <memory>
#include <algorithm>

#include "ray.h"

struct Intersection
{
	float t = 0.0f;
	std::shared_ptr<class Shape> shape;
	std::shared_ptr<class Sphere> object;
};

struct HitResult
{
	float t = 0.0f;
	std::shared_ptr<class Shape> shape;
	tuple position;
	tuple viewDirection;
	tuple normal;
	bool inside = false;
	tuple overPosition;
};

inline static bool operator==(const Intersection& a, const Intersection& b)
{
	return equal(a.t, b.t) && (a.shape == b.shape);
}

// No long needed, just preserve for old test cases
std::vector<Intersection> intersect(const std::shared_ptr<class Sphere>& sphere, const Ray& ray);

// No long needed, just preserve for old test cases
tuple normalAt(const std::shared_ptr<class Sphere>& sphere, const tuple& worldPoint);

// No long needed, just preserve for old test cases
tuple normalAt(const std::shared_ptr<class Shape>& shape, const tuple& worldPoint);

inline static bool compare(const Intersection& a, const Intersection& b)
{
	return a.t < b.t;
}

std::vector<Intersection> intersections(const std::initializer_list<Intersection>& intersectionList);

std::vector<Intersection> intersections(const std::vector<Intersection>& intersectionList);

Intersection hit(const std::vector<Intersection>& intersections);

std::vector<Intersection> intersectWorld(const class World& world, const Ray& ray);

HitResult prepareComputations(const Intersection& intersection, const Ray& ray);