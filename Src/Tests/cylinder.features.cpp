#include <catch2/catch_test_macros.hpp>

#include <cylinder.h>

// Chapter 13 Cylinder

SCENARIO("A ray misses a cylinder", "[cylinder]")
{
	GIVEN("cylinder = Cylinder()"
		"And direction = normalize(<direction>)"
		"And r = ray(<origin>, direction)")
	{
		auto cylinder = createCylinder();

		std::vector<tuple> origins;
		origins.emplace_back(point(1.0f, 0.0f, 0.0f));
		origins.emplace_back(point(0.0f, 0.0f, 0.0f));
		origins.emplace_back(point(1.0f, 0.0f, -5.0f));

		std::vector<tuple> directions;
		directions.emplace_back(vector(0.0f, 1.0f, 0.0f));
		directions.emplace_back(vector(0.0f, 1.0f, 0.0f));
		directions.emplace_back(vector(1.0f, 1.0f, 1.0f));

		WHEN("xs = localIntersect(cylinder, r)"
			"Examples:"
			"| origin					 | direction				|"
			"| point(1.0f, 0.0f,  0.0f)  | vector(0.0f, 1.0f, 0.0f) |"
			"| point(0.0f, 0.0f,  0.0f)  | vector(0.0f, 1.0f, 0.0f) |"
			"| point(0.0f, 0.0f, -5.0f)  | vector(1.0f, 1.0f, 1.0f) |")
		{
			for (size_t i = 0; i < origins.size(); i++)
			{
				auto direction = normalize(directions[i]);
				auto r = Ray(origins[i], direction);
				auto xs = cylinder->localIntersect(r);
				THEN("xs.count == 0" + std::to_string(i))
				{
					REQUIRE(xs.size() == 0);
				}
			}
		}
	}
}

SCENARIO("A ray strikes a cylinder", "[cylinder]")
{
	GIVEN("cylinder = Cylinder()"
		"And direction = normalize(<direction>)"
		"And r = ray(<origin>, direction)")
	{
		auto cylinder = createCylinder();

		std::vector<tuple> origins;
		origins.emplace_back(point(1.0f, 0.0f, -5.0f));
		origins.emplace_back(point(0.0f, 0.0f, -5.0f));
		origins.emplace_back(point(0.5f, 0.0f, -5.0f));

		std::vector<tuple> directions;
		directions.emplace_back(vector(0.0f, 0.0f, 1.0f));
		directions.emplace_back(vector(0.0f, 0.0f, 1.0f));
		directions.emplace_back(vector(0.1f, 1.0f, 1.0f));

		std::vector<std::tuple<float, float>> times;
		times.push_back({ 5.0f, 5.0f });
		times.push_back({ 4.0f, 6.0f });
		times.push_back({ 6.80798f, 7.08872f });

		WHEN("xs = localIntersect(cylinder, r)"
			"Examples:"
			"| origin					 | direction				| t0		| t1	  |"
			"| point(1.0f, 0.0f,  0.0f)  | vector(0.0f, 1.0f, 0.0f) | 5.0f		| 5.0f	  |"
			"| point(0.0f, 0.0f,  0.0f)  | vector(0.0f, 1.0f, 0.0f) | 4.0f		| 6.0f	  |"
			"| point(0.0f, 0.0f, -5.0f)  | vector(1.0f, 1.0f, 1.0f) | 6.80798f  | 7.08872f|")
		{
			for (size_t i = 0; i < origins.size(); i++)
			{
				auto direction = normalize(directions[i]);
				auto r = Ray(origins[i], direction);
				auto xs = cylinder->localIntersect(r);
				THEN("xs.count == 2"
					"xs[0].t = <t0>"
					"xs[1].t = <t1>" + std::to_string(i))
				{
					REQUIRE(xs.size() == 2);
					REQUIRE(equal(xs[0].t, std::get<0>(times[i])));
					REQUIRE(equal(xs[1].t, std::get<1>(times[i])));
				}
			}
		}
	}
}

