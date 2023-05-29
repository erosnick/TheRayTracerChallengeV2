#include <iostream>

#include "colors.h"
#include "sphere.h"
#include "canvas.h"
#include "light.h"
#include "shading.h"

#include "intersection.h"

int main(int argc, char* argv[])
{
	auto canvas = Canvas(400, 400);

	auto sphere = std::make_shared<Sphere>();

	sphere->setTransform(scale(1.0f, 0.5f, 1.0f));

	sphere->material = Material();
	sphere->material.color = color(1.0f, 0.2f, 1.0f);

	auto light = pointLight(point(-10.0f, 10.0f, -10.0f), Color::White);

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
				auto position = ray.at(result.t);
				auto normal = normalAt(result.object, position);
				auto viewDirection = -ray.direction;

				auto color = lighting(result.object->material, light, position, viewDirection, normal);

				canvas.writePixel(x, y, color);
			}
		}
	}

	canvas.writeToPPM("sphere.ppm");

	return 0;
}