#include <iostream>

#include "colors.h"
#include "canvas.h"
#include "light.h"
#include "shading.h"

#include "intersection.h"
#include "sphere.h"
#include "plane.h"
#include "timer.h"

#include "YAMLLoader.h"

World shadowTest()
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
	world.setName("ShadowTest");

	world.addLight(light);
	world.addObject(floor);
	world.addObject(leftWall);
	world.addObject(rightWall);
	world.addObject(left);
	world.addObject(middle);
	world.addObject(right);
	world.addObject(topRight);

	return world;
}

World planeTest()
{
	auto floor = createPlane();

	floor->setTransform(scale(0.5f, 0.5f, 0.5f));
	floor->material = Material();

	floor->material = Material();
	floor->material.color = color(1.0f, 0.9f, 0.9f);
	floor->material.specular = 0.0f;
	//floor->material.pattern = createCheckerPattern();
	////floor->material.pattern = createStripPattern();
	////floor->material.pattern->setTransform(rotateY(PI / 2.0f));
	//floor->material.pattern = createGradientPattern();
	//floor->material.pattern->setTransform(scale(5.0f, 5.0f, 5.0f));
	auto pattern1 = createStripPattern(Color::DarkGreen, Color::White);
	pattern1->setTransform(rotateY(PI / 2.0f));
	auto pattern2 = createStripPattern(Color::DarkGreen, Color::White);
	floor->material.pattern = createBlendPattern(pattern1, pattern2);

	pattern1 = createStripPattern(Color::RGB(146, 216, 250), Color::RGB(239, 134, 198));
	pattern1->setTransform(rotateY(PI / 6.0f) * scale(0.25f));
	pattern2 = createStripPattern();
	pattern2->setTransform(rotateY(-PI / 6.0f) * scale(0.25f));
	floor->material.pattern = createNestedPattern(pattern1, pattern2);
	//floor->material.pattern = createPerturbedPattern(createStripPattern());

	auto leftWall = createPlane();

	leftWall->material = Material();
	leftWall->material.color = color(1.0f, 0.9f, 0.9f);
	leftWall->setTransform(translate(0.0f, 0.0f, 5.0f) * rotateY(-PI / 4.0f) * rotateX(PI / 2.0f));

	auto rightWall = createPlane();

	rightWall->material = Material();
	rightWall->material.pattern = createRadialGradientPattern();
	rightWall->material.pattern->setTransform(scale(0.5f));
	rightWall->setTransform(translate(0.0f, 0.0f, 5.0f) * rotateY(PI / 4.0f) * rotateX(PI / 2.0f));

	auto ceiling = createPlane();

	ceiling->material = floor->material;
	ceiling->setTransform(translate(0.0f, 0.0f, 0.0f));

	auto platform = createPlane(0.5f, 0.5f);

	platform->material = floor->material;
	platform->material.pattern = createStripPattern();
	platform->material.pattern->setTransform(scale(0.25f));
	platform->setTransform(translate(1.0f, 1.0f, -5.0f));

	auto left = createSphere();
	left->setTransform(translate(-1.5f, 0.33f, -0.75f) * scale(0.33f));
	left->material = Material();
	left->material.color = color(1.0f, 0.8f, 0.1f);
	left->material.diffuse = 0.7f;
	left->material.specular = 0.3f;

	auto middle = createSphere();

	auto transform = translate(-0.5f, 1.0f, 0.5f) * rotateY(-PI / 3.0f) * rotateZ(-PI / 9.0f);
	middle->setTransform(transform);
	middle->material = Material();
	middle->material.pattern = createStripPattern(Color::RGB(14, 142, 71), Color::RGB(19, 192, 96));
	middle->material.pattern->setTransform(translate(0.3f, 0.0f, 0.0f) * scale(0.125f));
	middle->material.color = color(0.1f, 1.0f, 0.5f);
	middle->material.diffuse = 0.7f;
	middle->material.specular = 0.3f;

	auto right = createSphere();
	right->setTransform(translate(1.5f, 0.5f, -0.5f) * scale(0.5f));
	right->material = Material();
	right->material.color = color(0.5f, 1.0f, 0.1f);
	right->material.diffuse = 0.7f;
	right->material.specular = 0.3f;
	right->material.pattern = createGradientPattern(Color::RGB(146, 216, 250), Color::RGB(239, 134, 198));
	right->material.pattern->setTransform(scale(1.0f));

	auto topRight = createSphere();

	topRight->setTransform(translate(1.5f, 2.0f, -0.5f) * rotateX(PI / 4.0f) * rotateZ(PI / 4.0f) * scale(0.5, 0.25f, 0.5f));
	topRight->material = Material();
	topRight->material.color = color(1.0f, 0.0f, 0.0f);
	topRight->material.diffuse = 0.7f;
	topRight->material.specular = 0.3f;

	auto light = pointLight(point(-10.0f, 10.0f, -10.0f), Color::White);

	auto world = World();

	world.setName("PlaneTest");

	world.addLight(light);
	world.addObject(floor);
	world.addObject(leftWall);
	world.addObject(ceiling);
	world.addObject(platform);
	world.addObject(rightWall);
	world.addObject(left);
	world.addObject(middle);
	world.addObject(right);
	world.addObject(topRight);

	return world;
}

int main(int argc, char* argv[])
{
	auto world = shadowTest();

	world = planeTest();

	auto scene = loadScene("Assets/Scenes/cornellbox_empty.yaml");

	world.addObject(scene.world.getObject(0));

	AriaCore::Timer timer("Rendering");
	auto canvas = render(scene.camera, world);
	timer.PrintElaspedMillis();

	canvas.writeToPPM(world.getName());
	canvas.writeToPNG(world.getName());

	return 0;
}