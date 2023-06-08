#include <catch2/catch_test_macros.hpp>

#include <shape.h>
#include <group.h>
#include <sphere.h>

// Chapter 14 Groups

SCENARIO("Adding a child to a group", "[group]")
{
	GIVEN("g = Group()"
		"And s = testShape()")
	{
		auto g = createGroup();
		auto s = testShape();
		WHEN("addChild(g, s)")
		{
			g->addChild(s);
			THEN("g is not empty"
				"And g includes s"
				"And s.parent == g")
			{
				REQUIRE(!g->isEmpty());
				REQUIRE(s->parent == g);
			}
		}
	}
}

SCENARIO("Intersecting a ray with an empty group", "[group]")
{
	GIVEN("g = Group()"
		  "And r = Ray(point(0.0f, 0.0f, 0.0f), vector(0.0f, 0.0f, 1.0f))")
	{
		auto g = Group();
		auto r = Ray(point(0.0f, 0.0f, 0.0f), vector(0.0f, 0.0f, 1.0f));
		WHEN("xs = localIntersect(g, r)")
		{
			auto xs = g.localIntersect(r);
			THEN("xs is empty")
			{
				REQUIRE(xs.empty());
			}
		}
	}
}

SCENARIO("Intersecting a ray with a nonempty group", "[group]")
{
	GIVEN("g = Group()"
		"And s1 = Sphere()"
		"And s2 = Sphere()"
		"And setTransform(s2, translation(0.0f, 0.0f, -3.0f))"
		"And s3 = Sphere()"
		"And setTransform(s3, translation(5.0f, 0.0f, 0.0f))"
		"And addChild(g, s1)"
		"And addChild(g, s2)"
		"And addChild(g, s3)")
	{
		auto g = createGroup();
		auto s1 = createSphere();
		auto s2 = createSphere();
		s2->setTransform(translate(0.0f, 0.0f, -3.0f));
		auto s3 = createSphere();
		s3->setTransform(translate(5.0f, 0.0f, 0.0f));
		g->addChild(s1);
		g->addChild(s2);
		g->addChild(s3);
		WHEN("r = Ray(point(0.0f, 0.0f, -5.0f), vector(0.0f, 0.0f, 1.0f))"
			"And xs = localIntersect(g, r)")
		{
			auto r = Ray(point(0.0f, 0.0f, -5.0f), vector(0.0f, 0.0f, 1.0f));
			auto xs = g->localIntersect(r);
			THEN("xs.count == 4"
				"And xs[0].object == s2"
				"And xs[1].object == s2"
				"And xs[2].object == s1"
				"And xs[3].object == s1")
			{
				REQUIRE(xs.size() == 4);
				REQUIRE(xs[0].shape == s2);
				REQUIRE(xs[1].shape == s2);
				REQUIRE(xs[2].shape == s1);
				REQUIRE(xs[3].shape == s1);
			}
		}
	}
}

SCENARIO("Intersecting a transformed group", "[group]")
{
	GIVEN("g = Group()"
		"And setTransform(g, scaling(2.0f, 2.0f, 2.0f))"
		"And s = Sphere()"
		"And setTransform(s, translation(5.0f, 0.0f, 0.0f))"
		"And addChild(g, s)")
	{
		auto g = createGroup();
		g->setTransform(scale(2.0f));
		auto s = createSphere();
		s->setTransform(translate(5.0f, 0.0f, 0.0f));
		g->addChild(s);
		WHEN("r = Ray(point(10.0f, 0.0f, -10.0f), vector(0.0f, 0.0f, 1.0f))"
			"And xs = intersect(g, r)")
		{
			auto r = Ray(point(10.0f, 0.0f, -10.0f), vector(0.0f, 0.0f, 1.0f));
			auto xs = g->intersect(r);
			THEN("xs.count == 2")
			{
				REQUIRE(xs.size() == 2);
			}
		}
	}
}