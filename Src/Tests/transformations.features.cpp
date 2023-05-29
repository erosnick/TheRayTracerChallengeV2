#include <catch2/catch_test_macros.hpp>

#include "transforms.h"

// Chapter 4 Matrix Transformations

SCENARIO("Multiplying by a translation matrix", "[matrix]")
{
	GIVEN("transform = translation(5, -3, 2) And p = point(-3, 4, 5)")
	{
		auto transform = translate(5.0f, -3.0f, 2.0f);
		auto p = point(-3.0f, 4.0f, 5.0f);

		THEN("transform * p == point(2, 1, 7)")
		{
			REQUIRE(transform * p == point(2.0f, 1.0f, 7.0f));
		}
	}
}

SCENARIO("Multiplying by the inverse of a translation matrix", "[matrix]")
{
	GIVEN("transform = translation(5, -3, 2)"
		"And inv = inverse(transform)"
		"And p = point(-3, 4, 5)")
	{
		auto transform = translate(5.0f, -3.0f, 2.0f);
		auto inv = inverse(transform);
		auto p = point(-3.0f, 4.0f, 5.0f);

		THEN("transform * p == point(2, 1, 7)")
		{
			REQUIRE(inv * p == point(-8.0f, 7.0f, 3.0f));
		}
	}
}

SCENARIO("Translation does not affect vectors", "[matrix]")
{
	GIVEN("transform = translation(5, -3, 2) And v = vector(-3, 4, 5)")
	{
		auto transform = translate(5.0f, -3.0f, 2.0f);
		auto v = vector(-3.0f, 4.0f, 5.0f);

		THEN("transform * p == point(2, 1, 7)")
		{
			REQUIRE(transform * v == v);
		}
	}
}

SCENARIO("A scaling matrix applied to a point", "[matrix]")
{
	GIVEN("transform = translation(5, -3, 2) And v = vector(-3, 4, 5)")
	{
		auto transform = scale(2.0f, 3.0f, 4.0f);
		auto p = point(-4.0f, 6.0f, 8.0f);

		THEN("transform * p == point(-8.0f, 18.0f, 32.0f)")
		{
			REQUIRE(transform * p == point(-8.0f, 18.0f, 32.0f));
		}
	}
}

SCENARIO("A scaling matrix applied to a vector", "[matrix]")
{
	GIVEN("transform = translation(2.0f, 3.0f, 4.0f) And v = vector(-4.0f, 6.0f, 8.0f)")
	{
		auto transform = scale(2.0f, 3.0f, 4.0f);
		auto v = vector(-4.0f, 6.0f, 8.0f);

		THEN("transform * p == point(-8.0f, 18.0f, 32.0f)")
		{
			REQUIRE(transform * v == vector(-8.0f, 18.0f, 32.0f));
		}
	}
}

SCENARIO("Multiplying by the inverse of a scaling matrix", "[matrix]")
{
	GIVEN("transform = scaling(2.0f, 3.0f, 4.0f)"
		"And inv = inverse(transform)"
		"And v = vector(-4.0f, 6.0f, 8.0f)")
	{
		auto transform = scale(2.0f, 3.0f, 4.0f);
		auto inv = inverse(transform);
		auto v = vector(-4.0f, 6.0f, 8.0f);

		THEN("inv * v = vector(-2.0f, 2.0f, 2.0f)")
		{
			REQUIRE(inv * v == vector(-2.0f, 2.0f, 2.0f));
		}
	}
}

SCENARIO("Reflection is scaling by a negative value", "[matrix]")
{
	GIVEN("transform = scaling(-1.0f, 1.0f, 1.0f) And p = point(2.0f, 3.0f, 4.0f)")
	{
		auto transform = scale(-1.0f, 1.0f, 1.0f);
		auto p = point(2.0f, 3.0f, 4.0f);

		THEN("transform * p == point(-2.0f, 3.0f, 4.0f)")
		{
			REQUIRE(transform * p == point(-2.0f, 3.0f, 4.0f));
		}
	}
}

