#include <catch2/catch_test_macros.hpp>

#include <cstdint>

#include <tuple.h>
#include <canvas.h>

#include "utils.h"

SCENARIO("Colors are (red, green, blue) tuples", "[canvas]")
{
	GIVEN(ToString(c = color(-0.5f, 0.4f, 1.7f)))
	{
		auto c = color(-0.5f, 0.4f, 1.7f);

		THEN("Check members")
		{
			REQUIRE(equal(c.red, -0.5f));
			REQUIRE(equal(c.green, 0.4f));
			REQUIRE(equal(c.blue, 1.7f));
		}
	}
}

SCENARIO("Adding colors", "[canvas]")
{
	GIVEN(ToString(c1 = color(0.9f, 0.6f, 0.75f)
			   and c2 = color(0.7f, 0.1f, 0.25f)))
	{
		auto c1 = color(0.9f, 0.6f, 0.75f);
		auto c2 = color(0.7f, 0.1f, 0.25f);

		THEN("c1 + c2 = color(1.6, 0.7, 1.0")
		{
			REQUIRE(c1 + c2 == color(1.6f, 0.7f, 1.0f));
		}
	}
}

SCENARIO("Subtracting colors", "[canvas]")
{
	GIVEN(ToString(c1 = color(0.9f, 0.6f, 0.75f)
			   and c2 = color(0.7f, 0.1f, 0.25f)))
	{
		auto c1 = color(0.9f, 0.6f, 0.75f);
		auto c2 = color(0.7f, 0.1f, 0.25f);

		THEN("c1 - c2 == color(0.2, 0.5, 0.5)")
		{
			REQUIRE(c1 - c2 == color(0.2f, 0.5f, 0.5f));
		}
	}
}

SCENARIO("Multiplying ac olor by a scalar", "[canvas]")
{
	GIVEN(ToString(c = color(0.2f, 0.3f, 0.4f)))
	{
		auto c = color(0.2f, 0.3f, 0.4f);

		THEN("c * 2.0f = color(0.4f, 0.6f, 0.8f)")
		{
			REQUIRE(c * 2.0f == color(0.4f, 0.6f, 0.8f));
		}
	}
}

SCENARIO("Multiplying colors", "[canvas]")
{
	GIVEN(ToString(c1 = color(1.0f, 0.2f, 0.4f)
			   and c2 = color(0.9f, 1.0f, 0.1f)))
	{
		auto c1 = color(1.0f, 0.2f, 0.4f);
		auto c2 = color(0.9f, 1.0f, 0.1f);

		THEN("c * 2 == color(0.4f, 0.6f, 0.8f)")
		{
			REQUIRE(c1 * c2 == color(0.9f, 0.2f, 0.04f));
		}
	}
}


SCENARIO("Creating a canvas", "[canvas]")
{
	GIVEN(ToString(c = Canvas(10, 20)))
	{
		auto c = Canvas(10, 20);

		THEN("Checking members")
		{
			REQUIRE(c.width == 10);
			REQUIRE(c.height == 20);
			REQUIRE(c.black());
		}
	}
}

SCENARIO("Writing pixels to a canvas", "[canvas]")
{
	GIVEN(ToString(c = Canvas(10, 20)))
	{
		auto c = Canvas(10, 20);
		auto red = color(1.0f, 0.0f, 0.0f);

		WHEN("c.writePixel(2, 3, red)")
		{
			c.writePixel(2, 3, red);

			THEN("pixelAt(2, 3) == red")
			{
				REQUIRE(c.pixelAt(2, 3) == red);
			}
		}
	}
}

SCENARIO("Constructing the PPM header", "[canvas]")
{
	GIVEN(ToString(c = Canvas(5, 3)))
	{
		auto c = Canvas(5, 3);

		WHEN("c = writeToPPM()")
		{
			c.writeToPPM("render.ppm");

			THEN("lines 1-3 of ppm are")
			{
				REQUIRE(true);
			}
		}
	}
}

SCENARIO("Constructing the PPM pixel data", "[canvas]")
{
	GIVEN(ToString(c = Canvas(5, 3)))
	{
		auto c1 = color(1.5f, 0.0f, 0.0f);
		auto c2 = color(0.0f, 0.5f, 0.0f);
		auto c3 = color(-0.5f, 0.0f, 1.0f);

		WHEN("writePixel(0, 0, c1)"
			 "writePixel(2, 1, c2)"
			 "writePixel(4, 2, c3)")
		{
			auto c = Canvas(5, 3);

			c.writePixel(0, 0, c1);
			c.writePixel(2, 1, c2);
			c.writePixel(4, 2, c3);

			c.writeToPPM("render.ppm");

			THEN("lines 4-6 of ppm are")
			{
				REQUIRE(c.pixelAt(0, 0) == color(1.0f, 0.0f, 0.0f));
				REQUIRE(c.pixelAt(2, 1) == color(0.0f, 0.5f, 0.0f));
				REQUIRE(c.pixelAt(4, 2) == color(0.0f, 0.0f, 1.0f));
			}
		}
	}
}

struct Projectile
{
	tuple position;
	tuple velocity;
};

struct Environment
{
	tuple gravity;
	tuple wind;
};

Projectile projectile(const tuple& position, const tuple& velocity)
{
	return { position, velocity };
}

Environment environment(const tuple& gravity, const tuple& wind)
{
	return { gravity, wind };
}

Projectile tick(const Environment& environment, const Projectile& projectile)
{
	auto position = projectile.position + projectile.velocity;
	auto velocity = projectile.velocity + environment.gravity + environment.wind;

	return { position, velocity };
}

SCENARIO("Projectile", "[canvas]")
{
	GIVEN("")
	{
		auto start = point(0.0f, 1.0f, 0.0f);
		auto velocity = normalize(vector(1.0f, 1.8f, 0.0f)) * 11.25f;
		auto p = projectile(start, velocity);
		auto gravity = vector(0.0f, -0.1f, 0.0f);
		auto wind = vector(-0.01f, 0.0f, 0.0f);
		auto e = environment(gravity, wind);

		auto c = Canvas(900, 550);

		auto red = color(1.0f, 0.0f, 0.0f);

		for (int i = 0; i < 200; i++)
		{
			p = tick(e, p);

			auto x = static_cast<uint32_t>(p.position.x);
			auto y = c.height - static_cast<uint32_t>(p.position.y);

			c.writePixel(x, y, red);
		}

		c.writeToPPM("render.ppm");
	}
}