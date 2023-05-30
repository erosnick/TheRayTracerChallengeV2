#pragma once

#include <tuple.h>
#include <memory>

#include "colors.h"
#include "matrix.h"
#include "transforms.h"

class Pattern
{
public:
	Pattern(const tuple& inColor1, const tuple& inColor2)
	: color1(inColor1), color2(inColor2)
	{}

	void setTransform(const matrix4& inTransform)
	{
		transform = inTransform;
	}

	virtual tuple colorAt(const tuple& worldPosition)
	{
		return Color::White;
	}

	virtual tuple colorAt(const tuple& worldPosition, const matrix4& objectTransform)
	{
		return Color::White;
	}

	tuple color1 = Color::Black;
	tuple color2 = Color::White;

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
		if (std::fmodf(std::floor(worldPosition.x), 2.0f) == 0)
		{
			return color1;
		}

		return color2;
	}

	tuple colorAt(const tuple& worldPosition, const matrix4& objectTransform) override
	{
		auto objectPosition = inverse(objectTransform) * worldPosition;
		auto patternPosition = inverse(transform) * objectPosition;

		return colorAt(patternPosition);
	}
};

inline static std::shared_ptr<Pattern> createStripPattern(const tuple& inColor1 = Color::White, const tuple& inColor2 = Color::Black)
{
	return std::make_shared<StripPattern>(inColor1, inColor2);
}