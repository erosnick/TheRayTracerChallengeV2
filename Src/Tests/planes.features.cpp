#include <catch2/catch_test_macros.hpp>

#include "plane.h"

SCENARIO("The normal of a plane is constant everywhere", "[planes]")
{
	GIVEN("p = Plane()")
	{
		auto p = Plane();
		WHEN("n1 = localNormalAt(p, point(0.0f, 0.0f, 0.0f))"
			"And n2 = localNormalAt(p, point(10.0f, 0.0f, -10.0f))"
			"And n3 = localNormalAt(p, point(-5.0f, 0.0f, 150.0f))")
		{
			auto n1 = p.localNormalAt(point(0.0f, 0.0f, 0.0f));
			auto n2 = p.localNormalAt(point(10.0f, 0.0f, -10.0f));
			auto n3 = p.localNormalAt(point(-5.0f, 0.0f, 150.0f));
			THEN("n1 == vector(0.0f, 1.0f, 0.0f)"
				"And n2 == vector(0.0f, 1.0f, 0.0f)"
				"And n3 == vector(0.0f, 1.0f, 0.0f)")
			{
				REQUIRE(n1== vector(0.0f, 1.0f, 0.0f));
				REQUIRE(n2 == vector(0.0f, 1.0f, 0.0f));
				REQUIRE(n3 == vector(0.0f, 1.0f, 0.0f));
			}
		}
	}
}

SCENARIO("Intersect with a ray parallel to the plane", "[planes]")
{
	GIVEN("p = Plane()"
		"And r = Ray(point(0.0f, 10.0f, 0.0f), vector(0.0f, 0.0f, 1.0f))")
	{
		auto p = Plane();
		auto r = Ray(point(0.0f, 10.0f, 0.0f), vector(0.0f, 0.0f, 1.0f));
		WHEN("xs = localIntersect(p, r)")
		{
			auto xs = p.localIntersect(r);
			THEN("xs is empty")
			{
				REQUIRE(xs.size() == 0);
			}
		}
	}
}

SCENARIO("Intersect with a coplanar ray", "[planes]")
{
	GIVEN("p = Plane()"
		"And r = Ray(point(0.0f, 0.0f, 0.0f), vector(0.0f, 0.0f, 1.0f))")
	{
		auto p = Plane();
		auto r = Ray(point(0.0f, 0.0f, 0.0f), vector(0.0f, 0.0f, 1.0f));
		WHEN("xs = localIntersect(p, r)")
		{
			auto xs = p.localIntersect(r);
			THEN("xs is empty")
			{
				REQUIRE(xs.size() == 0);
			}
		}
	}
}

SCENARIO("A ray intersecting a plane from above", "[planes]")
{
	GIVEN("p = Plane()"
		"And r = Ray(point(0.0f, 1.0f, 0.0f), vector(0.0f, -1.0f, 0.0f))")
	{
		auto p = createPlane();
		auto r = Ray(point(0.0f, 1.0f, 0.0f), vector(0.0f, -1.0f, 1.0f));
		WHEN("xs = localIntersect(p, r)")
		{
			auto xs = p->localIntersect(r);
			THEN("xs.count == 1"
			"xs[0].t = 1.0f"
			"xs[0].shape == p)")
			{
				REQUIRE(xs.size() == 1);
				REQUIRE(xs[0].t == 1.0f);
				REQUIRE(xs[0].shape == p);
			}
		}
	}
}

SCENARIO("A ray intersecting a plane from below", "[planes]")
{
	GIVEN("p = Plane()"
		"And r = Ray(point(0.0f, -1.0f, 0.0f), vector(0.0f, 1.0f, 0.0f))")
	{
		auto p = createPlane();
		auto r = Ray(point(0.0f, -1.0f, 0.0f), vector(0.0f, 1.0f, 1.0f));
		WHEN("xs = localIntersect(p, r)")
		{
			auto xs = p->localIntersect(r);
			THEN("xs.count == 1"
				"xs[0].t = 1.0f"
				"xs[0].shape == p)")
			{
				REQUIRE(xs.size() == 1);
				REQUIRE(xs[0].t == 1.0f);
				REQUIRE(xs[0].shape == p);
			}
		}
	}
}