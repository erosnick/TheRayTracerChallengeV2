#pragma once

#include <cstdint>
#include "matrix.h"
#include "ray.h"

class Camera
{
public:
	Camera(int32_t inImageWidth, int32_t inImageHeight, float inFieldOfView)
		: imageWidth(inImageWidth), imageHeight(inImageHeight), fieldOfView(inFieldOfView), pixelSize(0.0f)
	{
		auto halfView = std::tan(fieldOfView / 2.0f);
		auto aspect = static_cast<float>(inImageWidth) / imageHeight;

		if (aspect >= 1.0f)
		{
			halfWidth = halfView;
			halfHeight = halfView / aspect;
		}
		else
		{
			halfWidth = halfView * aspect;
			halfHeight = halfView;
		}

		pixelSize = (halfWidth * 2.0f) / imageWidth;
	}

	Ray rayForPixel(int32_t x, int32_t y) const
	{
		// The offset from the edge of the canvas to the pixel's center
		auto xOffset = (x + 0.5f) * pixelSize;
		auto yOffset = (y + 0.5f) * pixelSize;

		// The untransformed coordinates of the pixel in world space
		// (remember that the camera looks toward -z, so +x is to the *left*)
		auto worldX = halfWidth - xOffset;
		auto worldY = halfHeight - yOffset;

		// Using the camera matrix, transform the canvas point and the origin,
		// then compute the ray's direction vector.
		// (remember that the canvas is at z = -1)
		// inverse(transform): camera space to world space
		auto pixel = inverse(transform) * point(worldX, worldY, -1.0f);
		auto origin = inverse(transform) * point(0.0f, 0.0f, 0.0f);

		auto direction = normalize(pixel - origin);

		return { origin, direction };
	}

	int32_t imageWidth;
	int32_t imageHeight;
	float halfWidth;
	float halfHeight;
	float fieldOfView;
	matrix4 transform;
	float pixelSize;
};