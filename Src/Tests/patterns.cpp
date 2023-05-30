#include <catch2/catch_test_macros.hpp>

#include <colors.h>
#include <pattern.h>
#include <material.h>
#include <shading.h>

// Chapter 10 Patterns

SCENARIO("Creating a stripe pattern", "[patterns]")
{
	auto black = Color::Black;
	auto white = Color::White;

	GIVEN("pattern = createStripPattern(white, black)")
	{
		auto pattern = createStripPattern(white, black);
		THEN("pattern.a == white"
			"And pattern.b == black")
		{
			REQUIRE(pattern->color1 == white);
			REQUIRE(pattern->color2 == black);
		}
	}
}

SCENARIO("A stripe pattern is constant in y", "[patterns]")
{
	auto black = Color::Black;
	auto white = Color::White;

	GIVEN("pattern = stripePattern(white, black)")
	{
		auto pattern = createStripPattern(white, black);
		THEN("colorAt(pattern, point(0.0f, 0.0f, 0.0f)) == white"
			"And colorAt(pattern, point(0.0f, 1.0f, 0.0f)) = white"
			"And colorAt(pattern, point(0.0f, 2.0f, 0.0f)) = white")
		{
			REQUIRE(pattern->colorAt(point(0.0f, 0.0f, 0.0f)) == white);
			REQUIRE(pattern->colorAt(point(0.0f, 1.0f, 0.0f)) == white);
			REQUIRE(pattern->colorAt(point(0.0f, 2.0f, 0.0f)) == white); 
		}
	}
}

SCENARIO("A stripe pattern is constant in z", "[patterns]")
{
	auto black = Color::Black;
	auto white = Color::White;

	GIVEN("pattern = stripePattern(white, black)")
	{
		auto pattern = createStripPattern(white, black);
		THEN("colorAt(pattern, point(0.0f, 0.0f, 0.0f)) == white"
			"And colorAt(pattern, point(0.0f, 0.0f, 1.0f)) = white"
			"And colorAt(pattern, point(0.0f, 0.0f, 2.0f)) = white")
		{
			REQUIRE(pattern->colorAt(point(0.0f, 0.0f, 0.0f)) == white);
			REQUIRE(pattern->colorAt(point(0.0f, 0.0f, 1.0f)) == white);
			REQUIRE(pattern->colorAt(point(0.0f, 0.0f, 2.0f)) == white);
		}
	}
}

SCENARIO("A stripe pattern is constant in x", "[patterns]")
{
	auto black = Color::Black;
	auto white = Color::White;

	GIVEN("pattern = stripePattern(white, black)")
	{
		auto pattern = createStripPattern(white, black);
		THEN("colorAt(pattern, point(0.0f, 0.0f, 0.0f)) == white"
			"And colorAt(pattern, point(0.9f, 0.0f, 0.0f)) = white"
			"And colorAt(pattern, point(1.0f, 0.0f, 0.0f)) = black"
			"And colorAt(pattern, point(-0.1f, 0.0f, 0.0f)) = black"
			"And colorAt(pattern, point(-1.0f, 0.0f, 0.0f)) = black"
			"And colorAt(pattern, point(-1.1f, 0.0f, 0.0f)) = white")
		{
			REQUIRE(pattern->colorAt(point(0.0f, 0.0f, 0.0f)) == white);
			REQUIRE(pattern->colorAt(point(0.9f, 0.0f, 0.0f)) == white);
			REQUIRE(pattern->colorAt(point(1.0f, 0.0f, 0.0f)) == black);
			REQUIRE(pattern->colorAt(point(-0.1f, 0.0f, 0.0f)) == black);
			REQUIRE(pattern->colorAt(point(-1.0f, 0.0f, 0.0f)) == black);
			REQUIRE(pattern->colorAt(point(-1.1f, 0.0f, 0.0f)) == white);
		}
	}
}

SCENARIO("Lighting with a pattern applied", "[patterns]")
{
	GIVEN("m.pattern = stripe_pattern(color(1, 1, 1), color(0, 0, 0))"
		"And m.ambient = 1.0f"
		"And m.diffuse = 0.0f"
		"And m.specular = 0.0f"
		"And viewDirection = vector(0.0f, 0.0f, -1.0f)"
		"And normal = vector(0.0f, 0.0f, -1.0f)"
		"And light = pointLight(point(0.0f, 0.0f, -10.0f), color(1.0f, 1.0f, 1.0f))")
	{
		auto m = Material();
		m.pattern = createStripPattern(Color::White, Color::Black);
		m.ambient = 1.0f;
		m.diffuse = 0.0f;
		m.specular = 0.0f;
		auto viewDirection = vector(0.0f, 0.0f, -1.0f);
		auto normal = vector(0.0f, 0.0f, -1.0f);
		auto light = pointLight(point(0.0f, 0.0f, -10.0f), color(1.0f, 1.0f, 1.0f));
		WHEN("c1 = lighting(m, light, point(0.9F, 0.0F, 0.0F), viewDirection, normal, false)"
			"And c2 == lighting(m, light, point(1.1f, 0.0f, 0.0f), viewDirection, normal, false)")
		{
			auto c1 = lighting(m, light, point(0.9F, 0.0F, 0.0F), viewDirection, normal, false);
			auto c2 = lighting(m, light, point(1.1f, 0.0f, 0.0f), viewDirection, normal, false);
			THEN("c1 == color(1.0f, 1.0f, 1.0f)"
				"And c2 == color(0.0f, 0.0f, 0.0f)")
			{
				REQUIRE(c1 == Color::White);
				REQUIRE(c2 == Color::Black);
			}
		}
	}
}