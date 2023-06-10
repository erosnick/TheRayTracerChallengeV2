#include <catch2/catch_test_macros.hpp>

#include <triangle.h>

#include <tuple.h>

// Chapter 15 Triangles

SCENARIO("Constructing a triangle", "[triangle]")
{
	GIVEN("p0 = point(0.0f, 1.0f, 0.0f)"
		  "And p1 = point(-1.0f, 0.0f, 0.0f)"
		  "And p2 = point(1.0f, 0.0f, 0.0f)"
		  "And t = Triangle(p1, p2, p3)")
	{
		auto p0 = point(0.0f, 1.0f, 0.0f);
		auto p1 = point(-1.0f, 0.0f, 0.0f);
		auto p2 = point(1.0f, 0.0f, 0.0f);
		auto t = Triangle(p0, p1, p2);
		THEN("t.p0 == p0"
			 "And t.p1 == p1"
			 "And t.p2 == p2"
			 "And t.e0 == vector(-1.0f, -1.0f, 0.0f)"
			 "And t.e1 == vector(1.0f, -1.0f, 0.0f)"
			 "And t.normal == vector(0.0f, 0.0f, -1.0f)")
		{
			REQUIRE(t.p0 == p0);
			REQUIRE(t.p1 == p1);
			REQUIRE(t.p2 == p2);
		}
	}
}

SCENARIO("Finding the normal on a triangle", "[triangle]")
{
	GIVEN("t = Triangle(point(0.0f, 1.0f, 0.0f), point(-1.0f, 0.0f, 0.0f), point(1.0f, 0.0f, 0.0f))")
	{
		auto t = Triangle(point(0.0f, 1.0f, 0.0f), point(-1.0f, 0.0f, 0.0f), point(1.0f, 0.0f, 0.0f));
		WHEN("n0 = localNormalAt(t, point(0.0f, 0.5f, 0.0f))"
			"And n0 = localNormalAt(t, point(-0.5f, 0.75f, 0.0f))"
			"And n0 = localNormalAt(t, point(0.5f, 0.25f, 0.0f))")
		{
			auto n0 = t.localNormalAt(point(0.0f, 0.5f, 0.0f));
			auto n1 = t.localNormalAt(point(-0.5f, 0.75f, 0.0f));
			auto n2 = t.localNormalAt(point(0.5f, 0.25f, 0.0f));
			THEN("n0 == t.normal"
				 "And n1 == t.normal"
				 "And n2 == t.normal")
			{
				REQUIRE(n0 == t.normal);
				REQUIRE(n1 == t.normal);
				REQUIRE(n2 == t.normal);
			}
		}
	}
}

SCENARIO("Intersecting a ray parallel to the triangle", "[triangle]")
{
	GIVEN("t = Triangle(point(0.0f, 1.0f, 0.0f), point(-1.0f, 0.0f, 0.0f), point(1.0f, 0.0f, 0.0f))"
	"r = Ray(point(0.0f, -1.0f, -2.0f), vector(0.0f, 1.0f, 0.0f))")
	{
		auto t = Triangle(point(0.0f, 1.0f, 0.0f), 
								  point(-1.0f, 0.0f, 0.0f), 
								  point(1.0f, 0.0f, 0.0f));
		auto r = Ray(point(0.0f, -1.0f, -2.0f), vector(0.0f, 1.0f, 0.0f));
		WHEN("xs = localIntersect(t, r)")
		{
			auto xs = t.localIntersect(r);
			THEN("xs is empty")
			{
				REQUIRE(xs.empty());
			}
		}
	}
}

SCENARIO("A ray misses the p0-p2 edge", "[triangle]")
{
	GIVEN("t = Triangle(point(0.0f, 1.0f, 0.0f), point(-1.0f, 0.0f, 0.0f), point(1.0f, 0.0f, 0.0f))"
		  "And r = Ray(point(1.0f, 1.0f, -2.0f), vector(0.0f, 0.0f, 1.0f))")
	{
		auto t = Triangle(point(0.0f, 1.0f, 0.0f), 
								  point(-1.0f, 0.0f, 0.0f), 
								point(1.0f, 0.0f, 0.0f));
		auto r = Ray(point(1.0f, 1.0f, -2.0f), vector(0.0f, 0.0f, 1.0f));
		WHEN("xs = localIntersect(t, r)")
		{
			auto xs = t.localIntersect(r);
			THEN("xs is empty")
			{
				REQUIRE(xs.empty());
			}
		}
	}
}

SCENARIO("A ray misses the p0-p1 edge", "[triangle]")
{
	GIVEN("t = Triangle(point(0.0f, 1.0f, 0.0f), point(-1.0f, 0.0f, 0.0f), point(1.0f, 0.0f, 0.0f))"
		  "And r = Ray(point(1.0f, 1.0f, -2.0f), vector(0.0f, 0.0f, 1.0f))")
	{
		auto t = createTriangle(point(0.0f, 1.0f, 0.0f), 
											      point(-1.0f, 0.0f, 0.0f), 
												  point(1.0f, 0.0f, 0.0f));
		auto r = Ray(point(1.0f, 1.0f, -2.0f), vector(0.0f, 0.0f, 1.0f));

		WHEN("xs = localIntersect(t, r)")
		{
			auto xs = t->localIntersect(r);
			THEN("xs is empty")
			{
				REQUIRE(xs.empty());
			}
		}
	}
}

SCENARIO("A ray misses the p1-p2 edge", "[triangle]")
{
	GIVEN("t = Triangle(point(0.0f, 1.0f, 0.0f), point(-1.0f, 0.0f, 0.0f), point(1.0f, 0.0f, 0.0f))"
		"And r = Ray(point(-1.0f, 1.0f, -2.0f), vector(0.0f, 0.0f, 1.0f))")
	{
		auto t = createTriangle(point(0.0f, 1.0f, 0.0f),
												  point(-1.0f, 0.0f, 0.0f),
												  point(1.0f, 0.0f, 0.0f));
		auto r = Ray(point(-1.0f, 1.0f, -2.0f), vector(0.0f, 0.0f, 1.0f));

		WHEN("xs = localIntersect(t, r)")
		{
			auto xs = t->localIntersect(r);
			THEN("xs is empty")
			{
				REQUIRE(xs.empty());
			}
		}
	}
}

SCENARIO("A ray strikes a triangle", "[triangle]")
{
	GIVEN("t = Triangle(point(0.0f, 1.0f, 0.0f), point(-1.0f, 0.0f, 0.0f), point(1.0f, 0.0f, 0.0f))"
		"And r = Ray(point(0.0f, 0.5f, -2.0f), vector(0.0f, 0.0f, 1.0f))")
	{
		auto t = createTriangle(point(0.0f, 1.0f, 0.0f),
												  point(-1.0f, 0.0f, 0.0f),
												  point(1.0f, 0.0f, 0.0f));
		auto r = Ray(point(0.0f, 0.5f, -2.0f), vector(0.0f, 0.0f, 1.0f));

		WHEN("xs = localIntersect(t, r)")
		{
			auto xs = t->localIntersect(r);
			THEN("xs.count == 1"
				"And xs[0].t == 2.0f")
			{
				REQUIRE(xs.size() == 1);
				REQUIRE(xs[0].t == 2.0f);
			}
		}
	}
}