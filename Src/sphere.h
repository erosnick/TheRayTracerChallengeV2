#pragma once

#include "ray.h"
#include <vector>
#include <memory>
#include "matrix.h"
#include "material.h"
#include "shape.h"

class Sphere : public Shape
{
public:
	Sphere() { }

	virtual std::vector<Intersection> localIntersect(const Ray& transformedRay) override 
	{ 
		// The vector from the sphere's center, to the ray origin
		// Remember: the sphere is centered at the world origin
		auto sphereToRay = transformedRay.origin - center;

		auto a = dot(transformedRay.direction, transformedRay.direction);
		auto b = 2.0f * dot(transformedRay.direction, sphereToRay);
		auto c = dot(sphereToRay, sphereToRay) - radius;

		auto discriminant = b * b - 4.0f * a * c;

		if (discriminant < 0.0f)
		{
			return {};
		}

		auto t1 = (-b - std::sqrtf(discriminant)) / (2.0f * a);
		auto t2 = (-b + std::sqrtf(discriminant)) / (2.0f * a);

		auto shape = shared_from_this();

		// https://blog.csdn.net/u013745174/article/details/52900870
		auto object = std::dynamic_pointer_cast<Sphere>(shared_from_this());

		return { { t1, shape, object, 0.0f, 0.0f },
				 { t2, shape, object, 0.0f, 0.0f } };
	}

	virtual tuple localNormalAt(const tuple& localPosition, const Intersection intersection = {}) const override
	{ 
		auto localNormal = localPosition - center;

		return  localNormal;
	}

	virtual bool boundingBox(BoundingBox& outputBox) override
	{
		auto a = translation - vector(scale.x, scale.y, scale.z);
		auto b = translation + vector(scale.x, scale.y, scale.z);

		outputBox = BoundingBox(a, b);

		return true;
	}

	tuple center{ 0.0f, 0.0f, 0.0f, 1.0f };
	float radius = 1.0f;
};

inline static bool operator==(const Sphere& a, const Sphere& b)
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

inline auto createGlassSphere(const matrix4& transform = matrix4(1.0f))
{
	auto sphere = std::make_shared<Sphere>();

	sphere->setTransform(transform);

	sphere->material.transparency = 1.0f;
	sphere->material.refractiveIndex = 1.5f;

	return sphere;
}