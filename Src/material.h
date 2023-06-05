#pragma once

#include "tuple.h"
#include "pattern.h"

struct Material
{
	tuple color{ 1.0f, 1.0f, 1.0f, 0.0f };
	float ambient = 0.1f;
	float diffuse = 0.9f;
	float specular = 0.9f;
	float shininess = 200.0f;
	float reflective = 0.0f;
	float transparency = 0.0f;
	float refractiveIndex = 1.0f;
	bool castShadow = true;
	std::shared_ptr<Pattern> pattern;
};

inline static bool operator==(const Material& a, const Material& b)
{
	return a.color == b.color &&
	equal(a.ambient, b.ambient) &&
	equal(a.diffuse, b.diffuse) &&
	equal(a.specular, b.specular) &&
	equal(a.shininess, b.shininess);
}

namespace Materials
{
	const Material White{ Color::White, 0.1f, 0.9f, 0.3f, 300.0f, 0.0f, 0.0f, 1.0f };
	const Material Red{ Color::Red, 0.1f, 0.9f, 0.3f, 300.0f, 0.0f, 0.0f, 1.0f };
	const Material Glass{ Color::Dark, 0.1f, 0.9f, 1.0f, 300.0f, 1.0f, 1.0f, 1.52f, false };
	const Material Water{ Color::Dark, 0.1f, 0.9f, 1.0f, 200.0f, 1.0f, 1.0f, 1.33f, false };
	const Material Diamond{ Color::Dark, 0.1f, 0.9f, 1.0f, 200.0f, 1.0f, 1.0f, 2.417f, false };
	const Material Mirror{ Color::Black, 0.1f, 0.9f, 1.0f, 200.0f, 1.0f, 0.0f, 1.0f };
}