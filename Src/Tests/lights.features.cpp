#include <catch2/catch_test_macros.hpp>

#include <light.h>

// Chapter 6 Light and Shadings

SCENARIO("A point light has a position and intensity", "[matrix]")
{
	GIVEN("intensity = color(1.0f, 1.0f, 1.0f)"
		"And position = point(0.0f, 0.0f, 0.0f)")
	{
		auto intensity = color(1.0f, 1.0f, 1.0f);
		auto position = point(0.0f, 0.0f, 0.0f);

		WHEN("light = pointLight(position, intensity)")
		{
			auto light = pointLight(position, intensity);
			THEN("light.position == position"
			"And light.intensity == intensity")
			{
				REQUIRE(light.position == position);
				REQUIRE(light.intensity == intensity);
			}
		}
	}
}