#include <catch2/catch_test_macros.hpp>

#include <ray.h>
#include <sphere.h>

#include <canvas.h>

#include "colors.h"
#include <iostream>

#include <intersection.h>

SCENARIO("Render test", "[matrix]")
{
	auto canvas = Canvas(400, 400);

	auto sphere = std::make_shared<Sphere>();

	auto origin = point(0.0f, 0.0f, -5.0f);

	auto imageWidth = 7.0f;
	auto imageHeight = 7.0f;

	auto imageDistance = 7.0f;

	auto halfWidth = imageWidth * 0.5f;
	auto halfHeight = imageHeight * 0.5f;

	auto u = imageWidth / canvas.width;
	auto v = imageHeight / canvas.height;

	for (int32_t y = 0; y < canvas.height; y++)
	{
		std::cout << "\rScanlines remaining: " << canvas.height - y << ' ' << std::flush;
		for (int32_t x = 0; x < canvas.width; x++)
		{
			auto direction = point(-halfWidth + x * u, halfHeight - y * v, imageDistance) - origin;
			auto ray = Ray(origin, normalize(direction));

			auto intersection = intersect(sphere, ray);

			auto result = hit(intersection);

			if (result.t > 0.0)
			{
				canvas.writePixel(x, y, Color::Red);
			}
		}
	}

	canvas.writeToPPM("sphere.ppm");
}