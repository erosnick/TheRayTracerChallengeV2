#pragma once

#include "tuple.h"
#include "material.h"
#include "light.h"
#include "colors.h"

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