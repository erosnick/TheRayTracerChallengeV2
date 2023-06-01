#pragma once

#include <tuple.h>
#include <memory>

#include "colors.h"
#include "matrix.h"
#include "transforms.h"

#include "PerlinNoise.hpp"
#include "SimplexNoise.h"
#include "perlin.h"

enum class PatternType : uint8_t
{
	Strip,
	Gradient,
	Ring,
	Checker,
	RadialGradient,
	Blend
};

class Pattern
{
public:
	Pattern() {}

	Pattern(const tuple& inColor1, const tuple& inColor2)
	: color1(inColor1), color2(inColor2)
	{}

	Pattern(const std::shared_ptr<Pattern>& inPattern1, const std::shared_ptr<Pattern>& inPattern2)
		: pattern1(inPattern1), pattern2(inPattern2)
	{}

	void setTransform(const matrix4& inTransform)
	{
		transform = inTransform;
	}

	virtual tuple colorAt(const tuple& worldPosition)
	{
		return Color::White;
	}

	tuple colorAt(const tuple& worldPosition, const matrix4& objectTransform)
	{
		auto objectPosition = inverse(objectTransform) * worldPosition;
		auto patternPosition = inverse(transform) * objectPosition;

		return colorAt(patternPosition);
	}

	tuple color1 = Color::Black;
	tuple color2 = Color::White;

	std::shared_ptr<Pattern> pattern1;
	std::shared_ptr<Pattern> pattern2;

	matrix4 transform;
};

class StripPattern : public Pattern
{
public:
	StripPattern(const tuple& inColor1, const tuple& inColor2)
	: Pattern(inColor1, inColor2)
	{}

	tuple colorAt(const tuple& worldPosition) override
	{
		if (equal(std::fmodf(std::floor(worldPosition.x), 2.0f), 0.0f))
		{
			return color1;
		}

		return color2;
	}
};

inline static std::shared_ptr<Pattern> createStripPattern(const tuple& color1 = Color::White, const tuple& color2 = Color::Black)
{
	return std::make_shared<StripPattern>(color1, color2);
}

class GradientPattern : public Pattern
{
public:
	GradientPattern(const tuple& inColor1, const tuple& inColor2)
	: Pattern(inColor1, inColor2)
	{}

	tuple colorAt(const tuple& worldPosition) override
	{
		auto x = (worldPosition.x + 1.0f) * 0.5f;
		x = worldPosition.x;
		auto distance = color2 - color1;
		auto fraction = x - std::floorf(x);

		return color1 + distance * fraction;
	}
};

inline static std::shared_ptr<Pattern> createGradientPattern(const tuple& color1 = Color::White, const tuple& color2 = Color::Black)
{
	return std::make_shared<GradientPattern>(color1, color2);
}

class RingPattern : public Pattern
{
public:
	RingPattern(const tuple& inColor1, const tuple& inColor2)
	: Pattern(inColor1, inColor2)
	{}

	tuple colorAt(const tuple& worldPosition) override
	{
		auto distance = std::sqrtf(worldPosition.x * worldPosition.x + worldPosition.z * worldPosition.z);
		if (equal(std::fmodf(std::floorf(distance), 2.0f), 0.0f))
		{
			return color1;
		}

		return color2;
	}
};

inline static std::shared_ptr<Pattern> createRingPattern(const tuple& color1 = Color::White, const tuple& color2 = Color::Black)
{
	return std::make_shared<RingPattern>(color1, color2);
}

class CheckerPattern : public Pattern
{
public:
	CheckerPattern(const tuple& inColor1, const tuple& inColor2)
	: Pattern(inColor1, inColor2)
	{}

	tuple colorAt(const tuple& worldPosition) override
	{
		auto y = 0.0f;

		if (std::fabsf(worldPosition.y) > EPSILON)
		{
			y = worldPosition.y;
		}

		auto sum = std::floorf(worldPosition.x) + std::floorf(y) + std::floorf(worldPosition.z);
		if (equal(std::fmodf(sum, 2.0f), 0.0f))
		{
			return color1;
		}

		return color2;
	}
};

inline static std::shared_ptr<Pattern> createCheckerPattern(const tuple& color1 = Color::White, const tuple& color2 = Color::Black)
{
	return std::make_shared<CheckerPattern>(color1, color2);
}

class RadialGradientPattern : public Pattern
{
public:
	RadialGradientPattern(const tuple& inColor1, const tuple& inColor2)
	: Pattern(inColor1, inColor2)
	{}

	tuple colorAt(const tuple& worldPosition) override
	{
		auto x = (worldPosition.x + 1.0f) * 0.5f;
		auto z = (worldPosition.z + 1.0f) * 0.5f;
		x = worldPosition.x;
		z = worldPosition.z;

		auto distance = std::sqrtf(x * x + z * z);
		auto colorDistance = color2 - color1;
		auto fraction = distance - std::floorf(distance);

		return color1 + colorDistance * fraction;
	}
};

