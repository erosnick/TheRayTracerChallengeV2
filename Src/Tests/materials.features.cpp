#include <catch2/catch_test_macros.hpp>

#include <material.h>

#include <sphere.h>

#include <light.h>

#include <shading.h>

// Chapter 6 Light and Shading

SCENARIO("The default material", "[material]")
{
	GIVEN("m = Material()")
	{
		auto m = Material();
		THEN("m.color = color(1.0f, 1.0f, 1.0f)"
			"And m.ambient == 0.1f"
			"And m.diffuse == 0.9f"
			"And m.specular == 0.9f"
			"And m.shininess == 200.0f")
		{
			REQUIRE(m.color == color(1.0f, 1.0f, 1.0f));
			REQUIRE(m.ambient == 0.1f);
			REQUIRE(m.diffuse == 0.9f);
			REQUIRE(m.specular == 0.9f);
			REQUIRE(m.shininess == 200.0f);
		}
	}
}

SCENARIO("A sphere has a default material", "[material]")
{
	GIVEN("s = Sphere()")
	{
		auto s = Sphere();
		WHEN("m = s.material")
		{
			auto m = s.material;
			THEN("m == Material()")
			{
				REQUIRE(m == Material());
			}
		}
	}
}

SCENARIO("Lighting with the eye between the light and the surface", "[matrix]")
{
	auto m = Material();
	auto position = point(0.0f, 0.0f, 0.0f);

	GIVEN("viewDirection = vector(0.0f, 0.0f, -1.0f)"
		"And normal = vector(0.0f, 0.0f, -1.0f)"
		"And light = pointLight(point(0.0f, 0.0f, -10.0f), color(1.0f, 1.0f, 1.0f))")
	{
		auto viewDirection = vector(0.0f, 0.0f, -1.0f);
		auto normal = vector(0.0f, 0.0f, -1.0f);
		auto light = pointLight(point(0.0f, 0.0f, -10.0f), color(1.0f, 1.0f, 1.0f));
		WHEN("result = lighting(m, light, position, eyev, normalv)")
		{
			auto result = lighting(m, light, position, viewDirection, normal);
			THEN("result == color(1.9f, 1.9f, 1.9f)")
			{
				REQUIRE(result == color(1.9f, 1.9f, 1.9f));
			}
		}
	}
}

SCENARIO(" Lighting with the eye between light and surface, eye offset 45��", "[matrix]")
{
	auto m = Material();
	auto position = point(0.0f, 0.0f, 0.0f);

	GIVEN("eye = vector(0.0f, ��2/2, -��2/2)"
		"And normal = vector(0.0f, 0.0f, -1.0f)"
		"And light = pointLight(point(0.0f, 0.0f, -10.0f), color(1.0f, 1.0f, 1.0f))")
	{
		auto eye = vector(0.0f, SQRT2 / 2.0f, -SQRT2 / 2.0f);
		auto normal = vector(0.0f, 0.0f, -1.0f);
		auto light = pointLight(point(0.0f, 0.0f, -10.0f), color(1.0f, 1.0f, 1.0f));
		WHEN("result = lighting(m, light, position, eyev, normalv)")
		{
			auto result = lighting(m, light, position, eye, normal);
			THEN("result == color(1.0f, 1.0f, 1.0f)")
			{
				REQUIRE(result == color(1.0f, 1.0f, 1.0f));
			}
		}
	}
}

SCENARIO("Lighting with eye opposite surface, light offset 45��", "[matrix]")
{
	auto m = Material();
	auto position = point(0.0f, 0.0f, 0.0f);

	GIVEN("viewDirection = vector(0.0f, 0.0f, -1.0f)"
		"And normal = vector(0.0f, 0.0f, -1.0f)"
		"And light = pointLight(point(0.0f, 10.0f, -10.0f), color(1.0f, 1.0f, 1.0f))")
	{
		auto viewDirection = vector(0.0f, 0.0f, -1.0f);
		auto normal = vector(0.0f, 0.0f, -1.0f);
		auto light = pointLight(point(0.0f, 10.0f, -10.0f), color(1.0f, 1.0f, 1.0f));
		WHEN("result = lighting(m, light, position, viewDirection, normalv)")
		{
			auto result = lighting(m, light, position, viewDirection, normal);
			THEN("result == color(0.7364f, 0.7364f, 0.7364f)")
			{
				REQUIRE(result == color(0.7364f, 0.7364f, 0.7364f));
			}
		}
	}
}

SCENARIO("Lighting with eye in the path of the reflection vector", "[matrix]")
{
	auto m = Material();
	auto position = point(0.0f, 0.0f, 0.0f);

	GIVEN("eye = vector(0.0f, -��2/2, -��2/2)"
		"And normal = vector(0.0f, 0.0f, -1.0f)"
		"And light = pointLight(point(0.0f, 10.0f, -10.0f), color(1.0f, 1.0f, 1.0f))")
	{
		auto eye = vector(0.0f, -SQRT2 / 2.0f, -SQRT2 / 2.0f);
		auto normal = vector(0.0f, 0.0f, -1.0f);
		auto light = pointLight(point(0.0f, 10.0f, -10.0f), color(1.0f, 1.0f, 1.0f));
		WHEN("result = lighting(m, light, position, eyev, normalv)")
		{
			auto result = lighting(m, light, position, eye, normal);
			THEN("result == color(1.6364f, 1.6364f, 1.6364f)")
			{
				REQUIRE(result == color(1.63639f, 1.63639f, 1.63639f));
			}
		}
	}
}

SCENARIO("Lighting with the light behind the surface", "[matrix]")
{
	auto m = Material();
	auto position = point(0.0f, 0.0f, 0.0f);

	GIVEN("eye = vector(0.0f, 0.0f, -1.0f)"
		"And normal = vector(0.0f, 0.0f, -1.0f)"
		"And light = pointLight(point(0.0f, 0.0f, 10.0f), color(1.0f, 1.0f, 1.0f))")
	{
		auto eye = vector(0.0f, 0.0f, -1.0f);
		auto normal = vector(0.0f, 0.0f, -1.0f);
		auto light = pointLight(point(0.0f, 0.0f, 10.0f), color(1.0f, 1.0f, 1.0f));
		WHEN("result = lighting(m, light, position, eyev, normalv)")
		{
			auto result = lighting(m, light, position, eye, normal);
			THEN("result == color(0.1f, 0.1f, 0.1f)")
			{
				REQUIRE(result == color(0.1f, 0.1f, 0.1f));
			}
		}
	}
}

