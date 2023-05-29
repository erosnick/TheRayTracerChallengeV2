// Chapter 6 Light and Shadings 

#include <catch2/catch_test_macros.hpp>

#include <sphere.h>

SCENARIO("A sphere has a default material", "[sphere]")
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

SCENARIO(" A sphere may be assigned a material", "[sphere]")
{
	GIVEN("s = Sphere()"
		"And m = Material()"
		"And m.ambient = 1")
	{
		auto s = Sphere();
		auto m = Material();
		m.ambient = 1.0f;
		WHEN("s.material = m")
		{
			s.material = m;
			auto m = s.material;
			THEN("s.material == m")
			{
				REQUIRE(s.material == m);
			}
		}
	}
}