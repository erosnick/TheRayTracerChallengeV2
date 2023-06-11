#include "pch.h"
#include "FileWatch.hpp"

#ifdef RGB
#undef RGB
#endif

#include "canvas.h"
#include "light.h"
#include "shading.h"

#include "intersection.h"
#include "sphere.h"
#include "plane.h"
#include "cube.h"
#include "cylinder.h"
#include "cone.h"
#include "group.h"
#include "triangle.h"
#include "csg.h"
#include "bvh.h"

#include "objLoader.h"
#include "YAMLLoader.h"

#define r radians
#define RX rotateX
#define RY rotateY
#define RZ rotateZ
#define S scale
#define T translate

Scene createDefaultScene(int32_t imageWidth = 320, int32_t imageHeight = 180)
{
	Scene scene;

	auto floor = createPlane();
	floor->material.pattern = createCheckerPattern(Colors::Grey, Colors::White);

	scene.world.addObject(floor);

	auto wall = createPlane();
	wall->setTransform(T(0.0f, 0.0f, 0.0f) * RX(r(90.0f)));

	wall->material.pattern = createCheckerPattern(Colors::Grey, Colors::White);

	scene.world.addObject(wall);

	auto light = pointLight(point(0.0f, 10.0f, -10.0f), Colors::White);

	scene.world.addLight(light);

	scene.camera = Camera(imageWidth, imageHeight, r(60.0f));
	scene.camera.transform = viewTransform(point(0.0f, 5.0f, -10.0f),
											  point(0.0f, 0.0f, 0.0f),
											  vector(0.0f, 1.0f, 0.0f));

	return scene;
}

Scene emptyCornelBox(int32_t imageWidth = 320, int32_t imageHeight = 180)
{
	Scene scene;

	auto floor = createPlane(0.5f, 0.5f);
	floor->setTransform(T(0.0f, -0.5f, -1.0f) * S(1.0f, 1.0f, 3.0f));
	floor->material.specular = 0.0f;

	scene.world.addObject(floor);

	auto ceiling = createPlane(0.5f, 0.5f);
	ceiling->setTransform(T(0.0f, 0.5f, -1.0f) * S(1.0f, 1.0f, 3.0f));
	ceiling->material.specular = 0.0f;

	scene.world.addObject(ceiling);

	auto leftWall = createPlane(0.5f, 0.5f);
	leftWall->setTransform(T(-0.5f, 0.0f, -1.0f) * RZ(r(90.0f)) * S(1.0f, 1.0f, 3.0f));
	leftWall->material.color = color(0.75f, 0.25f, -1.25f);
	leftWall->material.specular = 0.0f;

	scene.world.addObject(leftWall);

	auto middleWall = createPlane(0.5f, 0.5f);
	middleWall->setTransform(T(0.0f, 0.0f, 0.5f) * RX(r(90.0f)));
	middleWall->material.specular = 0.0f;

	scene.world.addObject(middleWall);

	auto rightWall = createPlane(0.5f, 0.5f);
	rightWall->setTransform(T(0.5f, 0.0f, -1.0f) * RZ(r(90.0f)) * S(1.0f, 1.0f, 3.0f));
	rightWall->material.color = color(0.25f, 0.75f, 0.25f);
	rightWall->material.specular = 0.0f;

	scene.world.addObject(rightWall);

	auto light = pointLight(point(0.0f, 0.0f, -0.5f), Colors::HalfWhite);

	scene.world.addLight(light);

	scene.camera = Camera(imageWidth, imageHeight, r(60.0f));
	scene.camera.transform = viewTransform(point(0.0f, 0.0f, -1.25f),
											      point(0.0f, 0.0f, 0.0f),
											      vector(0.0f, 1.0f, 0.0f));
	return scene;
}

