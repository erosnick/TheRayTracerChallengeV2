#include <catch2/catch_test_macros.hpp>

// Chapter 12 Cubes

#include <cube.h>

SCENARIO("A ray intersects a cube", "[cube]")
{
	GIVEN("c = Cube()"
		"And r = Ray(<origin>, <direction>)")
	{
		auto c = createCube();
		WHEN("xs = localIntersect(c, r)"
			"Examples:"
			"|		  | origin				 | direction			|  t1    | t2    |"
			"| +x	  | point( 5,    0.5, 0) | vector(-1,  0,  0)	|  4.0f  | 6.0f  |"
			"| -x	  | point(-5,    0.5, 0) | vector( 1,  0,  0)   |  4.0f  | 6.0f  |"
			"| +y	  | point( 0.5,  5,   0) | vector( 0, -1,  0)	|  4.0f  | 6.0f  |"
			"| -y	  | point( 0.5, -5,   0) | vector( 0,  1,  0)   |  4.0f  | 6.0f  |" 
			"| +z	  | point( 0.5,  0,   5) | vector( 0,  0, -1)   |  4.0f  | 6.0f  |"
			"| -z	  | point( 0.5,  0,  -5) | vector( 0,  0,  1)   |  4.0f  | 6.0f  |"
			"| inside | point( 0,0.  5,   0) | vector( 0,  0,  1)   | -1.0f  | 1.0f  |")
		{
			std::vector<tuple> origins;
			origins.emplace_back(point( 5.0f,  0.5f,  0.0f));
			origins.emplace_back(point(-5.0f,  0.5f,  0.0f));
			origins.emplace_back(point( 0.5f,  5.0f,  0.0f));
			origins.emplace_back(point( 0.5f, -5.0f,  0.0f));
			origins.emplace_back(point( 0.5f,  0.0f,  5.0f));
			origins.emplace_back(point( 0.5f,  0.0f, -5.0f));
			origins.emplace_back(point( 0.0f,  0.5f,  0.0f));

			std::vector<tuple> directions;
			directions.emplace_back(vector(-1.0f,  0.0f,  0.0f));
			directions.emplace_back(vector( 1.0f,  0.0f,  0.0f));
			directions.emplace_back(vector( 0.0f, -1.0f,  0.0f));
			directions.emplace_back(vector( 0.0f,  1.0f,  0.0f));
			directions.emplace_back(vector( 0.0f,  0.0f, -1.0f));
			directions.emplace_back(vector( 0.0f,  0.0f,  1.0f));
			directions.emplace_back(vector( 0.0f,  0.0f,  1.0f));

			std::vector<std::tuple<float, float>> times;
			times.push_back({  4.0f, 6.0f });
			times.push_back({  4.0f, 6.0f });
			times.push_back({  4.0f, 6.0f });
			times.push_back({  4.0f, 6.0f });
			times.push_back({  4.0f, 6.0f });
			times.push_back({  4.0f, 6.0f });
			times.push_back({ -1.0f, 1.0f });

			for (size_t i = 0; i < origins.size(); i++)
			{
				auto r = Ray(origins[i], directions[i]);
				auto xs = c->localIntersect(r);
				THEN("xs.count = 2"
					"And xs[0].t = <t1>"
					"And xs[1].t = <t2>" + std::to_string(i))
				{
					REQUIRE(xs.size() == 2);
					REQUIRE(xs[0].t == std::get<0>(times[i]));
					REQUIRE(xs[1].t == std::get<1>(times[i]));
				}
			}
		}
	}
}

