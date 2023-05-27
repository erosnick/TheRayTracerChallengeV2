#include <catch2/catch_test_macros.hpp>

#include <cstdint>

#include <tuple.h>

SCENARIO("A tuple with w = 1.0 is a point", "[tuple]")
{
	GIVEN("A tuple with w = 1.0")
	{
		auto a = tuple(4.3f, -4.2f, 3.1f, 1.0f);

		THEN("Compare component wise")
		{
			REQUIRE(equal(a.x, 4.3f));
			REQUIRE(equal(a.y, -4.2f));
			REQUIRE(equal(a.z, 3.1f));
			REQUIRE(equal(a.w, 1.0f));
			REQUIRE(a.isPoint());
			REQUIRE(!a.isVector());
		}
	}
}

SCENARIO("A tuple with w = 0.0 is a vector", "[tuple]")
{
	GIVEN("A tuple with w = 0.0")
	{
		auto a = tuple(4.3f, -4.2f, 3.1f, 0.0f);

		THEN("Compare component wise")
		{
			REQUIRE(equal(a.x, 4.3f));
			REQUIRE(equal(a.y, -4.2f));
			REQUIRE(equal(a.z, 3.1f));
			REQUIRE(equal(a.w, 0.0f));
			REQUIRE(!a.isPoint());
			REQUIRE(a.isVector());
		}
	}
}

SCENARIO("point() creates tuples with w = 1.0")
{
	GIVEN(ToString(point(4.0f, -4.0f, 3.0f)))
	{
		auto a = point(4.0f, -4.0f, 3.0f);

		THEN("Compare component wise")
		{
			REQUIRE(equal(a.x, 4.0f));
			REQUIRE(equal(a.y, -4.0f));
			REQUIRE(equal(a.z, 3.0f));
			REQUIRE(equal(a.w, 1.0f));
			REQUIRE(a.isPoint());
			REQUIRE(!a.isVector());
		}
	}
}

SCENARIO("vector() creates tuples with w = 0.0")
{
	GIVEN(ToString(vector(4.0f, -4.0f, 3.0f)))
	{
		auto a = vector(4.0f, -4.0f, 3.0f);

		THEN("Compare component wise")
		{
			REQUIRE(equal(a.x, 4.0f));
			REQUIRE(equal(a.y, -4.0f));
			REQUIRE(equal(a.z, 3.0f));
			REQUIRE(equal(a.w, 0.0f));
			REQUIRE(!a.isPoint());
			REQUIRE(a.isVector());
		}
	}
}

SCENARIO("Adding two tuples", "[tuple]")
{
	GIVEN(ToString(tuple(3.0f, -2.0f, 5.0f, 1.0f)
			   and tuple(-2.0f, 3.0f, 1.0f, 0.0f)))
	{
		auto a = tuple(3.0f, -2.0f, 5.0f, 1.0f);
		auto b = tuple(-2.0f, 3.0f, 1.0f, 0.0f);

		THEN("a1 + a2 == tuple(1, 1, 6, 1)")
		{
			REQUIRE(a + b == tuple(1.0f, 1.0f, 6.0f, 1.0f));
		}
	}
}

SCENARIO("Subtracting two points", "[tuple]")
{
	GIVEN(ToString(point(3.0f, 2.0f, 1.0f)
			   and vector(5.0f, 6.0f, 7.0f)))
	{
		auto p1 = point(3.0f, 2.0f, 1.0f);
		auto p2 = vector(5.0f, 6.0f, 7.0f);

		THEN("p1 - p2 == point(-2.0f, -4.0f, -6.0f)")
		{
			REQUIRE(p1 - p2 == point(-2.0f, -4.0f, -6.0f));
		}
	}
}

SCENARIO("Subtracting a vector from a point", "[tuple]")
{
	GIVEN(ToString(point(3.0f, 2.0f, 1.0f)
			   and vector(5.0f, 6.0f, 7.0f)))
	{
		auto v = point(3.0f, 2.0f, 1.0f);
		auto p = vector(5.0f, 6.0f, 7.0f);

		THEN("v - p == point(-2.0f, -4.0f, -6.0f)")
		{
			REQUIRE(v - p == point(-2.0f, -4.0f, -6.0f));
		}
	}
}

SCENARIO("Subtracting two vectors", "[tuple]")
{
	GIVEN(ToString(vector(3.0f, 2.0f, 1.0f)
			   and vector(5.0f, 6.0f, 7.0f)))
	{
		auto p = vector(3.0f, 2.0f, 1.0f);
		auto v = vector(5.0f, 6.0f, 7.0f);

		THEN("p - v == vector(-2.0f, -4.0f, -6.0f)")
		{
			REQUIRE(p - v == vector(-2.0f, -4.0f, -6.0f));
		}
	}
}

SCENARIO("Negating a tuple", "[tuple]")
{
	GIVEN(ToString(tuple(1.0f, -2.0f, 3.0f, -4.0f)))
	{
		auto a = tuple(1.0f, -2.0f, 3.0f, -4.0f);

		THEN("-a == tuple(-1.0f, 2.0f, -3.0f, 4.0f)")
		{
			REQUIRE(-a == tuple(-1.0f, 2.0f, -3.0f, 4.0f));
		}
	}
}