World shadowTest()
{
	auto floor = createSphere();

	floor->setScale(10.0f, 0.01f, 10.0f);
	floor->setTransform(S(10.0f, 0.01f, 10.0f));
	floor->material = Material();

	floor->material = Material();
	floor->material.color = color(1.0f, 0.9f, 0.9f);
	floor->material.specular = 0.0f;

	auto leftWall = createSphere();

	leftWall->setScale(0.0f, 0.05f, 10.0f);
	leftWall->setRotation(r(90.0f), -r(45.0f), 0.0f);
	leftWall->setTranslation(0.0f, 0.0f, 5.0f);

	leftWall->setTransform(T(0.0f, 0.0f, 5.0f) *
									  RY(-r(45.0f)) *
									  RX(r(90.0f)) *
									  S(10.0f, 0.05f, 10.0f));

	leftWall->material = floor->material;

	auto rightWall = createSphere();

	rightWall->setScale(10.0f, 0.05f, 10.0f);
	rightWall->setRotation(r(90.0f), r(45.0f), 0.0f);
	rightWall->setTranslation(0.0f, 0.0f, 5.0f);
	rightWall->setTransform(T(0.0f, 0.0f, 5.0f) *
									   RY(r(45.0f)) * RX(r(90.0f)) *
									   S(10.0f, 0.05f, 10.0f));

	rightWall->material = floor->material;

	auto left = createSphere();
	left->setScale(0.33f, 0.33f, 0.33f);
	left->setTranslation(-1.5f, 0.33f, -0.75f);
	left->setTransform(T(-1.5f, 0.33f, -0.75f) * S(0.33f));
	left->material = Material();
	left->material.color = color(1.0f, 0.8f, 0.1f);
	left->material.diffuse = 0.7f;
	left->material.specular = 0.3f;

	auto middle = createSphere();
	auto transform = T(-0.5f, 1.0f, 0.5f);
	middle->setTranslation(-0.5f, 1.0f, 0.5f);
	middle->setTransform(transform);
	middle->material = Material();
	middle->material.color = color(0.1f, 1.0f, 0.5f);
	middle->material.diffuse = 0.7f;
	middle->material.specular = 0.3f;

	auto right = createSphere();
	right->setScale(0.5f, 0.5f, 0.5f);
	right->setTranslation(1.5f, 0.5f, -0.5f);
	right->setTransform(T(1.5f, 0.5f, -0.5f) * S(0.5f));
	right->material = Material();
	right->material.color = color(0.5f, 1.0f, 0.1f);
	right->material.diffuse = 0.7f;
	right->material.specular = 0.3f;

	auto topRight = createSphere();
	topRight->setScale(0.5, 0.25f, 0.5f);
	topRight->setRotation(r(45.0f), 0.0f, r(45.0f));
	topRight->setTranslation(1.5f, 1.5f, -0.5f);;
	topRight->setTransform(T(1.5f, 1.5f, -0.5f) * RX(r(45.0f)) * RZ(r(45.0f)) * S(0.5, 0.25f, 0.5f));
	topRight->material = Material();
	topRight->material.color = color(1.0f, 0.0f, 0.0f);
	topRight->material.diffuse = 0.7f;
	topRight->material.specular = 0.3f;

	auto light = pointLight(point(-10.0f, 10.0f, -10.0f), Colors::White);

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

Scene planeTest()
{
	auto floor = createPlane();

	floor->setScale(0.5f, 0.5f, 0.5f);
	floor->setTransform(S(0.5f, 0.5f, 0.5f));
	floor->material = Material();

	floor->material = Material();
	floor->material.color = color(1.0f, 0.9f, 0.9f);
	floor->material.specular = 0.0f;
	//floor->material.pattern = createCheckerPattern();
	////floor->material.pattern = createStripPattern();
	////floor->material.pattern->setTransform(RY(r(90.0f)));
	//floor->material.pattern = createGradientPattern();
	//floor->material.pattern->setTransform(S(5.0f, 5.0f, 5.0f));
	auto pattern1 = createStripPattern(Colors::DarkGreen, Colors::White);
	pattern1->setTransform(RY(r(90.0f)));
	auto pattern2 = createStripPattern(Colors::DarkGreen, Colors::White);
	floor->material.pattern = createBlendPattern(pattern1, pattern2);

	pattern1 = createStripPattern(Colors::RGB(146, 216, 250), Colors::RGB(239, 134, 198));
	pattern1->setTransform(RY(r(30.0f)) * S(0.25f));
	pattern2 = createStripPattern();
	pattern2->setTransform(RY(-r(30.0f)) * S(0.25f));
	floor->material.pattern = createNestedPattern(pattern1, pattern2);
	//floor->material.pattern = createPerturbedPattern(createStripPattern());

	auto leftWall = createPlane();

	leftWall->material = Material();
	leftWall->material.color = color(1.0f, 0.9f, 0.9f);
	leftWall->setTranslation(0.0f, 0.0f, 5.0f);
	leftWall->setRotation(r(90.0f), -r(45.0f), 0.0f);
	leftWall->setTransform(T(0.0f, 0.0f, 5.0f) * RY(-r(45.0f)) * RX(r(90.0f)));

	auto rightWall = createPlane();

	rightWall->material = Material();
	rightWall->material.pattern = createRadialGradientPattern();
	rightWall->material.pattern->setTransform(S(0.5f));
	rightWall->setTranslation(0.0f, 0.0f, 5.0f);
	rightWall->setRotation(r(90.0f), r(45.0f), 0.0f);
	rightWall->setTransform(T(0.0f, 0.0f, 5.0f) * RY(r(45.0f)) * RX(r(90.0f)));

	auto ceiling = createPlane();

	ceiling->material = floor->material;
	ceiling->setTranslation(0.0f, 0.0f, 0.0f);
	ceiling->setTransform(T(0.0f, 0.0f, 0.0f));

	auto platform = createPlane(0.5f, 0.5f);

	platform->material = floor->material;
	platform->material.pattern = createStripPattern();
	platform->material.pattern->setTransform(S(0.25f));
	platform->setTranslation(1.0f, 1.0f, -5.0f);
	platform->setTransform(T(1.0f, 1.0f, -5.0f));

	auto left = createSphere();
	left->setScale(0.33f, 0.33f, 0.33f);
	left->setTranslation(-1.5f, 0.33f, -0.75f);
	left->setTransform(T(-1.5f, 0.33f, -0.75f) * S(0.33f));
	left->material = Material();
	left->material.color = color(1.0f, 0.8f, 0.1f);
	left->material.diffuse = 0.7f;
	left->material.specular = 0.3f;

	auto middle = createSphere();

	auto transform = T(-0.5f, 1.0f, 0.5f) * RY(-r(60.0f)) * RZ(-r(20.0f));
	
	middle->setRotation(0.0f, -r(60.0f), -r(20.0f));
	middle->setTranslation(-0.5f, 1.0f, 0.5f);
	middle->setTransform(transform);
	middle->material = Material();
	middle->material.pattern = createStripPattern(Colors::RGB(14, 142, 71), Colors::RGB(19, 192, 96));
	middle->material.pattern->setTransform(T(0.3f, 0.0f, 0.0f) * S(0.125f));
	middle->material.color = color(0.1f, 1.0f, 0.5f);
	middle->material.diffuse = 0.7f;
	middle->material.specular = 0.3f;

	auto right = createSphere();
	right->setTranslation(1.5f, 0.5f, -0.5f);	
	right->setScale(0.5f, 0.5f, 0.5f);
	right->setTransform(T(1.5f, 0.5f, -0.5f) * S(0.5f));
	right->material = Material();
	right->material.color = color(0.5f, 1.0f, 0.1f);
	right->material.diffuse = 0.7f;
	right->material.specular = 0.3f;
	right->material.pattern = createGradientPattern(Colors::RGB(146, 216, 250), Colors::RGB(239, 134, 198));
	right->material.pattern->setTransform(S(1.0f));

	auto topRight = createSphere();

	topRight->setScale(0.5, 0.25f, 0.5f);
	topRight->setRotation(r(45.0f), 0.0f, r(45.0f));
	topRight->setTranslation(1.5f, 2.0f, -0.5f);
	topRight->setTransform(T(1.5f, 2.0f, -0.5f) * RX(r(45.0f)) * RZ(r(45.0f)) * S(0.5, 0.25f, 0.5f));
	topRight->material = Material();
	topRight->material.color = color(1.0f, 0.0f, 0.0f);
	topRight->material.diffuse = 0.7f;
	topRight->material.specular = 0.3f;

	auto light = pointLight(point(-10.0f, 10.0f, -10.0f), Colors::White);

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

	Scene scene;
	scene.world = world;

	scene.camera = Camera(1280, 720, r(60.0f));
	scene.camera.transform = viewTransform(point(0.0f, 2.0f, -10.0f), point(0.0f, 0.0f, 0.0f), vector(0.0f, 1.0f, 0.0f));

	return scene;
}

Scene reflectionTest()
{
	auto floor = createPlane();

	floor->setScale(0.5f, 0.5f, 0.5f);
	floor->setTransform(S(0.5f, 0.5f, 0.5f));
	floor->material = Material();

	floor->material = Material();
	floor->material.color = color(1.0f, 0.9f, 0.9f);
	floor->material.specular = 0.0f;
	floor->material.metallic = 0.5f;
	floor->material.pattern = createCheckerPattern();
	//floor->material.pattern->setTransform(S(5.0f, 5.0f, 5.0f));

	auto ceiling = createPlane(10.0f, 10.0f);

	ceiling->material = floor->material;
	ceiling->setTranslation(0.0f, 11.0f, 0.0f);
	ceiling->setTransform(T(0.0f, 11.0f, 0.0f));

	auto leftWall = createPlane();

	leftWall->material = Material();
	leftWall->material.color = color(1.0f, 0.9f, 0.9f);
	leftWall->setTranslation(0.0f, 0.0f, 5.0f);
	leftWall->setRotation(r(90.0f), -r(45.0f), 0.0f);
	leftWall->setTransform(T(0.0f, 0.0f, 5.0f) * RY(-r(45.0f)) * RX(r(90.0f)));

	auto rightWall = createPlane();

	rightWall->material = Material();
	rightWall->material.color = color(1.0f, 0.9f, 0.9f);
	rightWall->setRotation(r(90.0f), r(45.0f), 0.0f);
	rightWall->setTranslation(0.0f, 0.0f, 5.0f);;
	rightWall->setTransform(T(0.0f, 0.0f, 5.0f) * RY(r(45.0f)) * RX(r(90.0f)));

	auto left = createSphere();
	left->setScale(0.33f, 0.33f, 0.33f);
	left->setTranslation(-1.5f, 0.33f, -0.75f);
	left->setTransform(T(-1.5f, 0.33f, -0.75f) * S(0.33f));
	left->material = Material();
	left->material.color = color(1.0f, 0.8f, 0.1f);
	left->material.diffuse = 0.7f;
	left->material.specular = 0.3f;
	left->material.metallic = 0.3f;

	auto middle = createSphere();
	auto transform = T(-0.5f, 1.0f, 0.5f);
	middle->setTranslation(-0.5f, 1.0f, 0.5f);
	middle->setTransform(transform);
	middle->material = Materials::Mirror;

	auto right = createSphere();
	right->setScale(0.5f, 0.5f, 0.5f);
	right->setTranslation(1.5f, 0.5f, -0.5f);
	right->setTransform(T(1.5f, 0.5f, -0.5f) * S(0.5f));
	right->material = Materials::Glass;

	auto light = pointLight(point(-10.0f, 10.0f, -10.0f), Colors::White);

	auto world = World();
	world.setName("ReflectionTest");

	world.addLight(light);

	world.addObject(floor);
	world.addObject(ceiling);
	world.addObject(leftWall);
	world.addObject(rightWall);
	world.addObject(left);
	world.addObject(middle);
	world.addObject(right);

	Scene scene;
	scene.world = world;

	scene.camera = Camera(1280, 720, r(60.0f));
	scene.camera.transform = viewTransform(point(0.0f, 2.0f, -10.0f), point(0.0f, 0.0f, 0.0f), vector(0.0f, 1.0f, 0.0f));

	return scene;
}

World poolScene()
{
	World world;
	world.setName("PoolScene");

	auto wall = createPlane();

	wall->setRotation(r(90.0f), 0.0f, 0.0f);
	wall->setTranslation(0.0f, 0.0f, -20.0f);
	wall->setTransform(T(0.0f, 0.0f, -20.0f) * RX(r(90.0f)));
	wall->material.pattern = createCheckerPattern(Colors::Grey, Colors::White);
	wall->material.pattern->setTransform(S(0.25f));
	world.addObject(wall);

	auto floor = createPlane();

	floor->setTranslation(0.0f, -5.0f, 0.0f);
	floor->setTransform(T(0.0f, -5.0f, 0.0f));
	//floor->material.pattern = createCheckerPattern(Colors::RGB(196, 156, 92), Colors::RGB(126, 193, 89));
	floor->material.pattern = createCheckerPattern();
	floor->material.pattern->setTransform(S(0.5f));
	world.addObject(floor);

	auto middle = createSphere();
	auto transform = T(0.0f, -0.5f, 0.5f) * S(0.5f);
	middle->setScale(0.5f, 0.5f, 0.5f);
	middle->setTranslation(0.0f, -0.5f, 0.5f);
	middle->setTransform(transform);
	middle->material = Material();
	middle->material.color = color(1.0f, 0.0f, 0.0f);
	middle->material.diffuse = 0.7f;
	middle->material.specular = 0.3f;
	middle->material.metallic = 0.0f;
	middle->material.transparency = 0.0f;
	middle->material.refractiveIndex = 1.52f;

	world.addObject(middle);

	auto window = createPlane(1.0f, 1.0f);
	window->setRotation(r(90.0f), 0.0f, 0.0f);
	window->setTranslation(0.0f, 0.5f, 0.0f);
	window->setTransform(T(0.0f, 0.5f, 0.0f) * RX(r(90.0f)));
	window->material = Material();
	window->material = Materials::Glass;
	//world.addObject(window);

	auto water = createPlane();
	water->setTranslation(0.0f, 0.0f, 0.0f);
	water->setTransform(T(0.0f, 0.0f, 0.0f));
	water->material = Materials::Water;

	world.addObject(water);

	auto light = pointLight(point(0.0f, 30.0f, 20.0f), Colors::White);

	world.addLight(light);

	return world;
}

World glassCubeTest()
{
	World world;
	world.setName("GlassCubeTest");

	auto floor = createPlane();
	floor->material.pattern = createCheckerPattern();
	floor->material.metallic = 0.0f;
	floor->material.transparency = 0.0f;

	world.addObject(floor);

	auto cube = createCube();
	cube->setRotation(0.0f, r(180.0f), 0.0f);
	cube->setTranslation(-1.5f, 1.0f, 0.0f);
	cube->setTransform(T(-1.5f, 1.0f, 0.0f) * RY(r(180.0f)));
	cube->material = Materials::Glass;

	world.addObject(cube);

	auto sphere = createSphere();
	sphere->setRotation(0.0f, r(45.0f), 0.0f);
	sphere->setTranslation(1.5f, 1.0f, 0.0f);
	sphere->setTransform(T(1.5f, 1.0f, 0.0f) * RY(r(45.0f)));
	sphere->material = Materials::Glass;

	world.addObject(sphere);

	auto light = pointLight(point(10.0f, 10.0f, -10.0f), Colors::White);

	world.addLight(light);

	return world;
}

World cubeTest()
{
	World world;
	world.setName("CubeTest");

	auto floor = createPlane();
	floor->material.pattern = createCheckerPattern();
	floor->material.metallic = 0.5f;
	floor->material.transparency = 0.0f;

	world.addObject(floor);

	auto cube = createCube();
	cube->setScale(0.0f, r(45.0f), 0.0f);
	cube->setTranslation(-1.5f, 1.0f, 0.0f);
	cube->setTransform(T(-1.5f, 1.0f, 0.0f) * RY(r(45.0f)));
	cube->material = Materials::Glass;
	cube->material.metallic = 0.0f;

	world.addObject(cube);

	auto sphere = createSphere();
	sphere->setRotation(0.0f, r(45.0f), 0.0f);
	sphere->setTranslation(1.5f, 1.0f, 0.0f);
	sphere->setTransform(T(1.5f, 1.0f, 0.0f) * RY(r(45.0f)));
	sphere->material = Materials::Red;

	world.addObject(sphere);

	auto light = pointLight(point(10.0f, 10.0f, -10.0f), Colors::White);

	world.addLight(light);

	return world;
}

Scene cylinderTest()
{
	Scene scene;

	scene.camera = Camera(1280, 720, r(60.0f));
	scene.camera.transform = viewTransform(point(0.0f, 5.0f, -10.0f), 
										      point(0.0f, 0.0f, 0.0f), 
											  vector(0.0f, 1.0f, 0.0f));

	auto world = World();
	world.setName("CylinderTest");

	auto floor = createPlane();
	floor->material.pattern = createCheckerPattern();
	//floor->material.pattern = createStripPattern();

	//world.addObject(floor);

	auto cylinderLeft = createCylinder();
	cylinderLeft->setTransform(T(-1.5f, 0.0f, 0.0f));
	cylinderLeft->material = Materials::Glass;
	//world.addObject(cylinderLeft);

	auto cylinderMiddle = createCylinder(1.0f, 2.0f, true);
	//cylinderMiddle->setTransform(T(0.0f, 0.0f, -3.0f) * S(0.5f));
	cylinderMiddle->material = Materials::Red;
	//world.addObject(cylinderMiddle);

	auto cone = createCone(-1.0f, 0.0f, true);
	cone->setScale(1.0f, 1.5f, 1.0f);
	cone->setTransform(T(0.0f, 1.0f, 0.0f) * S(1.0f, 1.5f, 1.0f));
	cone->material = Materials::Red;
	world.addObject(cone);

	auto cylinderRight = createCylinder();
	cylinderRight->setTransform(T(1.5f, 0.0f, 0.0f));
	cylinderRight->material = Materials::Mirror;

	//world.addObject(cylinderRight);

	auto light = pointLight(point(10.0f, 10.0f, -10.0f), Colors::White);

	world.addLight(light);

	scene.world = world;

	return scene;
}

std::shared_ptr<Shape> hexagonCorner()
{
	auto corner = createSphere();
	
	corner->setTransform(T(0.0f, 0.0f, -1.0f) * S(0.25f));

	return corner;
}

std::shared_ptr<Shape> hexagonEdge()
{
	auto edge = createCylinder(0.0f, 1.0f);

	edge->setTransform(T(0.0f, 0.0f, -1.0f) *
								  RY(-r(30.0f)) *
								  RZ(-r(90.0f)) *
								  S(0.25f, 1.0f, 0.25f));

	return edge;
}

std::shared_ptr<Shape> hexagonSide()
{
	auto side = createGroup();

	side->addChild(hexagonCorner());
	side->addChild(hexagonEdge());

	return side;
}

std::shared_ptr<Shape> hexagon()
{
	auto hex = createGroup();

	for (int i = 0; i < 6; i++)
	{
		auto side = hexagonSide();
		side->setTransform(RY(i * r(60.0f)));
		hex->addChild(side);
	}

	return hex;
}

Scene groupTest()
{
	World world;
	world.setName("GroupTest");

	auto floor = createPlane();
	floor->material.pattern = createCheckerPattern();

	world.addObject(floor);

	auto hex = hexagon();
	hex->setTransform(T(0.0f, 1.0f, 0.0f));
	world.addObject(hex);

	auto light = pointLight(point(10.0f, 10.0f, -10.0f), Colors::White);

	world.addLight(light);

	Scene scene;
	scene.world = world;

	scene.camera = Camera(1280, 720, r(60.0f));
	scene.camera.transform = viewTransform(point(0.0f, 5.0f, -10.0f),
											  point(0.0f, 0.0f, 0.0f),
											  vector(0.0f, 1.0f, 0.0f));

	return scene;
}

Scene triangleTest()
{
	Scene scene = createDefaultScene();

	scene.world.setName("TriangleTest");

	auto triangle = createTriangle(point(-1.0f, 0.0f, 0.0f),
													 point(1.0f, 0.0f, 0.0f),
													 point(0.0f, 1.0f, 0.0f));

	triangle->material = Materials::Red;

	scene.world.addObject(triangle);

	return scene;
}

Scene objLoaderTest()
{
	Scene scene = emptyCornelBox(1024, 1024);

	scene.world.setName("ObjLoaderTest");

	auto parser = parseObjFile("Assets/Models/teapot.obj");

	auto teapot = objToGroup(parser);
	teapot->setTransform(T(0.0f, -0.5f, 0.25f) * RY(r(30.0f)) * S(0.13f));
	teapot->material.color = Colors::RGB(229, 171, 168) * 0.2f;
	teapot->material.metallic = 1.0f;

	scene.world.addObject(teapot);

	parser = parseObjFile("Assets/Models/low-poly teddy bear.obj");

	auto bear = objToGroup(parser);
	bear->setTransform(T(-0.25f, -0.4f, -0.24f) * RY(r(30.0f)) * S(0.2f));
	bear->material.specular = 0.0f;
	bear->material.color = Colors::Purple;

	scene.world.addObject(bear);

	parser = parseObjFile("Assets/Models/cow.obj");

	auto cow = objToGroup(parser);
	cow->setTransform(T(0.25f, -0.385f, -0.2f) * RY(-r(30.0f)) * S(0.16f));
	cow->material.specular = 0.0f;
	cow->material.color = Colors::RGB(99, 99, 247);

	scene.world.addObject(cow);

	auto sphere = createSphere(T(0.0f, -0.25f, 0.0f) * S(0.125f));
	sphere->material.color = Colors::Pink * 0.5f;
	sphere->material.metallic = 0.5f;
	//scene.world.addObject(sphere);

	auto cube = createCube();
	cube->setTransform(T(0.0f, 3.0f, 0.0f) * S(3.0f));

	//scene.world.addObject(cube);

	return scene;
}

Scene csgTest()
{
	Scene scene = createDefaultScene(1280, 720);
	scene.world.setName("CSGTest");

	scene.camera.transform = viewTransform(point(0.0f, 5.0f, -20.0f),
											  point(0.0f, 0.0f, 0.0f),
											  vector(0.0f, 1.0f, 0.0f));

	auto sphere1 = createSphere();
	//sphere->setTransform(T(-1.0f, 0.0f, 0.0f));
	sphere1->setTransform(T(0.5f, 0.5f, 0.0f));
	sphere1->material = Materials::Red;

	auto cube1 = createCube();
	//cube->setTransform(T(1.0f, 0.0f, 0.0f));
	cube1->setTransform(T(0.0f, 0.0f, 0.0f));
	cube1->material = Materials::Yellow;

	auto intersection = createCSG(Operation::Intersection, sphere1, cube1);
	intersection->setTransform(T(-3.0f, 1.0f, -5.0f) * RY(r(45.0f)));

	scene.world.addObject(intersection);

	auto sphere2 = createSphere();
	sphere2->setTransform(T(0.5f, 0.5f, -0.5f));
	sphere2->material = Materials::Red;

	auto cube2 = createCube();
	cube2->setTransform(T(0.0f, 0.0f, 0.0f));
	cube2->material = Materials::Yellow;

	cube2->setTransform(T(0.0f, 0.0f, 0.0f));

	auto difference = createCSG(Operation::Difference, cube2, sphere2);
	difference->setTransform(T(3.0f, 1.0f, -5.0f));

	scene.world.addObject(difference);

	auto sphere3 = createSphere();
	sphere3->setTransform(T(-0.5f, 0.0f, 0.0f));
	sphere3->material = Materials::Glass;

	auto sphere4 = createSphere();
	sphere4->setTransform(T(0.5f, 0.0f, 0.0f));
	sphere4->material = Materials::Glass;

	auto unionCSG = createCSG(Operation::Union, sphere3, sphere4);
	unionCSG->setTransform(T(0.0f, 1.0f, -5.0f));

	scene.world.addObject(unionCSG);

	return scene;
}

Scene pbrTest()
{
	Scene scene;
	scene.world.setName("PBRTest");

	auto floor = createPlane();
	floor->material.pattern = createCheckerPattern();

	scene.world.addObject(floor);

	auto wall = createPlane();
	wall->setTransform(T(0.0f, 0.0f, 0.0f) * RX(r(90.0f)));
	//wall->material = Materials::Grey;
	wall->material.pattern = createCheckerPattern();

	scene.world.addObject(wall);

	auto light1 = pointLight(point(-5.0f, 10.0f, -10.0f), Colors::White * 300.0f);

	scene.world.addLight(light1);

	auto light2 = pointLight(point(5.0f, 10.0f, -10.0f), Colors::White * 300.0f);

	scene.world.addLight(light2);

	scene.camera = Camera(1920, 1080, r(60.0f));
	scene.camera.transform = viewTransform(point(0.0f, 5.0f, -20.0f),
											  point(0.0f, 0.0f, 0.0f),
											  vector(0.0f, 1.0f, 0.0f));

	auto sphere = createSphere(T(0.0f, 1.0f, -3.0f));
	sphere->material = Materials::Red;
	sphere->material.metallic = 0.1f;

	scene.world.addObject(sphere);

	return scene;
}

Scene blenderScene(const std::string& path)
{
	Scene scene = loadScene(path);
	//Scene scene = loadScene("Assets/Scenes/BlenderScene.yaml");

	return scene;
}

void renderScene(const std::string& path)
{
	auto scene = blenderScene(path);

	//std::shared_ptr<Shape> bvhNode = std::make_shared<BVHNode>(scene.world.getObjects(), 0.0f, 0.0f);

	auto camera = Camera(1280, 720, r(60.0f));
	camera.transform = viewTransform(point(0.0f, 1.0f, -10.0f), point(0.0f, 1.0f, 0.0f), vector(0.0f, 1.0f, 0.0f));

	AriaCore::Timer timer("Rendering");
	auto canvas = render(scene.camera, scene.world, true, 5);
	timer.PrintElaspedMillis();

	//canvas.writeToPPM(scene.world.getName());
	canvas.writeToPNG(scene.world.getName());
}

int main(int argc, char* argv[])
{
	auto scene = pbrTest();

	//auto [world, camera] = cornelBox();

	auto camera = Camera(1280, 720, r(60.0f));
	camera.transform = viewTransform(point(0.0f, 2.0f, -10.0f), point(0.0f, 0.0f, 0.0f), vector(0.0f, 1.0f, 0.0f));
	////world.addObject(scene.world.getObject(0));

	//auto scene = cylinderTest();

	AriaCore::Timer timer("Rendering");
	auto canvas = render(scene.camera, scene.world, true, 5);
	timer.PrintElaspedMillis();

	////canvas.writeToPPM(scene.world.getName());
	canvas.writeToPNG(scene.world.getName());

	//renderScene("Assets/Scenes/BlenderCornelBox.yaml");
	//renderScene("Assets/Scenes/House.yaml");

	const std::string SceneBase = "./Assets/Scenes/";

	filewatch::FileWatch<std::string> watch(
		SceneBase,
		[&](const std::string& path, const filewatch::Event change_type) {
			switch (change_type)
			{
			case filewatch::Event::added:
				std::cout << "The file was added to the directory." << '\n';
				break;
			case filewatch::Event::removed:
				std::cout << "The file was removed from the directory." << '\n';
				break;
			case filewatch::Event::modified:
			{
				std::cout << path << "was modified. This can be a change in the time stamp or attributes." << '\n';

				renderScene(SceneBase + path);
			}
				break;
			case filewatch::Event::renamed_old:
				std::cout << "The file was renamed and this is the old name." << '\n';
				break;
			case filewatch::Event::renamed_new:
				std::cout << "The file was renamed and this is the new name." << '\n';
				break;
			};
		}
	);

	system("pause");

	return 0;
}