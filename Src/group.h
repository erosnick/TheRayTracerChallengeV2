#pragma once

#include "shape.h"

class Group : public Shape
{
public:
	Group() = default;

	virtual std::vector<Intersection> localIntersect(const Ray& transformedRay) override 
	{ 
		if (shapes.empty())
		{
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

	virtual tuple localNormalAt(const tuple& localPosition) const override
	{ 
		return {}; 
	}

	virtual bool boundingBox(float time0, float time1, AABB& outputBox) override
	{
		return true;
	}

	void addChild(const std::shared_ptr<Shape>& shape)
	{
		auto child = shape;
		child->parent = shared_from_this();
		shapes.emplace_back(child);
	}

	bool isEmpty() const { return shapes.empty(); }

	std::vector<std::shared_ptr<Shape>> shapes;
};

std::shared_ptr<Group> createGroup()
{
	return std::make_shared<Group>();
}