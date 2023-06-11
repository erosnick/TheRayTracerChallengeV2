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
	float metallic = 0.0f;		// Metallic
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
	const Material White{ Colors::White, 0.1f, 0.9f, 0.3f, 300.0f, 0.0f, 0.0f, 1.0f };
	const Material Black{ Colors::Black, 0.1f, 0.9f, 0.3f, 300.0f, 0.0f, 0.0f, 1.0f };
	const Material Grey{ Colors::Grey, 0.1f, 0.9f, 0.3f, 300.0f, 0.0f, 0.0f, 1.0f };
	const Material Red{ Colors::Red, 0.1f, 0.9f, 0.3f, 300.0f, 0.0f, 0.0f, 1.0f };
	const Material Green{ Colors::Green, 0.1f, 0.9f, 0.3f, 300.0f, 0.0f, 0.0f, 1.0f };
	const Material Blue{ Colors::Blue, 0.1f, 0.9f, 0.3f, 300.0f, 0.0f, 0.0f, 1.0f };
	const Material Yellow{ Colors::Yellow, 0.1f, 0.9f, 0.3f, 300.0f, 0.0f, 0.0f, 1.0f };
	const Material Glass{ Colors::Dark, 0.1f, 0.9f, 1.0f, 300.0f, 1.0f, 1.0f, 1.52f, false };
	const Material Water{ Colors::Dark, 0.1f, 0.9f, 1.0f, 200.0f, 1.0f, 1.0f, 1.33f, false };
	const Material Diamond{ Colors::Dark, 0.1f, 0.9f, 1.0f, 200.0f, 1.0f, 1.0f, 2.417f, false };
	const Material Mirror{ Colors::Black, 0.1f, 0.9f, 1.0f, 200.0f, 1.0f, 0.0f, 1.0f };
}