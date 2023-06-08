#include <catch2/catch_test_macros.hpp>

#include <shape.h>
#include <group.h>
#include <sphere.h>
#include <transforms.h>

// Chapter 9 Planes

SCENARIO("The default transformation", "[shape]")
{
	GIVEN("s = testShape()")
	{
		auto s = testShape();
		THEN("s.transform == identityMatrix")
		{
			REQUIRE(s->transform == matrix4());
		}
	}
}

SCENARIO("Assigning a transformation", "[shape]")
{
	GIVEN("s = testShape()")
	{
		auto s = testShape();
		WHEN("s->transform = translate(2.0f, 3.0f, 4.0f)")
		{
			s->transform = translate(2.0f, 3.0f, 4.0f);
			THEN("s.transform = translate(2.0f, 3.0f, 4.0f)")
			{
				REQUIRE(s->transform == translate(2.0f, 3.0f, 4.0f));
			}
		}
	}
}

SCENARIO("The default material", "[shape]")
{
	GIVEN("s = testShape()")
	{
		auto s = testShape();
		WHEN("m = s.material")
		{
			auto m = s->material;
			THEN("m == material()")
			{
				REQUIRE(m == Material());
			}
		}
	}
}

SCENARIO("Assigning a material", "[shape]")
{
	GIVEN("s = testShape()"
		"And m = Material()"
		"And m.ambient = 1.0f")
	{
		auto s = testShape();
		auto m = Material();
		m.ambient = 1.0f;
		WHEN("s.material = m")
		{
			s->material = m;
			THEN("s.material == m")
			{
				REQUIRE(s->material == m);
			}
		}
	}
}

SCENARIO("Intersecting a scaled shape with a ray", "[shape]")
{
	GIVEN("r = Ray(point(0.0f, 0.0f, -5.0f), vector(0.0f, 0.0f, 1.0f))"
		"And s = testShape()")
	{
		auto r = Ray(point(0.0f, 0.0f, -5.0f), vector(0.0f, 0.0f, 1.0f));
		auto s = testShape();
		WHEN("s.transform = scale(2.0f, 2.0f, 2.0f))"
			"And xs = intersect(s, r)")
		{
			s->transform = scale(2.0f, 2.0f, 2.0f);
			auto xs = s->intersect(r);
			THEN("s.savedRay.origin = point(0.0f, 0.0f, -2.5f)"
				"And s.savedRay.direction = vector(0.0f, 0.0f, 0.5f)")
			{
				REQUIRE(s->savedRay.origin == point(0.0f, 0.0f, -2.5f));
				REQUIRE(s->savedRay.direction == vector(0.0f, 0.0f, 0.5f));
			}
		}
	}
}

SCENARIO("Intersecting a translated shape with a ray", "[shape]")
{
	GIVEN("r = ray(point(0.0f, 0.0f, -5.0f), vector(0.0f, 0.0f, 1.0f))"
		"And s ¡û testShape()")
	{
		auto r = Ray(point(0.0f, 0.0f, -5.0f), vector(0.0f, 0.0f, 1.0f));
		auto s = testShape();
		WHEN("s.transform = scale(5.0f, 0.0f, 0.0f))"
			"And xs = intersect(s, r)")
		{
			s->transform = translate(5.0f, 0.0f, 0.0f);
			auto xs = s->intersect(r);
			THEN("s.savedRay.origin = point(-5.0f, 0.0f, -5.0f)"
				"And s.savedRay.direction = vector(0.0f, 0.0f, 1.0f)")
			{
				REQUIRE(s->savedRay.origin == point(-5.0f, 0.0f, -5.0f));
				REQUIRE(s->savedRay.direction == vector(0.0f, 0.0f, 1.0f));
			}
		}
	}
}

SCENARIO("Computing the normal on a translated shape", "[shape]")
{
	GIVEN("s = testShape()")
	{
		auto s = testShape();
		WHEN("s.transform = translation(0.0f, 1.0f, 0.0f))"
			"And n = normalAt(s, point(0.0f, 1.70711f, -0.70711f))")
		{
			s->transform = translate(0.0f, 1.0f, 0.0f);
			auto n = normalAt(s, point(0.0f, 1.70711f, -0.70711f));
			THEN("n == vector(0.0f, 0.70711f, -0.70711f)")
			{
				REQUIRE(n == vector(0.0f, 0.70711f, -0.70711f));
			}
		}
	}
}