SCENARIO("Rotating a point around the x axis", "[matrix].0f")
{
	GIVEN("p = point(0.0f, 1.0f, 0.0f)"
		"And halfQuarter = rotateX(¦Ð / 4.0f)"
		"And fullQuarter = rotateX(¦Ð / 2.0f)")
	{
		auto p = point(0.0f, 1.0f, 0.0f);
		auto halfQuarter = rotateX(PI / 4.0f);
		auto fullQuarter = rotateX(PI / 2.0f);

		THEN("halfQuarter * p == point(0.0f, ¡Ì2/2, ¡Ì2/2)"
			"And fullQuarter * p == point(0.0f, 0.0f, 1.0f)")
		{
			REQUIRE(halfQuarter * p == point(0.0f, SQRT2 / 2.0f, SQRT2 / 2.0f));
			REQUIRE(fullQuarter * p == point(0.0f, 0.0f, 1.0f));
		}
	}
}

SCENARIO("The inverse of an x-rotation rotates in the opposite direction", "[matrix].0f")
{
	GIVEN("p = point(0, 1, 0)"
		"And half_quarter = rotation_x(¦Ð / 4)"
		"And inv = inverse(half_quarter)")
	{
		auto p = point(0.0f, 1.0f, 0.0f);
		auto halfQuarter = rotateX(PI / 4.0f);
		auto inv = inverse(halfQuarter);

		THEN("inv * p = point(0, ¡Ì2/2, -¡Ì2/2)")
		{
			REQUIRE(inv * p == point(0.0f, SQRT2 / 2.0f, -SQRT2 / 2.0f));
		}
	}
}

SCENARIO("Rotating a point around the y axis", "[matrix].0f")
{
	GIVEN("p = point(0.0f, 0.0f, 1.0f)"
		"And halfQuarter = rotateY(¦Ð / 4.0f)"
		"And fullQuarter = rotateY(¦Ð / 2.0f)")
	{
		auto p = point(0.0f, 0.0f, 1.0f);
		auto halfQuarter = rotateY(PI / 4.0f);
		auto fullQuarter = rotateY(PI / 2.0f);

		THEN("halfQuarter * p == point(¡Ì2/2, 0.0f, ¡Ì2/2)"
			"And fullQuarter * p == point(1.0f, 0.0f, 0.0f)")
		{
			REQUIRE(halfQuarter * p == point(SQRT2 / 2.0f, 0.0f, SQRT2 / 2.0f));
			REQUIRE(fullQuarter * p == point(1.0f, 0.0f, 0.0f));
		}
	}
}

SCENARIO("Rotating a point around the z axis", "[matrix].0f")
{
	GIVEN("p = point(0.0f, 1.0f, 0.0f)"
		"And halfQuarter = rotateZ(¦Ð / 4.0f)"
		"And fullQuarter = rotateZ(¦Ð / 2.0f)")
	{
		auto p = point(0.0f, 1.0f, 0.0f);
		auto halfQuarter = rotateZ(PI / 4.0f);
		auto fullQuarter = rotateZ(PI / 2.0f);

		THEN("halfQuarter * p == point(¡Ì2/2, 0.0f, ¡Ì2/2)"
			"And fullQuarter * p == point(1.0f, 0.0f, 0.0f)")
		{
			REQUIRE(halfQuarter * p == point(-SQRT2 / 2.0f, SQRT2 / 2.0f, 0.0f));
			REQUIRE(fullQuarter * p == point(-1.0f, 0.0f, 0.0f));
		}
	}
}

