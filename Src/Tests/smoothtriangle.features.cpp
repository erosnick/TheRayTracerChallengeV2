#include <catch2/catch_test_macros.hpp>

#include <triangle.h>

// Chapter 15 Triangles

SCENARIO("Constructing a smooth triangle", "[smoothtriangle]")
{
	// Background
	auto p0 = point(0, 1, 0);
	auto p1 = point(-1, 0, 0);
	auto p2 = point(1, 0, 0);
	auto n0 = vector(0, 1, 0);
	auto n1 = vector(-1, 0, 0);
	auto n2 = vector(1, 0, 0);
	auto tri = Triangle(p0, p1, p2, n0, n1, n2);

		THEN("Then tri.p0 == p0"
			 "And tri.p1 == p1"
			 "And tri.p2 == p2"
			 "And tri.n0 == n0"
			 "And tri.n1 == n1"
			 "And tri.n2 == n2")
		{
			REQUIRE(tri.p0 == p0);
			REQUIRE(tri.p1 == p1);
			REQUIRE(tri.p2 == p2);
			REQUIRE(tri.n0 == n0);
			REQUIRE(tri.n1 == n1);
			REQUIRE(tri.n2 == n2);
		}
}

SCENARIO("An intersection with a smooth triangle stores u/v", "[smoothtriangle]")
{
	// Background
	auto p0 = point(0, 1, 0);
	auto p1 = point(-1, 0, 0);
	auto p2 = point(1, 0, 0);
	auto n0 = vector(0, 1, 0);
	auto n1 = vector(-1, 0, 0);
	auto n2 = vector(1, 0, 0);
	auto tri = createSmoothTriangle(p0, p1, p2, n0, n1, n2);

	WHEN("r = Ray(point(-0.2f, 0.3f, -2.0f), vector(0.0f, 0.0f, 1.0f))"
		"And xs = localIntersect(tri, r)")
	{
		auto r = Ray(point(-0.2f, 0.3f, -2.0f), vector(0.0f, 0.0f, 1.0f));
		auto xs = tri->localIntersect(r);
		THEN("xs[0].u == 0.45f"
			"And xs[0].v == 0.25f")
		{
			REQUIRE(xs[0].u == 0.45f);
			REQUIRE(xs[0].v == 0.25f);
		}
	}
}

SCENARIO("A smooth triangle uses u/v to interpolate the normal", "[smoothtriangle]")
{
	// Background
	auto p0 = point(0, 1, 0);
	auto p1 = point(-1, 0, 0);
	auto p2 = point(1, 0, 0);
	auto n0 = vector(0, 1, 0);
	auto n1 = vector(-1, 0, 0);
	auto n2 = vector(1, 0, 0);
	auto tri = createSmoothTriangle(p0, p1, p2, n0, n1, n2);

	WHEN("i = intersectionWithUV(1.0f, tri, 0.45f, 0.25f)"
		"And n = normalAt(tri, point(0.0f, 0.0f, 0.0f), i)")
	{
		auto i = intersectionWithUV(1.0f, tri, 0.45f, 0.25f);
		auto n = tri->normalAt(point(0.0f, 0.0f, 0.0f), i);
		THEN("n == vector(-0.5547f, 0.83205f, 0.0f)")
		{
			REQUIRE(n == vector(-0.5547f, 0.83205f, 0.0f));
		}
	}
}

SCENARIO(" Preparing the normal on a smooth triangle", "[smoothtriangle]")
{
	// Background
	auto p0 = point(0, 1, 0);
	auto p1 = point(-1, 0, 0);
	auto p2 = point(1, 0, 0);
	auto n0 = vector(0, 1, 0);
	auto n1 = vector(-1, 0, 0);
	auto n2 = vector(1, 0, 0);
	auto tri = createSmoothTriangle(p0, p1, p2, n0, n1, n2);

	WHEN("i = intersectionWithUV(1.0f, tri, 0.45f, 0.25f)"
		"And r = Ray(point(-0.2f, 0.3f, -2.0f), vector(0.0f, 0.0f, 1.0f))"
		"And xs = intersections(i)"
		"And comps = prepareComputations(i, r, xs)")
	{
		auto i = intersectionWithUV(1.0f, tri, 0.45f, 0.25f);
		auto r = Ray(point(-0.2f, 0.3f, -2.0f), vector(0.0f, 0.0f, 1.0f));
		auto xs = sortIntersections({ i });
		auto comps = prepareComputations(i, r, xs);
		THEN("comps.normalv == vector(-0.5547f, 0.83205f, 0.0f)")
		{
			REQUIRE(comps.normal == vector(-0.5547f, 0.83205f, 0.0f));
		}
	}
}