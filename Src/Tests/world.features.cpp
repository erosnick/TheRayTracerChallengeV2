#include <catch2/catch_test_macros.hpp>

#include <world.h>

#include <intersection.h>
#include <shading.h>

SCENARIO("Creating a world", "[world]")
{
	GIVEN("w - World()")
	{
		auto w = World();
		THEN("w contains no objects"
			"And w has no light source")
		{
			REQUIRE(w.objectCount() == 0);
			REQUIRE(w.lightCount() == 0);
		}
	}
}

SCENARIO("The default world", "[world]")
{
	GIVEN("light = point_light(point(-10.0f, 10.0f, -10.0f), color(1.0f, 1.0f, 1.0f))"
		"And s1 = Sphere() with:"
		"| material.color	 | (0.8f, 1.0f, 0.6f) |"
		"| material.diffuse  | 0.7f |"
		"| material.specular | 0.2f |"
		"And s2 = Sphere() with :"
		"| transform | scale(0.5f, 0.5f, 0.5f) | ")
	{
		auto light = pointLight(point(-10.0f, 10.0f, -10.0f), Color::White);
		auto s1 = createSphere();
		s1->material.color = color(0.8f, 1.0f, 0.6f);
		s1->material.diffuse = 0.7f;
		s1->material.specular = 0.2f;

		auto s2 = createSphere(scale(0.5f, 0.5f, 0.5f));

		WHEN("w = defaultWorld()")
		{
			auto w = defaultWorld();
			THEN("w.light == light")
			{
				REQUIRE(w.getLights()[0] == light);
				REQUIRE(*w.getObject(0).get() == *s1.get());
				REQUIRE(*w.getObject(1).get() == *s2.get());
			}
		}
	}
}

SCENARIO("Intersect a world with a ray", "[world]")
{
	GIVEN("w = defaultWorld()"
		"And r = Ray(point(0.0f, 0.0f, -5.0f), vector(0.0f, 0.0f, 1.0f))")
	{
		auto w = defaultWorld();
		auto r = Ray(point(0.0f, 0.0f, -5.0f), vector(0.0f, 0.0f, 1.0f));
		WHEN("xs = intersectWorld(w, r)")
		{
			auto xs = intersectWorld(w, r);
			THEN("xs.count == 4"
				"And xs[0].t == 4.0f"
				"And xs[1].t == 4.5f"
				"And xs[2].t == 5.5f"
				"And xs[3].t == 6")
			{
				REQUIRE(xs.size() == 4);
				REQUIRE(xs[0].t == 4.0f);
				REQUIRE(xs[1].t == 4.5f);
				REQUIRE(xs[2].t == 5.5f);
				REQUIRE(xs[3].t == 6.0f);
			}
		}
	}
}

SCENARIO("The color when a ray misses", "[world]")
{
	GIVEN("w = defaultWorld()"
		"And r = Ray(point(0.0f, 0.0f, -5.0f), vector(0.0f, 1.0f, 0.0f))")
	{
		auto w = defaultWorld();
		auto r = Ray(point(0.0f, 0.0f, -5.0f), vector(0.0f, 1.0f, 0.0f));
		WHEN("c = colorAt(w, r)")
		{
			auto c = colorAt(w, r);
			THEN("c == color(0.0f, 0.0f, 0.0f)")
			{
				REQUIRE(c == color(0.0f, 0.0f, 0.0f));
			}
		}
	}
}

SCENARIO("The color when a ray hits", "[world]")
{
	GIVEN("w = defaultWorld()"
		"And r = ray(point(0.0f, 0.0f, -5.0f), vector(0.0f, 0.0f, 1.0f))")
	{
		auto w = defaultWorld();
		auto r = Ray(point(0.0f, 0.0f, -5.0f), vector(0.0f, 0.0f, 1.0f));
		WHEN("c = colorAt(w, r)")
		{
			auto c = colorAt(w, r);
			THEN("c == color(0.38066f, 0.47583f, 0.2855f)")
			{
				REQUIRE(c == color(0.38066f, 0.47583f, 0.2855f));
			}
		}
	}
}

SCENARIO("The color with an intersection behind the ray", "[world]")
{
	GIVEN("w = defaultWorld()"
		"And outer = the first object in w"
		"And outer.material.ambient = 1.0f"
		"And inner = the second object in w"
		"And inner.material.ambient = 1.0f"
		"And r = Ray(point(0.0f, 0.0f, 0.75f), vector(0.0f, 0.0f, -1.0f))")
	{
		auto w = defaultWorld();
		auto outer = w.getObject(0);
		outer->material.ambient = 1.0f;
		auto inner = w.getObject(1);
		inner->material.ambient = 1.0f;
		auto r = Ray(point(0.0f, 0.0f, 0.75f), vector(0.0f, 0.0f, -1.0f));
		WHEN("c = colorAt(w, r)")
		{
			auto c = colorAt(w, r);
			THEN("c == inner->material.color")
			{
				REQUIRE(c == inner->material.color);
			}
		}
	}
}

