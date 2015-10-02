// Copyright © 2015 Mikko Ronkainen <firstname@mikkoronkainen.com>
// License: MIT, see the LICENSE file.

#pragma once

#include <vector>

#include "cereal/cereal.hpp"

#include "Raytracing/Primitives/Primitive.h"
#include "Math/Vector3.h"

namespace Raycer
{
	class Ray;
	struct Intersection;
	class AABB;
	class EulerAngle;

	struct BlinnBlobDescription
	{
		Vector3 position;
		double radius = 1.0;
		double blobbiness = 4.0;
		bool isNegative = false;

		template <class Archive>
		void serialize(Archive& ar)
		{
			ar(CEREAL_NVP(position),
				CEREAL_NVP(radius),
				CEREAL_NVP(blobbiness),
				CEREAL_NVP(isNegative));
		}
	};

	class BlinnBlob : public Primitive
	{
	public:

		friend class Scene;
		friend class CLScene;

		void initialize(const Scene& scene);
		bool intersect(const Ray& ray, Intersection& intersection, std::vector<Intersection>& intersections);
		AABB getAABB() const;
		void transform(const Vector3& scale, const EulerAngle& rotate, const Vector3& translate);

		std::vector<BlinnBlobDescription> blobs;
		int solverIterations = 16;

	private:

		friend class cereal::access;

		template <class Archive>
		void serialize(Archive& ar)
		{
			ar(cereal::make_nvp("primitive", cereal::base_class<Primitive>(this)),
				CEREAL_NVP(blobs),
				CEREAL_NVP(solverIterations));
		}
	};
}
