#pragma once

#include <fstream>
#include <vector>

#include "tuple.h"
#include "group.h"
#include "triangle.h"

struct Parser
{
	Parser()
	{
		defaultGroup = createGroup();
		vertices.emplace_back(point(0.0f, 0.0f, 0.0f));
		normals.emplace_back(vector(0.0f, 0.0f, 0.0f));
		texcoords.emplace_back(vector(0.0f, 0.0f, 0.0f));
	}

	void addGroup(const std::shared_ptr<Group>& group)
	{
		groups.emplace_back(group);
	}

	void addDefaultGroup()
	{
		groups.emplace_back(defaultGroup);
	}

	auto getGroup(int32_t index)
	{
		if (index > groups.size() - 1)
		{
			throw std::invalid_argument("Invalid vector index");
		}
		return groups[index];
	}

	std::vector<tuple> vertices;
	std::vector<tuple> normals;
	std::vector<tuple> texcoords;
	std::shared_ptr<Group> defaultGroup;
	std::vector<std::shared_ptr<Group>> groups;
	int32_t readLineCount = 0;
	int32_t ignoreLineCount = 0;
};

inline static std::vector<std::string> splitString(const std::string& str, char delimiter)
{
	std::vector<std::string> tokens;
	std::istringstream iss(str);
	std::string token;
	while (std::getline(iss, token, delimiter))
		tokens.push_back(token);
	return tokens;
}

inline static std::vector<std::string> splitString(const std::string& str, const std::string& delimiter)
{
	std::vector<std::string> result;
	std::string::size_type start = 0;
	std::string::size_type end = str.find(delimiter);

	while (end != std::string::npos) 
	{
		result.push_back(str.substr(start, end - start));
		start = end + delimiter.length();
		end = str.find(delimiter, start);
	}

	result.push_back(str.substr(start));
	return result;
}

inline static tuple parseVector(const std::string& str)
{
	std::vector<std::string> components = splitString(str, L' ');
	if (components.size() != 2 && components.size() != 3)
		throw std::invalid_argument("Invalid vector format");
	float x = std::stof(components[0]);
	float y = std::stof(components[1]);
	float z = 0.0f;

	if (components.size() > 2)
	{
		z = std::stof(components[2]);
	}
	return { x, y, z, 1.0f };
}

inline static std::vector<uint32_t> parseIndices(const std::string& str)
{
	std::vector<std::string> components = splitString(str, L' ');

	std::vector<uint32_t> indices;

	for (const auto& c : components)
	{
		indices.push_back(std::stoi(c));
	}

	return indices;
}

inline static std::vector<uint32_t> parseIndices(const std::vector<std::string>& strArray)
{
	std::vector<uint32_t> indices;

	for (const auto& s : strArray)
	{
		if (s.empty())
		{
			indices.push_back(0);
		}
		else
		{
			indices.push_back(std::stoi(s));
		}
	}

	return indices;
}

inline static auto fanTriangulation(const std::vector<tuple>& vertices, const std::vector<tuple>& normals = {}, const std::vector<tuple>& texcoords = {})
{
	std::vector<std::shared_ptr<Shape>> triangls;
	
	for (size_t index = 2; index < vertices.size() - 1; index++)
	{
		std::shared_ptr<Shape> triangle;
		if (normals.empty())
		{
			triangle = createTriangle(vertices[1], vertices[index], vertices[index + 1]);

		}
		else
		{
			triangle = createSmoothTriangle(vertices[1], vertices[index], vertices[index + 1],
											 normals[1], normals[index], normals[index + 1],
											 texcoords[1], texcoords[index], texcoords[index + 1]);
		}
		triangls.emplace_back(triangle);
	}

	return triangls;
}

