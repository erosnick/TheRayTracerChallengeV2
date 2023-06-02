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

				auto ws1 = dynamic_cast<Sphere*>(w.getObject(0).get());
				auto ws2 = dynamic_cast<Sphere*>(w.getObject(1).get());

				REQUIRE(*ws1 == *s1);
				REQUIRE(*ws2 == *s2);
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
			auto c = colorAt(w, r, false);
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
			auto c = shadeHit(w, comps, 1);
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

SCENARIO("shadeHit() is given an intersection in shad", "[world]")
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

// Chapter 11 Reflection and Refraction

SCENARIO("The reflected color for a nonreflective material", "[world]")
{
	GIVEN(" w = defaultWorld()"
		"And r = Ray(point(0.0f, 0.0f, 0.0f), vector(0.0f, 0.0f, 1.0f))"
		"And shape = the second object in w"
		"And shape.material.ambient = 1.0f"
		"And i = intersection(1.0f, shape)")
	{
		auto w = defaultWorld();
		auto r = Ray(point(0, 0, 0), vector(0, 0, 1));
		auto shape = w.getObject(1);
		shape->material.ambient = 1.0f;
		auto i = Intersection{ 1.0f, shape };
		WHEN("comps = prepareComputations(i, r)"
			 "And color = reflectedColor(w, comps)")
		{
			auto comps = prepareComputations(i, r);
			auto c = reflectedColor(w, comps, 0);
			THEN("color == color(0.0F, 0.0F, 0.0F)")
			{
				REQUIRE(c == color(0.0f, 0.0f, 0.0f));
			}
		}
	}
}

SCENARIO("The reflected color for a reflective material", "[world]")
{
	GIVEN("w = defaultWorld()"
			"And shape = Plane() with:"
			"| material.reflective | 0.5 |"
			"| transform | translation(0.0f, -1.0f, 0.0f) |"
			"And shape is added to w"
			"And r = Ray(point(0.0f, 0.0f, -3.0f), vector(0.0f, -¡Ì2 / 2.0f, ¡Ì2 / 2.0f))"
			"And i = intersection(¡Ì2, shape)")
	{
		auto w = defaultWorld();
		auto r = Ray(point(0.0f, 0.0f, -3.0f), vector(0.0f, -SQRT2 / 2.0f, SQRT2 / 2.0f));
		auto i = Intersection{ SQRT2, w.getObject(2) };
		WHEN("comps = prepareComputations(i, r)"
			"And color = reflectedColor(w, comps)")
		{
			auto comps = prepareComputations(i, r);
			auto c = reflectedColor(w, comps, 1);
			THEN("c == color(0.19032f, 0.2379f, 0.14274f)")
			{
				REQUIRE(c == color(0.19032f, 0.2379f, 0.14274f));
			}
		}
	}
}

SCENARIO("shadeHit() with a reflective material", "[world]")
{
	GIVEN("w = defaultWorld()"
		"And shape = Plane() with:"
		"| material.reflective | 0.5 |"
		"| transform | translation(0.0f, -1.0f, 0.0f) |"
		"And shape is added to w"
		"And r = Ray(point(0.0f, 0.0f, -3.0f), vector(0.0f, -¡Ì2 / 2.0f, ¡Ì2 / 2.0f))"
		"And i = intersection(¡Ì2, shape)")
	{
		auto w = defaultWorld();
		auto r = Ray(point(0.0f, 0.0f, -3.0f), vector(0.0f, -SQRT2 / 2.0f, SQRT2 / 2.0f));
		auto i = Intersection{ SQRT2, w.getObject(2) };
		WHEN("comps = prepareComputations(i, r)"
			"And color = shadeHit(w, comps)")
		{
			auto comps = prepareComputations(i, r);
			auto c = shadeHit(w, comps);
			THEN("c == color(0.87677f, 0.92436f, 0.82918f)")
			{
				REQUIRE(c == color(0.87677f, 0.92436f, 0.82918f));
			}
		}
	}
}

