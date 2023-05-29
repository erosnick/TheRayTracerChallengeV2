#include <catch2/catch_test_macros.hpp>

#include <intersection.h>

// Chapter 5 Ray-Sphere intersections

SCENARIO("An intersection encapsulates t and object", "[intersection]")
{
	GIVEN("s = Sphere()")
	{
		auto s = std::make_shared<Sphere>();

		WHEN("i = Intersection(3.5f, s.id)")
		{
			auto i = Intersection{ 3.5f, s };
			THEN("i.t == 3.5f")
			{
				REQUIRE(i.object == s);
			}
		}
	}
}

SCENARIO("Aggregating intersections", "[intersection]")
{
	GIVEN("s = Sphere()"
		"And i1 = intersection(1.0f, s)"
		"And i2 = intersection(2.0f, s)")
	{
		auto s = std::make_shared<Sphere>();

		auto i1 = Intersection{ 1.0f, s };
		auto i2 = Intersection{ 2.0f, s };

		WHEN("xs = intersections(i1, i2)")
		{
			auto xs = intersections({ i1, i2 });

			THEN("xs.count == 2.0f"
				"And xs[0].t == 1.0f"
				"And xs[1].t == 2.0f")
			{
				REQUIRE(xs.size() == 2);
				REQUIRE(xs[0].t == 1.0f);
				REQUIRE(xs[1].t == 2.0f);
			}
		}
	}
}

SCENARIO("The hit, when all intersections have positive t", "[intersection]")
{
	GIVEN("s = Sphere()"
		"And i1 = intersection(1, s)"
		"And i2 = intersection(2, s)"
		"And xs = intersections(i2, i1)")
	{
		auto s = std::make_shared<Sphere>();
		auto i1 = Intersection{ 1.0f, s };
		auto i2 = Intersection{ 2.0f, s };
		auto xs = intersections({ i2, i1 });

		WHEN("i = hit(xs)")
		{
			auto i = hit(xs);

			THEN("i == i1")
			{
				REQUIRE(i == i1);
			}
		}
	}
}

SCENARIO("The hit, when some intersections have negative t", "[intersection]")
{
	GIVEN("s = Sphere()"
		"And i1 = intersection(-1.0f, s)"
		"And i2 = intersection(1.0f, s)"
		"And xs = intersections(i2, i1)")
	{
		auto s = std::make_shared<Sphere>();
		auto i1 = Intersection{ -1.0f, s };
		auto i2 = Intersection{ 1.0f, s };
		auto xs = intersections({ i2, i1 });

		WHEN("i = hit(xs)")
		{
			auto i = hit(xs);

			THEN("i == i2")
			{
				REQUIRE(i == i2);
			}
		}
	}
}

SCENARIO("The hit, when all intersections have negative t", "[intersection]")
{
	GIVEN("s = Sphere()"
		"And i1 = intersection(-2.0f, s)"
		"And i2 = intersection(-1.0f, s)"
		"And xs = intersections(i2, i1)")
	{
		auto s = std::make_shared<Sphere>();
		auto i1 = Intersection{ -2.0f, s };
		auto i2 = Intersection{ -1.0f, s };
		auto xs = intersections({ i2, i1 });

		WHEN("i = hit(xs)")
		{
			auto i = hit(xs);

			THEN("i == i2")
			{
				REQUIRE(i.t == 0.0f);
			}
		}
	}
}

SCENARIO("The hit is always the lowest nonnegative intersection", "[intersection]")
{
	GIVEN("s = Sphere()"
		"And i1 = intersection(5.0f, s)"
		"And i2 = intersection(7.0f, s)"
		"And i3 = intersection(-3.0f, s)"
		"And i4 = intersection(2.0f, s)"
		"And xs = intersections(i1, i2, i3, i4)")
	{
		auto s = std::make_shared<Sphere>();
		auto i1 = Intersection{ 5.0f, s };
		auto i2 = Intersection{ 7.0f, s };
		auto i3 = Intersection{ -3.0f, s };
		auto i4 = Intersection{ 2.0f, s };
		auto xs = intersections({ i1, i2, i3, i4 });

		WHEN("i = hit(xs)")
		{
			auto i = hit(xs);

			THEN("i == i4")
			{
				REQUIRE(i == i4);
			}
		}
	}
}

