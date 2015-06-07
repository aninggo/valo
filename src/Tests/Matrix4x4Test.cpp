// Copyright © 2015 Mikko Ronkainen <firstname@mikkoronkainen.com>
// License: MIT, see the LICENSE file.

#include <cmath>

#include "catch/catch.hpp"

#include "Math/Matrix4x4.h"
#include "Math/Vector3.h"

using namespace Raycer;

TEST_CASE("Matrix4x4 functionality", "[matrix4x4]")
{
	Matrix4x4 m1 = Matrix4x4(
		1.0, 2.0, 3.0, 4.0,
		5.0, 6.0, 7.0, 8.0,
		9.0, 1.0, 2.0, 3.0,
		4.0, 5.0, 6.0, 7.0);

	Matrix4x4 m2 = Matrix4x4(
		1.0, 2.0, 3.0, 4.0,
		5.0, 6.0, 7.0, 8.0,
		9.0, 1.0, 2.0, 3.0,
		4.0, 5.0, 6.0, 7.0);

	Matrix4x4 m3 = Matrix4x4(
		9.0, 8.0, 7.0, 6.0,
		5.0, 4.0, 3.0, 2.0,
		1.0, 2.0, 3.0, 4.0,
		5.0, 6.0, 7.0, 8.0);

	Matrix4x4 m4 = Matrix4x4(
		9.0, 8.0, 7.0, 0.0,
		5.0, 4.0, 3.0, 0.0,
		1.0, 2.0, 3.0, 0.0,
		0.0, 0.0, 0.0, 1.0);

	Vector3 v1 = Vector3(1.0, 3.0, 5.0);

	REQUIRE(m1 == m2);
	REQUIRE(m1 != m3);

	REQUIRE((m1 + m2) == Matrix4x4(
		2.0, 4.0, 6.0, 8.0,
		10.0, 12.0, 14.0, 16.0,
		18.0, 2.0, 4.0, 6.0,
		8.0, 10.0, 12.0, 14.0));

	REQUIRE((m1 - m2) == Matrix4x4::ZERO);

	REQUIRE((m1 * 2.0) == Matrix4x4(
		2.0, 4.0, 6.0, 8.0,
		10.0, 12.0, 14.0, 16.0,
		18.0, 2.0, 4.0, 6.0,
		8.0, 10.0, 12.0, 14.0));

	REQUIRE((2.0 * m1) == Matrix4x4(
		2.0, 4.0, 6.0, 8.0,
		10.0, 12.0, 14.0, 16.0,
		18.0, 2.0, 4.0, 6.0,
		8.0, 10.0, 12.0, 14.0));

	REQUIRE((m1 / 2.0) == Matrix4x4(
		0.5, 1.0, 1.5, 2.0,
		2.5, 3.0, 3.5, 4.0,
		4.5, 0.5, 1.0, 1.5,
		2.0, 2.5, 3.0, 3.5));

	REQUIRE((m1 * m3) == Matrix4x4(
		42.0, 46.0, 50.0, 54.0,
		122.0, 126.0, 130.0, 134.0,
		103.0, 98.0, 93.0, 88.0,
		102.0, 106.0, 110.0, 114.0));

	REQUIRE((m1 * m2 * m3) == Matrix4x4(
		1003.0, 1016.0, 1029.0, 1042.0,
		2479.0, 2520.0, 2561.0, 2602.0,
		1012.0, 1054.0, 1096.0, 1138.0,
		2110.0, 2144.0, 2178.0, 2212.0));

	REQUIRE((m4 * v1) == Vector3(68.0, 32.0, 22.0));

	REQUIRE((-m1) == Matrix4x4(
		-1.0, -2.0, -3.0, -4.0,
		-5.0, -6.0, -7.0, -8.0,
		-9.0, -1.0, -2.0, -3.0,
		-4.0, -5.0, -6.0, -7.0));

	REQUIRE(m1.transposed() == Matrix4x4(
		1.0, 5.0, 9.0, 4.0,
		2.0, 6.0, 1.0, 5.0,
		3.0, 7.0, 2.0, 6.0,
		4.0, 8.0, 3.0, 7.0));

	REQUIRE(Matrix4x4::scale(1.0, 2.0, 3.0) == Matrix4x4(
		1.0, 0.0, 0.0, 0.0,
		0.0, 2.0, 0.0, 0.0,
		0.0, 0.0, 3.0, 0.0,
		0.0, 0.0, 0.0, 1.0));

	REQUIRE(Matrix4x4::translate(1.0, 2.0, 3.0) == Matrix4x4(
		1.0, 0.0, 0.0, 1.0,
		0.0, 1.0, 0.0, 2.0,
		0.0, 0.0, 1.0, 3.0,
		0.0, 0.0, 0.0, 1.0));

	REQUIRE((Matrix4x4::translate(1.0, 2.0, 3.0) * Matrix4x4::scale(1.0, 2.0, 3.0)) == Matrix4x4(
		1.0, 0.0, 0.0, 1.0,
		0.0, 2.0, 0.0, 2.0,
		0.0, 0.0, 3.0, 3.0,
		0.0, 0.0, 0.0, 1.0));

	REQUIRE((Matrix4x4::scale(1.0, 2.0, 3.0) * Matrix4x4::translate(1.0, 2.0, 3.0)) == Matrix4x4(
		1.0, 0.0, 0.0, 1.0,
		0.0, 2.0, 0.0, 4.0,
		0.0, 0.0, 3.0, 9.0,
		0.0, 0.0, 0.0, 1.0));

	REQUIRE(Matrix4x4::rotateX(0) == Matrix4x4(
		1.0, 0.0, 0.0, 0.0,
		0.0, 1.0, 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		0.0, 0.0, 0.0, 1.0));

	REQUIRE(Matrix4x4::rotateX(180.0) == Matrix4x4(
		1.0, 0.0, 0.0, 0.0,
		0.0, -1.0, 0.0, 0.0,
		0.0, 0.0, -1.0, 0.0,
		0.0, 0.0, 0.0, 1.0));

	REQUIRE(Matrix4x4::rotateX(90.0) == Matrix4x4(
		1.0, 0.0, 0.0, 0.0,
		0.0, 0.0, -1.0, 0.0,
		0.0, 1.0, 0.0, 0.0,
		0.0, 0.0, 0.0, 1.0));
}