static Parser parseObjFile(const std::string& path)
{
	Parser parser;

	std::ifstream file(path);

	std::shared_ptr<Group> group;
	
	if (file.is_open())
	{
		while (file.good())
		{
			std::string line;
			std::getline(file, line);

			if (line[0] != 'v' && line[0] != 'f' && line[0] != 'g')
			{
				parser.ignoreLineCount++;
				continue;
			}

			if (line[0] == 'v')
			{
				if (line[1] == 'n')
				{
					line = line.substr(3);
					auto normal = parseVector(line);
					normal.w = 0.0f;
					parser.normals.emplace_back(normal);
					continue;
				}

				if (line[1] == 't')
				{
					line = line.substr(3);
					auto texcoord = parseVector(line);
					parser.texcoords.emplace_back(texcoord);
					continue;
				}

				line = line.substr(2);
				auto position = parseVector(line);
				parser.vertices.emplace_back(position);

				continue;
			}

			if (line[0] == 'f')
			{
				line = line.substr(2);

				auto findDoubleSlash = line.find("//");
				auto findSlash = line.find("/");

				if (findSlash != std::string::npos || findDoubleSlash != std::string::npos)
				{
					// 1//3 2//1 3//2 ->
					// {1//3, 2//1, 3//2}
					auto faceIndices = splitString(line, " ");
					// 1//3 ->
					// {1, 3}
					std::vector<std::vector<std::string>> indices;
					for (size_t i = 0; i < faceIndices.size(); i++)
					{
						indices.emplace_back(splitString(faceIndices[i], "/"));
					}

					std::vector<uint32_t> vertexIndices;
					std::vector<uint32_t> normalIndices;
					std::vector<uint32_t> texcoordIndices;
					std::vector<tuple> vertices(1);
					std::vector<tuple> normals(1);
					std::vector<tuple> texcoords(1);

					for (size_t i = 0; i < indices.size(); i++)
					{
						auto index = parseIndices(indices[i]);
						vertexIndices.emplace_back(index[0]);
						texcoordIndices.emplace_back(index[1]);
						normalIndices.emplace_back(index[2]);

						auto position = parser.vertices[vertexIndices[i]];
						vertices.emplace_back(position);

						auto normal = parser.normals[normalIndices[i]];
						normals.emplace_back(normal);

						auto texcoord = parser.texcoords[texcoordIndices[i]];
						texcoords.emplace_back(texcoord);
					}

					if (vertices.size() > 4)
					{
						auto triangles = fanTriangulation(vertices, normals, texcoords);

						parser.defaultGroup->addChildren(triangles);

						if (group != nullptr)
						{
							group->addChildren(triangles);
						}

						continue;
					}

					auto triangle = createSmoothTriangle(vertices[1], vertices[2], vertices[3], 
																			normals[1], normals[2], normals[3],
																			texcoords[1], texcoords[2], texcoords[3]);

					parser.defaultGroup->addChild(triangle);

					if (group != nullptr)
					{
						group->addChild(triangle);
					}

					continue;
				}

				auto indices = parseIndices(line);

				if (indices.size() == 3)
				{
					auto p0 = parser.vertices[indices[0]];
					auto p1 = parser.vertices[indices[1]];
					auto p2 = parser.vertices[indices[2]];
					auto triangle = createTriangle(p0, p1, p2);
					parser.defaultGroup->addChild(triangle);

					if (group != nullptr)
					{
						group->addChild(triangle);
					}
				}
				else
				{
					std::vector<tuple> vertices;
					vertices.emplace_back(point(0.0f, 0.0f, 0.0f));
					for (size_t i = 0; i < indices.size(); i++)
					{
						vertices.emplace_back(parser.vertices[indices[i]]);
					}
					auto triangles = fanTriangulation(vertices);

					parser.defaultGroup->addChildren(triangles);

					if (group != nullptr)
					{
						group->addChildren(triangles);
					}
				}

				continue;
			}

			if (line[0] == 'g')
			{
				group = createGroup();
				parser.addGroup(group);
				continue;
			}
		}
	}

	if (parser.groups.empty())
	{
		parser.addDefaultGroup();
	}

	return parser;
}

inline static auto objToGroup(const Parser& parser)
{
	auto group = createGroup();

	if (parser.groups.size() == 1)
	{
		group = parser.defaultGroup;
		return group;
	}
	
	for (const auto& g : parser.groups)
	{
		group->addChild(g);
	}

	return group;
}