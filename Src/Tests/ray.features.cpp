#include <catch2/catch_test_macros.hpp>

#include <ray.h>
#include <sphere.h>

#include <canvas.h>

#include "colors.h"
#include <iostream>

SCENARIO("Creating and querying a ray", "[matrix]")
{
	GIVEN("origin = point(1.0f, 2.0f, 3.0f)"
		"And direction = vector(4.0f, 5.0f, 6.0f)")
	{
		auto origin = point(1.0f, 2.0f, 3.0f);
		auto direction = vector(4.0f, 5.0f, 6.0f);

		WHEN("r = Ray(origin, direction)")
		{
			auto r = Ray(origin, direction);

			THEN("r.origin == origin"
				"And r.direction == direction")
			{
				REQUIRE(r.origin == origin);
				REQUIRE(r.direction == direction);
			}
		}
	}
}

SCENARIO("Computing a point from a distance", "[matrix]")
{
	GIVEN("r = Ray(point(2.0f, 3.0f, 4.0f), vector(1.0f, 0.0f, 0.0f))")
	{
		auto r = Ray(point(2.0f, 3.0f, 4.0f), vector(1.0f, 0.0f, 0.0f));

		THEN("position(r, 0.0f) = point(2.0f, 3.0f, 4.0f)"
			"And position(r, 1.0f) = point(3.0f, 3.0f, 4.0f)"
			"And position(r, -1.0f) = point(1.0f, 3.0f, 4.0f)"
			"And position(r, 2.5f) = point(4.5f, 3.0f, 4.0f)")
		{
			REQUIRE(r.at(0.0f) == point(2.0f, 3.0f, 4.0f));
			REQUIRE(r.at(1.0f) == point(3.0f, 3.0f, 4.0f));
			REQUIRE(r.at(-1.0f) == point(1.0f, 3.0f, 4.0f));
			REQUIRE(r.at(2.5f) == point(4.5f, 3.0f, 4.0f));
		}
	}
}

SCENARIO("A ray intersects a sphere at two points", "[matrix]")
{
	GIVEN("r = Ray(point(0.0f, 0.0f, -5.0f), vector(0.0f, 0.0f, 1.0f))"
		 "And s = Sphere()")
	{
		auto r = Ray(point(0.0f, 0.0f, -5.0f), vector(0.0f, 0.0f, 1.0f));
		auto s = std::make_shared<Sphere>();

		WHEN("xs = intersect(s, r)")
		{
			auto xs = intersect(s, r);

			THEN("xs.count == 2.0f"
				"And xs[0] == 4.0f"
				"And xs[1] == 6.0f")
			{
				REQUIRE(xs.size() == 2);
				REQUIRE(xs[0].t == 4.0f);
				REQUIRE(xs[1].t == 6.0f);
			}
		}
	}
}

SCENARIO("A ray intersects a sphere at a tangent", "[matrix]")
{
	GIVEN("r = Ray(point(0.0f, 0.0f, -5.0f), vector(0.0f, 0.0f, 1.0f))"
		"And s = Sphere()")
	{
		auto r = Ray(point(0.0f, 1.0f, -5.0f), vector(0.0f, 0.0f, 1.0f));
		auto s = std::make_shared<Sphere>();

		WHEN("xs = intersect(s, r)")
		{
			auto xs = intersect(s, r);

			THEN("xs.count == 2.0f"
				"And xs[0] == 5.0f"
				"And xs[1] == 5.0f")
			{
				REQUIRE(xs.size() == 2);
				REQUIRE(xs[0].t == 5.0f);
				REQUIRE(xs[1].t == 5.0f);
			}
		}
	}
}

SCENARIO("A ray misses a sphere", "[matrix]")
{
	GIVEN("r = Ray(point(0.0f, 2.0f, -5.0f), vector(0.0f, 0.0f, 1.0f))"
		"And s = Sphere()")
	{
		auto r = Ray(point(0.0f, 2.0f, -5.0f), vector(0.0f, 0.0f, 1.0f));
		auto s = std::make_shared<Sphere>();

		WHEN("xs = intersect(s, r)")
		{
			auto xs = intersect(s, r);

			THEN("xs.count == 0")
			{
				REQUIRE(xs.size() == 0);
			}
		}
	}
}