SCENARIO("Normal vector on a cylinder", "[cylinder]")
{
	GIVEN("cylinder = Cylinder()"
		"And direction = normalize(<direction>)"
		"And r = ray(<origin>, direction)")
	{
		auto cylinder = createCylinder();

		std::vector<tuple> positions;
		positions.emplace_back(point(1.0f, 0.0f, 0.0f));
		positions.emplace_back(point(0.0f, 5.0f, -1.0f));
		positions.emplace_back(point(0.0f, -2.0f, 1.0f));
		positions.emplace_back(point(-1.0f, 1.0f, 0.0f));

		std::vector<tuple> normals;
		normals.emplace_back(vector(1.0f, 0.0f, 0.0f));
		normals.emplace_back(vector(0.0f, 0.0f, -1.0f));
		normals.emplace_back(vector(0.0f, 0.0f, 1.0f));
		normals.emplace_back(vector(-1.0f, 0.0f, 0.0f));

		WHEN("n = localNormalAt(cylinder, <point>)"
			"Examples:"
			"| point					   | normal					    |"
			"| point( 1.0f,  0.0f,  0.0f)  | vector( 1.0f, 0.0f,  0.0f) |"
			"| point( 0.0f,  5.0f, -1.0f)  | vector( 0.0f, 0.0f, -1.0f) |"
			"| point( 0.0f, -2.0f,  1.0f)  | vector( 1.0f, 0.0f,  1.0f) |"
			"| point(-1.0f,  1.0f,  0.0f)  | vector(-1.0f, 0.0f,  0.0f) |")
		{
			for (size_t i = 0; i < positions.size(); i++)
			{
				auto n = cylinder->localNormalAt(positions[i]);
				THEN("n == <normal>" + std::to_string(i))
				{
					REQUIRE(n == normals[i]);
				}
			}
		}
	}
}

SCENARIO("The default minimum and maximum for a cylinder", "[cylinder]")
{
	GIVEN("cylinder = Cylinder()")
	{
		auto cylinder = Cylinder();
		THEN("cylinder.minimum == -infinity"
			"And cyl.maximum == infinity")
		{
			REQUIRE(cylinder.minimum == -std::numeric_limits<float>::max());
			REQUIRE(cylinder.maximum == std::numeric_limits<float>::max());
		}
	}
}

SCENARIO("Intersecting a constrained cylinder", "[cylinder]")
{
	GIVEN("cylinder = Cylinder()"
		"cylinder.minimum = 1.0f"
		"And cylinder.maximum = 2.0f"
		"And direction = normalize(<direction>)"
		"And r = Ray(<point>, direction)")
	{
		auto cylinder = createCylinder(1.0f, 2.0f);

		std::vector<tuple> origins;
		origins.emplace_back(point(0.0f, 1.5f, 0.0f));
		origins.emplace_back(point(0.0f, 3.0f, -5.0f));
		origins.emplace_back(point(0.0f, 0.0f, -5.0f));
		origins.emplace_back(point(0.0f, 2.0f, -5.0f));
		origins.emplace_back(point(0.0f, 1.0f, -5.0f));
		origins.emplace_back(point(0.0f, 1.5f, -2.0f));

		std::vector<tuple> directions;
		directions.emplace_back(vector(0.1f, 1.0f, 0.0f));
		directions.emplace_back(vector(0.0f, 0.0f, 1.0f));
		directions.emplace_back(vector(0.0f, 0.0f, 1.0f));
		directions.emplace_back(vector(0.0f, 0.0f, 1.0f));
		directions.emplace_back(vector(0.0f, 0.0f, 1.0f));
		directions.emplace_back(vector(0.0f, 0.0f, 1.0f));

		std::vector<size_t> counts = { 0, 0, 0, 0, 0, 2 };

		WHEN("xs = localIntersect(cylinder, r)"
			"Examples:"
			"|   | point					| direction					 | count  |"
			"| 1 | point(0.0f, 1.5f,  0.0f) | vector(0.1f, 1.0f, 0.0f)   | 0	  |"
			"| 2 | point(0.0f, 3.0f, -5.0f) | vector(0.0f, 0.0f, 1.0f)   | 0	  |"
			"| 3 | point(0.0f, 0.0f, -5.0f) | vector(0.0f, 0.0f, 1.0f)   | 0	  |"
			"| 4 | point(0.0f, 2.0f, -5.0f) | vector(0.0f, 0.0f, 1.0f)   | 0	  |"
			"| 5 | point(0.0f, 1.0f, -5.0f) | vector(0.0f, 0.0f, 1.0f)   | 0	  |"
			"| 6 | point(0.0f, 1.5f, -2.0f)	| vector(0.0f, 0.0f, 1.0f)   | 2	  |")
		{
			for (size_t i = 0; i < origins.size(); i++)
			{
				auto direction = normalize(directions[i]);
				auto r = Ray(origins[i], direction);
				auto xs = cylinder->localIntersect(r);

				THEN("xs.count == <count>" + std::to_string(i))
				{
					REQUIRE(xs.size() == counts[i]);
				}
			}
		}
	}
}

SCENARIO("The default closed value for a cylinder", "[cylinder]")
{
	GIVEN("cylinder = Cylinder()")
	{
		auto cylinder = Cylinder();
		THEN("cylinder.closed == false")
		{
			REQUIRE(cylinder.closed == false);
		}
	}
}

