#include <catch2/catch_test_macros.hpp>

#include <sphere.h>
#include <intersection.h>

// Chapter 5 Ray-Sphere Intersections

SCENARIO("A ray intersects a sphere at two points", "[sphere]")
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

SCENARIO("A ray intersects a sphere at a tangent", "[sphere]")
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

SCENARIO("A ray misses a sphere", "[sphere]")
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

SCENARIO("A ray originates inside a sphere", "[sphere]")
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
				REQUIRE(xs[1].t == 1.0f);
			}
		}
	}
}

SCENARIO("A sphere is behind a ray", "[sphere]")
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

SCENARIO("Intersect sets the object on the intersection", "[sphere]")
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

SCENARIO("A sphere's default transformation", "[sphere]")
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

SCENARIO("Changing a sphere's transformation", "[sphere]")
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

SCENARIO("Intersecting a scaled sphere with a ray", "[sphere]")
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

SCENARIO("Intersecting a translated sphere with a ray", "[sphere]")
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

// Chapter 6 Light and Shadings 

SCENARIO("The normal on a sphere at a point on the x axis", "[sphere]")
{
	GIVEN(" s = Sphere()")
	{
		auto s = std::make_shared<Sphere>();

		WHEN("n = normalAt(s, point(1.0f, 0.0f, 0.0f))")
		{
			auto n = normalAt(s, point(1.0f, 0.0f, 0.0f));

			THEN("n == vector(1.0f, 0.0f, 0.0f)")
			{
				REQUIRE(n == vector(1.0f, 0.0f, 0.0f));
			}
		}
	}
}

SCENARIO("The normal on a sphere at a point on the y axis", "[sphere]")
{
	GIVEN(" s = Sphere()")
	{
		auto s = std::make_shared<Sphere>();

		WHEN("n = normalAt(s, point(0.0f, 1.0f, 0.0f))")
		{
			auto n = normalAt(s, point(0.0f, 1.0f, 0.0f));

			THEN("n == vector(0.0f, 1.0f, 0.0f)")
			{
				REQUIRE(n == vector(0.0f, 1.0f, 0.0f));
			}
		}
	}
}

SCENARIO("The normal on a sphere at a point on the z axis", "[sphere]")
{
	GIVEN(" s = Sphere()")
	{
		auto s = std::make_shared<Sphere>();

		WHEN("n = normalAt(s, point(0.0f, 0.0f, 1.0f))")
		{
			auto n = normalAt(s, point(0.0f, 0.0f, 1.0f));

			THEN("n == vector(0.0f, 0.0f, 1.0f)")
			{
				REQUIRE(n == vector(0.0f, 0.0f, 1.0f));
			}
		}
	}
}

SCENARIO("The normal on a sphere at a nonaxial point", "[sphere]")
{
	GIVEN(" s = Sphere()")
	{
		auto s = std::make_shared<Sphere>();

		WHEN("n = normalAt(s, point(¡Ì3/3, ¡Ì3/3, ¡Ì3/3))")
		{
			auto n = normalAt(s, point(SQRT3 / 3.0f, SQRT3 / 3.0f, SQRT3 / 3.0f));

			THEN("n == vector(¡Ì3/3, ¡Ì3/3, ¡Ì3/3)")
			{
				REQUIRE(n == vector(SQRT3 / 3.0f, SQRT3 / 3.0f, SQRT3 / 3.0f));
			}
		}
	}
}

SCENARIO("he normal is a normalized vector", "[sphere]")
{
	GIVEN(" s = Sphere()")
	{
		auto s = std::make_shared<Sphere>();

		WHEN("n = normalAt(s, point(¡Ì3/3, ¡Ì3/3, ¡Ì3/3))")
		{
			auto n = normalAt(s, point(SQRT3 / 3.0f, SQRT3 / 3.0f, SQRT3 / 3.0f));

			THEN("n == normalize(n)")
			{
				REQUIRE(n == normalize(n));
			}
		}
	}
}

SCENARIO("Computing the normal on a translated sphere", "[sphere]")
{
	GIVEN(" s = Sphere()"
		"And s.setTransform(translate(0.0f, 1.0f, 0.0f))")
	{
		auto s = std::make_shared<Sphere>();

		s->setTransform(translate(0.0f, 1.0f, 0.0f));

		WHEN("n = normalAt(s, point(0.0f, 1.70711f, -0.70711f))")
		{
			auto n = normalAt(s, point(0.0f, 1.70711f, -0.70711f));

			THEN("n == vector(0.0f, 0.70711f, -0.70711f)")
			{
				REQUIRE(n == vector(0.0f, 0.70711f, -0.70711f));
			}
		}
	}
}

SCENARIO("Computing the normal on a transformed sphere", "[sphere]")
{
	GIVEN(" s = Sphere()"
		"And = scale(1.0f, 0.5f, 1.0f) * rotationZ(¦Ð / 5.0f)"
		"And s.setTransform(translate(0.0f, 1.0f, 0.0f))")
	{
		auto s = std::make_shared<Sphere>();

		auto m = scale(1.0f, 0.5f, 1.0f) * rotateZ(PI / 5.0f);

		s->setTransform(m);

		WHEN("n = normalAt(s, point(0, ¡Ì2/2, -¡Ì2/2))")
		{
			auto n = normalAt(s, point(0.0f, SQRT2 / 2.0f, -SQRT2 / 2.0f));

			THEN("n == vector(0.0f, 0, 0.97014, -0.24254)")
			{
				REQUIRE(n == vector(0.0f, 0.97014f, -0.24254f));
			}
		}
	}
}

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

// Chapter 11 Reflection and Refraction

SCENARIO("A helper for producing a sphere with a glassy material", "[sphere]")
{
	GIVEN("s = createGlassSphere()")
	{
		auto s = createGlassSphere();
		auto m = Material();
		THEN("s.transform = identityMatrix"
			"And s.material.transparency = 1.0f"
			"And s.material.refractiveIndex = 1.5f")
		{
			REQUIRE(s->transform == matrix4(1.0f));
			REQUIRE(s->material.transparency == 1.0f);
			REQUIRE(s->material.refractiveIndex == 1.5f);
		}
	}
}