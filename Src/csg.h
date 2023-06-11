#pragma once

#include "shape.h"

enum class Operation : uint8_t
{
	Union,
	Intersection,
	Difference
};

inline static bool intersectionAllowed(Operation operation, bool leftHit, bool inLeft, bool inRight)
{
	if (operation == Operation::Union)
	{
		return (leftHit && !inRight) || (!leftHit && !inLeft);
	}
	else if (operation == Operation::Intersection)
	{
		return (leftHit && inRight) || (!leftHit && inLeft);
	}
	else if (operation == Operation::Difference)
	{
		return (leftHit && !inRight) || (!leftHit && inLeft);
	}
	return false;
}

class CSG : public Shape
{
public:
	CSG(Operation inOperation, const std::shared_ptr<Shape>& inLeft, const std::shared_ptr<Shape>& inRight)
	: operation(inOperation), left(inLeft), right(inRight)
	{
	}

	virtual void setTransform(const matrix4& inTransform) override
	{
		Shape::setTransform(inTransform);
	}

	virtual std::vector<Intersection> localIntersect(const Ray& transformedRay) override
	{
		auto leftIntersections = left->intersect(transformedRay);
		auto rightIntersections = right->intersect(transformedRay);

		std::vector<Intersection> combinedIntersections;

		combinedIntersections.insert(combinedIntersections.end(), leftIntersections.begin(), leftIntersections.end());
		combinedIntersections.insert(combinedIntersections.end(), rightIntersections.begin(), rightIntersections.end());

		sortIntersections(combinedIntersections);

		return filterIntersections(combinedIntersections);
	}

	virtual bool boundingBox(BoundingBox& outputBox) override
	{
		return true;
	}

	virtual bool contains(const std::shared_ptr<Shape>& shape) override
	{
		return left->contains(shape) || right->contains(shape);
	}

	std::vector<Intersection> filterIntersections(const std::vector<Intersection>& intersections)
	{
		// Begin outside of both children
		auto inLeft = false;
		auto inRight = false;

		// Prepare a list to receive the filtered intersections
		std::vector<Intersection> result;

		for (const auto& intersection : intersections)
		{
			// If intersection.shape is part of the "left" child, then leftHit is true
			auto leftHit = left->contains(intersection.shape);

			if (intersectionAllowed(operation, leftHit, inLeft, inRight))
			{
				result.emplace_back(intersection);
			}

			// Depending on which object was hit, toggle either inLeft or inRight
			if (leftHit)
			{
				inLeft = !inLeft;
			}
			else
			{
				inRight = !inRight;
			}
		}

		return result;
	}

	Operation operation;
	std::shared_ptr<Shape> left;
	std::shared_ptr<Shape> right;
};

std::shared_ptr<CSG> createCSG(Operation operation, const std::shared_ptr<Shape>& left, const std::shared_ptr<Shape>& right)
{
	auto csg = std::make_shared<CSG>(operation, left, right);

	// Important!
	// If you forget to set it, when the CSG itself sets the transformation causes the 
	// left and right sub-object normals to be calculated incorrectly.
	left->parent = csg;
	right->parent = csg;
	return csg;
}