SCENARIO("Intersecting the caps of a closed cylinder", "[cylinder]")
{
	GIVEN("cylinder = Cylinder()"
		"cylinder.minimum = 1.0f"
		"And cylinder.maximum = 2.0f"
		"And direction = normalize(<direction>)"
		"And r = Ray(<point>, direction)")
	{
		auto cylinder = createCylinder(1.0f, 2.0f, true);

		std::vector<tuple> origins;
		origins.emplace_back(point(0.0f, 3.0f, 0.0f));
		origins.emplace_back(point(0.0f, 3.0f, -2.0f));
		origins.emplace_back(point(0.0f, 4.0f, -2.0f));
		origins.emplace_back(point(0.0f, 0.0f, -2.0f));
		origins.emplace_back(point(0.0f, -1.0f, -2.0f));

		std::vector<tuple> directions;
		directions.emplace_back(vector(0.0f, -1.0f, 0.0f));
		directions.emplace_back(vector(0.0f, -1.0f, 2.0f));
		directions.emplace_back(vector(0.0f, -1.0f, 1.0f));
		directions.emplace_back(vector(0.0f, 1.0f, 2.0f));
		directions.emplace_back(vector(0.0f, 1.0f, 1.0f));

		std::vector<size_t> counts = { 2, 2, 2, 2, 2 };

		WHEN("xs = localIntersect(cylinder, r)"
			"Examples:"
			"|   | point					 | direction				 | count |"
			"| 1 | point(0.0f,  3.0f,  0.0f) | vector(0.0f, -1.0f, 0.0f) | 2	 |"
			"| 2 | point(0.0f,  3.0f, -2.0f) | vector(0.0f, -1.0f, 2.0f) | 2	 |"
			"| 3 | point(0.0f,  4.0f, -2.0f) | vector(0.0f, -1.0f, 1.0f) | 2	 |"
			"| 4 | point(0.0f,  0.0f, -2.0f) | vector(0.0f,  1.0f, 2.0f) | 2	 |"
			"| 5 | point(0.0f, -1.0f, -2.0f) | vector(0.0f,  1.0f, 1.0f) | 2	 |")
		{
			for (size_t i = 0; i < origins.size(); i++)
			{
				auto direction = normalize(directions[i]);
				auto r = Ray(origins[i], direction);
				auto xs = cylinder->localIntersect(r);

				THEN("xs.count == <count>" + std::to_string(i))
				{
					REQUIRE(xs.size() == counts[i]);
				}
			}
		}
	}
}

SCENARIO("The normal vector on a cylinder's end cap", "[cylinder]")
{
	GIVEN("cylinder = Cylinder()"
		"And direction = normalize(<direction>)"
		"And r = ray(<origin>, direction)")
	{
		auto cylinder = createCylinder(1.0f, 2.0f, true);

		std::vector<tuple> positions;
		positions.emplace_back(point(0.0f, 1.0f, 0.0f));
		positions.emplace_back(point(0.5f, 1.0f, 0.0f));
		positions.emplace_back(point(0.0f, 1.0f, 0.5f));
		positions.emplace_back(point(0.0f, 2.0f, 0.0f));
		positions.emplace_back(point(0.5f, 2.0f, 0.0f));
		positions.emplace_back(point(0.0f, 2.0f, 0.5f));

		std::vector<tuple> normals;
		normals.emplace_back(vector(0.0f, -1.0f, 0.0f));
		normals.emplace_back(vector(0.0f, -1.0f, 0.0f));
		normals.emplace_back(vector(0.0f, -1.0f, 0.0f));
		normals.emplace_back(vector(0.0f,  1.0f, 0.0f));
		normals.emplace_back(vector(0.0f,  1.0f, 0.0f));
		normals.emplace_back(vector(0.0f,  1.0f, 0.0f));

		WHEN("n = localNormalAt(cylinder, <point>)"
			"Examples:"
			"| point					| normal					|"
			"| point(0.0f, 1.0f, 0.0f)  | vector(0.0f, -1.0f, 0.0f) |"
			"| point(0.5f, 1.0f, 0.0f)  | vector(0.0f, -1.0f, 0.0f) |"
			"| point(0.0f, 1.0f, 0.5f)  | vector(0.0f, -1.0f, 0.0f) |"
			"| point(0.0f, 2.0f, 0.0f)  | vector(0.0f,  1.0f, 0.0f) |"
			"| point(0.5f, 2.0f, 0.0f)  | vector(0.0f,  1.0f, 0.0f) |"
			"| point(0.0f, 2.0f, 0.5f)  | vector(0.0f,  1.0f, 0.0f) |")
		{
			for (size_t i = 0; i < positions.size(); i++)
			{
				auto n = cylinder->localNormalAt(positions[i]);
				THEN("n == <normal>" + std::to_string(i))
				{
					REQUIRE(n == normals[i]);
				}
			}
		}
	}
}