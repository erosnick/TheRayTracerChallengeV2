#pragma once

#include "world.h"
#include "camera.h"

struct Scene
{
	Scene() {}

	World world;
	Camera camera;
};