SCENARIO("Shading an intersection", "[world]")
{
	GIVEN("w = defaultWorld()"
		"And r = Ray(point(0.0f, 0.0f, -5.0f), vector(0.0f, 0.0f, 1.0f))"
		"And shape = the first object in w"
		"And i = intersection(4.0f, shape)")
	{
		auto w = defaultWorld();
		auto r = Ray(point(0.0f, 0.0f, -5.0f), vector(0.0f, 0.0f, 1.0f));
		auto shape = w.getObject(0);
		auto i = Intersection{ 4.0f, shape };
		WHEN("comps = prepareComputations(i, r)"
			"And c = shadeHit(w, comps)")
		{
			auto comps = prepareComputations(i, r);
			auto c = shadeHit(w, comps);
			THEN("Then c == color(0.38066f, 0.47583f, 0.2855f)")
			{
				REQUIRE(c == color(0.38066f, 0.47583f, 0.2855f));
			}
		}
	}
}

SCENARIO("Shading an intersection from the inside", "[world]")
{
	GIVEN("w = defaultWorld()"
		"And w.light = pointLight(point(0.0f, 0.25f, 0.0f), color(1.0f, 1.0f, 1.0f))"
		"And r = Ray(point(0.0f, 0.0f, 0.0f), vector(0.0f, 0.0f, 1.0f))"
		"And shape = the second object in w"
		"And i = intersection(0.5f, shape)")
	{
		auto w = defaultWorld();
		w.getLight(0) = pointLight(point(0.0f, 0.25f, 0.0f), color(1.0f, 1.0f, 1.0f));
		auto r = Ray(point(0.0f, 0.0f, 0.0f), vector(0.0f, 0.0f, 1.0f));
		auto shape = w.getObject(1);
		auto i = Intersection{ 0.5f, shape };
		WHEN("comps = prepareComputations(i, r)"
			"And c = shadeHit(w, comps)")
		{
			auto comps = prepareComputations(i, r);
			auto c = shadeHit(w, comps);
			THEN("Then c == color(0.90498f, 0.90498f, 0.90498f)")
			{
				REQUIRE(c == color(0.90498f, 0.90498f, 0.90498f));
			}
		}
	}
}

// Chapter 8 Shadows

SCENARIO("There is no shadow when nothing is collinear with point and light", "[world]")
{
	GIVEN("w = defaultWorld()"
		"And p = point(0, 10, 0)")
	{
		auto w = defaultWorld();
		auto p = point(0.0f, 10.0f, 0.0f);
		THEN("isShadowed(w, p) is false")
		{
			REQUIRE(isShadowed(w, p)[0] == false);
		}
	}
}

SCENARIO("The shadow when an object is between the point and the light", "[world]")
{
	GIVEN("w = defaultWorld()"
		"And p = point(10.0f, -10.0f, 10.0f)")
	{
		auto w = defaultWorld();
		auto p = point(10.0f, -10.0f, 10.0f);
		THEN("isShadowed(w, p) is false")
		{
			REQUIRE(isShadowed(w, p)[0] == true);
		}
	}
}

SCENARIO("There is no shadow when an object is behind the light", "[world]")
{
	GIVEN("w = defaultWorld()"
		"And p = point(-20.0f, 20.0f, -20.0f)")
	{
		auto w = defaultWorld();
		auto p = point(-20.0f, 20.0f, -20.0f);
		THEN("isShadowed(w, p) is false")
		{
			REQUIRE(isShadowed(w, p)[0] == false);
		}
	}
}

SCENARIO("There is no shadow when an object is behind the point", "[world]")
{
	GIVEN("w = defaultWorld()"
		"And p = point(0, 10, 0)")
	{
		auto w = defaultWorld();
		auto p = point(-2.0f, 2.0f, -2.0f);
		THEN("isShadowed(w, p) is false")
		{
			REQUIRE(isShadowed(w, p)[0] == false);
		}
	}
}

SCENARIO("shadeHit() is given an intersection in shad", "[world.features]")
{
	GIVEN("w = World()"
		"And w.light = pointLight(point(0.0f, 0.0f, -10.0f), color(1.0f, 1.0f, 1.0f))"
		"And s1 = Sphere()"
		"And s1 is added to w"
		"And s2 = Sphere() with:"
		"| transform | translation(0.0f, 0.0f, 10.0f) |"
		"And s2 is added to w"
		"And r = Ray(point(0.0f, 0.0f, 5.0f), vector(0.0f, 0.0f, 1.0f))"
		"And i = intersection(4.0f, s2)")
	{
		auto w = World();
		auto light = pointLight(point(0.0f, 0.0f, -10.0f), color(1.0f, 1.0f, 1.0f));
		w.addLight(light);
		auto s1 = createSphere();
		w.addObject(s1);
		auto s2 = createSphere();
		s2->setTransform(translate(0.0f, 0.0f, 10.0f));
		w.addObject(s2);
		auto r = Ray(point(0.0f, 0.0f, 5.0f), vector(0.0f, 0.0f, 1.0f));
		auto i = Intersection{ 4.0f, s2 };
		WHEN("comps = prepareComputations(i, r)"
			"And c = shadeHit(w, comps)")
		{
			auto comps = prepareComputations(i, r);
			auto c = shadeHit(w, comps);
			THEN("c == color(0.1f, 0.1f, 0.1f)")
			{
				REQUIRE(c == color(0.1f, 0.1f, 0.1f));
			}
		}
	}
}