SCENARIO("A shearing transformation moves x in proportion to y", "[matrix]")
{
	GIVEN("transform = shearing(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f)"
		"And p = point(2.0f, 3.0f, 4.0f)")
	{
		auto transform = shearing(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
		auto p = point(2.0f, 3.0f, 4.0f);

		THEN("transform * p == point(5.0f, 3.0f, 4.0f)")
		{
			REQUIRE(transform * p == point(5.0f, 3.0f, 4.0f));
		}
	}
}

SCENARIO("A shearing transformation moves x in proportion to z", "[matrix]")
{
	GIVEN("transform = shearing(0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f)"
		"And p = point(2.0f, 3.0f, 4.0f)")
	{
		auto transform = shearing(0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
		auto p = point(2.0f, 3.0f, 4.0f);

		THEN("transform * p == point(6.0f, 3.0f, 4.0f)")
		{
			REQUIRE(transform * p == point(6.0f, 3.0f, 4.0f));
		}
	}
}

SCENARIO("A shearing transformation moves y in proportion to x", "[matrix]")
{
	GIVEN("transform = shearing(0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f)"
		"And p = point(2.0f, 3.0f, 4.0f)")
	{
		auto transform = shearing(0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
		auto p = point(2.0f, 3.0f, 4.0f);

		THEN("transform * p == point(2.0f, 5.0f, 4.0f)")
		{
			REQUIRE(transform * p == point(2.0f, 5.0f, 4.0f));
		}
	}
}

SCENARIO("A shearing transformation moves y in proportion to z", "[matrix]")
{
	GIVEN("transform = shearing(0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f)"
		"And p = point(2.0f, 3.0f, 4.0f)")
	{
		auto transform = shearing(0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
		auto p = point(2.0f, 3.0f, 4.0f);

		THEN("transform * p == point(2.0f, 5.0f, 4.0f)")
		{
			REQUIRE(transform * p == point(2.0f, 7.0f, 4.0f));
		}
	}
}

SCENARIO("A shearing transformation moves z in proportion to y", "[matrix]")
{
	GIVEN("transform = shearing(0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f)"
		"And p = point(2.0f, 3.0f, 4.0f)")
	{
		auto transform = shearing(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
		auto p = point(2.0f, 3.0f, 4.0f);

		THEN("transform * p == point(2.0f, 3.0f, 7.0f)")
		{
			REQUIRE(transform * p == point(2.0f, 3.0f, 7.0f));
		}
	}
}

SCENARIO("Individual transformations are applied in sequence", "[matrix]")
{
	GIVEN("p1 = point(1.0f, 0.0f, 1.0f)"
		"And A = rotateX(¦Ð / 2.0f)"
		"And B = scale(5.0f, 5.0f, 5.0f)"
		"And C = translate(10.0f, 5.0f, 7.0f)")
	{
		auto A = rotateX(PI / 2.0f);
		auto B = scale(5.0f, 5.0f, 5.0f);
		auto C = translate(10.0f, 5.0f, 7.0f);

		auto p1 = point(1.0f, 0.0f, 1.0f);

		WHEN("p2 = A * p1"
			"p3 = B * p2"
			"p4 = C * p3")
		{
			auto p2 = A * p1;

			THEN("p2 == point(1.0f, -1.0f, 0.0f)")
			{
				REQUIRE(p2 == point(1.0f, -1.0f, 0.0f));
			}

			auto p3 = B * p2;

			THEN("p3 == point(5.0f, -5.0f, 0.0f)")
			{
				REQUIRE(p3 == point(5.0f, -5.0f, 0.0f));
			}

			auto p4 = C * p3;

			THEN("p4 == point(15.0f, 0.0f, 7.0f)")
			{
				REQUIRE(p4 == point(15.0f, 0.0f, 7.0f));
			}
		}
	}
}

SCENARIO("Chained transformations must be applied in reverse order", "[matrix]")
{
	GIVEN("p = point(1.0f, 0.0f, 1.0f)"
		"And A = rotateX(¦Ð / 2.0f)"
		"And B = scale(5.0f, 5.0f, 5.0f)"
		"And C = translate(10.0f, 5.0f, 7.0f)")
	{
		auto p = point(1.0f, 0.0f, 1.0f);
		auto A = rotateX(PI / 2.0f);
		auto B = scale(5.0f, 5.0f, 5.0f);
		auto C = translate(10.0f, 5.0f, 7.0f);

		WHEN("T = C * B * A")
		{
			auto T = C * B * A;

			THEN("T * p == point(15, 0, 7)")
			{
				REQUIRE(T * p == point(15, 0, 7));
			}
		}
	}
}