#include <catch2/catch_test_macros.hpp>

#include <csg.h>
#include <sphere.h>
#include <cube.h>

// Chapter 16 Constructive Solid Geometry (CSG)

SCENARIO("CSG is created with an operation and two shapes", "[csg]")
{
	GIVEN("s1 = Cphere()"
		"And s2 = Cube()")
	{
		auto s1 = createSphere();
		auto s2 = createCube();
		WHEN(R"(c = CSG("union", s1, s2)")
		{
			auto c = createCSG(Operation::Union, s1, s2);


			THEN(R"(c.operation == "union")"
				"And c.left == s1"
				"And c.right == s2"
				"And s1.parent == c"
				"And s2.parent == c")
			{
				REQUIRE(c->operation == Operation::Union);
				REQUIRE(c->left == s1);
				REQUIRE(c->right == s2);
				REQUIRE(s1->parent == c);
				REQUIRE(s2->parent == c);
			}
		}
	}
}

SCENARIO("Evaluating the rule for a CSG operation", "[csg]")
{
	WHEN("Examples:"
		"| op			 | lhit   | inl    | inr   | result |"
		"| union		 | true   | true   | true  | false  |"
		"| union		 | true   | true   | false | true   |"
		"| union		 | true   | false  | true  | false  |"
		"| union		 | true   | false  | false | true   |"
		"| union		 | false  | true   | true  | false  |"
		"| union		 | false  | true   | false | false  |"
		"| union		 | false  | false  | true  | true   |"
		"| union		 | false  | false  | false | true   |"
		"| intersection  | true   | true   | true  | true   |"
		"| intersection  | true   | true   | false | false  |"
		"| intersection  | true   | false  | true  | true   |"
		"| intersection  | true   | false  | false | false  |"
		"| intersection  | false  | true   | true  | true   |"
		"| intersection  | false  | true   | false | true   |"
		"| intersection  | false  | false  | true  | false  |"
		"| intersection  | false  | false  | false | false  |"
		"| difference    | true   | true   | true  | false  |"
		"| difference    | true   | true   | false | true   |"
		"| difference    | true   | false  | true  | false  |"
		"| difference    | true   | false  | false | true   |"
		"| difference    | false  | true   | true  | true   |"
		"| difference    | false  | true   | false | true   |"
		"| difference    | false  | false  | true  | false  |"
		"| difference    | false  | false  | false | false  |")
	{
		std::vector<Operation> operations(8, Operation::Union);
		std::fill_n(std::back_inserter(operations), 8, Operation::Intersection);
		std::fill_n(std::back_inserter(operations), 8, Operation::Difference);

		std::vector<bool> leftHits{ true, true, true, true, false, false, false, false,
									 true, true, true, true, false, false, false, false,
									 true, true, true, true, false, false, false, false };
		std::vector<bool> inLefts{ true, true, false, false, true, true, false, false, 
								    true, true, false, false, true, true, false, false,
								    true, true, false, false, true, true, false, false };
		std::vector<bool> inRights{ true, false, true, false, true, false, true, false,
									 true, false, true, false, true, false, true, false,
									 true, false, true, false, true, false, true, false };
		std::vector<bool> results{ false, true, false, true, false, false, true, true,
									true, false, true, false, true, true, false, false,
									false, true, false, true, true, true, false, false };

		for (size_t i = 0; i < results.size(); i++)
		{
			THEN("result == <result>" + std::to_string(i))
			{
				auto result = intersectionAllowed(operations[i], leftHits[i], inLefts[i], inRights[i]);
				REQUIRE(result == results[i]);
			}
		}
	}
}

SCENARIO("Filtering a list of intersections", "[csg]")
{
	GIVEN("s1 = Sphere()"
		"And s2 = Cube()"
		"And c = CSG(<operation>, s1, s2)"
		"And xs = intersections(1:s1, 2:s2, 3:s1, 4:s2)")
	{
		auto s1 = createSphere();
		auto s2 = createCube();
		auto xs = sortIntersections(
			{ { 1.0f, s1 }, { 2.0f, s2 }, { 3.0f, s1 }, { 4.0f, s2 } });
		WHEN("result = filterIntersections(c, xs)"
			"Examples:"
			"| operation	| x0 | x1 |"
			"| union		| 0  | 3  |"
			"| intersection | 1  | 2  |"
			"| difference	| 0  | 1  |")
		{
			std::vector<Operation> operations{Operation::Union, 
											   Operation::Intersection, 
											   Operation::Difference};
			std::vector<int32_t> x0s{ 0, 1, 0 };
			std::vector<int32_t> x1s{ 3, 2, 1 };

			for (size_t i = 0; i < operations.size(); i++)
			{
				auto c = CSG(operations[i], s1, s2);
				auto result = c.filterIntersections(xs);
				THEN("result.count == 2"
					"And result[0] == xs[<x0>]"
					"And result[1] == xs[<x1>]" + std::to_string(i))
				{
					REQUIRE(result.size() == 2);
					REQUIRE(result[0] == xs[x0s[i]]);
					REQUIRE(result[1] == xs[x1s[i]]);
				}
			}
		}
	}
}

SCENARIO("A ray misses a CSG object", "[csg]")
{
	GIVEN("c = CSG(union, Sphere(), Cube())"
		"And r = Ray(point(0.0f, 2.0f, -5.0f), vector(0.0f, 0.0f, 1.0f))")
	{
		auto c = createCSG(Operation::Union, createSphere(), createCube());
		auto r = Ray(point(0.0f, 2.0f, -5.0f), vector(0.0f, 0.0f, 1.0f));
		WHEN("xs = localIntersect(c, r)")
		{
			auto xs = c->localIntersect(r);
			THEN("xs is empty")
			{
				REQUIRE(xs.empty());
			}
		}
	}
}

SCENARIO("A ray hits a CSG object", "[csg]")
{
	GIVEN("s1 = Sphere()"
		"And s2 = Sphere()"
		"And setTransform(s2, translation(0.0f, 0.0f, 0.5f))"
		"And c = CSG(union, s1, s2)"
		"And r = Ray(point(0.0f, 0.0f, -5.0f), vector(0.0f, 0.0f, 1.0f)))")
	{
		auto s1 = createSphere();
		auto s2 = createSphere();
		s2->setTransform(translate(0.0f, 0.0f, 0.5f));
		auto c = createCSG(Operation::Union, s1, s2);
		auto r = Ray(point(0.0f, 0.0f, -5.0f), vector(0.0f, 0.0f, 1.0f));
		WHEN("xs = localIntersect(c, r)")
		{
			auto xs = c->localIntersect(r);
			THEN("xs.count == 2"
				"And xs[0].t == 4.0f"
				"And xs[0].object == s1"
				"And xs[1].t == 6.5f"
				"And xs[1].object == s2")
			{
				REQUIRE(xs.size() == 2);
				REQUIRE(xs[0].t == 4.0f);
				REQUIRE(xs[0].shape == s1);
				REQUIRE(xs[1].t == 6.5f);
				REQUIRE(xs[1].shape == s2);
			}
		}
	}
}