SCENARIO("A ray misses a cube", "[cube]")
{
	GIVEN("c = Cube()"
		"And r = Ray(<origin>, <direction>)")
	{
		auto c = createCube();
		WHEN("xs = localIntersect(c, r)"
			"Examples:"
			"| origin					  | direction			"
			"| point(-2.0f,  0.0f,  0.0f) | vector( 0.2673f, 0.5345f, 0.8018f)"
			"| point( 0.0f, -2.0f,  0.0f) | vector( 0.8018f, 0.2673f, 0.5345f)"
			"| point( 0.0f,  0.0f, -2.0f) | vector( 0.5345f, 0.8018f, 0.2673f)"
			"| point( 2.0f,  0.0f,  2.0f) | vector( 0,		 0,		 -1		 )"
			"| point( 0.0f,  2.0f,  2.0f) | vector( 0,		-1,		  0		 )"
			"| point( 2.0f,  2.0f,  0.0f) | vector(-0,		 0,		  0		 )")
		{
			std::vector<tuple> origins;
			origins.emplace_back(point(-2.0f,  0.0f,  0.0f));
			origins.emplace_back(point( 0.0f, -2.0f,  0.0f));
			origins.emplace_back(point( 0.0f,  0.0f, -2.0f));
			origins.emplace_back(point( 2.0f,  0.0f,  2.0f));
			origins.emplace_back(point( 0.0f,  2.0f,  2.0f));
			origins.emplace_back(point( 2.0f,  2.0f,  0.0f));

			std::vector<tuple> directions;
			directions.emplace_back(vector( 0.2673f, 0.5345f, 0.8018f));
			directions.emplace_back(vector( 0.8018f, 0.2673f, 0.5345f));
			directions.emplace_back(vector( 0.5345f, 0.8018f, 0.2673f));
			directions.emplace_back(vector( 0.0f,	 0.0f,	-1.0f));
			directions.emplace_back(vector( 0.0f,	-1.0f,	 0.0f));
			directions.emplace_back(vector(-1.0f,	 0.0f,	 0.0f));

			for (size_t i = 0; i < origins.size(); i++)
			{
				auto r = Ray(origins[i], directions[i]);
				auto xs = c->localIntersect(r);
				THEN("xs.count == 0"
					"And xs[0].t == <t1>"
					"And xs[1].t == <t2>" + std::to_string(i))
				{
					REQUIRE(xs.size() == 0);
				}
			}
		}
	}
}

SCENARIO("The normal on the surface of a cube", "[cube]")
{
	GIVEN("c = Cube()"
		"And p = <point>")
	{
		auto c = createCube();
		WHEN("normal = localNormalAt(c, p)"
			"Examples:"
			"| origin					  | normal"
			"| point( 1.0f,  0.5f, -0.8f) | vector( 1.0f,  0.0f,  0.0f)"
			"| point(-1.0f, -0.2f,  0.9f) | vector(-1.0f,  0.0f,  0.0f)"
			"| point(-0.4f,  1.0f, -0.1f) | vector( 0.0f,  1.0f,  0.0f)"
			"| point( 0.3f, -1.0f, -0.7f) | vector( 0.0f, -1.0f,  0.0f)"
			"| point(-0.6f,  0.3f,	1.0f) | vector( 0.0f,  0.0f,  1.0f)"
			"| point( 0.4f,  0.4f, -1.0f) | vector( 0.0f,  0.0f, -1.0f)"
			"| point( 1.0f,	 1.0f,  1.0f) | vector( 1.0f,  0.0f,  0.0f)"
			"| point(-1.0f, -1.0f, -1.0f) | vector(-1.0f,  0.0f,  0.0f)"
		)
		{
			std::vector<tuple> points;
			points.emplace_back(point( 1.0f,  0.5f, -0.8f));
			points.emplace_back(point(-1.0f, -0.2f,  0.9f));
			points.emplace_back(point(-0.4f,  1.0f, -0.1f));
			points.emplace_back(point( 0.3f, -1.0f, -0.7f));
			points.emplace_back(point(-0.6f,  0.3f,	 1.0f));
			points.emplace_back(point( 0.4f,  0.4f, -1.0f));
			points.emplace_back(point( 1.0f,  1.0f,  1.0f));
			points.emplace_back(point(-1.0f, -1.0f, -1.0f));

			std::vector<tuple> normals;
			normals.emplace_back(vector( 1.0f,  0.0f,  0.0f));
			normals.emplace_back(vector(-1.0f,  0.0f,  0.0f));
			normals.emplace_back(vector( 0.0f,  1.0f,  0.0f));
			normals.emplace_back(vector( 0.0f, -1.0f,  0.0f));
			normals.emplace_back(vector( 0.0f,  0.0f,  1.0f));
			normals.emplace_back(vector( 0.0f,  0.0f, -1.0f));
			normals.emplace_back(vector( 1.0f,  0.0f,  0.0f));
			normals.emplace_back(vector(-1.0f,  0.0f,  0.0f));

			for (size_t i = 0; i < points.size(); i++)
			{
				auto p = points[i];
				auto normal = c->localNormalAt(p);
				THEN("normal == <normal>" + std::to_string(i))
				{
					REQUIRE(normal == normals[i]);
				}
			}
		}
	}
}

