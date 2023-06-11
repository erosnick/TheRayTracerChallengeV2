#pragma once

#include "shape.h"

class Group : public Shape
{
public:
	Group() = default;

	virtual void setTransform(const matrix4& inTransform) override
	{
		Shape::setTransform(inTransform);

		//aabb.min = transform * aabb.min;
		//aabb.max = transform * aabb.max;
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
};

std::shared_ptr<Group> createGroup()
{
	return std::make_shared<Group>();
}