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
#include "bvh.h"

#include "YAMLLoader.h"

Scene createDefaultScene()
{
	Scene scene;

	auto floor = createPlane();
	floor->material.pattern = createCheckerPattern();

	scene.world.addObject(floor);

	auto light = pointLight(point(10.0f, 10.0f, -10.0f), Colors::White);

	scene.world.addLight(light);

	scene.camera = Camera(1280, 720, radians(60.0f));
	scene.camera.transform = viewTransform(point(0.0f, 5.0f, -10.0f),
											  point(0.0f, 0.0f, 0.0f),
											  vector(0.0f, 1.0f, 0.0f));

	return scene;
}

World shadowTest()
{
	auto floor = createSphere();

	floor->setScale(10.0f, 0.01f, 10.0f);
	floor->setTransform(scale(10.0f, 0.01f, 10.0f));
	floor->material = Material();

	floor->material = Material();
	floor->material.color = color(1.0f, 0.9f, 0.9f);
	floor->material.specular = 0.0f;

	auto leftWall = createSphere();

	leftWall->setScale(0.0f, 0.05f, 10.0f);
	leftWall->setRotation(PI / 2.0f, -PI / 4.0f, 0.0f);
	leftWall->setTranslation(0.0f, 0.0f, 5.0f);

	leftWall->setTransform(translate(0.0f, 0.0f, 5.0f) *
									  rotateY(-PI / 4.0f) *
									  rotateX(PI / 2.0f) *
									  scale(10.0f, 0.05f, 10.0f));

	leftWall->material = floor->material;

	auto rightWall = createSphere();

	rightWall->setScale(10.0f, 0.05f, 10.0f);
	rightWall->setRotation(PI / 2.0f, PI / 4.0f, 0.0f);
	rightWall->setTranslation(0.0f, 0.0f, 5.0f);
	rightWall->setTransform(translate(0.0f, 0.0f, 5.0f) *
									   rotateY(PI / 4.0f) * rotateX(PI / 2.0f) *
									   scale(10.0f, 0.05f, 10.0f));

	rightWall->material = floor->material;

	auto left = createSphere();
	left->setScale(0.33f, 0.33f, 0.33f);
	left->setTranslation(-1.5f, 0.33f, -0.75f);
	left->setTransform(translate(-1.5f, 0.33f, -0.75f) * scale(0.33f));
	left->material = Material();
	left->material.color = color(1.0f, 0.8f, 0.1f);
	left->material.diffuse = 0.7f;
	left->material.specular = 0.3f;

	auto middle = createSphere();
	auto transform = translate(-0.5f, 1.0f, 0.5f);
	middle->setTranslation(-0.5f, 1.0f, 0.5f);
	middle->setTransform(transform);
	middle->material = Material();
	middle->material.color = color(0.1f, 1.0f, 0.5f);
	middle->material.diffuse = 0.7f;
	middle->material.specular = 0.3f;

	auto right = createSphere();
	right->setScale(0.5f, 0.5f, 0.5f);
	right->setTranslation(1.5f, 0.5f, -0.5f);
	right->setTransform(translate(1.5f, 0.5f, -0.5f) * scale(0.5f));
	right->material = Material();
	right->material.color = color(0.5f, 1.0f, 0.1f);
	right->material.diffuse = 0.7f;
	right->material.specular = 0.3f;

	auto topRight = createSphere();
	topRight->setScale(0.5, 0.25f, 0.5f);
	topRight->setRotation(PI / 4.0f, 0.0f, PI / 4.0f);
	topRight->setTranslation(1.5f, 1.5f, -0.5f);;
	topRight->setTransform(translate(1.5f, 1.5f, -0.5f) * rotateX(PI / 4.0f) * rotateZ(PI / 4.0f) * scale(0.5, 0.25f, 0.5f));
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
	auto pattern1 = createStripPattern(Colors::DarkGreen, Colors::White);
	pattern1->setTransform(rotateY(PI / 2.0f));
	auto pattern2 = createStripPattern(Colors::DarkGreen, Colors::White);
	floor->material.pattern = createBlendPattern(pattern1, pattern2);

	pattern1 = createStripPattern(Colors::RGB(146, 216, 250), Colors::RGB(239, 134, 198));
	pattern1->setTransform(rotateY(PI / 6.0f) * scale(0.25f));
	pattern2 = createStripPattern();
	pattern2->setTransform(rotateY(-PI / 6.0f) * scale(0.25f));
	floor->material.pattern = createNestedPattern(pattern1, pattern2);
	//floor->material.pattern = createPerturbedPattern(createStripPattern());

	auto leftWall = createPlane();

	leftWall->material = Material();
	leftWall->material.color = color(1.0f, 0.9f, 0.9f);
	leftWall->setTranslation(0.0f, 0.0f, 5.0f);
	leftWall->setRotation(PI / 2.0f, -PI / 4.0f, 0.0f);
	leftWall->setTransform(translate(0.0f, 0.0f, 5.0f) * rotateY(-PI / 4.0f) * rotateX(PI / 2.0f));

	auto rightWall = createPlane();

	rightWall->material = Material();
	rightWall->material.pattern = createRadialGradientPattern();
	rightWall->material.pattern->setTransform(scale(0.5f));
	rightWall->setTranslation(0.0f, 0.0f, 5.0f);
	rightWall->setRotation(PI / 2.0f, PI / 4.0f, 0.0f);
	rightWall->setTransform(translate(0.0f, 0.0f, 5.0f) * rotateY(PI / 4.0f) * rotateX(PI / 2.0f));

	auto ceiling = createPlane();

	ceiling->material = floor->material;
	ceiling->setTranslation(0.0f, 0.0f, 0.0f);
	ceiling->setTransform(translate(0.0f, 0.0f, 0.0f));

	auto platform = createPlane(0.5f, 0.5f);

	platform->material = floor->material;
	platform->material.pattern = createStripPattern();
	platform->material.pattern->setTransform(scale(0.25f));
	platform->setTranslation(1.0f, 1.0f, -5.0f);
	platform->setTransform(translate(1.0f, 1.0f, -5.0f));

	auto left = createSphere();
	left->setScale(0.33f, 0.33f, 0.33f);
	left->setTranslation(-1.5f, 0.33f, -0.75f);
	left->setTransform(translate(-1.5f, 0.33f, -0.75f) * scale(0.33f));
	left->material = Material();
	left->material.color = color(1.0f, 0.8f, 0.1f);
	left->material.diffuse = 0.7f;
	left->material.specular = 0.3f;

	auto middle = createSphere();

	auto transform = translate(-0.5f, 1.0f, 0.5f) * rotateY(-PI / 3.0f) * rotateZ(-PI / 9.0f);
	
	middle->setRotation(0.0f, -PI / 3.0f, -PI / 9.0f);
	middle->setTranslation(-0.5f, 1.0f, 0.5f);
	middle->setTransform(transform);
	middle->material = Material();
	middle->material.pattern = createStripPattern(Colors::RGB(14, 142, 71), Colors::RGB(19, 192, 96));
	middle->material.pattern->setTransform(translate(0.3f, 0.0f, 0.0f) * scale(0.125f));
	middle->material.color = color(0.1f, 1.0f, 0.5f);
	middle->material.diffuse = 0.7f;
	middle->material.specular = 0.3f;

	auto right = createSphere();
	right->setTranslation(1.5f, 0.5f, -0.5f);	
	right->setScale(0.5f, 0.5f, 0.5f);
	right->setTransform(translate(1.5f, 0.5f, -0.5f) * scale(0.5f));
	right->material = Material();
	right->material.color = color(0.5f, 1.0f, 0.1f);
	right->material.diffuse = 0.7f;
	right->material.specular = 0.3f;
	right->material.pattern = createGradientPattern(Colors::RGB(146, 216, 250), Colors::RGB(239, 134, 198));
	right->material.pattern->setTransform(scale(1.0f));

	auto topRight = createSphere();

	topRight->setScale(0.5, 0.25f, 0.5f);
	topRight->setRotation(PI / 4.0f, 0.0f, PI / 4.0f);
	topRight->setTranslation(1.5f, 2.0f, -0.5f);
	topRight->setTransform(translate(1.5f, 2.0f, -0.5f) * rotateX(PI / 4.0f) * rotateZ(PI / 4.0f) * scale(0.5, 0.25f, 0.5f));
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

	scene.camera = Camera(1280, 720, radians(60.0f));
	scene.camera.transform = viewTransform(point(0.0f, 2.0f, -10.0f), point(0.0f, 0.0f, 0.0f), vector(0.0f, 1.0f, 0.0f));

	return scene;
}

