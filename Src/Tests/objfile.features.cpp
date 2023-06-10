#include <catch2/catch_test_macros.hpp>

#include <objLoader.h>
#include <triangle.h>

// Chapter 15 Triangles

SCENARIO("Ignoring unrecognized lines", "[objfile]")
{
	GIVEN("gibberish = a file containing:"
		"There was a young lady named Bright"
		"who traveled much faster than light."
		"She set out one day"
		"in a relative way,"
		"and came back the previous night.")
	{
		WHEN("parser = parseObjFile(gibberish)")
		{
			auto parser = parseObjFile("Assets/Models/test1.obj");
			THEN("parser = parseObjFile(gibberish)")
			{
				REQUIRE(parser.ignoreLineCount == 5);
			}
		}
	}
}

SCENARIO("Vertex records", "[objfile]")
{
	GIVEN("file = a file containing:"
		"v -1 1 0"
		"v -1.0000 0.5000 0.0000"
		"v  1 0 0"
		"v  1 1 0")
	{
		WHEN("parser = parseObjFile(file)")
		{
			auto parser = parseObjFile("Assets/Models/test2.obj");

			THEN("parser.vertices[1] == point(-1.0f, 1.0f, 0.0f)"
				"And parser.vertices[2] == point(-1.0f, 0.5f, 0.0f)"
				"And parser.vertices[3] == point(1.0f, 0.0f, 0.0f)"
				"And parser.vertices[4] == point(1.0f, 1.0f, 0.0f)")
			{
				REQUIRE(parser.vertices[1] == point(-1.0f, 1.0f, 0.0f));
				REQUIRE(parser.vertices[2] == point(-1.0f, 0.5f, 0.0f));
				REQUIRE(parser.vertices[3] == point(1.0f, 0.0f, 0.0f));
				REQUIRE(parser.vertices[4] == point(1.0f, 1.0f, 0.0f));
			}
		}
	}
}

SCENARIO("Parsing triangle faces", "[objfile]")
{
	GIVEN("file = a file containing:"
		"v -1 1 0"
		"v -1 0 0"
		"v 1 0 0"
		"v 1 1 0"
		"f 1 2 3"
		"		"
		"f 1 3 4")
	{
		WHEN("parser = parseObjFile(file)"
			"And g = parser.defaultGroup"
			"And t1 = first child of g"
			"And t2 = second child of g")
		{
			auto parser = parseObjFile("Assets/Models/test3.obj");
			auto g = parser.defaultGroup;
			auto t1 = std::dynamic_pointer_cast<Triangle>(g->getChild(0));
			auto t2 = std::dynamic_pointer_cast<Triangle>(g->getChild(1));

			THEN("t1.p1 == parser.vertices[1]"
				"And t1.p1 == parser.vertices[2]"
				"And t1.p2 == parser.vertices[3]"
				"And t2.p0 == parser.vertices[1]"
				"And t2.p1 == parser.vertices[3]"
				"And t2.p2 == parser.vertices[4]")
			{
				REQUIRE(t1->p0 == parser.vertices[1]);
				REQUIRE(t1->p1 == parser.vertices[2]);
				REQUIRE(t1->p2 == parser.vertices[3]);
				REQUIRE(t2->p0 == parser.vertices[1]);
				REQUIRE(t2->p1 == parser.vertices[3]);
				REQUIRE(t2->p2 == parser.vertices[4]);
			}
		}
	}
}

SCENARIO("Triangulating polygons", "[objfile]")
{
	GIVEN("file = a file containing:"
		"v -1 1 0"
		"v -1 0 0"
		"v 1 0 0"
		"v 1 1 0"
		"v 0 2 0"
		"		"
		"f 1 2 3 4 5")
	{
		WHEN("parser = parseObjFile(file)"
			"And g = parser.defaultGroup"
			"And t1 = first child of g"
			"And t2 = second child of g"
			"And t3 = third child of g")
		{
			auto parser = parseObjFile("Assets/Models/test4.obj");
			auto g = parser.defaultGroup;
			auto t1 = std::dynamic_pointer_cast<Triangle>(g->getChild(0));
			auto t2 = std::dynamic_pointer_cast<Triangle>(g->getChild(1));
			auto t3 = std::dynamic_pointer_cast<Triangle>(g->getChild(2));

			THEN("t1.p1 == parser.vertices[1]"
				"And t1.p1 == parser.vertices[2]"
				"And t1.p2 == parser.vertices[3]"
				"And t2.p0 == parser.vertices[1]"
				"And t2.p1 == parser.vertices[3]"
				"And t2.p2 == parser.vertices[4]")
			{
				REQUIRE(t1->p0 == parser.vertices[1]);
				REQUIRE(t1->p1 == parser.vertices[2]);
				REQUIRE(t1->p2 == parser.vertices[3]);
				REQUIRE(t2->p0 == parser.vertices[1]);
				REQUIRE(t2->p1 == parser.vertices[3]);
				REQUIRE(t2->p2 == parser.vertices[4]);
				REQUIRE(t3->p0 == parser.vertices[1]);
				REQUIRE(t3->p1 == parser.vertices[4]);
				REQUIRE(t3->p2 == parser.vertices[5]);
			}
		}
	}
}

