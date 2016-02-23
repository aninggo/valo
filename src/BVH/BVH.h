// Copyright © 2015 Mikko Ronkainen <firstname@mikkoronkainen.com>
// License: MIT, see the LICENSE file.

#pragma once

#include <memory>
#include <vector>

#include "Tracing/AABB.h"

namespace Raycer
{
	class Triangle;
	class Ray;
	class Intersection;

	enum class BVHType { BVH1, BVH4, BVH8, SBVH1 };

	struct BVHSplitInput
	{
		std::vector<Triangle*>* trianglePtrs;
		std::vector<float>* rightScores;
		uint64_t start;
		uint64_t end;
	};

	struct BVHSplitOutput
	{
		uint64_t index = 0;
		uint64_t axis = 0;
		AABB leftAABB;
		AABB rightAABB;
		bool failed = false;
	};

	class BVH
	{
	public:

		virtual ~BVH() {}

		virtual void build(std::vector<Triangle>& triangles, uint64_t maxLeafSize) = 0;
		virtual bool intersect(const std::vector<Triangle>& triangles, const Ray& ray, Intersection& intersection) const = 0;

		static std::unique_ptr<BVH> getBVH(BVHType type);

	protected:

		static BVHSplitOutput calculateSplit(const BVHSplitInput& input);
	};
}
