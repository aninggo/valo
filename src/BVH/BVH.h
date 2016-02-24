﻿// Copyright © 2015 Mikko Ronkainen <firstname@mikkoronkainen.com>
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

	enum class BVHType { BVH1, BVH4 };

	struct BVHBuildTriangle
	{
		Triangle* triangle;
		Aabb aabb;
		Vector3 center;
	};

	struct BVHSplitCache
	{
		Aabb aabb;
		float cost;
	};

	struct BVHSplitOutput
	{
		uint64_t index;
		uint64_t axis;
		Aabb fullAabb;
		Aabb leftAabb;
		Aabb rightAabb;
	};

	class BVH
	{
	public:

		virtual ~BVH() {}

		virtual void build(std::vector<Triangle>& triangles, uint64_t maxLeafSize) = 0;
		virtual bool intersect(const std::vector<Triangle>& triangles, const Ray& ray, Intersection& intersection) const = 0;

		static std::unique_ptr<BVH> getBVH(BVHType type);

	protected:

		static BVHSplitOutput calculateSplit(std::vector<BVHBuildTriangle>& buildTriangles, std::vector<BVHSplitCache>& cache, uint64_t start, uint64_t end);
	};
}
