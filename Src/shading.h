#pragma once

#include "tuple.h"
#include "material.h"
#include "light.h"
#include "colors.h"
#include "intersection.h"
#include "camera.h"
#include "canvas.h"
#include "world.h"

#include <thread>
#include <atomic>
#include <execution>
#include <mutex>

tuple lighting(const Material& material, const PointLight& light, const tuple& position, const tuple& viewDirection, const tuple& normal, float inShadow = false);
tuple lighting(const Material& material, const std::shared_ptr<Shape>& shape, const PointLight& light, const tuple& position, const tuple& viewDirection, const tuple& normal, float inShadow);

tuple shadeHit(const World& world, const HitResult& hitResult, int32_t depth = 1);
tuple colorAt(const World& world, const Ray& ray, int32_t depth = 1);
Canvas render(const Camera& camera, const World& world, bool useBackgroundColor, int32_t maxDepth);
std::vector<bool> isShadowed(const World& world, const tuple& position);
tuple reflectedColor(const World& world, const HitResult& hitResult, int32_t depth);
tuple refractedColor(const World& world, const HitResult& hitResult, int32_t depth);
float schlick(const HitResult& hitResult);

tuple lighting(const Material& material, const PointLight& light, const tuple& position, const tuple& viewDirection, const tuple& normal, float inShadow)
{
	// Combine the surface color with the light's color/intensity
	auto effectiveColor = material.color * light.intensity;

	if (material.pattern != nullptr)
	{
		effectiveColor = material.pattern->colorAt(position) * light.intensity;
	}

	// Find the direction to the light source
	auto lightDirection = normalize(light.position - position);

	// Compute the ambient contribution
	auto ambient = effectiveColor * material.ambient;

	if (inShadow)
	{
		return ambient;
	}

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

tuple lighting(const Material& material, const std::shared_ptr<Shape>& shape, const PointLight& light, const tuple& position, const tuple& viewDirection, const tuple& normal, float inShadow)
{
	// Combine the surface color with the light's color/intensity
	auto effectiveColor = material.color * light.intensity;

	if (material.pattern != nullptr)
	{
		//effectiveColor = material.pattern->colorAt(position) * light.intensity;
		effectiveColor = material.pattern->colorAt(position, shape->transform) * light.intensity;
	}

	// Find the direction to the light source
	auto lightDirection = normalize(light.position - position);

	// Compute the ambient contribution
	auto ambient = effectiveColor * material.ambient;

	if (inShadow)
	{
		return ambient;
	}

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

	float distance = length(light.position - position);
	float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	// Add the three contributions together to get the final shading
	return ambient + (diffuse + specular) * attenuation;
}

tuple shadeHit(const World& world, const HitResult& hitResult, int32_t depth)
{
	tuple finalColor;

	auto shadowResult = isShadowed(world, hitResult.overPosition);

	for (int32_t i = 0; i < world.lightCount(); i++)
	{
		//finalColor += lighting(hitResult.shape->material, world.getLights()[i], hitResult.position, hitResult.viewDirection, hitResult.normal, shadowResult[i]);
		finalColor += lighting(hitResult.shape->material, hitResult.shape, world.getLights()[i], hitResult.position, hitResult.viewDirection, hitResult.normal, shadowResult[i]);
	}

	auto reflected = reflectedColor(world, hitResult, depth);
	auto refracted = refractedColor(world, hitResult, depth);

	auto material = hitResult.shape->material;

	if (material.reflective > 0.0f && material.transparency > 0.0f)
	{
		auto reflectance = schlick(hitResult);
		return finalColor + reflected * reflectance + refracted * (1.0f - reflectance);
	}

	return finalColor + reflected + refracted;
}

inline static tuple computeBackgroundColor(const Ray& ray)
{
	auto t = 0.5f * (ray.direction.y + 1.0f);
	return (1.0f - t) * color(1.0f, 1.0f, 1.0f) + t * color(0.5f, 0.7f, 1.0f);
}

tuple colorAt(const World& world, const Ray& ray, int32_t depth)
{
	auto intersections = intersectWorld(world, ray);

	auto intersection = hit(intersections);

	auto backgroundColor = Color::Background;// computeBackgroundColor(ray);

	if (intersection.t > 0.0f)
	{
		auto hitResult = prepareComputations(intersection, ray, intersections);
		hitResult.backgroundColor = backgroundColor;
		return shadeHit(world, hitResult, depth);
	}

	return backgroundColor;
}

Canvas render(const Camera& camera, const World& world, bool useBackgroundColor, int32_t maxDepth)
{
	auto image = Canvas(camera.imageWidth, camera.imageHeight);
	constexpr int32_t samplesPerPixel = 1;

	// Total number of iterations or tasks
	int32_t pixelCount = camera.imageWidth * camera.imageHeight;

	// Variable to track completed iterations
	std::atomic_int completedIterations = 0;

	// Mutex to ensure thread-safe access to completedIterations
	std::mutex mtx;

	std::vector<int32_t> imageHorizontalIterator;
	std::vector<int32_t> imageVerticalIterator;

	imageHorizontalIterator.resize(camera.imageWidth);
	imageVerticalIterator.resize(camera.imageHeight);

	for (int32_t i = 0; i < camera.imageWidth; i++)
	{
		imageHorizontalIterator[i] = i;
	}

	for (int32_t i = 0; i < camera.imageHeight; i++)
	{
		imageVerticalIterator[i] = i;
	}

	std::cout << "Start Rendering...\n";

	std::for_each(std::execution::par, imageVerticalIterator.begin(), imageVerticalIterator.end(),
	[&](int32_t y)
	{
		// Update completedIterations atomically
		completedIterations.fetch_add(camera.imageWidth, std::memory_order_relaxed);
		printf("\rScanlines remaining: %.0f%%",  completedIterations / static_cast<float>(pixelCount) * 100.0f);

		std::for_each(std::execution::par, imageHorizontalIterator.begin(), imageHorizontalIterator.end(),
		[&, y](int32_t x)
			{
				auto finalColor = Color::Black;
				for (auto sample = 0; sample < samplesPerPixel; sample++)
				{
					auto rx = randomFloat();
					auto ry = randomFloat();
					auto ray = camera.rayForPixel(static_cast<float>(x), static_cast<float>(y));
					finalColor += colorAt(world, ray, maxDepth);
				}
				image.writePixel(x, y, finalColor / static_cast<float>(samplesPerPixel));
			});
	});

	printf("\nRendering done.\n");

	return image;
}

inline std::vector<bool> isShadowed(const World & world, const tuple & position)
{
	std::vector<bool> shadowResult(world.lightCount(), false);

	for (int32_t i = 0; i < world.lightCount(); i++)
	{
		auto toLight = world.getLight(i).position - position;

		auto distance = length(toLight);
		auto direction = normalize(toLight);

		auto ray = Ray(position, direction);
		auto intersections = intersectWorld(world, ray);
		auto intersection = hit(intersections);

		if (intersection.t > 0.0f && intersection.t < distance)
		{
			shadowResult[i] = true;

			if (!intersection.shape->material.castShadow)
			{
				shadowResult[i] = false;
			}
		}
	}

	return shadowResult;
}

tuple reflectedColor(const World& world, const HitResult& hitResult, int32_t depth)
{
	if (equal(hitResult.shape->material.reflective, 0.0f) || depth == 0)
	{
		return Color::Black;
	}

	auto reflectedRay = Ray(hitResult.overPosition, hitResult.reflectVector);
	auto color = colorAt(world, reflectedRay, depth - 1);

	return color * hitResult.shape->material.reflective;
}

tuple refractedColor(const World& world, const HitResult& hitResult, int32_t depth)
{
	if (equal(hitResult.shape->material.transparency, 0.0f) || depth == 0)
	{
		return Color::Black;
	}

	// Find the ratio of first index of refraction to the second.
	// (Yup, this is inverted from the definition of Snell's Law.)
	auto nRatio = hitResult.n1 / hitResult.n2;

	// cos(thetaI) is the same as the dot product of the two vectors
	auto cosI = dot(hitResult.viewDirection, hitResult.normal);

	// Find sin(thetaT) ^ 2 via trigonometric identity
	auto sin2T = nRatio * nRatio * (1.0f - cosI * cosI);

	if (sin2T > 1.0f)
	{
		return Color::Black;
	}

	// Find cos(thetaT) via trigonometric identity
	auto cosT = std::sqrtf(1.0f - sin2T);

	// Compute the direction of the refracted ray
	auto direction = hitResult.normal * (nRatio * cosI - cosT) - hitResult.viewDirection * nRatio;

	// Create the refracted ray
	auto refractedRay = Ray(hitResult.underPosition, direction);

	// Find the color of the refracted ray, making sure to multiply
	// by the transparency value to account for any opacity
	auto color = colorAt(world, refractedRay, depth - 1);

	return color * hitResult.shape->material.transparency;
}

float schlick(const HitResult& hitResult)
{
	// Find the cosine of the angle between the eye and normal vectors
	auto cos = dot(hitResult.viewDirection, hitResult.normal);

	// Total internal reflection can only occur if n1 > n2
	if (hitResult.n1 > hitResult.n2)
	{
		auto n = hitResult.n1 / hitResult.n2;
		auto sin2T = n * n * (1.0f - cos * cos);
		if (sin2T > 1.0f)
		{
			return 1.0f;
		}

		// Compute cosine of theta_t using trig identity
		auto cosT = std::sqrtf(1.0f - sin2T);

		// When n1 > n2, use cos(theta_t) instead
		cos = cosT;
	}

	auto r0 = std::powf(((hitResult.n1 - hitResult.n2) / (hitResult.n1 + hitResult.n2)), 2.0f);

	return r0 + (1.0f - r0) * std::powf((1.0f - cos), 5.0f);
}