inline static std::shared_ptr<Pattern> createRadialGradientPattern(const tuple& color1 = Color::White, const tuple& color2 = Color::Black)
{
	return std::make_shared<RadialGradientPattern>(color1, color2);
}

class NestedPattern : public Pattern
{
public:
	NestedPattern(const std::shared_ptr<Pattern>& inPattern1, const std::shared_ptr<Pattern>& inPattern2)
	: Pattern(inPattern1, inPattern2)
	{}

	tuple colorAt(const tuple& worldPosition) override
	{
		auto y = 0.0f;

		if (std::fabsf(worldPosition.y) > EPSILON)
		{
			y = worldPosition.y;
		}

		auto patternPosition1 = inverse(pattern1->transform) * worldPosition;
		auto patternPosition2 = inverse(pattern2->transform) * worldPosition;

		auto sum = std::floorf(worldPosition.x) + std::floorf(y) + std::floorf(worldPosition.z);

		if (equal(std::fmodf(sum, 2.0f), 0.0f))
		{
			return pattern1->colorAt(patternPosition1);
		}

		return pattern2->colorAt(patternPosition2);
	}
};

inline static std::shared_ptr<Pattern> createNestedPattern(const std::shared_ptr<Pattern>& pattern1, const std::shared_ptr<Pattern>& pattern2)
{
	return std::make_shared<NestedPattern>(pattern1, pattern2);
}

class BlendPattern : public Pattern
{
public:
	BlendPattern(const std::shared_ptr<Pattern>& inPattern1, const std::shared_ptr<Pattern>& inPattern2)
	: Pattern(inPattern1, inPattern2)
	{}

	tuple colorAt(const tuple& worldPosition) override
	{
		if (pattern1 != nullptr && pattern2 != nullptr)
		{
			auto patternPosition1 = inverse(pattern1->transform) * worldPosition;
			auto patternPosition2 = inverse(pattern2->transform) * worldPosition;

			return pattern1->colorAt(patternPosition1) * 0.5f + pattern2->colorAt(patternPosition2) * 0.5f;
		}

		return Color::Black;
	}
};

inline static std::shared_ptr<Pattern> createBlendPattern(const std::shared_ptr<Pattern>& pattern1, const std::shared_ptr<Pattern>& pattern2)
{
	return std::make_shared<BlendPattern>(pattern1, pattern2);
}

class PerturbedPattern : public Pattern
{
public:
	PerturbedPattern(const std::shared_ptr<Pattern>& inPattern)
		: Pattern(inPattern, inPattern)
	{
		//const siv::PerlinNoise::seed_type seed = 123456u;
		//perlin = siv::PerlinNoise{ seed };
	}

	tuple colorAt(const tuple& worldPosition) override
	{
		//const float Scale = 0.01f;
		//const int32_t Octaves = 8;

		//const double noiseX = perlin.octave2D_01((worldPosition.x * Scale), (worldPosition.y * Scale), Octaves);
		//const double noiseY = perlin.octave2D_01((worldPosition.y * Scale), (worldPosition.z * Scale), Octaves);
		//const double noiseZ = perlin.octave2D_01((worldPosition.z * Scale), (worldPosition.x * Scale), Octaves);

		//tuple newPosition = { worldPosition.x + static_cast<float>(noiseX * 0.2f),
		//					  worldPosition.y + static_cast<float>(noiseY * 0.2f), 
		//					  worldPosition.z + static_cast<float>(noiseZ * 0.2f), 1.0f };
		//float scale = 400.f;
		//float offset_x = 5.9f;
		//float offset_y = 5.1f;
		//float offset_z = 0.05f;
		//float lacunarity = 1.99f;
		//float persistance = 0.5f;

		//const float x = static_cast<float>(worldPosition.x - 400.0f / 2 + offset_x * scale);
		//const float z = static_cast<float>(worldPosition.z - 400.0f / 2 + offset_y * scale);

		//const SimplexNoise simplex(0.1f / scale, 0.5f, lacunarity, persistance); // Amplitude of 0.5 for the 1st octave : sum ~1.0f
		//const int octaves = static_cast<int>(5 + std::log(scale)); // Estimate number of octaves needed for the current scale
		//const float noise = simplex.fractal(octaves, x, z) + offset_z;

		//tuple newPosition = { worldPosition.x + static_cast<float>(noise),
		//					  worldPosition.y + static_cast<float>(noise),
		//					  worldPosition.z + static_cast<float>(noise), 1.0f };

		perlin noise;
		vec3 p{ worldPosition.x, worldPosition.y, worldPosition.z };
		auto value = static_cast<float>(noise.noise(p)) * 0.5f;

		tuple newPosition = { worldPosition.x + value,
							  worldPosition.y + value,
							  worldPosition.z + value, 1.0f };

		if (pattern1 != nullptr)
		{
			return pattern1->colorAt(newPosition);
		}

		return Color::Black;
	}

	//siv::PerlinNoise perlin;
};

inline static std::shared_ptr<Pattern> createPerturbedPattern(const std::shared_ptr<Pattern>& pattern)
{
	return std::make_shared<PerturbedPattern>(pattern);
}
