// Copyright © 2015 Mikko Ronkainen <firstname@mikkoronkainen.com>
// License: MIT, see the LICENSE file.

#pragma once

#include "Math/Vector3.h"

/*

OrthoNormal Basis

left-handed coordinate system
-z is towards the monitor

u = right
v = up
w = forward (normal)

*/

namespace Raycer
{
	class Matrix4x4;

	class ONB
	{
	public:

		ONB();
		ONB(const Vector3& u, const Vector3& v, const Vector3& w);

		ONB transformed(const Matrix4x4& tranformation) const;

		static ONB fromNormal(const Vector3& normal, const Vector3& up = Vector3::ALMOST_UP);

		static const ONB UP;

		Vector3 u = Vector3(0.0, 0.0, 0.0);
		Vector3 v = Vector3(0.0, 0.0, 0.0);
		Vector3 w = Vector3(0.0, 0.0, 0.0);
	};
}
