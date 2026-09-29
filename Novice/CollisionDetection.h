#pragma once
#include "MatrixFunc.h"

Vector3 Project(const Vector3& v1, const Vector3& v2);

Vector3 ClosestPoint(const Vector3& point, const Segment& segment);

bool IsCollisionSphere(const Sphere& s1, const Sphere& s2);

bool IsCollisionPlane(const Sphere& s1, const Plane& plane);

bool IsCollisionSegment(const Segment& segment, const Plane& plane);

bool IsCollisionTriangle(const Triangle& triangle, const Segment& segment);

bool IsCollisionAABB(const AABB& a, const AABB& b);

bool IsCollisionBoxSphere(const AABB& aabb, const Sphere& sphere);

bool IsCollisionBoxLine(const AABB& aabb, const Segment& segment);