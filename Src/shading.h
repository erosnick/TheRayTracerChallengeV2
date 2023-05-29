#pragma once

#include "tuple.h"
#include "material.h"
#include "light.h"
#include "colors.h"
#include "intersection.h"
#include "camera.h"
#include "canvas.h"

tuple lighting(const Material& material, const PointLight& light, const tuple& position, const tuple& viewDirection, const tuple& normal)
{
	// Combine the surface color with the light's color/intensity
	auto effectiveColor = material.color * light.intensity;

	// Find the direction to the light source
	auto lightDirection = normalize(light.position - position);

	// Compute the ambient contribution
	auto ambient = effectiveColor * material.ambient;

	// lightDotNormal represents the cosine of the angle between the
	// light vector and the normal vector.A negative number means the
	// light is on the other side of the surface
	auto lightDotNormal = dot(lightDirection, normal);

	auto diffuse = Color::Black;
	auto specular = Color::Black;

	if (lightDotNormal < 0.0f)
	{
		diffuse = Color::Black;
		specular = Color::Black;
	}
	else
	{
		// Compute the diffuse contribution
		diffuse = effectiveColor * material.diffuse * lightDotNormal;

		// reflectDotEye represents the cosine of the angle between the
		// reflection vector and the eye vector.A negative number means the
		// light reflects away from the eye.
		auto reflectVector = reflect(-lightDirection, normal);
		auto reflectDotEye = dot(reflectVector, viewDirection);

		if (reflectDotEye <= 0.0f)
		{
			specular = Color::Black;
		}
		else
		{
			// Compute the specular contribution
			auto factor = pow(reflectDotEye, material.shininess);
			specular = light.intensity * material.specular * factor;
		}
	}

	// Add the three contributions together to get the final shading
	return ambient + diffuse + specular;
}

inline tuple shadeHit(const World& world, const HitResult& hitResult)
{
	return lighting(hitResult.object->material, world.getLights()[0], hitResult.position, hitResult.viewDirection, hitResult.normal);
}

tuple colorAt(const World& world, const Ray& ray)
{
	auto intersections = intersectWorld(world, ray);

	auto intersection = hit(intersections);

	if (intersection.t > 0.0f)
	{
		auto hitResult = prepareComputations(intersection, ray);

		return shadeHit(world, hitResult);
	}

	auto t = 0.5f * (ray.direction.y + 1.0f);
	return (1.0f - t) * color(1.0f, 1.0f, 1.0f) + t * color(0.5f, 0.7f, 1.0f);
}

Canvas render(const Camera& camera, const World& world)
{
	auto image = Canvas(camera.imageWidth, camera.imageHeight);

	for (int32_t y = 0; y < camera.imageHeight; y++)
	{
		std::cerr << "\rScanlines remaining: " << camera.imageHeight - y << ' ' << std::flush;
		for (int32_t x = 0; x < camera.imageWidth; x++)
		{
			auto ray = camera.rayForPixel(x, y);
			auto color = colorAt(world, ray);
			image.writePixel(x, y, color);
		}
	}

	return image;
}