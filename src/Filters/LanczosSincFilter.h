// Copyright © 2015 Mikko Ronkainen <firstname@mikkoronkainen.com>
// License: MIT, see the LICENSE file.

#pragma once

#include "Filters/Filter.h"

namespace Raycer
{
	class Vector2;

	class LanczosSincFilter : public Filter
	{
	public:

		explicit LanczosSincFilter(uint64_t radiusX = 2, uint64_t radiusY = 2);

		void setRadius(uint64_t radiusX, uint64_t radiusY);

		float getWeightX(float x) override;
		float getWeightY(float y) override;
	};
}
