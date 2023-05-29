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