Scene reflectionTest()
{
	auto floor = createPlane();

	floor->setScale(0.5f, 0.5f, 0.5f);
	floor->setTransform(scale(0.5f, 0.5f, 0.5f));
	floor->material = Material();

	floor->material = Material();
	floor->material.color = color(1.0f, 0.9f, 0.9f);
	floor->material.specular = 0.0f;
	floor->material.reflective = 0.5f;
	floor->material.pattern = createCheckerPattern();
	//floor->material.pattern->setTransform(scale(5.0f, 5.0f, 5.0f));

	auto ceiling = createPlane(10.0f, 10.0f);

	ceiling->material = floor->material;
	ceiling->setTranslation(0.0f, 11.0f, 0.0f);
	ceiling->setTransform(translate(0.0f, 11.0f, 0.0f));

	auto leftWall = createPlane();

	leftWall->material = Material();
	leftWall->material.color = color(1.0f, 0.9f, 0.9f);
	leftWall->setTranslation(0.0f, 0.0f, 5.0f);
	leftWall->setRotation(PI / 2.0f, -PI / 4.0f, 0.0f);
	leftWall->setTransform(translate(0.0f, 0.0f, 5.0f) * rotateY(-PI / 4.0f) * rotateX(PI / 2.0f));

	auto rightWall = createPlane();

	rightWall->material = Material();
	rightWall->material.color = color(1.0f, 0.9f, 0.9f);
	rightWall->setRotation(PI / 2.0f, PI / 4.0f, 0.0f);
	rightWall->setTranslation(0.0f, 0.0f, 5.0f);;
	rightWall->setTransform(translate(0.0f, 0.0f, 5.0f) * rotateY(PI / 4.0f) * rotateX(PI / 2.0f));

	auto left = createSphere();
	left->setScale(0.33f, 0.33f, 0.33f);
	left->setTranslation(-1.5f, 0.33f, -0.75f);
	left->setTransform(translate(-1.5f, 0.33f, -0.75f) * scale(0.33f));
	left->material = Material();
	left->material.color = color(1.0f, 0.8f, 0.1f);
	left->material.diffuse = 0.7f;
	left->material.specular = 0.3f;
	left->material.reflective = 0.3f;

	auto middle = createSphere();
	auto transform = translate(-0.5f, 1.0f, 0.5f);
	middle->setTranslation(-0.5f, 1.0f, 0.5f);
	middle->setTransform(transform);
	middle->material = Materials::Mirror;

	auto right = createSphere();
	right->setScale(0.5f, 0.5f, 0.5f);
	right->setTranslation(1.5f, 0.5f, -0.5f);
	right->setTransform(translate(1.5f, 0.5f, -0.5f) * scale(0.5f));
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

	scene.camera = Camera(1280, 720, radians(60.0f));
	scene.camera.transform = viewTransform(point(0.0f, 2.0f, -10.0f), point(0.0f, 0.0f, 0.0f), vector(0.0f, 1.0f, 0.0f));

	return scene;
}

