#include <iostream>

#include "colors.h"
#include "sphere.h"
#include "canvas.h"
#include "light.h"
#include "shading.h"

#include "intersection.h"

#include "timer.h"

int main(int argc, char* argv[])
{
	auto floor = createSphere();

	floor->setTransform(scale(10.0f, 0.01f, 10.0f));
	floor->material = Material();

	floor->material = Material();
	floor->material.color = color(1.0f, 0.9f, 0.9f);
	floor->material.specular = 0.0f;

		auto leftWall = createSphere();

	leftWall->setTransform(translate(0.0f, 0.0f, 5.0f) * 
									 rotateY(-PI / 4.0f) * 
									 rotateX(PI / 2.0f) * 
									 scale(10.0f, 0.05f, 10.0f));

	leftWall->material = floor->material;

	auto rightWall = createSphere();
	rightWall->setTransform(translate(0.0f, 0.0f, 5.0f) *
									  rotateY(PI / 4.0f) * rotateX(PI / 2.0f) * 
									  scale(10.0f, 0.05f, 10.0f));

	rightWall->material = floor->material;

	auto left = createSphere();
	left->setTransform(translate(-1.5f, 0.33f, -0.75f) * scale(0.33f));
	left->material = Material();
	left->material.color = color(1.0f, 0.8f, 0.1f);
	left->material.diffuse = 0.7f;
	left->material.specular = 0.3f;

	auto middle = createSphere();
	auto transform = translate(-0.5f, 1.0f, 0.5f);
	middle->setTransform(transform);
	middle->material = Material();
	middle->material.color = color(0.1f, 1.0f, 0.5f);
	middle->material.diffuse = 0.7f;
	middle->material.specular = 0.3f;

	auto right = createSphere();
	right->setTransform(translate(1.5f, 0.5f, -0.5f) * scale(0.5f));
	right->material = Material();
	right->material.color = color(0.5f, 1.0f, 0.1f);
	right->material.diffuse = 0.7f;
	right->material.specular = 0.3f;

	auto topRight = createSphere();
	topRight->setTransform(translate(1.5f, 1.5f, -0.5f) * rotateX(PI / 4.0F) * rotateZ(PI / 4.0F) * scale(0.5, 0.25f, 0.5f));
	topRight->material = Material();
	topRight->material.color = color(1.0f, 0.0f, 0.0f);
	topRight->material.diffuse = 0.7f;
	topRight->material.specular = 0.3f;

	auto light = pointLight(point(-10.0f, 10.0f, -10.0f), Color::White);

	auto world = World();
	world.addLight(light);
	world.addObject(floor);
	world.addObject(leftWall);
	world.addObject(rightWall);
	world.addObject(left);
	world.addObject(middle);
	world.addObject(right);
	world.addObject(topRight);

	auto camera = Camera(800, 400, radians(60.0f));

	camera.transform = viewTransform(point(0.0f, 1.5f, -5.0f), point(0.0f, 1.0f, 0.0f), vector(0.0f, 1.0f, 0.0f));

	AriaCore::ScopedTimer timer("Rendering");
	auto canvas = render(camera, world);

	canvas.writeToPPM("scene.ppm");
	canvas.writeToPNG("scene.png");

	return 0;
}