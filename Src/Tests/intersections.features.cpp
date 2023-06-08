#include <catch2/catch_test_macros.hpp>

#include <intersection.h>

#include <sphere.h>
#include <plane.h>
#include <tuple>
#include <world.h>
#include <shading.h>

// Chapter 5 Ray-Sphere intersections

SCENARIO("An intersection encapsulates t and object", "[intersection]")
{
	GIVEN("s = Sphere()")
	{
		auto s = std::make_shared<Sphere>();

		WHEN("i = Intersection(3.5f, s.id)")
		{
			auto i = Intersection{ 3.5f, s, s };
			THEN("i.t == 3.5f")
			{
				REQUIRE(i.t == 3.5f);
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

// Chapter 11 Reflection and Refraction

SCENARIO("Precomputing the reflection vector", "[intersections]")
{
	GIVEN("shape = Plane()"
		"And r = Ray(point(0.0f, 1.0f, -1.0f), vector(0.0f, -¡Ì2 / 2.0f, ¡Ì2 / 2.0f))"
		"And i = intersection(¡Ì2, shape)")
	{
		auto shape = createPlane();
		auto r = Ray(point(0.0f, 1.0f, -1.0f), vector(0.0f, -SQRT2 / 2.0f, SQRT2 / 2.0f));
		auto i = Intersection{ SQRT2, shape };
		WHEN("comps = prepareComputations(i, r)")
		{
			auto comps = prepareComputations(i, r);
			THEN("comps.reflectVector == vector(0.0f, ¡Ì2/2, ¡Ì2/2)")
			{
				REQUIRE(comps.reflectVector == vector(0.0f, SQRT2 / 2.0f, SQRT2 / 2.0f));
			}
		}
	}
}

SCENARIO("Finding n1 and n2 at various intersections", "[intersections]")
{
	GIVEN("A = createGlassSphere() with:"
		"| transform | scaling(2.0f, 2.0f, 2.0f) |"
		"| material.refractiveIndex | 1.5f |"
		"And B = createGlassSphere() with:"
		"| transform | translation(0.0f, 0.0f, -0.25f) |"
		"| material.refractiveIndex | 2.0f |"
		"And C = createGlassSphere() with :"
		"| transform | translation(0.0f, 0.0f, 0.25f) |"
		"| material.refractiveIndex | 2.5f |"
		"And r = Ray(point(0.0f, 0.0f, -4.0f), vector(0.0f, 0.0f, 1.0f))"
		"And xs = intersections(2:A, 2.75f:B, 3.25f:C, 4.75f:B, 5.25f:C, 6:A)")
	{
		auto A = createGlassSphere();
		A->setTransform(scale(2.0f, 2.0f, 2.0f));
		A->material.refractiveIndex = 1.5f;
		auto B = createGlassSphere();
		B->setTransform(translate(0.0f, 0.0f, -0.25f));
		B->material.refractiveIndex = 2.0f;
		auto C = createGlassSphere();
		C->setTransform(translate(0.0f, 0.0f, 0.25f));
		C->material.refractiveIndex = 2.5f;
		auto r = Ray(point(0.0f, 0.0f, -4.0f), vector(0.0f, 0.0f, 1.0f));
		auto xs = intersections({ { 2.0f, A }, { 2.75f, B }, { 3.25f, C }, { 4.75f, B }, { 5.25f, C }, { 6.0f, A } });
		WHEN("comps = preparecomputations(xs[<index>], r, xs)"
			"Examples:"
			"| index | n1  | n2  |"
			"| 0	 | 1.0 | 1.5 |"
			"| 1	 | 1.5 | 2.0 |"
			"| 2	 | 2.0 | 2.5 |"
			"| 3	 | 2.5 | 2.5 |"
			"| 4	 | 2.5 | 1.5 |"
			"| 5	 | 1.5 | 1.0 |")
		{
			std::vector<std::tuple<float, float>> refractiveIndices;

			refractiveIndices.push_back({ 1.0f, 1.5f });
			refractiveIndices.push_back({ 1.0f, 1.5f });
			refractiveIndices.push_back({ 2.0f, 2.5f });
			refractiveIndices.push_back({ 2.5f, 2.5f });
			refractiveIndices.push_back({ 2.5f, 1.5f });
			refractiveIndices.push_back({ 1.5f, 1.0f });

			for (size_t i = 0; i < refractiveIndices.size(); i++)
			{
				auto comps = prepareComputations(xs[i], r, xs);
				THEN("comps.n1 == <n1>"
					"And comps.n2 == <n2>")
				{
					REQUIRE(comps.n1 == std::get<0>(refractiveIndices[i]));
					REQUIRE(comps.n2 == std::get<1>(refractiveIndices[i]));
				}
			}
		}
	}
}

SCENARIO("The under point is offset below the surface", "[intersections]")
{
	GIVEN("r = Ray(point(0.0f, 0.0f, -5.0f), vector(0.0f, 0.0f, 1.0f))"
		"And shape = createGlassSphere() with:"
		"| transform | translation(0.0f, 0.0f, 1.0f) |"
		"And i = intersection(5, shape)"
		"And xs = intersections(i)")
	{
		auto r = Ray(point(0.0f, 0.0f, -5.0f), vector(0.0f, 0.0f, 1.0f));
		auto shape = createGlassSphere();
		shape->setTransform(translate(0.0f, 0.0f, 1.0f));
		auto i = Intersection{ 5.0f, shape };
		auto xs = intersections({ {5.0f, shape } });
		WHEN("comps = prepareComputations(i, r, xs)")
		{
			auto comps = prepareComputations(i, r, xs);
			THEN("comps.underPoint.z > EPSILON / 2.0f"
				"And comps.point.z < comps.underPoint.z")
			{
				REQUIRE(comps.underPosition.z > EPSILON / 2.0f);
				REQUIRE(comps.position.z < comps.underPosition.z);
			}
		}
	}
}

SCENARIO("The refracted color with an opaque surface", "[intersections]")
{
	GIVEN("w = defaultWorld()"
		"And shape = the first object in w"
		"And r = Ray(point(0.0f, 0.0f, -5.0f), vector(0.0f, 0.0f, 1.0f))"
		"And xs = intersections(4.0f:shape, 6.0f:shape)")
	{
		auto w = defaultWorld();
		auto shape = w.getObject(0);
		auto r = Ray(point(0.0f, 0.0f, -5.0f), vector(0.0f, 0.0f, 1.0f));
		auto xs = intersections({ { 4.0f, shape }, { 6.0f, shape } });
		WHEN("comps = prepareComputations(xs[0], r, xs)"
			"And c = refractedColor(w, comps, 5.0f)")
		{
			auto comps = prepareComputations(xs[0], r, xs);
			auto c = refractedColor(w, comps, 5);
			THEN("c == color(0.0f, 0.0f, 0.0f)")
			{
				REQUIRE(c == Colors::Black);
			}
		}
	}
}

SCENARIO("The refracted color at the maximum recursive depth", "[intersections]")
{
	GIVEN("w = defaultWorld()"
		"And shape = the first object in w"
		"And shape has :"
		"| material.transparency | 1.0f |"
		"| material.refractive_index | 1.5f |"
		"And r = Ray(point(0.0f, 0.0f, -5.0f), vector(0.0f, 0.0f, 1.0f))"
		"And xs = intersections(4.0f:shape, 6.0f:shape)")
	{
		auto w = defaultWorld();
		auto shape = w.getObject(0);
		shape->material.transparency = 1.0f;
		shape->material.refractiveIndex = 1.5f;
		auto r = Ray(point(0.0f, 0.0f, -5.0f), vector(0.0f, 0.0f, 1.0f));
		auto xs = intersections({ { 4.0f, shape }, { 6.0f, shape } });
		WHEN("comps = prepareComputations(xs[0], r, xs)"
			"And c = refractedColor(w, comps, 0)")
		{
			auto comps = prepareComputations(xs[0], r, xs);
			auto c = refractedColor(w, comps, 0);
			THEN("c == color(0.0f, 0.0f, 0.0f)")
			{
				REQUIRE(c == Colors::Black);
			}
		}
	}
}

SCENARIO("The refracted color under total internal reflection", "[intersections]")
{
	GIVEN("w = defaultWorld()"
		"And shape = the first object in w"
		"And shape has :"
		"| material.transparency | 1.0f |"
		"| material.refractiveIndex | 1.5f |"
		"And r = Ray(point(0.0f, 0.0f, ¡Ì2 / 2.0f), vector(0.0f, 1.0f, 0.0f))"
		"And xs = intersections(-¡Ì2 / 2.0f:shape, ¡Ì2 / 2.0f:shape)")
	{
		auto w = defaultWorld();
		auto shape = w.getObject(0);
		shape->material.transparency = 1.0f;
		shape->material.refractiveIndex = 1.5f;
		auto r = Ray(point(0.0f, 0.0f, SQRT2 / 2.0f), vector(0.0f, 1.0f, 0.0f));
		auto xs = intersections({ { -SQRT2 / 2.0f, shape}, {SQRT2 / 2.0f, shape } });
		// NOTE: this time you're inside the sphere, so you need
		// to look at the second intersection, xs[1], not xs[0]
		WHEN("comps = preparecomputations(xs[1], r, xs)"
			"And c = refractedColor(w, comps, 5)")
		{
			auto comps = prepareComputations(xs[1], r, xs);
			auto c = refractedColor(w, comps, 5);
			THEN("c == color(0.0f, 0.0f, 0.0f")
			{
				REQUIRE(c == Colors::Black);
			}
		}
	}
}

SCENARIO("The Schlick approximation under total internal reflection", "[intersections]")
{
	GIVEN("shape = createGlassSphere()"
		"And r = Ray(point(0.0f, 0.0f, ¡Ì2 / 2.0f), vector(0.0f, 1.0f, 0.0f))"
		"And xs = intersections(-¡Ì2 / 2.0f:shape, ¡Ì2 / 2.0f:shape)"
		"When comps = prepareComputations(xs[1], r, xs)")
	{
		auto shape = createGlassSphere();
		auto r = Ray(point(0.0f, 0.0f, SQRT2 / 2.0f), vector(0.0f, 1.0f, 0.0f));
		auto xs = intersections({ { -SQRT2 / 2.0f, shape }, { SQRT2 / 2.0f, shape } });
		WHEN("comps = prepareComputations(xs[1], r, xs)"
			"And reflectance = schlick(comps)")
		{
			auto comps = prepareComputations(xs[1], r, xs);
			auto reflectance = schlick(comps);
			THEN("reflectance == 1.0f")
			{
				REQUIRE(reflectance == 1.0f);
			}
		}
	}
}

SCENARIO("The Schlick approximation with a perpendicular viewing angle", "[intersections]")
{
	GIVEN("shape = createGlassSphere()"
		"And r = Ray(point(0.0f, 0.0f, 0.0f), vector(0.0f, 1.0f, 0.0f))"
		"And xs = intersections(-1.0f:shape, 1.0f:shape)")
	{
		auto shape = createGlassSphere();
		auto r = Ray(point(0.0f, 0.0f, 0.0f), vector(0.0f, 1.0f, 0.0f));
		auto xs = intersections({ { -1.0f, shape }, { 1.0f, shape } });
		WHEN("comps = prepareComputations(xs[1], r, xs)"
			"And reflectance = schlick(comps)")
		{
			auto comps = prepareComputations(xs[1], r, xs);
			auto reflectance = schlick(comps);
			THEN("reflectance == 0.04f;")
			{
				REQUIRE(equal(reflectance, 0.04f));
			}
		}
	}
}

SCENARIO("The Schlick approximation with small angle and n2 > n1", "[intersections]")
{
	GIVEN("shape = createGlassSphere()"
		"And r = Ray(point(0.0f, 0.99f, -2.0f), vector(0.0f, 0.0f, 1.0f))"
		"And xs = intersections(1.8589:shape)")
	{
		auto shape = createGlassSphere();
		auto r = Ray(point(0.0f, 0.99f, -2.0f), vector(0.0f, 0.0f, 1.0f));
		auto xs = intersections({ { 1.8589f, shape } });
		WHEN("comps = prepareComputations(xs[1], r, xs)"
			"And reflectance = schlick(comps)")
		{
			auto comps = prepareComputations(xs[0], r, xs);
			auto reflectance = schlick(comps);
			THEN("reflectance == 0.48873f;")
			{
				REQUIRE(equal(reflectance, 0.48873f));
			}
		}
	}
}