World poolScene()
{
	World world;
	world.setName("PoolScene");

	auto wall = createPlane();

	wall->setRotation(PI / 2.0f, 0.0f, 0.0f);
	wall->setTranslation(0.0f, 0.0f, -20.0f);
	wall->setTransform(translate(0.0f, 0.0f, -20.0f) * rotateX(PI / 2.0f));
	wall->material.pattern = createCheckerPattern(Colors::Grey, Colors::White);
	wall->material.pattern->setTransform(scale(0.25f));
	world.addObject(wall);

	auto floor = createPlane();

	floor->setTranslation(0.0f, -5.0f, 0.0f);
	floor->setTransform(translate(0.0f, -5.0f, 0.0f));
	//floor->material.pattern = createCheckerPattern(Colors::RGB(196, 156, 92), Colors::RGB(126, 193, 89));
	floor->material.pattern = createCheckerPattern();
	floor->material.pattern->setTransform(scale(0.5f));
	world.addObject(floor);

	auto middle = createSphere();
	auto transform = translate(0.0f, -0.5f, 0.5f) * scale(0.5f);
	middle->setScale(0.5f, 0.5f, 0.5f);
	middle->setTranslation(0.0f, -0.5f, 0.5f);
	middle->setTransform(transform);
	middle->material = Material();
	middle->material.color = color(1.0f, 0.0f, 0.0f);
	middle->material.diffuse = 0.7f;
	middle->material.specular = 0.3f;
	middle->material.reflective = 0.0f;
	middle->material.transparency = 0.0f;
	middle->material.refractiveIndex = 1.52f;

	world.addObject(middle);

	auto window = createPlane(1.0f, 1.0f);
	window->setRotation(PI / 2.0f, 0.0f, 0.0f);
	window->setTranslation(0.0f, 0.5f, 0.0f);
	window->setTransform(translate(0.0f, 0.5f, 0.0f) * rotateX(PI / 2.0f));
	window->material = Material();
	window->material = Materials::Glass;
	//world.addObject(window);

	auto water = createPlane();
	water->setTranslation(0.0f, 0.0f, 0.0f);
	water->setTransform(translate(0.0f, 0.0f, 0.0f));
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
	floor->material.reflective = 0.0f;
	floor->material.transparency = 0.0f;

	world.addObject(floor);

	auto cube = createCube();
	cube->setRotation(0.0f, PI / 1.0f, 0.0f);
	cube->setTranslation(-1.5f, 1.0f, 0.0f);
	cube->setTransform(translate(-1.5f, 1.0f, 0.0f) * rotateY(PI / 1.0f));
	cube->material = Materials::Glass;

	world.addObject(cube);

	auto sphere = createSphere();
	sphere->setRotation(0.0f, PI / 4.0f, 0.0f);
	sphere->setTranslation(1.5f, 1.0f, 0.0f);
	sphere->setTransform(translate(1.5f, 1.0f, 0.0f) * rotateY(PI / 4.0f));
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
	floor->material.reflective = 0.5f;
	floor->material.transparency = 0.0f;

	world.addObject(floor);

	auto cube = createCube();
	cube->setScale(0.0f, PI / 4.0f, 0.0f);
	cube->setTranslation(-1.5f, 1.0f, 0.0f);
	cube->setTransform(translate(-1.5f, 1.0f, 0.0f) * rotateY(PI / 4.0f));
	cube->material = Materials::Glass;
	cube->material.reflective = 0.0f;

	world.addObject(cube);

	auto sphere = createSphere();
	sphere->setRotation(0.0f, PI / 4.0f, 0.0f);
	sphere->setTranslation(1.5f, 1.0f, 0.0f);
	sphere->setTransform(translate(1.5f, 1.0f, 0.0f) * rotateY(PI / 4.0f));
	sphere->material = Materials::Red;

	world.addObject(sphere);

	auto light = pointLight(point(10.0f, 10.0f, -10.0f), Colors::White);

	world.addLight(light);

	return world;
}

