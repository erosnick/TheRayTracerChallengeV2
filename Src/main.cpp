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

World reflectionAndRefractionTest()
{
	auto floor = createPlane();

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
	ceiling->setTransform(translate(0.0f, 11.0f, 0.0f));

	auto leftWall = createPlane();

	leftWall->material = Material();
	leftWall->material.color = color(1.0f, 0.9f, 0.9f);
	leftWall->setTransform(translate(0.0f, 0.0f, 5.0f) * rotateY(-PI / 4.0f) * rotateX(PI / 2.0f));

	auto rightWall = createPlane();

	rightWall->material = Material();
	rightWall->material.color = color(1.0f, 0.9f, 0.9f);
	rightWall->setTransform(translate(0.0f, 0.0f, 5.0f) * rotateY(PI / 4.0f) * rotateX(PI / 2.0f));

	auto left = createSphere();
	left->setTransform(translate(-1.5f, 0.33f, -0.75f) * scale(0.33f));
	left->material = Material();
	left->material.color = color(1.0f, 0.8f, 0.1f);
	left->material.diffuse = 0.7f;
	left->material.specular = 0.3f;
	left->material.reflective = 0.3f;

	auto middle = createSphere();
	auto transform = translate(-0.5f, 1.0f, 0.5f);
	middle->setTransform(transform);
	middle->material = Materials::Mirror;

	auto right = createSphere();
	right->setTransform(translate(1.5f, 0.5f, -0.5f) * scale(0.5f));
	right->material = Materials::Glass;

	auto light = pointLight(point(-10.0f, 10.0f, -10.0f), Color::White);

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

	return world;
}

World poolScene()
{
	World world;
	world.setName("PoolScene");

	auto wall = createPlane();

	wall->setTransform(translate(0.0f, 0.0f, -20.0f) * rotateX(PI / 2.0f));
	wall->material.pattern = createCheckerPattern(Color::Grey, Color::White);
	wall->material.pattern->setTransform(scale(0.25f));
	world.addObject(wall);

	auto floor = createPlane();

	floor->setTransform(translate(0.0f, -5.0f, 0.0f));
	//floor->material.pattern = createCheckerPattern(Color::RGB(196, 156, 92), Color::RGB(126, 193, 89));
	floor->material.pattern = createCheckerPattern();
	floor->material.pattern->setTransform(scale(0.5f));
	world.addObject(floor);

	auto middle = createSphere();
	auto transform = translate(0.0f, -0.5f, 0.5f) * scale(0.5f);
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
	window->setTransform(translate(0.0f, 0.5f, 0.0f) * rotateX(PI / 2.0f));
	window->material = Material();
	window->material = Materials::Glass;
	//world.addObject(window);

	auto water = createPlane();

	water->setTransform(translate(0.0f, 0.0f, 0.0f));
	water->material = Materials::Water;

	world.addObject(water);

	auto light = pointLight(point(0.0f, 30.0f, 20.0f), Color::White);

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
	cube->setTransform(translate(-1.5f, 1.0f, 0.0f) * rotateY(PI / 1.0f));
	cube->material = Materials::Glass;

	world.addObject(cube);

	auto sphere = createSphere();
	sphere->setTransform(translate(1.5f, 1.0f, 0.0f) * rotateY(PI / 4.0f));
	sphere->material = Materials::Glass;

	world.addObject(sphere);

	auto light = pointLight(point(10.0f, 10.0f, -10.0f), Color::White);

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
	cube->setTransform(translate(-1.5f, 1.0f, 0.0f) * rotateY(PI / 4.0f));
	cube->material = Materials::Glass;
	cube->material.reflective = 0.0f;

	world.addObject(cube);

	auto sphere = createSphere();
	sphere->setTransform(translate(1.5f, 1.0f, 0.0f) * rotateY(PI / 4.0f));
	sphere->material = Materials::Red;

	world.addObject(sphere);

	auto light = pointLight(point(10.0f, 10.0f, -10.0f), Color::White);

	world.addLight(light);

	return world;
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
	//auto world = glassCubeTest();

	//auto [world, camera] = cornelBox();

	//auto camera = Camera(1280, 720, radians(60.0f));
	//camera.transform = viewTransform(point(0.0f, 2.0f, -10.0f), point(0.0f, 0.0f, 0.0f), vector(0.0f, 1.0f, 0.0f));
	////world.addObject(scene.world.getObject(0));

	//AriaCore::Timer timer("Rendering");
	//auto canvas = render(camera, world, true, 5);
	//timer.PrintElaspedMillis();

	//canvas.writeToPPM(world.getName());
	//canvas.writeToPNG(world.getName());

	//renderScene("Assets/Scenes/BlenderCornelBox.yaml");
	renderScene("Assets/Scenes/House.yaml");

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