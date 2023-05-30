#pragma once

#include <tuple.h>
#include <memory>

#include "colors.h"

class Pattern
{
public:
	Pattern(const tuple& inColor1, const tuple& inColor2)
	: color1(inColor1), color2(inColor2)
	{}

	virtual tuple colorAt(const tuple& position)
	{
		return Color::White;
	}
	tuple color1 = Color::Black;
	tuple color2 = Color::White;
};

class StripPattern : public Pattern
{
public:
	StripPattern(const tuple& inColor1, const tuple& inColor2)
	: Pattern(inColor1, inColor2)
	{}

	tuple colorAt(const tuple& position) override
	{
		if (std::fmodf(std::floor(position.x), 2.0f) == 0)
		{
			return color1;
		}

		return color2;
	}
};

inline static std::shared_ptr<Pattern> createStripPattern(const tuple& inColor1 = Color::White, const tuple& inColor2 = Color::Black)
{
	return std::make_shared<StripPattern>(inColor1, inColor2);
}