Scene cylinderTest()
{
	Scene scene;

	scene.camera = Camera(1280, 720, radians(60.0f));
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
	cylinderLeft->setTransform(translate(-1.5f, 0.0f, 0.0f));
	cylinderLeft->material = Materials::Glass;
	//world.addObject(cylinderLeft);

	auto cylinderMiddle = createCylinder(1.0f, 2.0f, true);
	//cylinderMiddle->setTransform(translate(0.0f, 0.0f, -3.0f) * scale(0.5f));
	cylinderMiddle->material = Materials::Red;
	//world.addObject(cylinderMiddle);

	auto cone = createCone(-1.0f, 0.0f, true);
	cone->setScale(1.0f, 1.5f, 1.0f);
	cone->setTransform(translate(0.0f, 1.0f, 0.0f) * scale(1.0f, 1.5f, 1.0f));
	cone->material = Materials::Red;
	world.addObject(cone);

	auto cylinderRight = createCylinder();
	cylinderRight->setTransform(translate(1.5f, 0.0f, 0.0f));
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
	
	corner->setTransform(translate(0.0f, 0.0f, -1.0f) * scale(0.25f));

	return corner;
}

std::shared_ptr<Shape> hexagonEdge()
{
	auto edge = createCylinder(0.0f, 1.0f);

	edge->setTransform(translate(0.0f, 0.0f, -1.0f) *
								  rotateY(-PI / 6.0f) *
								  rotateZ(-PI / 2.0f) *
								  scale(0.25f, 1.0f, 0.25f));

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
		side->setTransform(rotateY(i * PI / 3.0f));
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
	hex->setTransform(translate(0.0f, 1.0f, 0.0f));
	world.addObject(hex);

	auto light = pointLight(point(10.0f, 10.0f, -10.0f), Colors::White);

	world.addLight(light);

	Scene scene;
	scene.world = world;

	scene.camera = Camera(1280, 720, radians(60.0f));
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

Scene blenderScene(const std::string& path)
{
	Scene scene = loadScene(path);
	//Scene scene = loadScene("Assets/Scenes/BlenderScene.yaml");

	return scene;
}

void renderScene(const std::string& path)
{
	auto scene = blenderScene(path);

	std::shared_ptr<Shape> bvhNode = std::make_shared<BVHNode>(scene.world.getObjects(), 0.0f, 0.0f);

	auto camera = Camera(1280, 720, radians(60.0f));
	camera.transform = viewTransform(point(0.0f, 1.0f, -10.0f), point(0.0f, 1.0f, 0.0f), vector(0.0f, 1.0f, 0.0f));

	AriaCore::Timer timer("Rendering");
	auto canvas = render(scene.camera, scene.world, true, 5);
	timer.PrintElaspedMillis();

	//canvas.writeToPPM(scene.world.getName());
	canvas.writeToPNG(scene.world.getName());
}

int main(int argc, char* argv[])
{
	auto scene = triangleTest();

	//auto [world, camera] = cornelBox();

	auto camera = Camera(1280, 720, radians(60.0f));
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