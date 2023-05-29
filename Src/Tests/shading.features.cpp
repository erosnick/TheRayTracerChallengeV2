#include <catch2/catch_test_macros.hpp>

#include <ray.h>
#include <sphere.h>

#include <canvas.h>

#include <intersection.h>

#include "colors.h"
#include <iostream>

SCENARIO("The normal on a sphere at a point on the x axis", "[shading]")
{
	GIVEN(" s = Sphere()")
	{
		auto s = std::make_shared<Sphere>();

		WHEN("n = normalAt(s, point(1.0f, 0.0f, 0.0f))")
		{
			auto n = normalAt(s, point(1.0f, 0.0f, 0.0f));

			THEN("n == vector(1.0f, 0.0f, 0.0f)")
			{
				REQUIRE(n == vector(1.0f, 0.0f, 0.0f));
			}
		}
	}
}

SCENARIO("The normal on a sphere at a point on the y axis", "[shading]")
{
	GIVEN(" s = Sphere()")
	{
		auto s = std::make_shared<Sphere>();

		WHEN("n = normalAt(s, point(0.0f, 1.0f, 0.0f))")
		{
			auto n = normalAt(s, point(0.0f, 1.0f, 0.0f));

			THEN("n == vector(0.0f, 1.0f, 0.0f)")
			{
				REQUIRE(n == vector(0.0f, 1.0f, 0.0f));
			}
		}
	}
}

SCENARIO("The normal on a sphere at a point on the z axis", "[shading]")
{
	GIVEN(" s = Sphere()")
	{
		auto s = std::make_shared<Sphere>();

		WHEN("n = normalAt(s, point(0.0f, 0.0f, 1.0f))")
		{
			auto n = normalAt(s, point(0.0f, 0.0f, 1.0f));

			THEN("n == vector(0.0f, 0.0f, 1.0f)")
			{
				REQUIRE(n == vector(0.0f, 0.0f, 1.0f));
			}
		}
	}
}

SCENARIO("The normal on a sphere at a nonaxial point", "[shading]")
{
	GIVEN(" s = Sphere()")
	{
		auto s = std::make_shared<Sphere>();

		WHEN("n = normalAt(s, point(¡Ì3/3, ¡Ì3/3, ¡Ì3/3))")
		{
			auto n = normalAt(s, point(SQRT3 / 3.0f, SQRT3 / 3.0f, SQRT3 / 3.0f));

			THEN("n == vector(¡Ì3/3, ¡Ì3/3, ¡Ì3/3)")
			{
				REQUIRE(n == vector(SQRT3 / 3.0f, SQRT3 / 3.0f, SQRT3 / 3.0f));
			}
		}
	}
}

SCENARIO("he normal is a normalized vector", "[shading]")
{
	GIVEN(" s = Sphere()")
	{
		auto s = std::make_shared<Sphere>();

		WHEN("n = normalAt(s, point(¡Ì3/3, ¡Ì3/3, ¡Ì3/3))")
		{
			auto n = normalAt(s, point(SQRT3 / 3.0f, SQRT3 / 3.0f, SQRT3 / 3.0f));

			THEN("n == normalize(n)")
			{
				REQUIRE(n == normalize(n));
			}
		}
	}
}

SCENARIO("Computing the normal on a translated sphere", "[shading]")
{
	GIVEN(" s = Sphere()"
	"And s.setTransform(translate(0.0f, 1.0f, 0.0f))")
	{
		auto s = std::make_shared<Sphere>();

		s->setTransform(translate(0.0f, 1.0f, 0.0f));

		WHEN("n = normalAt(s, point(0.0f, 1.70711f, -0.70711f))")
		{
			auto n = normalAt(s, point(0.0f, 1.70711f, -0.70711f));

			THEN("n == vector(0.0f, 0.70711f, -0.70711f)")
			{
				REQUIRE(n == vector(0.0f, 0.70711f, -0.70711f));
			}
		}
	}
}

SCENARIO("Computing the normal on a transformed sphere", "[shading]")
{
	GIVEN(" s = Sphere()"
		"And = scale(1.0f, 0.5f, 1.0f) * rotationZ(¦Ð / 5.0f)"
		"And s.setTransform(translate(0.0f, 1.0f, 0.0f))")
	{
		auto s = std::make_shared<Sphere>();

		auto m = scale(1.0f, 0.5f, 1.0f) * rotateZ(PI / 5.0f);

		s->setTransform(m);

		WHEN("n = normalAt(s, point(0, ¡Ì2/2, -¡Ì2/2))")
		{
			auto n = normalAt(s, point(0.0f, SQRT2 / 2.0f, -SQRT2 / 2.0f));

			THEN("n == vector(0.0f, 0, 0.97014, -0.24254)")
			{
				REQUIRE(n == vector(0.0f, 0.97014f, -0.24254f));
			}
		}
	}
}