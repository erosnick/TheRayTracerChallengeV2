#pragma once

#include "ray.h"
#include <vector>
#include <memory>
#include "matrix.h"
#include "material.h"
#include "shape.h"

class MovingSphere : public Shape
{
public:
	MovingSphere() = default;

	MovingSphere(const tuple& inTranslation0 = point(0.0f),
				 const tuple& inTranslation1 = point(0.0f),
				 float inTime0 = 0.0f, float inTime1 = 0.5f)
	: translation0(inTranslation0), translation1(inTranslation1), time0(inTime0), time1(inTime1)
	{}

	virtual std::vector<Intersection> intersect(const Ray& ray) override
	{
		auto newTranslation = computeTranslation(ray.time);

		transform(0, 3) = newTranslation.x;
		transform(1, 3) = newTranslation.y;
		transform(2, 3) = newTranslation.z;

		return Shape::intersect(ray);
	}

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

	tuple computeTranslation(float time) const
	{
		return translation0 + ((time - time0) / (time1 - time0)) * (translation1 - translation0);
	}

	tuple center{ 0.0f, 0.0f, 0.0f, 1.0f };
	tuple translation0;
	tuple translation1;
	float time0 = 0.0f;
	float time1 = 0.0f;
	float radius = 1.0f;
};

inline static bool operator==(const MovingSphere& a, const MovingSphere& b)
{
	return (a.center == b.center) &&
		   (a.radius == b.radius) &&
		   (a.transform == b.transform) &&
		   (a.material == b.material);
}

inline auto createMovingSphere(const matrix4& transform = matrix4(1.0f))
{
	auto x = transform(0, 3);
	auto y = transform(1, 3);
	auto z = transform(2, 3);

	auto sphere = std::make_shared<MovingSphere>(point(x, y, z), point(x + 1.0f, y, z));

	sphere->setTransform(transform);

	return sphere;
}