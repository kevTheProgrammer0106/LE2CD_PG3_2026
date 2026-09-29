#include "CollisionDetection.h"

#include <Novice.h>
#define _USE_MATH_DEFINES
#include<math.h>
#include <cmath>
#include <assert.h>
#include <algorithm>

Vector3 Project(const Vector3& v1, const Vector3& v2)
{
	float dotProduct = Dot(v1, v2);
	float bLSquared = Dot(v2, v2);  // |b|²
	assert(bLSquared != 0.0f);
	float scale = dotProduct / bLSquared;
	return { scale * v2.x, scale * v2.y, scale * v2.z };
}

Vector3 ClosestPoint(const Vector3& point, const Segment& segment)
{
	Vector3 a = Subtract(point, segment.origin);
	float t = Dot(a, segment.diff) / Dot(segment.diff, segment.diff);
	t = std::clamp(t, 0.0f, 1.0f);  // clamp to [0,1] for segment
	return {
		segment.origin.x + t * segment.diff.x,
		segment.origin.y + t * segment.diff.y,
		segment.origin.z + t * segment.diff.z
	};
}

bool IsCollisionSphere(const Sphere& s1, const Sphere& s2)
{
	// 2つの球の中心点問の距離を求める
	float distance = Length(s2.center - s1.center);
	// 半径の合計よりも短ければ衝突
	if (distance <= s1.radius + s2.radius)
	{
		return true;
	}
	return false;
}
bool IsCollisionPlane(const Sphere& s1, const Plane& plane)
{
	// 球の中心と平面との距離（符号付き）
	float distance = Dot(plane.normal, s1.center) - plane.distance;
	// 距離の絶対値が半径以下なら衝突
	return std::fabsf(distance) <= s1.radius;
}
bool IsCollisionSegment(const Segment& segment, const Plane& plane)
{
	// まず垂直判定を行うために、法線と線の内積を求める
	float dot = Dot(plane.normal, segment.diff);

	// 垂直 = 平行であるので、衝突しているはずがない
	if (dot == 0.0f)
	{
		return false;
	}
	// tを求める
	float t = (plane.distance - Dot(segment.origin, plane.normal)) / dot;
	if (t >= 0.0f && t <= 1.0f)
	{
		return true;
	}
	return false;
}

bool IsCollisionTriangle(const Triangle& triangle, const Segment& segment)
{
	// 三角形の法線を求める（2辺の外積）
	Vector3 edge1 = Subtract(triangle.vertices[1], triangle.vertices[0]);
	Vector3 edge2 = Subtract(triangle.vertices[2], triangle.vertices[0]);
	Vector3 normal = Normalize(Cross(edge1, edge2));

	// 法線と線分の方向ベクトルの内積（垂直＝平行なら衝突しない）
	float dot = Dot(normal, segment.diff);
	if (dot == 0.0f)
	{
		return false;
	}

	// 平面とのtを求める（vertices[0]を平面上の点として使用）
	float t = Dot(Subtract(triangle.vertices[0], segment.origin), normal) / dot;

	// tが[0,1]の範囲外なら、線分は平面と交差しない
	if (t < 0.0f || t > 1.0f)
	{
		return false;
	}

	// 交点pを求める
	Vector3 p = Add(segment.origin, Multiply(t, segment.diff));

	// pが三角形の内側にあるかを、各辺との外積の向きで判定する
	Vector3 v01 = Subtract(triangle.vertices[1], triangle.vertices[0]);
	Vector3 v1p = Subtract(p, triangle.vertices[1]);
	Vector3 cross01 = Cross(v01, v1p);

	Vector3 v12 = Subtract(triangle.vertices[2], triangle.vertices[1]);
	Vector3 v2p = Subtract(p, triangle.vertices[2]);
	Vector3 cross12 = Cross(v12, v2p);

	Vector3 v20 = Subtract(triangle.vertices[0], triangle.vertices[2]);
	Vector3 v0p = Subtract(p, triangle.vertices[0]);
	Vector3 cross20 = Cross(v20, v0p);

	// 全て同じ方向を向いていれば、pは三角形の内側
	if (Dot(cross01, normal) >= 0.0f &&
		Dot(cross12, normal) >= 0.0f &&
		Dot(cross20, normal) >= 0.0f)
	{
		return true;
	}
	return false;
}

bool IsCollisionAABB(const AABB& a, const AABB& b)
{
	if ((a.min.x <= b.max.x && a.max.x >= b.min.x) && // x軸
		(a.min.y <= b.max.y && a.max.y >= b.min.y) && // y軸
		(a.min.z <= b.max.z && a.max.z >= b.min.z)) // // z軸
	{
		return true;
	}
	return false;
}
bool IsCollisionBoxSphere(const AABB& aabb, const Sphere& sphere)
{
	//最近接点を求める
	Vector3 closestPoint = {
	std::clamp(sphere.center.x, aabb.min.x, aabb.max.x),
	std::clamp(sphere.center.y, aabb.min.y, aabb.max.y),
	std::clamp(sphere.center.z, aabb.min.z, aabb.max.z)
	};
	// 最近接点と球の中心との距離を求める
	float distance = Length(closestPoint - sphere.center);
	// 距離が半径よりも小さいければ衝突
	if (distance <= sphere.radius)
	{
		return true;
	}
	return false;
}

bool IsCollisionBoxLine(const AABB& aabb, const Segment& segment)
{
	// X
	float tNearX = (aabb.min.x - segment.origin.x) / segment.diff.x;
	float tFarX = (aabb.max.x - segment.origin.x) / segment.diff.x;
	if (tNearX > tFarX)
	{
		float temp = tNearX;
		tNearX = tFarX;
		tFarX = temp;
	}

	// Y
	float tNearY = (aabb.min.y - segment.origin.y) / segment.diff.y;
	float tFarY = (aabb.max.y - segment.origin.y) / segment.diff.y;
	if (tNearY > tFarY)
	{
		float temp = tNearY;
		tNearY = tFarY;
		tFarY = temp;
	}

	// Z
	float tNearZ = (aabb.min.z - segment.origin.z) / segment.diff.z;
	float tFarZ = (aabb.max.z - segment.origin.z) / segment.diff.z;
	if (tNearZ > tFarZ)
	{
		float temp = tNearZ;
		tNearZ = tFarZ;
		tFarZ = temp;
	}

	// AABBとの衝突点 (貫通点)のtが小さい方
	float tmin = max(max(tNearX, tNearY), tNearZ);

	// AABBとの衝突点 (貫通点)のtが大きい方
	float tmax = min(min(tFarX, tFarY), tFarZ);

	if (tmin <= tmax && tmax >= 0.0f && tmin <= 1.0f)
	{
		return true;
	}
	return false;
}