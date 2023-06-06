#pragma once

#include "shape.h"

inline bool boxCompare(const std::shared_ptr<Shape>& a, const std::shared_ptr<Shape>& b, int32_t axis)
{
	AABB boxA;
	AABB boxB;

	if (!a->boundingBox(0, 0, boxA) || !b->boundingBox(0, 0, boxB))
	{
		std::cerr << "No bounding box in BVHNode constructor.\n";
	}

	return boxA.min()[axis] < boxB.min()[axis];
}

inline bool boxXCompare(const std::shared_ptr<Shape>& a, const std::shared_ptr<Shape>& b)
{
	return boxCompare(a, b, 0);
}

inline bool boxYCompare(const std::shared_ptr<Shape>& a, const std::shared_ptr<Shape>& b)
{
	return boxCompare(a, b, 1);
}

inline bool boxZCompare(const std::shared_ptr<Shape>& a, const std::shared_ptr<Shape>& b)
{
	return boxCompare(a, b, 2);
}

class BVHNode : public Shape
{
public:
	BVHNode() = default;
	BVHNode(const std::vector<std::shared_ptr<Shape>>& objects, float time0, float time1)
		: BVHNode(objects, 0, objects.size(), time0, time1)
	{

	}

	BVHNode(const std::vector<std::shared_ptr<Shape>>& objects, size_t start, size_t end, float time0, float time1)
	{
		auto localObjects = objects;

		int32_t axis = randomInt(0, 2);

		auto comparator = (axis == 0) ? boxXCompare
											   : (axis == 1) ? boxYCompare
															 : boxZCompare;

		size_t objectSpan = end - start;

		if (objectSpan == 1)
		{
			left = right = localObjects[start];
		}
		else if (objectSpan == 2)
		{
			if (comparator(localObjects[start], localObjects[start + 1]))
			{
				left = localObjects[start];
				right = localObjects[start + 1];
			}
			else
			{
				left = localObjects[start + 1];
				right = localObjects[start];
			}
		}
		else
		{
			std::sort(localObjects.begin() + start, localObjects.begin() + end, comparator);

			auto mid = start + objectSpan / 2;
			left = std::make_shared<BVHNode>(localObjects, start, mid, time0, time1);
			right = std::make_shared<BVHNode>(localObjects, mid, end, time0, time1);
		}

		AABB boxLeft;
		AABB boxRight;

		if (!left->boundingBox(time0, time1, boxLeft) ||
			!right->boundingBox(time0, time1, boxRight))
		{
			std::cerr << "No bounding box in BVHNode constructor.\n";
		}

		box = surroundingBox(boxLeft, boxRight);
	}

	virtual std::vector<Intersection> localIntersect(const Ray& transformedRay) override
	{
		if (!box.hit(transformedRay, EPSILON, INFINITY))
		{
			return { { 0.0f,  shared_from_this() } };
		}

		auto hitLeft = left->localIntersect(transformedRay);
		auto hitRight = right->localIntersect(transformedRay);

		auto result = hitLeft.empty() ? hitRight : hitLeft;

		return result;
	}

	virtual bool boundingBox(float time0, float time1, AABB& outputBox) override
	{
		outputBox = box;
		return true;
	}

	std::shared_ptr<Shape> left;
	std::shared_ptr<Shape> right;
	AABB box;
};