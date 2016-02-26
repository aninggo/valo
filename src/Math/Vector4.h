// Copyright © 2016 Mikko Ronkainen <firstname@mikkoronkainen.com>
// License: MIT, see the LICENSE file.

#pragma once

#include "cereal/cereal.hpp"

namespace Raycer
{
	class Vector3;

	class Vector4
	{
	public:

		explicit Vector4(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 0.0f);
		explicit Vector4(const Vector3& v, float w = 0.0f);

		friend Vector4 operator+(const Vector4& v, const Vector4& w);
		friend Vector4 operator-(const Vector4& v, const Vector4& w);
		friend Vector4 operator*(const Vector4& v, const Vector4& w);
		friend Vector4 operator*(const Vector4& v, float s);
		friend Vector4 operator*(float s, const Vector4& v);
		friend Vector4 operator/(const Vector4& v, const Vector4& w);
		friend Vector4 operator/(const Vector4& v, float s);
		friend Vector4 operator-(const Vector4& v);

		friend bool operator==(const Vector4& v, const Vector4& w);
		friend bool operator!=(const Vector4& v, const Vector4& w);
		friend bool operator>(const Vector4& v, const Vector4& w);
		friend bool operator<(const Vector4& v, const Vector4& w);

		Vector4& operator+=(const Vector4& v);
		Vector4& operator-=(const Vector4& v);
		Vector4& operator*=(const Vector4& v);
		Vector4& operator*=(float s);
		Vector4& operator/=(const Vector4& v);
		Vector4& operator/=(float s);
		float operator[](uint64_t index) const;

		float getElement(uint64_t index) const;
		void setElement(uint64_t index, float value);
		float length() const;
		float lengthSquared() const;
		void normalizeLength();
		Vector4 normalizedLength() const;
		void normalizeForm();
		Vector4 normalizedForm() const;
		void inverse();
		Vector4 inversed() const;
		bool isZero() const;
		bool isNan() const;
		bool isNormal() const;
		float dot(const Vector4& v) const;
		std::string toString() const;
		Vector3 toVector3() const;

		static Vector4 lerp(const Vector4& v1, const Vector4& v2, float t);
		static Vector4 abs(const Vector4& v);

		float x;
		float y;
		float z;
		float w;

	private:

		friend class cereal::access;

		template <class Archive>
		void serialize(Archive& ar)
		{
			ar(CEREAL_NVP(x),
				CEREAL_NVP(y),
				CEREAL_NVP(z),
				CEREAL_NVP(w));
		}
	};
}
