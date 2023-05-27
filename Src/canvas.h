#pragma once

#include <cstdint>
#include <memory>
#include <string>

#include <tuple.h>

class Canvas
{
public:
	Canvas(int32_t inWidth, int32_t inHeight)
	: width(inWidth), height(inHeight)
	{
		pixels = std::make_unique<tuple[]>(width * height);
	}

	inline void writePixel(int32_t x, int32_t y, const tuple& color)
	{
		if ((x < 0 || x > width - 1) || (y < 0 || y > height - 1))
		{
			return;
		}

		tuple result = color;

		result.red = clamp(color.red, 0.0f, 1.0f);
		result.green = clamp(color.green, 0.0f, 1.0f);
		result.blue = clamp(color.blue, 0.0f, 1.0f);

		pixels[y * width + x] = result;
	}

	inline tuple pixelAt(int32_t x, int32_t y) const
	{
		return pixels[y * width + x];
	}

	void writeToPPM(const std::string& path);

	bool black();
public:
	std::unique_ptr<tuple[]> pixels;
	int32_t width;
	int32_t height;
};