#pragma once

#include "ray.h"
#include <vector>

#include "matrix.h"
#include "material.h"

struct Sphere
{
	Sphere() { id++; }

	void setTransform(const matrix4& inTransform)
	{
		transform = inTransform;
	}

	static int32_t id;

	tuple center{ 0.0f, 0.0f, 0.0f, 1.0f };
	float radius = 1.0f;
	matrix4 transform;
	Material material;
};

int32_t Sphere::id = 0;

bool operator==(const Sphere& a, const Sphere& b)
{
	return (a.center == b.center) &&
		   (a.radius == b.radius) &&
		   (a.transform == b.transform) &&
		   (a.material == b.material);
}

inline auto createSphere(const matrix4& transform = matrix4(1.0f))
{
	auto sphere = std::make_shared<Sphere>();

	sphere->setTransform(transform);

	return sphere;
}