SCENARIO("Triangles in groups", "[objfile]")
{
	GIVEN("file = the file triangles.obj")
	{
		WHEN("parser = parseObjFile(file)"
			"And g1 = FirstGroup from parser"
			"And g2 = SecondGroup from parser"
			"And t1 = first child of g1"
			"And t2 = first child of g2")
		{
			auto parser = parseObjFile("Assets/Models/triangles.obj");
			auto g1 = parser.groups[0];
			auto g2 = parser.groups[1];
			auto t1 = std::dynamic_pointer_cast<Triangle>(g1->getChild(0));
			auto t2 = std::dynamic_pointer_cast<Triangle>(g2->getChild(0));
			THEN("t1.p0 == parser.vertices[1]"
				"And t1.p1 == parser.vertices[2]"
				"And t1.p2 == parser.vertices[3]"
				"And t2.p0 == parser.vertices[1]"
				"And t2.p1 == parser.vertices[3]"
				"And t2.p2 == parser.vertices[4]")
			{
				REQUIRE(t1->p0 == parser.vertices[1]);
				REQUIRE(t1->p1 == parser.vertices[2]);
				REQUIRE(t1->p2 == parser.vertices[3]);
				REQUIRE(t2->p0 == parser.vertices[1]);
				REQUIRE(t2->p1 == parser.vertices[3]);
				REQUIRE(t2->p2 == parser.vertices[4]);
			}
		}
	}
}

SCENARIO("Converting an OBJ file to a group", "[objfile]")
{
	GIVEN("file = the file triangles.obj"
		"And parser = parseObjFile(file)")
	{
		auto parser = parseObjFile("Assets/Models/triangles.obj");
		WHEN("g = objToGroup(parser)")
		{
			auto g = objToGroup(parser);
			THEN("g includes FirstGroup from parser"
				"And g includes SecondGroup from parser")
			{
				REQUIRE(g->contains(parser.getGroup(0)));
				REQUIRE(g->contains(parser.getGroup(1)));
			}
		}
	}
}

SCENARIO("Vertex normal records", "[objfile]")
{
	GIVEN("file = a file containing:"
		"vn 0 0 1"
		"vn 0.707 0 - 0.707"
		"vn 1 2 3")
	{
		WHEN("parser = parseObjFile(file)")
		{
			auto parser = parseObjFile("Assets/Models/test5.obj");
			THEN("parser.normals[1] == vector(0.0f, 0.0f, 1.0f)"
				"And parser.normals[2] == vector(0.707f, 0.0f, -0.707f)"
				"And parser.normals[3] == vector(1.0f, 2.0f, 3.0f)")
			{
				REQUIRE(parser.normals[1] == vector(0.0f, 0.0f, 1.0f));
				REQUIRE(parser.normals[2] == vector(0.707f, 0.0f, -0.707f));
				REQUIRE(parser.normals[3] == vector(1.0f, 2.0f, 3.0f));
			}
		}
	}
}

SCENARIO("Faces with normals", "[objfile]")
{
	GIVEN("file = a file containing:"
		"v 0 1 0"
		"v -1 0 0"
		"v 1 0 0"
		"vn -1 0 0"
		"vn 1 0 0"
		"vn 0 1 0"
		"f 1//3 2//1 3//2"
		"f 1/0/3 2/102/1 3/14/2")
	{
		WHEN("parser = parseObjFile(file)"
			"And g = parser.defaultGroup"
			"And t1 = first child of g"
			"And t2 = second child of g")
		{
			auto parser = parseObjFile("Assets/Models/test6.obj");
			auto g = parser.defaultGroup;
			auto t1 = std::dynamic_pointer_cast<Triangle>(g->getChild(0));
			auto t2 = std::dynamic_pointer_cast<Triangle>(g->getChild(1));
			THEN("t1.p0 = parser.vertices[1]"
				"And t1.p1 = parser.vertices[2]"
				"And t1.p2 = parser.vertices[3]"
				"And t1.n0 = parser.normals[3]"
				"And t1.n1 = parser.normals[1]"
				"And t1.n2 = parser.normals[2]"
				"And t2 == t1)")
			{
				REQUIRE(t1->p0 == parser.vertices[1]);
				REQUIRE(t1->p1 == parser.vertices[2]);
				REQUIRE(t1->p2 == parser.vertices[3]);
				REQUIRE(t1->n0 == parser.normals[3]);
				REQUIRE(t1->n1 == parser.normals[1]);
				REQUIRE(t1->n2 == parser.normals[2]);
				REQUIRE(t2 == t1);
			}
		}
	}
}