// Chapter 7 Making a Scene

SCENARIO("Precomputing the state of an intersection", "[intersections]")
{
	GIVEN("r = Ray(point(0.0f, 0.0f, -5.0f), vector(0.0f, 0.0f, 1.0f))"
		"And shape = Sphere()"
		"And i = intersection(4.0f, shape)")
	{
		auto r = Ray(point(0.0f, 0.0f, -5.0f), vector(0.0f, 0.0f, 1.0f));
		auto shape = createSphere();
		auto i = Intersection{ 4.0f, shape };
		WHEN("comps = prepareComputations(i, r)")
		{
			auto comps = prepareComputations(i, r);
			THEN("comps.t == i.t"
				"And comps.object == i.object"
				"And comps.point == point(0¡£0f, 0.0f, -1.0f)"
				"And comps.eyev == vector(0.0f, 0.0f, -1.0f)"
				"And comps.normalv == vector(0.0f, 0.0f, -1.0f)")
			{
				REQUIRE(comps.t == i.t);
				REQUIRE(comps.position == point(0.0f, 0.0f, -1.0f));
				REQUIRE(comps.viewDirection == vector(0.0f, 0.0f, -1.0f));
				REQUIRE(comps.normal == vector(0.0f, 0.0f, -1.0f));
			}
		}
	}
}

SCENARIO("The hit, when an intersection occurs on the outside", "[intersections]")
{
	GIVEN("r = Ray(point(0.0f, 0.0f, -5.0f), vector(0.0f, 0.0f, 1.0f))"
		"And shape = Sphere()"
		"And i = intersection(4.0f, shape)")
	{
		auto r = Ray(point(0.0f, 0.0f, -5.0f), vector(0.0f, 0.0f, 1.0f));
		auto shape = createSphere();
		auto i = Intersection{ 4.0f, shape };
		WHEN("comps = prepareComputations(i, r)")
		{
			auto comps = prepareComputations(i, r);
			THEN("comps.inside == false")
			{
				REQUIRE(comps.inside == false);
			}
		}
	}
}

SCENARIO("The hit, when an intersection occurs on the inside", "[intersections]")
{
	GIVEN("r = Ray(point(0.0f, 0.0f, 0.0f), vector(0.0f, 0.0f, 1.0f))"
		"And shape = Sphere()"
		"And i = intersection(1.0f, shape)")
	{
		auto r = Ray(point(0.0f, 0.0f, 0.0f), vector(0.0f, 0.0f, 1.0f));
		auto shape = createSphere();
		auto i = Intersection{ 1.0f, shape };
		WHEN("comps = prepareComputations(i, r)")
		{
			auto comps = prepareComputations(i, r);
			THEN("comps.inside == false")
			{
				REQUIRE(comps.position == point(0.0f, 0.0f, 1.0f));
				REQUIRE(comps.viewDirection == vector(0.0f, 0.0f, -1.0f));
				REQUIRE(comps.inside == true);
				// Normal would have been (0, 0, 1), but is inverted!
				REQUIRE(comps.normal == vector(0.0f, 0.0f, -1.0f));
			}
		}
	}
}

// Chapter 8 Shadows

SCENARIO("The hit should offset the point", "[intersections]")
{
	GIVEN("r = Ray(point(0.0f, 0.0f, -5.0f), vector(0.0f, 0.0f, 1.0f))"
		"And shape = Sphere() with:"
		"| transform | translation(0.0f, 0.0f, 1.0f) |"
		"And i = intersection(5.0f, shape)")
	{
		auto r = Ray(point(0.0f, 0.0f, -5.0f), vector(0.0f, 0.0f, 1.0f));
		auto shape = createSphere();
		shape->setTransform(translate(0.0f, 0.0f, 1.0f));
		auto i = Intersection{ 5.0f, shape };
		WHEN("comps = prepareComputations(i, r)")
		{
			auto comps = prepareComputations(i, r);
			THEN("comps.overPoint.z < -EPSILON / 2.0f"
				"And comps.position.z > comps.overPosition.z")
			{
				REQUIRE(comps.overPosition.z < -EPSILON / 2.0f);
			}
		}
	}
}