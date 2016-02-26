// Copyright © 2016 Mikko Ronkainen <firstname@mikkoronkainen.com>
// License: MIT, see the LICENSE file.

#include "Precompiled.h"

#include "Math/EulerAngle.h"
#include "Math/Vector3.h"
#include "Math/MathUtils.h"

using namespace Raycer;

EulerAngle::EulerAngle(float pitch_, float yaw_, float roll_) : pitch(pitch_), yaw(yaw_), roll(roll_)
{
}

namespace Raycer
{
	EulerAngle operator+(const EulerAngle& e1, const EulerAngle& e2)
	{
		return EulerAngle(e1.pitch + e2.pitch, e1.yaw + e2.yaw, e1.roll + e2.roll);
	}

	EulerAngle operator-(const EulerAngle& e1, const EulerAngle& e2)
	{
		return EulerAngle(e1.pitch - e2.pitch, e1.yaw - e2.yaw, e1.roll - e2.roll);
	}

	EulerAngle operator*(const EulerAngle& e, float s)
	{
		return EulerAngle(e.pitch * s, e.yaw * s, e.roll * s);
	}

	EulerAngle operator*(float s, const EulerAngle& e)
	{
		return EulerAngle(e.pitch * s, e.yaw * s, e.roll * s);
	}

	EulerAngle operator-(const EulerAngle& e)
	{
		return EulerAngle(-e.pitch, -e.yaw, -e.roll);
	}
}

EulerAngle& EulerAngle::operator+=(const EulerAngle& e)
{
	*this = *this + e;
	return *this;
}

EulerAngle& EulerAngle::operator-=(const EulerAngle& e)
{
	*this = *this - e;
	return *this;
}

void EulerAngle::clampPitch()
{
	if (pitch > 89.0f)
		pitch = 89.0f;

	if (pitch < -89.0f)
		pitch = -89.0f;
}

void EulerAngle::normalize()
{
	while (std::abs(pitch) > 180.0f)
		pitch += (pitch > 0.0f) ? -360.0f : 360.0f;

	while (std::abs(yaw) > 180.0f)
		yaw += (yaw > 0.0f) ? -360.0f : 360.0f;

	while (std::abs(roll) > 180.0f)
		roll += (roll > 0.0f) ? -360.0f : 360.0f;
}

Vector3 EulerAngle::getDirection() const
{
	Vector3 result;

	// is [0 0 -1] when angles are zero
	result.x = -std::sin(MathUtils::degToRad(yaw)) * std::cos(MathUtils::degToRad(pitch));
	result.y = std::sin(MathUtils::degToRad(pitch));
	result.z = -std::cos(MathUtils::degToRad(yaw)) * std::cos(MathUtils::degToRad(pitch));

	return result.normalized();
}

EulerAngle EulerAngle::lerp(const EulerAngle& e1, const EulerAngle& e2, float t)
{
	EulerAngle result;
	float oneMinusT = 1.0f - t;

	result.pitch = e1.pitch * oneMinusT + e2.pitch * t;
	result.yaw = e1.yaw * oneMinusT + e2.yaw * t;
	result.roll = e1.roll * oneMinusT + e2.roll * t;

	return result;
}
