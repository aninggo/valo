// Copyright © 2016 Mikko Ronkainen <firstname@mikkoronkainen.com>
// License: MIT, see the LICENSE file.

#pragma once

#include "Core/Common.h"
#include "Math/Vector2.h"

namespace Raycer
{
	class Vector2;

	class TentFilter
	{
	public:

		CUDA_CALLABLE float getWeight(float s);
		CUDA_CALLABLE float getWeight(const Vector2& point);

		CUDA_CALLABLE Vector2 getRadius();

		Vector2 radius = Vector2(1.0f, 1.0f);
	};
}