SCENARIO("Multiplying a tuple by a scalar", "[tuple]")
{
	GIVEN(ToString(tuple(1.0f, -2.0f, 3.0f, -4.0f)))
	{
		auto a = tuple(1.0f, -2.0f, 3.0f, -4.0f);

		THEN("a * 3.5f == tuple(1.0f, -2.0f, 3.0f, -4.0f)")
		{
			REQUIRE(a * 3.5f == tuple(3.5f, -7.0f, 10.5f, -14.0f));
		}
	}
}

SCENARIO("Dividing a tuple by a scalar", "[tuple]")
{
	GIVEN(ToString(tuple(1.0f, -2.0f, 3.0f, -4.0f)))
	{
		auto a = tuple(1.0f, -2.0f, 3.0f, -4.0f);

		THEN("a / 2.0f == tuple(0.5f, -1.0f, 1.5f, -2.0f)")
		{
			REQUIRE(a / 2.0f == tuple(0.5f, -1.0f, 1.5f, -2.0f));
		}
	}
}

SCENARIO("Computing the magnitude of vector(1, 0, 0)", "[tuple]")
{
	GIVEN(ToString(vector(1, 0, 0)))
	{
		auto v = vector(1, 0, 0);

		THEN("length(v) == 1.0f")
		{
			REQUIRE(equal(length(v), 1.0f));
		}
	}
}

SCENARIO("Computing the magnitude of vector(0, 1, 0)", "[tuple]")
{
	GIVEN(ToString(vector(0, 1, 0)))
	{
		auto v = vector(0, 1, 0);

		THEN("length(v) == 1.0f")
		{
			REQUIRE(equal(length(v), 1.0f));
		}
	}
}

SCENARIO("Computing the magnitude of vector(0, 0, 1)", "[tuple]")
{
	GIVEN(ToString(vector(0, 0, 1)))
	{
		auto v = vector(0, 0, 1);

		THEN("length(v) == 1.0f")
		{
			REQUIRE(equal(length(v), 1.0f));
		}
	}
}

SCENARIO("Computing the magnitude of vector(1, 2, 3)", "[tuple]")
{
	GIVEN(ToString(vector(1, 2, 3)))
	{
		auto v = vector(1, 2, 3);

		THEN("length(v) == ¡Ì14")
		{
			REQUIRE(equal(length(v), std::sqrtf(14.0f)));
		}
	}
}

SCENARIO("Computing the magnitude of vector(-1, -2, -3)", "[tuple]")
{
	GIVEN(ToString(vector(-1, -2, -3)))
	{
		auto v = vector(-1, -2, -3);

		THEN("length(v) == ¡Ì14")
		{
			REQUIRE(equal(length(v), std::sqrtf(14.0f)));
		}
	}
}

SCENARIO("Normalizing vector(4, 0, 0) gives (1, 0, 0)")
{
	GIVEN(ToString(vector(4.0f, 0.0f, 0.0f)))
	{
		auto v = vector(4.0f, 0.0f, 0.0f);

		THEN("normalize(v) == vector(1, 0, 0)")
		{
			REQUIRE(normalize(v) == vector(1.0f, 0.0f, 0.0f));
		}
	}
}

SCENARIO("Normalizing vector(1, 2,3)")
{
	GIVEN(ToString(vector(1.0f, 2.0f, 3.0f)))
	{
		auto v = vector(1.0f, 2.0f, 3.0f);

		THEN("normalize(v) == vector(0.26726, 0.53452, 0.80178)")
		{
			REQUIRE(normalize(v) == vector(0.26726f, 0.53452f, 0.80178f));
		}
	}
}

SCENARIO("The magnitude of a normalized vector")
{
	GIVEN(ToString(vector(1.0f, 2.0f, 3.0f)))
	{
		auto v = vector(1.0f, 2.0f, 3.0f);

		WHEN("norm = normalize(v)")
		{
			auto norm = normalize(v);
			THEN("length(norm) == 1.0f")
			{
				REQUIRE(equal(length(norm), 1.0f));
			}
		}
	}
}

SCENARIO("The dot product of two tuples")
{
	GIVEN(ToString(a = vector(1.0f, 2.0f, 3.0f)
			   and b = vector(2.0f, 3.0f, 4.0f)))
	{
		auto a = vector(1.0f, 2.0f, 3.0);
		auto b = vector(2.0f, 3.0f, 4.0f);

		THEN("dot(a, b) == 20.0f")
		{
			REQUIRE(equal(dot(a, b), 20.0f));
		}
	}
}

SCENARIO("The cross product of two vectors")
{
	GIVEN(ToString(a = vector(1.0f, 2.0f, 3.0f)
			   and b = vector(2.0f, 3.0f, 4.0f)))
	{
		auto a = vector(1.0f, 2.0f, 3.0f);
		auto b = vector(2.0f, 3.0f, 4.0f);

		THEN("cross(a, b) == vector(-1.0f, 2.0f, -1.0f)")
		{
			REQUIRE(cross(a, b) == vector(-1.0f, 2.0f, -1.0f));
		}

		THEN("cross(b, a) == vector(1.0f, -2.0f, 1.0f)")
		{
			REQUIRE(cross(b, a) == vector(1.0f, -2.0f, 1.0f));
		}
	}
}