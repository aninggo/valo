// Copyright © 2015 Mikko Ronkainen <firstname@mikkoronkainen.com>
// License: MIT, see the LICENSE file.

#include <algorithm>
#include <cmath>
#include <limits>

#include "Raytracing/Primitives/Triangle.h"
#include "Raytracing/Ray.h"
#include "Raytracing/Intersection.h"
#include "Raytracing/AABB.h"

using namespace Raycer;

void Triangle::initialize()
{
}

// Möller-Trumbore algorithm
// http://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-rendering-a-triangle/moller-trumbore-ray-triangle-intersection
void Triangle::intersect(const Ray& ray, Intersection& intersection) const
{
	Vector3 v0v1 = vertices[1] - vertices[0];
	Vector3 v0v2 = vertices[2] - vertices[0];

	Vector3 pvec = ray.direction.cross(v0v2);
	double determinant = v0v1.dot(pvec);

	// ray and triangle are parallel -> no intersection
	if (fabs(determinant) < std::numeric_limits<double>::epsilon())
		return;

	double invDeterminant = 1.0 / determinant;

	Vector3 tvec = ray.origin - vertices[0];
	double u = tvec.dot(pvec) * invDeterminant;

	if (u < 0.0 || u > 1.0)
		return;

	Vector3 qvec = tvec.cross(v0v1);
	double v = ray.direction.dot(qvec) * invDeterminant;

	if (v < 0.0 || (u + v) > 1.0)
		return;

	double t = v0v2.dot(qvec) * invDeterminant;

	// intersection is behind ray origin
	if (t < 0.0)
		return;

	// another intersection is closer
	if (t > intersection.distance)
		return;

	double w = 1.0 - u - v;

	// order has been swapped uvw -> wuv
	Vector3 interpolatedNormal = w * normals[0] + u * normals[1] + v * normals[2];
	Vector2 interpolatedTexcoord = w * texcoords[0] + u * texcoords[1] + v * texcoords[2];

	intersection.wasFound = true;
	intersection.distance = t;
	intersection.position = ray.origin + (t * ray.direction);
	intersection.normal = interpolatedNormal;
	intersection.texcoord = interpolatedTexcoord;
}

AABB Triangle::getAABB() const
{
	Vector3 min;
	min.x = std::min(vertices[0].x, std::min(vertices[1].x, vertices[2].x));
	min.y = std::min(vertices[0].y, std::min(vertices[1].y, vertices[2].y));
	min.z = std::min(vertices[0].z, std::min(vertices[1].z, vertices[2].z));

	Vector3 max;
	max.x = std::max(vertices[0].x, std::max(vertices[1].x, vertices[2].x));
	max.y = std::max(vertices[0].y, std::max(vertices[1].y, vertices[2].y));
	max.z = std::max(vertices[0].z, std::max(vertices[1].z, vertices[2].z));

	return AABB(min, max);
}