SCENARIO("Computing the normal on a transformed shape", "[shape]")
{
	GIVEN("s = testShape()"
	"And m = scale(1.0f, 0.5f, 1.0f) * rotationZ(¦Ð / 5.0f)")
	{
		auto s = testShape();
		auto m = scale(1.0f, 0.5f, 1.0f) * rotateZ(PI / 5.0f);
		WHEN("s.transform = m"
			"And n = normalAt(s, point(0, ¡Ì2 / 2, -¡Ì2 / 2))")
		{
			s->transform = m;
			auto n = normalAt(s, point(0.0f, SQRT2 / 2.0f, -SQRT2 / 2.0f));
			THEN("n == vector(0.0f, 0.97014f, -0.24254f)")
			{
				REQUIRE(n == vector(0.0f, 0.97014f, -0.24254f));
			}
		}
	}
}

// Chapter 14 Groups

SCENARIO("Creating a new group", "[shape]")
{
	GIVEN("g = Group()")
	{
		THEN("g.transform == identityMatrix"
			"And g is empty")
		{
			REQUIRE(true);
		}
	}
}

SCENARIO("A shape has a parent attribute", "[shape]")
{
	GIVEN("s = testShape()")
	{
		auto s = testShape();
		THEN("s.parent is nothing")
		{
			REQUIRE(s->parent == nullptr);
		}
	}
}

SCENARIO("Converting a point from world to object space", "[shape]")
{
	GIVEN("g1 = Group()"
		"And setTransform(g1, rotation_y(¦Ð / 2.0f))"
		"And g2 = Group()"
		"And setTransform(g2, scaling(2.0f, 2.0f, 2.0f))"
		"And addChild(g1, g2)"
		"And s = Sphere()"
		"And setTransform(s, translation(5.0f, 0.0f, 0.0f))"
		"And addChild(g2, s)")
	{
		auto g1 = createGroup();
		g1->setTransform(rotateY(PI / 2.0f));
		auto g2 = createGroup();
		g2->setTransform(scale(2.0f));
		g1->addChild(g2);
		auto s = createSphere();
		s->setTransform(translate(5.0f, 0.0f, 0.0f));
		g2->addChild(s);
		WHEN("p = worldToObject(s, point(-2.0f, 0.0f, -10.0f))")
		{
			auto p = s->worldToObject(point(-2.0f, 0.0f, -10.0f));
			THEN("p == point(0.0f, 0.0f, -1.0f)")
			{
				REQUIRE(p == point(0.0f, 0.0f, -1.0f));
			}
		}
	}
}

SCENARIO("Converting a normal from object to world space", "[shape]")
{
	GIVEN("g1 = Group()"
		"And setTransform(g1, rotation_y(¦Ð / 2.0f))"
		"And g2 = Group()"
		"And setTransform(g2, scaling(1.0f, 2.0f, 3.0f))"
		"And addChild(g1, g2)"
		"And s = Sphere()"
		"And setTransform(s, translation(5.0f, 0.0f, 0.0f))"
		"And addChild(g2, s)")
	{
		auto g1 = createGroup();
		g1->setTransform(rotateY(PI / 2.0f));
		auto g2 = createGroup();
		g2->setTransform(scale(1.0f, 2.0f, 3.0f));
		g1->addChild(g2);
		auto s = createSphere();
		s->setTransform(translate(5.0f, 0.0f, 0.0f));
		g2->addChild(s);
		WHEN("n = normalToWorld(s, vector(¡Ì3/3, ¡Ì3/3, ¡Ì3/3))")
		{
			auto n = s->normalToWorld(vector(SQRT3 / 3, SQRT3 / 3, SQRT3 / 3));
			THEN("n == vector(0.2857f, 0.4286f, -0.8571f)")
			{
				REQUIRE(n == vector(0.2857f, 0.4286f, -0.8571f));
			}
		}
	}
}

SCENARIO("Finding the normal on a child object", "[shape]")
{
	GIVEN("g1 = Group()"
		"And setTransform(g1, rotation_y(¦Ð / 2.0f))"
		"And g2 = Group()"
		"And setTransform(g2, scaling(1.0f, 2.0f, 3.0f))"
		"And addChild(g1, g2)"
		"And s = Sphere()"
		"And setTransform(s, translation(5.0f, 0.0f, 0.0f))"
		"And addChild(g2, s)")
	{
		auto g1 = createGroup();
		g1->setTransform(rotateY(PI / 2.0f));
		auto g2 = createGroup();
		g2->setTransform(scale(1.0f, 2.0f, 3.0f));
		g1->addChild(g2);
		auto s = createSphere();
		s->setTransform(translate(5.0f, 0.0f, 0.0f));
		g2->addChild(s);
		WHEN("n = normalAat(s, point(1.7321f, 1.1547f, -5.5774f))")
		{
			auto n = s->normalToWorld(vector(SQRT3 / 3, SQRT3 / 3, SQRT3 / 3));
			THEN("n = vector(0.2857f, 0.4286f, -0.8571f)")
			{
				REQUIRE(n == vector(0.2857f, 0.4286f, -0.8571f));
			}
		}
	}
}