#include <catch2/catch_test_macros.hpp>

#include <camera.h>
#include <world.h>
#include <shading.h>

// Chapter 7 Making a Scene

SCENARIO("Constructing a camera", "[camera]")
{
	GIVEN("imageWidth = 160"
		"And imageHeight = 120"
		"And fieldOfView = ¦Ð / 2.0f")
	{
		auto imageWidth = 160;
		auto imageHeight = 120;
		auto fieldOfView = PI / 2.0F;
		WHEN("c = camera(imageWidth, imageHeight, fieldOfView)")
		{
			auto c = Camera(imageWidth, imageHeight, fieldOfView);
			THEN("c.imageWidth = 160"
				"And c.imageHeight = 120"
				"And c.fieldOfView = ¦Ð / 2"
				"And c.transform = identity_matrix")
			{
				REQUIRE(c.imageWidth == 160);
				REQUIRE(c.imageHeight == 120);
				REQUIRE(c.fieldOfView == PI / 2.0f);
				REQUIRE(c.transform == matrix4());
			}
		}
	}
}

SCENARIO("The pixel size for a horizontal canvas", "[camera]")
{
	GIVEN("c = Camera(200, 125, ¦Ð / 2.0f)")
	{
		auto c = Camera(200, 125, PI / 2.0f);
		THEN("c.pixelSize == 0.01f")
		{
			REQUIRE(c.pixelSize == 0.01f);
		}
	}
}

SCENARIO("The pixel size for a vertical canvas", "[camera]")
{
	GIVEN("c = Camera(125, 200, ¦Ð / 2.0f)")
	{
		auto c = Camera(125, 200, PI / 2.0f);
		THEN("c.pixelSize == 0.01f")
		{
			REQUIRE(equal(c.pixelSize, 0.01f));
		}
	}
}

SCENARIO("Constructing a ray through the center of the canvas", "[camera]")
{
	GIVEN("c = Camera(201, 101, ¦Ð / 2.0f)")
	{
		auto c = Camera(201, 101, PI / 2.0f);
		WHEN("r = c.rayForPixel(100, 50)")
		{
			auto r = c.rayForPixel(100, 50);
			THEN("r.origin == point(0.0f, 0.0f, 0.0f)"
				"And r.direction == vector(0.0f, 0.0f, -1.0f)")
			{
				REQUIRE(r.origin == point(0.0f, 0.0f, 0.0f));
				REQUIRE(r.direction == vector(0.0f, 0.0f, -1.0f));
			}
		}
	}
}

SCENARIO("Constructing a ray through the corner of the canvas", "[camera]")
{
	GIVEN("c = Camera(201, 101, ¦Ð / 2.0f)")
	{
		auto c = Camera(201, 101, PI / 2.0f);
		WHEN("r = c.rayForPixel(100, 50)")
		{
			auto r = c.rayForPixel(0, 0);
			THEN("r.origin == point(0.0f, 0.0f, 0.0f)"
				"And r.direction == vector(0.66519f, 0.33259f, -0.66851f)")
			{
				REQUIRE(r.origin == point(0.0f, 0.0f, 0.0f));
				REQUIRE(r.direction == vector(0.66519f, 0.33259f, -0.66851f));
			}
		}
	}
}

SCENARIO("Constructing a ray when the camera is transformed", "[camera]")
{
	GIVEN("c = Camera(201, 101, ¦Ð / 2.0f)")
	{
		auto c = Camera(201, 101, PI / 2.0f);
		WHEN("r = c.rayForPixel(100, 50)"
		"r = ray_for_pixel(c, 100, 50)")
		{
			c.transform = rotateY(PI / 4.0F) * translate(0.0f, -2.0f, 5.0f);
			auto r = c.rayForPixel(100, 50);
			THEN("r.origin == point(0.0f, 0.0f, 0.0f)"
				"And r.direction == vector(0.66519f, 0.33259f, -0.66851f)")
			{
				REQUIRE(r.origin == point(0.0f, 2.0f, -5.0f));
				REQUIRE(r.direction == vector(SQRT2 / 2.0f, 0.0f, -SQRT2 / 2.0f));
			}
		}
	}
}

SCENARIO("Rendering a world with a camera", "[camera]")
{
	GIVEN("w = defaultWorld()"
		"And c = Camera(11, 11, ¦Ð / 2.0f)"
		"And from = point(0.0f, 0.0f, -5.0f)"
		"And to = point(0.0f, 0.0f, 0.0f)"
		"And up = vector(0.0f, 1.0f, 0.0f)"
		"And c.transform = viewTransform(from, to, up)")
	{
		auto w = defaultWorld();
		auto c = Camera(11, 11, PI / 2.0f);
		auto from = point(0.0f, 0.0f, -5.0f);
		auto to = point(0.0f, 0.0f, 0.0f);
		auto up = vector(0.0f, 1.0f, 0.0f);
		c.transform = viewTransform(from, to, up);
		WHEN("image = render(c, w)")
		{
			auto image = render(c, w);
			THEN("image.pixelAt(5, 5) == color(0.38066f, 0.47583f, 0.2855f)")
			{
				REQUIRE(image.pixelAt(5, 5) == color(0.38066f, 0.47583f, 0.2855f));
			}
		}
	}
}