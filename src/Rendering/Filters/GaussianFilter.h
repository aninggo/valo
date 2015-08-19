// Copyright © 2015 Mikko Ronkainen <firstname@mikkoronkainen.com>
// License: MIT, see the LICENSE file.

#pragma once

#include "Rendering/Filters/Filter.h"

namespace Raycer
{
	class GaussianFilter : public Filter
	{
	public:

		GaussianFilter(double alpha = 4.0);

		double getWeight(double x);
		double getWeight(double x, double y);
		double getWeight(const Vector2& point);

	private:

		double alpha;
	};
}