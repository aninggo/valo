﻿// Copyright © 2016 Mikko Ronkainen <firstname@mikkoronkainen.com>
// License: MIT, see the LICENSE file.

#pragma once

#include "Tracers/Tracer.h"
#include "Samplers/RandomSampler.h"

namespace Raycer
{
	class Intersection;

	class PathtracerRecursive : public Tracer
	{
	public:

		uint64_t getPixelSampleCount(const Scene& scene) const override;
		uint64_t getSamplesPerPixel(const Scene& scene) const override;

	protected:

		void trace(const Scene& scene, Film& film, const Vector2& pixelCenter, uint64_t pixelIndex, Random& random, uint64_t& rayCount, uint64_t& pathCount) override;

	private:

		Color traceRecursive(const Scene& scene, const Ray& ray, Random& random, uint64_t depth, uint64_t& pathCount);
		Color calculateIndirectLight(const Scene& scene, const Intersection& intersection, Random& random, uint64_t depth, uint64_t& pathCount);

		RandomSampler sampler;
	};
}
