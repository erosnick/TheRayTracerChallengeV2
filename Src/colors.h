#pragma once

#include "tuple.h"

namespace Color
{
	const tuple White{ 1.0f, 1.0f, 1.0f, 0.0f };
	const tuple Black{ 0.0f, 0.0f, 0.0f, 0.0f };

	const tuple Red{ 1.0f, 0.0f, 0.0f, 0.0f };
	const tuple Green{ 0.0f, 1.0f, 0.0f, 0.0f };
	const tuple Blue{ 0.0f, 0.0f, 1.0f, 0.0f };

	inline static tuple RGB(int32_t red, int32_t green, int32_t blue)
	{
		return color(red / 255.0f, green / 255.0f, blue / 255.0f);
	}
}