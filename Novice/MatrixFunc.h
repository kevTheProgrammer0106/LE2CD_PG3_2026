#pragma once
#include <cstdint>
#include <cmath>

struct Vector2 {
	float x, y;

	// 複合代入演算子 (Compound Assignment Operator Vector3)
	Vector2& operator*=(float s) {
		x *= s;
		y *= s;
		return *this;
	};
	Vector2& operator-=(const Vector2& v) {
		x -= v.x;
		y -= v.y;
		return *this;
	};
	Vector2& operator+=(const Vector2& v) {
		x += v.x;
		y += v.y;
		return *this;
	};
	Vector2& operator/=(float s) {
		x /= s;
		y /= s;
		return *this;
	};
};


struct Vector3 {
	float x, y, z;

	// 複合代入演算子 (Compound Assignment Operator Vector3)
	Vector3& operator*=(float s) 
	{ 
		x *= s; 
		y *= s;
		z *= s;
		return *this;
	};
	Vector3& operator-=(const Vector3& v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	};
	Vector3& operator+=(const Vector3& v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	};
	Vector3& operator/=(float s)
	{
		x /= s;
		y /= s;
		z /= s;
		return *this;
	};
};

struct Matrix4x4 {
	float m[4][4];
};

struct ObjectParameters {
	Vector3 scale;
	Vector3 rotate;
	Vector3 translate;
};

struct Sphere {
	Vector3 center; //!< 中心点
	float radius; //!< 半径
};

struct Plane {
	Vector3 normal; //!< 法線
	float distance; //!< 距離
};

struct Triangle {
	Vector3 vertices[3]; //!< 頂点
};

struct Segment {
	Vector3 origin; // !< 始点
	Vector3 diff; // !< 終点への差分ベクトル
};

struct AABB {
	Vector3 min; //!< 最小点
	Vector3 max; //!< 最大点
};

//  contangent関数
float cotf(float value);

// ベクトルの加算
Vector3 Add(const Vector3& v1, const Vector3& v2);
Matrix4x4 Add(const Matrix4x4& m1, const Matrix4x4& m2);

// ベクトルの減算
Vector3 Subtract(const Vector3& v1, const Vector3& v2);
Matrix4x4 Subtract(const Matrix4x4& m1, const Matrix4x4& m2);

Vector3 Multiply(const Vector3& v1, const Vector3& v2);
Vector3 Multiply(const float& v1, const Vector3& v2);
// 行列のスカラー倍
Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);

// 二頂点演算子(Binary Operator Vector3)
inline Vector3 operator+(const Vector3& v1, const Vector3& v2)
{
	return Add(v1, v2);
}
inline Vector3 operator-(const Vector3& v1, const Vector3& v2)
{
	return Subtract(v1, v2);
}
inline Vector3 operator*(float s, const Vector3& v)
{
	return Multiply(s, v);
}
inline Vector3 operator*(const Vector3& v, float s)
{
	return s * v;
}
inline Vector3 operator/(const Vector3& v, float s)
{
	return Multiply(1.0f / s, v);
}

// 単頂点演算子(Unary Operator Vector3)
inline Vector3 operator-(const Vector3& v)
{
	return { -v.x, -v.y, -v.z };
}
inline Vector3 operator+(const Vector3& v)
{
	return v;
}

// 二頂点演算子(Binary Operator Matrix4x4)
inline Matrix4x4 operator+(const Matrix4x4& m1, const Matrix4x4& m2)
{
	return Add(m1, m2);
}
inline Matrix4x4 operator-(const Matrix4x4& m1, const Matrix4x4& m2)
{
	return Subtract(m1, m2);
}
inline Matrix4x4 operator*(const Matrix4x4& m1, const Matrix4x4& m2)
{
	return Multiply(m1, m2);
}

// ベクトルの内積
float Dot(const Vector3& v1, const Vector3& v2);

// ベクトルの長さ(ノルム)
float Length(const Vector3& v);

// ベクトルの正規化
Vector3 Normalize(const Vector3& v);

Vector3 Transform(const Vector3& vector, const Matrix4x4& m);

Matrix4x4 Inverse(Matrix4x4 matrix);

Vector3 Perpendicular(const Vector3& vector);

// 単位行列の生成
Matrix4x4 MakeIdentityMatrix();
Matrix4x4 MakeTranslateMatrix(const Vector3& translate);

Matrix4x4 MakeScaleMatrix(const Vector3& scale);
// 1. X軸回転行列
Matrix4x4 MakeRotateXMatrix(float radian);
// 2. Y軸回転行列
Matrix4x4 MakeRotateYMatrix(float radian);
// 3. Z軸回転行列
Matrix4x4 MakeRotateZMatrix(float radian);

Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);

//1. 透視投影行列
Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip);
//2. 正射影行列
Matrix4x4 MakeOrthoGraphicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip);
//3. ビューポート変換行列
Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth);


static const int kRowHeight = 20;
static const int kColumnWidth = 60;
void MatrixScreenPrint(int x, int y, const Matrix4x4& matrix, const char* label);

void VectorScreenPrint(int x, int y, const Vector3& vector, const char* label);

Vector3 Cross(const Vector3& v1, const Vector3& v2);

void DrawGrid(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix);

void DrawSphere(const Sphere& sphere, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewPortMatrix, uint32_t color);

void DrawPlane(const Plane& plane, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewPortMatrix, uint32_t color);

void DrawTriangle(const Triangle& triangle, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewPortMatrix, uint32_t color);

void DrawAABB(const AABB& aabb, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);

Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t);

Vector3 CreateBezier(const Vector3& controlPoint0, const Vector3& controlPoint1, const Vector3& controlPoint2, float t);

void DrawBezier(const Vector3& controlPoint0, const Vector3& controlPoint1, const Vector3& controlPoint2, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, float pointSize, uint32_t color);