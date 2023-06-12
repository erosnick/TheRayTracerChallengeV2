#pragma once

#include "shape.h"

#include "cube.h"

class Group : public Shape
{
public:
	Group() = default;

	virtual void setTransform(const matrix4& inTransform) override
	{
		Shape::setTransform(inTransform);

		auto minX = aabb.min.x;
		auto minY = aabb.min.y;
		auto minZ = aabb.min.z;

		auto maxX = aabb.max.x;
		auto maxY = aabb.max.y;
		auto maxZ = aabb.max.z;

		std::vector<tuple> cornerPoints =
		{
			{ minX, minY, minZ, 1.0 },
			{ minX, minY, maxZ, 1.0 },
			{ minX, maxY, minZ, 1.0 },
			{ minX, maxY, maxZ, 1.0 },
			{ maxX, minY, minZ, 1.0 },
			{ maxX, minY, maxZ, 1.0 },
			{ maxX, maxY, minZ, 1.0 },
			{ maxX, maxY, maxZ, 1.0 }
		};

		// Theoretically, the AABB of the Group only needs to be scaled and rotated, 
		// and should not be translated, because based on the current implementation 
		// the ray will be transformed to the space where the Group is located, in fact 
		// the Group itself is always located at the origin of the Object space, scaling,
		// rotation, and translation are all for the ray.
		std::for_each(cornerPoints.begin(), cornerPoints.end(),
		[&](tuple& point)
		{
			point = rotateZ(rotation.z) * 
					rotateY(rotation.y) * 
				    rotateX(rotation.x) * scale * point;
		});

		tuple min = point(std::numeric_limits<float>::max());
		tuple max = point(-std::numeric_limits<float>::max());

		for (const auto& point : cornerPoints)
		{
			min.x = std::min(min.x, point.x);
			min.y = std::min(min.y, point.y);
			min.z = std::min(min.z, point.z);

			max.x = std::max(max.x, point.x);
			max.y = std::max(max.y, point.y);
			max.z = std::max(max.z, point.z);
		}

		aabb = BoundingBox(min, max);

		auto scaleX = (aabb.max.x - aabb.min.x) / scale.x;
		auto scaleY = (aabb.max.y - aabb.min.y) / scale.y;
		auto scaleZ = (aabb.max.z - aabb.min.z) / scale.z;

		cube = createCube(scaleX * 0.5f);
		cube->material = Materials::DarkRed;
		cube->material.refractiveIndex = 1.0f;
		cube->material.transparency = 1.0f;

		shapes.emplace_back(cube);
	}

	virtual std::vector<Intersection> localIntersect(const Ray& transformedRay) override 
	{ 
		if (shapes.empty())
		{
			return {};
		}

		if (!aabb.hit(transformedRay))
		{
			//std::cout << "Miss" << std::endl;
			return {};
		}

		//auto aabbResult = cube->localIntersect(transformedRay);

		//if (aabbResult.empty())
		//{
			//return {};
		//}

		std::vector<Intersection> result;

		for (const auto& shape : shapes)
		{
			auto intersections = shape->intersect(transformedRay);
			result.insert(result.end(), intersections.begin(), intersections.end());
		}

		sortIntersections(result);

		return result;
	}

	virtual tuple localNormalAt(const tuple& localPosition, const Intersection intersection = {}) const override
	{ 
		return {}; 
	}

	virtual bool boundingBox(BoundingBox& outputBox) override
	{
		outputBox = aabb;

		return true;
	}

	void addChild(const std::shared_ptr<Shape>& shape)
	{
		auto child = shape;
		child->parent = shared_from_this();
		shapes.emplace_back(child);

		BoundingBox box;
		child->boundingBox(box);

		aabb = surroundingBox(aabb, box);
	}

	void addChildren(const std::vector<std::shared_ptr<Shape>>& inShapes)
	{
		for (const auto& shape : inShapes)
		{
			addChild(shape);
		}

		BoundingBox box;
		boundingBoxOfShapes(inShapes, box);

		aabb = surroundingBox(aabb, box);
	}

	auto getChild(int32_t index)
	{
		if (index > shapes.size() - 1)
		{
			throw std::invalid_argument("Invalid vector index");
		}
		return shapes[index];
	}

	virtual bool contains(const std::shared_ptr<Shape>& shape) override
	{
		return std::find(shapes.begin(), 
						  shapes.end(), 
						   shape) != shapes.end();
	}

	bool isEmpty() const { return shapes.empty(); }

	std::vector<std::shared_ptr<Shape>> shapes;

	std::shared_ptr<Shape> cube;
};

std::shared_ptr<Group> createGroup()
{
	return std::make_shared<Group>();
}