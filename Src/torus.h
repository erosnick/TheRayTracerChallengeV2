#pragma once

#include "shape.h"
#include "maths.h"

class Torus : public Shape
{
public:
	Torus(float inSweptRadius = 2.0f, float inTubeRadius = 1.0f)
	: sweptRadius(inSweptRadius), tubeRadius(inTubeRadius)
	{
	}

	virtual std::vector<Intersection> localIntersect(const Ray& transformedRay) override
	{ 
		double x1 = transformedRay.origin.x;
		double y1 = transformedRay.origin.y;
		double z1 = transformedRay.origin.z;

		double d1 = transformedRay.direction.x; 
		double d2 = transformedRay.direction.y; 
		double d3 = transformedRay.direction.z;

		double coeffs[5];	// coefficient array for the quartic equation
		double roots[4];	// solution array for the quartic equation

		// define the coefficients of the quartic equation

		double sum_d_sqrd = d1 * d1 + d2 * d2 + d3 * d3;
		double e = x1 * x1 + y1 * y1 + z1 * z1 - sweptRadius * sweptRadius - tubeRadius * tubeRadius;
		double f = x1 * d1 + y1 * d2 + z1 * d3;
		double four_a_sqrd = 4.0f * sweptRadius * sweptRadius;

		coeffs[0] = e * e - four_a_sqrd * (tubeRadius * tubeRadius - y1 * y1); 	// constant term
		coeffs[1] = 4.0f * f * e + 2.0f * four_a_sqrd * y1 * d2;
		coeffs[2] = 2.0f * sum_d_sqrd * e + 4.0f * f * f + four_a_sqrd * d2 * d2;
		coeffs[3] = 4.0f * sum_d_sqrd * f;
		coeffs[4] = sum_d_sqrd * sum_d_sqrd;  					// coefficient of t^4

		// find roots of the quartic equation

		int num_real_roots = Math::solveQuartic(coeffs, roots);

		bool	intersected = false;
		double 	t = std::numeric_limits<float>::max();

		if (num_real_roots == 0)  // ray misses the torus
		{
			{};
		}

		// find the smallest root greater than kEpsilon, if any
		// the roots array is not sorted
		for (int j = 0; j < num_real_roots; j++)
		{
			if (roots[j] > EPSILON_HIGH_PRECISION) {
				intersected = true;
				if (roots[j] < t)
					t = roots[j];
			}
		}

		if (!intersected)
		{
			return {};
		}

		return { { static_cast<float>(t), shared_from_this() }};
	} 

	virtual tuple localNormalAt(const tuple& localPosition, const Intersection intersection = {}) const override
	{
		auto paramSquared = sweptRadius * sweptRadius + tubeRadius * tubeRadius;

		auto x = localPosition.x;
		auto y = localPosition.y;
		auto z = localPosition.z;
		auto sumSquared = x * x + y * y + z * z;

		auto normal =  vector(
			4.0f * x * (sumSquared - paramSquared),
			4.0f * y * (sumSquared - paramSquared + 2.0f * sweptRadius * sweptRadius),
			4.0f * z * (sumSquared - paramSquared));

		return normalize(normal);
	}

	virtual bool boundingBox(BoundingBox& outputBox) override
	{
		return true;
	}

	float sweptRadius = 2.0f;
	float tubeRadius = 1.0f;
};

std::shared_ptr<Shape> createTorus(float sweptRadius = 2.0f, float tubeRadius = 1.0f)
{
	return std::make_shared<Torus>(sweptRadius, tubeRadius);
}