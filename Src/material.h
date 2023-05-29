#pragma once

#include "tuple.h"

struct Material
{
	tuple color{ 1.0f, 1.0f, 1.0f, 0.0f };
	float ambient = 0.1f;
	float diffuse = 0.9f;
	float specular = 0.9f;
	float shininess = 200.0f;
};

bool operator==(const Material& a, const Material& b)
{
	return a.color == b.color &&
		equal(a.ambient, b.ambient) &&
		equal(a.diffuse, b.diffuse) &&
		equal(a.specular, b.specular) &&
		equal(a.shininess, b.shininess);
}