#include <catch2/catch_test_macros.hpp>

#include <boundingbox.h>

// Bonus Chapter: Bounding boxes and hierarchies

SCENARIO("Creating an empty bounding box", "[boundingbox]")
{
	GIVEN("box = boundingbox(empty)")
	{
		auto box = BoundingBox();
		THEN("box.min == point(infinity, infinity, infinity)"
			"And box.max == point(-infinity, -infinity, -infinity)")
		{
			REQUIRE(box.min == point(std::numeric_limits<float>::max()));
			REQUIRE(box.max == point(-std::numeric_limits<float>::max()));
		}
	}
}

SCENARIO("Creating a bounding box with volume", "[boundingbox]")
{
	GIVEN("box = boundingbox(min = point(-1.0f, -2.0f, -3.0f) max = point(3.0f, 2.0f, 1.0f))")
	{
		auto box = BoundingBox(point(-1.0f, -2.0f, -3.0f), point(3.0f, 2.0f, 1.0f));
		THEN("box.min == point(-1.0f, -2.0f, -3.0f)"
			"And box.max == point(3.0f, 2.0f, 1.0f)")
		{
			REQUIRE(box.min == point(-1.0f, -2.0f, -3.0f));
			REQUIRE(box.max == point(3.0f, 2.0f, 1.0f));
		}
	}
}

SCENARIO("Adding points to an empty bounding box", "[boundingbox]")
{
	GIVEN("box = boundingbox(empty)"
		"And p1 = point(-5.0f, 2.0f, 0.0f)"
		"And p2 = point(7.0f, 0.0f, -3.0f)")
	{
		auto box = BoundingBox();
		auto p1 = point(-5.0f, 2.0f, 0.0f);
		auto p2 = point(7.0f, 0.0f, -3.0f);
		WHEN("p1 is added to box"
			"And p2 is added to box")
		{
			box.addPoint(p1);
			box.addPoint(p2);
			THEN("box.min == point(-5.0f, 0.0f, -3.0f)"
				"And box.max == point(7.0f, 2.0f, 0.0f)")
			{
				REQUIRE(box.min == point(-5.0f, 0.0f, -3.0f));
				REQUIRE(box.max == point(7.0f, 2.0f, 0.0f));
			}
		}
	}
}