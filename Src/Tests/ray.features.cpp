#include <catch2/catch_test_macros.hpp>

#include <ray.h>
#include <sphere.h>

#include <canvas.h>

#include "colors.h"
#include <iostream>

// Chapter 5 Ray-Sphere Intersections

SCENARIO("Creating and querying a ray", "[matrix]")
{
	GIVEN("origin = point(1.0f, 2.0f, 3.0f)"
		"And direction = vector(4.0f, 5.0f, 6.0f)")
	{
		auto origin = point(1.0f, 2.0f, 3.0f);
		auto direction = vector(4.0f, 5.0f, 6.0f);

		WHEN("r = Ray(origin, direction)")
		{
			auto r = Ray(origin, direction);

			THEN("r.origin == origin"
				"And r.direction == direction")
			{
				REQUIRE(r.origin == origin);
				REQUIRE(r.direction == direction);
			}
		}
	}
}

SCENARIO("Computing a point from a distance", "[ray]")
{
	GIVEN("r = Ray(point(2.0f, 3.0f, 4.0f), vector(1.0f, 0.0f, 0.0f))")
	{
		auto r = Ray(point(2.0f, 3.0f, 4.0f), vector(1.0f, 0.0f, 0.0f));

		THEN("position(r, 0.0f) = point(2.0f, 3.0f, 4.0f)"
			"And position(r, 1.0f) = point(3.0f, 3.0f, 4.0f)"
			"And position(r, -1.0f) = point(1.0f, 3.0f, 4.0f)"
			"And position(r, 2.5f) = point(4.5f, 3.0f, 4.0f)")
		{
			REQUIRE(r.at(0.0f) == point(2.0f, 3.0f, 4.0f));
			REQUIRE(r.at(1.0f) == point(3.0f, 3.0f, 4.0f));
			REQUIRE(r.at(-1.0f) == point(1.0f, 3.0f, 4.0f));
			REQUIRE(r.at(2.5f) == point(4.5f, 3.0f, 4.0f));
		}
	}
}

SCENARIO("Translating a ray", "[matrix]")
{
	GIVEN("r = ray(point(1.0f, 2.0f, 3.0f), vector(0.0f, 1.0f, 0.0f))"
		"And m = translation(3.0f, 4.0f, 5.0f)")
	{
		auto r = Ray(point(1.0f, 2.0f, 3.0f), vector(0.0f, 1.0f, 0.0f));
		auto m = translate(3.0f, 4.0f, 5.0f);

		WHEN("r2 = transform(r, m)")
		{
			auto r2 = transform(r, m);

			THEN("r2.origin == point(4.0f, 6.0f, 8.0f)"
				"And r2.direction == vector(0.0f, 1.0f, 0.0f)")
			{
				REQUIRE(r2.origin == point(4.0f, 6.0f, 8.0f));
				REQUIRE(r2.direction == vector(0.0f, 1.0f, 0.0f));
			}
		}
	}
}

SCENARIO("Scaling a ray", "[matrix]")
{
	GIVEN("r = ray(point(1.0f, 2.0f, 3.0f), vector(0.0f, 1.0f, 0.0f))"
		"And m = scale(2.0f, 3.0f, 4.0f)")
	{
		auto r = Ray(point(1.0f, 2.0f, 3.0f), vector(0.0f, 1.0f, 0.0f));
		auto m = scale(2.0f, 3.0f, 4.0f);

		WHEN("r2 = transform(r, m)")
		{
			auto r2 = transform(r, m);

			THEN("r2.origin == point(2.0f, 6.0f, 12.0f)"
				"And r2.direction == vector(0.0f, 3.0f, 0.0f)")
			{
				REQUIRE(r2.origin == point(2.0f, 6.0f, 12.0f));
				REQUIRE(r2.direction == vector(0.0f, 3.0f, 0.0f));
			}
		}
	}
}

SCENARIO("Render test", "[matrix]")
{
	auto canvas = Canvas(100, 100);

	auto sphere = std::make_shared<Sphere>();

	auto origin = vector(0.0f, 0.0f, -5.0f);

	auto halfWidth = canvas.width * 0.5f;
	auto halfHeight = canvas.height * 0.5f;

	auto u = 1.0f / canvas.width;
	auto v = 1.0f / canvas.height;

	for (int32_t y = 0; y < canvas.height; y++)
	{
		std::cout << "\rScanlines remaining: " << canvas.height - y << ' ' << std::flush;
		for (int32_t x = 0; x < canvas.width; x++)
		{
			auto direction = vector(-halfWidth + x * u, -halfHeight + y * v, 0.0f) - origin;
			auto ray = Ray(origin, normalize(direction));

			auto intersection = intersect(sphere, ray);

			if (intersection.size() > 0)
			{
				auto intersectionList = intersections({ intersection[0], intersection[1] });

				auto result = hit(intersectionList);

				if (result.t > 0.0)
				{
					canvas.writePixel(x, y, Color::Red);
				}
			}
		}
	}

	canvas.writeToPPM("sphere.ppm");
}