SCENARIO("colorAt() with mutually reflective surfaces", "[world]")
{
	GIVEN("w = World()"
		"And w.light = pointLight(point(0.0f, 0.0f, 0.0f), color(1.0f, 1.0f, 1.0f))"
		"And lower = Plane() with:"
		"| material.reflective | 1.0f |"
		"| transform | translation(0.0f, -1.0f, 0.0f) |"
		"And lower is added to w"
		"And upper = Plane() with :"
		"| material.reflective | 1.0 |"
		"| transform | translation(0.0f, 1.0f, 0.0f) |"
		"And upper is added to w"
		"And r = ray(point(0.0f, 0.0f, 0.0f), vector(0.0f, 1.0f, 0.0f))")
	{
		auto w = World();
		w.addLight(pointLight(point(0.0f, 0.0f, 0.0f), Color::White));
		auto lower = createPlane();
		lower->material.reflective = 1.0f;
		lower->setTransform(translate(0.0f, -1.0f, 0.0f));
		w.addObject(lower);
		auto upper = createPlane();
		upper->material.reflective = 1.0f;
		upper->setTransform(translate(0.0f, 1.0f, 0.0f));
		auto r = Ray(point(0.0f, 0.0f, 0.0f), vector(0.0f, 1.0f, 0.0f));
		THEN("colorAt(w, r) should terminate successfull")
		{
			colorAt(w, r);
			REQUIRE(true);
		}
	}
}                           

SCENARIO("The reflected color at the maximum recursive depth", "[world]")
{
	GIVEN("w = defaultWorld()"
		"And shape = Plane() with:"
		"| material.reflective | 0.5 |"
		"| transform | translation(0.0f, -1.0f, 0.0f) |"
		"And shape is added to w"
		"And r = Ray(point(0.0f, 0.0f, -3.0f), vector(0.0f, -¡Ì2 / 2.0f, ¡Ì2 / 2.0f))"
		"And i = intersection(¡Ì2, shape)")
	{
		auto w = defaultWorld();
		auto r = Ray(point(0.0f, 0.0f, -3.0f), vector(0.0f, -SQRT2 / 2.0f, SQRT2 / 2.0f));
		auto i = Intersection{ SQRT2, w.getObject(2) };
		WHEN("comps = prepareComputations(i, r)"
			"And color = reflectedColor(w, comps)")
		{
			auto comps = prepareComputations(i, r);
			auto c = reflectedColor(w, comps, 0);
			THEN("c == color(0.87677f, 0.92436f, 0.82918f)")
			{
				REQUIRE(c == color(0.0f, 0.0f, 0.0f));
			}
		}
	}
}

SCENARIO("The refracted color with a refracted ray", "[world]")
{
	GIVEN("w = defaultWorld()"
		"And A = the first object in w"
		"And A has :"
		"| material.ambient | 1.0f |"
		"| material.pattern | testPattern() |"
		"And B = the second object in w"
		"And B has :"
		"| material.transparency | 1.0f |"
		"| material.refractiveIndex | 1.5f |"
		"And r = Ray(point(0.0f, 0.0f, 0.1f), vector(0.0f, 1.0f, 0.0f))"
		"And xs = intersections(-0.9899f:A, -0.4899f:B, 0.4899f:B, 0.9899f:A)")
	{
		auto w = defaultWorld();
		auto A = w.getObject(0);
		A->material.ambient = 1.0f;
		A->material.pattern = std::make_shared<TestPattern>();
		auto B = w.getObject(1);
		B->material.transparency = 1.0f;
		B->material.refractiveIndex = 1.5f;
		auto r = Ray(point(0.0f, 0.0f, 0.1f), vector(0.0f, 1.0f, 0.0f));
		auto xs = intersections({ { -0.9899f, A }, { -0.4899f, B }, { 0.4899f, B }, { 0.9899f, A } });
		WHEN("comps = prepareComputations(xs[2], r, xs)"
			"And c = refractedColor(w, comps, 5)")
		{
			auto comps = prepareComputations(xs[2], r, xs);
			auto c = refractedColor(w, comps, 5);
			THEN("c == color(0.0f, 0.99888f, 0.04725f)")
			{
				REQUIRE(c == color(0.0f, 0.99888f, 0.04725f));
			}
		}
	}
}

