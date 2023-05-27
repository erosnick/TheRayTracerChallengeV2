#include "canvas.h"

#include "utils.h"

#include <fstream>

#include <omp.h>

void Canvas::writeToPPM(const std::string& path)
{
	std::ofstream file(path);

	file << "P3\n";
	file << width << " " << height << std::endl;
	file << 255 << std::endl;

	char buffer[32];

	for (int32_t y = 0; y < height; y++)
	{
		for (int32_t x = 0; x < width; x++)
		{
			auto pixel = pixelAt(x, y);

			pixel *= 255.0f;

			sprintf_s(buffer, "%d %d %d", static_cast<uint32_t>(pixel.red), 
														  static_cast<uint32_t>(pixel.green), 
														  static_cast<uint32_t>(pixel.blue));

			file << buffer << std::endl;
		}
	}

	file.close();
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
