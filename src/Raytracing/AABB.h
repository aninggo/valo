// Copyright © 2015 Mikko Ronkainen <firstname@mikkoronkainen.com>
// License: MIT, see the LICENSE file.

#pragma once

#include "Math/Vector3.h"

namespace Raycer
{
	class Ray;

	class AABB
	{
	public:

		AABB(const Vector3& min, const Vector3& max);

		bool intersects(const Ray& ray) const;
		void expand(const AABB& other);

		Vector3 min;
		Vector3 max;
	};
}
