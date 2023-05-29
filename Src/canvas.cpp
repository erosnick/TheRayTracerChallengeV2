#include "canvas.h"

#include "utils.h"

#include <fstream>
#include <sstream>
#include <vector>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

void Canvas::writeToPPM(const std::string& path)
{
	std::ofstream file(path);

	auto ppm = std::string();
	ppm.append("P3\n");
	ppm.append(std::to_string(width) + " " + std::to_string(height) + "\n");
	ppm.append(std::to_string(255) + "\n");

	std::stringstream ss;
	ss << ppm;

	for (int32_t y = 0; y < height; y++)
	{
		for (int32_t x = 0; x < width; x++)
		{
			auto pixel = pixelAt(x, y);

			pixel *= 255.0f;

			ss << static_cast<int32_t>(pixel.red) << " "
			   << static_cast<int32_t>(pixel.green) << " "
			   << static_cast<int32_t>(pixel.blue) << "\n";
		}
	}

	file.write(ss.str().c_str(), ss.str().size());

	file.close();
}

void Canvas::writeToPNG(const std::string& path)
{
	std::vector<uint8_t> data;

	for (int32_t y = 0; y < height; y++)
	{
		for (int32_t x = 0; x < width; x++)
		{
			auto pixel = pixelAt(x, y);

			pixel *= 255.0f;

			data.push_back(static_cast<uint8_t>(pixel.red));
			data.push_back(static_cast<uint8_t>(pixel.green));
			data.push_back(static_cast<uint8_t>(pixel.blue));
		}
	}

	stbi_write_png(path.c_str(), width, height, 3, data.data(), width * 3);
}

bool Canvas::black()
{
	bool result = true;

	auto size = width * height;

	for (int32_t i = 0; i < size; i++)
	{
		if (!equal(pixels[i], tuple(0.0f, 0.0f, 0.0f, 0.0f)))
		{
			result = false;
		}
	}

	return result;
}