SCENARIO("shadeHit() with a transparent material", "[world]")
{
	GIVEN("w = defaultWorld()"
		"And floor = Plane() with:"
		"| transform | translation(0.0f, -1.0f, 0.0f) |"
		"| material.transparency | 0.5f |"
		"| material.refractiveIndex | 1.5f |"
		"And floor is added to w"
		"And ball = Sphere() with :"
		"| material.color | (1.0f, 0.0f, 0.0f) |"
		"| material.ambient | 0.5f |"
		"| transform | translation(0.0f, -3.5f, -0.5f) |"
		"And ball is added to w"
		"And r = Ray(point(0.0f, 0.0f, -3.0f), vector(0.0f, -¡Ì2 / 2.0f, ¡Ì2 / 2.0f))"
		"And xs = intersections(¡Ì2:floor)")
	{
		auto w = defaultWorld();
		auto floor = createPlane();
		floor->setTransform(translate(0.0f, -1.0f, 0.0f));
		floor->material.transparency = 0.5f;
		floor->material.refractiveIndex = 1.5f;
		w.addObject(floor);
		auto ball = createSphere();
		ball->material.color = color(1.0f, 0.0f, 0.0f);
		ball->material.ambient = 0.5f;
		ball->setTransform(translate(0.0f, -3.5f, -0.5f));
		w.addObject(ball);
		auto r = Ray(point(0.0f, 0.0f, -3.0f), vector(0.0f, -SQRT2 / 2.0f, SQRT2 / 2.0f));
		auto xs = intersections({ { SQRT2, floor } });
		WHEN("comps = prepareComputations(xs[0], r, xs)"
			"And c = shadeHit(w, comps, 5)")
		{
			auto comps = prepareComputations(xs[0], r, xs);
			auto c = shadeHit(w, comps, 5);
			THEN("c == color(0.93642f, 0.68642f, 0.68642f)")
			{
				REQUIRE(c == color(0.93642f, 0.68642f, 0.68642f));
			}
		}
	}
}

SCENARIO("shadeHit() with a reflective, transparent material", "[world]")
{
	GIVEN("w = defaultWorld()"
		"And r = Ray(point(0.0f, 0.0f, -3.0f), vector(0.0f, -¡Ì2 / 2.0f, ¡Ì2 / 2.0f))"
		"And floor = Plane() with:"
		"| transform | translation(0.0f, -1.0f, 0.0f) |"
		"| material.reflective | 0.5f |"
		"| material.transparency | 0.5f |"
		"| material.refractiveIndex | 1.5f |"
		"And floor is added to w"
		"And ball = Sphere() with:"
		"| material.color | (1.0f, 0.0f, 0.0f) |"
		"| material.ambient | 0.5f |"
		"| transform | translation(0.0f, -3.5f, -0.5f) |"
		"And ball is added to w"
		"And xs = intersections(¡Ì2:floor)")
	{
		auto w = defaultWorld();
		auto r = Ray(point(0.0f, 0.0f, -3.0f), vector(0.0f, -SQRT2 / 2.0f, SQRT2 / 2.0f));
		auto floor = createPlane();
		floor->setTransform(translate(0.0f, -1.0f, 0.0f));
		floor->material.reflective = 0.5f;
		floor->material.transparency = 0.5f;
		floor->material.refractiveIndex = 1.5f;
		w.addObject(floor);
		auto ball = createSphere();
		ball->material.color = color(1.0f, 0.0f, 0.0f);
		ball->material.ambient = 0.5f;
		ball->setTransform(translate(0.0f, -3.5f, -0.5f));
		w.addObject(ball);
		auto xs = intersections({ { SQRT2, floor } });
		WHEN("comps = prepareComputations(xs[0], r, xs)"
			"And color = shadeHit(w, comps, 5)")
		{
			auto comps = prepareComputations(xs[0], r, xs);
			auto c = shadeHit(w, comps, 5);
			THEN("c == color(0.93391f, 0.69643f, 0.69243f)")
			{
				REQUIRE(c == color(0.93391f, 0.69643f, 0.69243f));
			}
		}
	}
}