SCENARIO("A ray originates inside a sphere", "[matrix]")
{
	GIVEN("r = Ray(point(0.0f, 0.0f, 0.0f), vector(0.0f, 0.0f, 1.0f))"
		"And s = Sphere()")
	{
		auto r = Ray(point(0.0f, 0.0f, 0.0f), vector(0.0f, 0.0f, 1.0f));
		auto s = std::make_shared<Sphere>();

		WHEN("xs = intersect(s, r)")
		{
			auto xs = intersect(s, r);

			THEN("xs.count == 2"
				"And xs[0] = -1.0"
				"And xs[1] = 1.0")
			{
				REQUIRE(xs.size() == 2);
				REQUIRE(xs[0].t == -1.0f);
				REQUIRE(xs[1].t ==  1.0f);
			}
		}
	}
}

SCENARIO("A sphere is behind a ray", "[matrix]")
{
	GIVEN("r = Ray(point(0.0f, 0.0f, 5.0f), vector(0.0f, 0.0f, 1.0f))"
		"And s = Sphere()")
	{
		auto r = Ray(point(0.0f, 0.0f, 5.0f), vector(0.0f, 0.0f, 1.0f));
		auto s = std::make_shared<Sphere>();

		WHEN("xs = intersect(s, r)")
		{
			auto xs = intersect(s, r);

			THEN("xs.count == 2"
				"And xs[0] = -6.0"
				"And xs[1] = 4.0")
			{
				REQUIRE(xs.size() == 2);
				REQUIRE(xs[0].t == -6.0f);
				REQUIRE(xs[1].t == -4.0f);
			}
		}
	}
}

SCENARIO("An intersection encapsulates t and object", "[matrix]")
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

SCENARIO("Aggregating intersections", "[matrix]")
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

SCENARIO("Intersect sets the object on the intersection", "[matrix]")
{
	GIVEN("r = ray(point(0.0f, 0.0f, -5.0f), vector(0.0f, 0.0f, 1.0f))"
		"And s = Sphere()")
	{
		auto r = Ray(point(0.0f, 0.0f, -5.0f), vector(0.0f, 0.0f, 1.0f));
		auto s = std::make_shared<Sphere>();

		auto i1 = Intersection{ 1.0f, s };
		auto i2 = Intersection{ 2.0f, s };

		WHEN("xs = intersect(s, r)")
		{
			auto xs = intersect(s, r);

			THEN("xs.count == 2.0f"
				"And xs[0].t == 1.0f"
				"And xs[1].t == 2.0f")
			{
				REQUIRE(xs.size() == 2);
				REQUIRE(xs[0].object == s);
				REQUIRE(xs[1].object == s);
			}
		}
	}
}

SCENARIO("The hit, when all intersections have positive t", "[matrix]")
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

SCENARIO("The hit, when some intersections have negative t", "[matrix]")
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

SCENARIO("The hit, when all intersections have negative t", "[matrix]")
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

