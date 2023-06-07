#include <catch2/catch_test_macros.hpp>

#include <cone.h>

SCENARIO("Intersecting a cone with a ray", "[cone]")
{
	GIVEN("cone = Cone()"
		"And direction = normalize(<direction>)"
		"And r = Ray(<origin>, direction)")
	{
		auto cone = createCone();

		std::vector<tuple> origins;
		origins.emplace_back(point(0.0f, 0.0f, -5.0f));
		origins.emplace_back(point(0.0f, 0.0f, -5.0f));
		origins.emplace_back(point(1.0f, 1.0f, -5.0f));

		std::vector<tuple> directions;
		directions.emplace_back(vector( 0.0f,  0.0f, 1.0f));
		directions.emplace_back(vector( 1.0f,  1.0f, 1.0f));
		directions.emplace_back(vector(-0.5f, -1.0f, 1.0f));

		std::vector<std::tuple<float, float>> times;
		times.push_back({ 5.0f,     5.0f });
		times.push_back({ 8.66025f, 8.66025f });
		times.push_back({ 4.55006f, 49.44994f });

		WHEN("xs = localIntersect(shape, r)"
			"Examples:"
			"| origin					| direction					  | t0	     | t1		  |"
			"| point(0.0f, 0.0f, -5.0f) | vector( 0.0f,  0.0f, 1.0f)  | 5.0f	 | 5.0f		  |"
			"| point(0.0f, 0.0f, -5.0f) | vector( 1.0f,  1.0f, 1.0f)  | 8.66025f | 8.66025f   |"
			"| point(1.0f, 1.0f, -5.0f) | vector(-0.5f, -1.0f, 1.0f)  | 4.55006f | 49.44994f  |")
		{
			for (size_t i = 0; i < origins.size(); i++)
			{
				auto direction = normalize(directions[i]);
				auto r = Ray(origins[i], direction);
				auto xs = cone->localIntersect(r);
				THEN("xs.count == 2"
					"And xs[0].t == <t0>"
					"And xs[1].t == <t1>" + std::to_string(i))
				{
					REQUIRE(xs.size() == 2);
					REQUIRE(equal(xs[0].t, std::get<0>(times[i])));
					REQUIRE(equal(xs[1].t, std::get<1>(times[i])));
				}
			}
		}
	}
}

//SCENARIO("Intersecting a cone with a ray parallel to one of its halves", "[cone]")
//{
//	GIVEN("cone = Cone()"
//		"And direction = normalize(vector(0.0f, 1.0f, 1.0f))"
//		"And r = Ray(point(0.0f, 0.0f, -1.0f), direction)")
//	{
//		auto cone = createCone();
//		auto direction = normalize(vector(0.0f, 1.0f, 1.0f));
//		auto r = Ray(point(0.0f, 0.0f, -1.0f), direction);
//		WHEN("xs = localIntersect(cone, r)")
//		{
//			auto xs = cone->localIntersect(r);
//			THEN("xs.count == 1"
//				 "And xs[0].t = 0.35355f")
//			{
//				REQUIRE(xs.size() == 1);
//				REQUIRE(equal(xs[0].t, 0.35355f));
//			}
//		}
//	}
//}
//
//SCENARIO("Intersecting a cone's end caps", "[cone]")
//{
//	GIVEN("cone = Cone()"
//		"And direction = normalize(<direction>)"
//		"And r = Ray(<origin>, direction)")
//	{
//		auto cone = createCone(-0.5f, 0.5f, true);
//
//		std::vector<tuple> origins;
//		origins.emplace_back(point(0.0f, 0.0f, -5.0f));
//		origins.emplace_back(point(0.0f, 0.0f, -0.25f));
//		origins.emplace_back(point(0.0f, 0.0f, -0.25f));
//
//		std::vector<tuple> directions;
//		directions.emplace_back(vector(0.0f, 1.0f, 0.0f));
//		directions.emplace_back(vector(0.0f, 1.0f, 1.0f));
//		directions.emplace_back(vector(0.0f, 1.0f, 0.0f));
//
//		std::vector<size_t> counts = { 0, 2, 4 };
//
//		WHEN("xs = localIntersect(shape, r)"
//			"Examples:"
//			"| origin					 | direction				 | count |"
//			"| point(0.0f, 0.0f, -5.0f)  | vector(0.0f, 1.0f, 0.0f)  | 0     |"
//			"| point(0.0f, 0.0f, -0.25f) | vector(0.0f, 1.0f, 1.0f)  | 2     |"
//			"| point(0.0f, 0.0f, -0.25f) | vector(0.0f, 1.0f, 0.0f)  | 4     |")
//		{
//			for (size_t i = 0; i < origins.size(); i++)
//			{
//				auto direction = normalize(directions[i]);
//				auto r = Ray(origins[i], direction);
//				auto xs = cone->localIntersect(r);
//				THEN("xs.count == 2"
//					"And xs[0].t == <t0>"
//					"And xs[1].t == <t1>" + std::to_string(i))
//				{
//					REQUIRE(xs.size() == counts[i]);
//				}
//			}
//		}
//	}
//}
//
//SCENARIO("Computing the normal vector on a cone", "[cone]")
//{
//	GIVEN("cone = Cone()")
//	{
//		auto cone = Cone();
//
//		std::vector<tuple> positions;
//		positions.emplace_back(point( 0.0f,  0.0f, 0.0f));
//		positions.emplace_back(point( 1.0f,  1.0f, 1.0f));
//		positions.emplace_back(point(-1.0f, -1.0f, 0.0f));
//
//		std::vector<tuple> normals;
//		normals.emplace_back(vector( 0.0f,  0.0f,  0.0f));
//		normals.emplace_back(vector( 1.0f, -SQRT2, 1.0f));
//		normals.emplace_back(vector(-1.0f,  1.0f,  0.0f));
//
//		WHEN("n = localNormalAt(cone, <point>)"
//			"Examples:"
//			"| point					 | normal					   |"
//			"| point( 0.0f,  0.0f, 0.0f) | vector( 0.0f,  0.0f, 0.0f)  |"
//			"| point( 1.0f,  1.0f, 1.0f) | vector( 1.0f, -¡Ì2, 1.0f)	   |"
//			"| point(-1.0f, -1.0f, 0.0f) | vector(-1.0f,  1.0f, 0.0f)  |")
//		{
//			for (size_t i = 0; i < positions.size(); i++)
//			{
//				auto n = cone.localNormalAt(positions[i]);
//				THEN("n == <normal>")
//				{
//					REQUIRE(n == normals[i]);
//				}
//			}
//		}
//	}
//}