SCENARIO("The hit is always the lowest nonnegative intersection", "[matrix]")
{
	GIVEN("s = Sphere()"
		"And i1 = intersection(5.0f, s)"
		"And i2 = intersection(7.0f, s)"
		"And i3 = intersection(-3.0f, s)"
		"And i4 = intersection(2.0f, s)"
		"And xs = intersections(i1, i2, i3, i4)")
	{
		auto s = std::make_shared<Sphere>();
		auto i1 = Intersection{  5.0f, s };
		auto i2 = Intersection{  7.0f, s };
		auto i3 = Intersection{ -3.0f, s };
		auto i4 = Intersection{  2.0f, s };
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

SCENARIO("Translating a ray", "[matrix]")
{
	GIVEN("r = ray(point(1.0f, 2.0f, 3.0f), vector(0.0f, 1.0f, 0.0f))"
		"And m = translation(3.0f, 4.0f, 5.0f)")
	{
		auto r = Ray(point(1.0f, 2.0f, 3.0f), vector(0.0f, 1.0f, 0.0f));
		auto m = translate(3.0f, 4.0f, 5.0f);

		WHEN("r2 = transform(r, m)")
		{
			auto r2 = transform(r, m);

			THEN("r2.origin == point(4.0f, 6.0f, 8.0f)"
				"And r2.direction == vector(0.0f, 1.0f, 0.0f)")
			{
				REQUIRE(r2.origin == point(4.0f, 6.0f, 8.0f));
				REQUIRE(r2.direction == vector(0.0f, 1.0f, 0.0f));
			}
		}
	}
}

SCENARIO("Scaling a ray", "[matrix]")
{
	GIVEN("r = ray(point(1.0f, 2.0f, 3.0f), vector(0.0f, 1.0f, 0.0f))"
		"And m = scale(2.0f, 3.0f, 4.0f)")
	{
		auto r = Ray(point(1.0f, 2.0f, 3.0f), vector(0.0f, 1.0f, 0.0f));
		auto m = scale(2.0f, 3.0f, 4.0f);

		WHEN("r2 = transform(r, m)")
		{
			auto r2 = transform(r, m);

			THEN("r2.origin == point(2.0f, 6.0f, 12.0f)"
				"And r2.direction == vector(0.0f, 3.0f, 0.0f)")
			{
				REQUIRE(r2.origin == point(2.0f, 6.0f, 12.0f));
				REQUIRE(r2.direction == vector(0.0f, 3.0f, 0.0f));
			}
		}
	}
}

SCENARIO("A sphere's default transformation", "[matrix]")
{
	GIVEN("s = Sphere()")
	{
		auto s = Sphere();

		THEN("s.transform = identity_matrix")
		{
			REQUIRE(s.transform == matrix4(1.0));
		}
	}
}

SCENARIO("Changing a sphere's transformation", "[matrix]")
{
	GIVEN("s = Sphere()"
		"And t = translate(2.0f, 3.0f, 4.0f)")
	{
		auto s = Sphere();
		auto t = translate(2.0f, 3.0f, 4.0f);
		WHEN("s.transform = t)")
		{
			s.setTransform(t);
			THEN("s.transform == t")
			{
				REQUIRE(s.transform == t);
			}
		}
	}
}

SCENARIO("Intersecting a scaled sphere with a ray", "[matrix]")
{
	GIVEN("r = ray(point(0.0f, 0.0f, -5.0f), vector(0.0f, 0.0f, 1.0f))"
		"And s = Sphere()")
	{
		auto r = Ray(point(0.0f, 0.0f, -5.0f), vector(0.0f, 0.0f, 1.0f));
		auto s = std::make_shared<Sphere>();
		s->setTransform(scale(2.0f, 2.0f, 2.0f));

		WHEN("s.transform = scale(2.0f, 2.0f, 2.0f)"
			"And xs ¡û intersect(s, r)")
		{
			auto xs = intersect(s, r);
			THEN("xs.count == 2.0f"
				"And xs[0].t == 3.0f"
				"And xs[1].t == 7.0f")
			{
				REQUIRE(xs.size() == 2.0f);
				REQUIRE(xs[0].t == 3.0f);
				REQUIRE(xs[1].t == 7.0f);
			}
		}
	}
}

SCENARIO("Intersecting a translated sphere with a ray", "[matrix]")
{
	GIVEN("r = ray(point(0.0f, 0.0f, -5.0f), vector(0.0f, 0.0f, 1.0f))"
		"And s = Sphere()")
	{
		auto r = Ray(point(0.0f, 0.0f, -5.0f), vector(0.0f, 0.0f, 1.0f));
		auto s = std::make_shared<Sphere>();
		s->setTransform(translate(5.0f, 0.0f, 0.0f));

		WHEN("s.transform = scale(2.0f, 2.0f, 2.0f)"
			"And xs ¡û intersect(s, r)")
		{
			auto xs = intersect(s, r);
			THEN("xs.count == 2.0f"
				"And xs[0].t == 3.0f"
				"And xs[1].t == 7.0f")
			{
				REQUIRE(xs.size() == 0.0f);
			}
		}
	}
}

SCENARIO("Render test", "[matrix]")
{
	auto canvas = Canvas(100, 100);

	auto sphere = std::make_shared<Sphere>();

	auto origin = vector(0.0f, 0.0f, -5.0f);

	auto halfWidth = canvas.width * 0.5f;
	auto halfHeight = canvas.height * 0.5f;

	auto u = 1.0f / canvas.width;
	auto v = 1.0f / canvas.height;

	for (int32_t y = 0; y < canvas.height; y++)
	{
		std::cout << "\rScanlines remaining: " << canvas.height - y << ' ' << std::flush;
		for (int32_t x = 0; x < canvas.width; x++)
		{
			auto direction = vector(-halfWidth + x * u, -halfHeight + y * v, 0.0f) - origin;
			auto ray = Ray(origin, normalize(direction));

			auto intersection = intersect(sphere, ray);

			if (intersection.size() > 0)
			{
				auto intersectionList = intersections({ intersection[0], intersection[1] });

				auto result = hit(intersectionList);

				if (result.t > 0.0)
				{
					canvas.writePixel(x, y, Color::Red);
				}
			}
		}
	}

	canvas.writeToPPM("sphere.ppm");
}