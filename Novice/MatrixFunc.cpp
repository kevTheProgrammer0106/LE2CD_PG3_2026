#include "MatrixFunc.h"

#include <Novice.h>
#define _USE_MATH_DEFINES
#include<math.h>
#include <cmath>
#include <assert.h>
#include <algorithm>

float cotf(float value)
{
	return 1.0f / tanf(value);
}
// ベクトルの加算
Vector3 Add(const Vector3& v1, const Vector3& v2) {
	Vector3 result;
	result.x = v1.x + v2.x;
	result.y = v1.y + v2.y;
	result.z = v1.z + v2.z;
	return result;
}
// ベクトルの減算
Vector3 Subtract(const Vector3& v1, const Vector3& v2) {
	Vector3 result;
	result.x = v1.x - v2.x;
	result.y = v1.y - v2.y;
	result.z = v1.z - v2.z;
	return result;
}

Vector3 Multiply(const Vector3& v1, const Vector3& v2) {
	Vector3 result;
	result.x = v1.x * v2.x;
	result.y = v1.y * v2.y;
	result.z = v1.z * v2.z;
	return result;
}
Vector3 Multiply(const float& v1, const Vector3& v2) {
	Vector3 result;
	result.x = v1 * v2.x;
	result.y = v1 * v2.y;
	result.z = v1 * v2.z;
	return result;
}
// 行列のスカラー倍
Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 result;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			result.m[i][j] = 0.0f;
			for (int k = 0; k < 4; k++)
			{
				result.m[i][j] += m1.m[i][k] * m2.m[k][j];
			}
		}
	}

	return result;
}

// ベクトルの内積
float Dot(const Vector3& v1, const Vector3& v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}
// ベクトルの長さ(ノルム)
float Length(const Vector3& v) {
	return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}
// ベクトルの正規化
Vector3 Normalize(const Vector3& v) {
	float length = Length(v);

	if (length == 0.0f) {
		return { 0.0f,0.0f,0.0f };
	}

	return {
		v.x / length,
		v.y / length,
		v.z / length
	};
}


Vector3 Transform(const Vector3& vector, const Matrix4x4& m) {
	Vector3 result;
	float w;
	result.x = vector.x * m.m[0][0] + vector.y * m.m[1][0] + vector.z * m.m[2][0] + m.m[3][0];
	result.y = vector.x * m.m[0][1] + vector.y * m.m[1][1] + vector.z * m.m[2][1] + m.m[3][1];
	result.z = vector.x * m.m[0][2] + vector.y * m.m[1][2] + vector.z * m.m[2][2] + m.m[3][2];
	w = vector.x * m.m[0][3] + vector.y * m.m[1][3] + vector.z * m.m[2][3] + m.m[3][3];

	assert(w != 0.0f);
	result.x /= w;
	result.y /= w;
	result.z /= w;
	return result;
}

Matrix4x4 Inverse(Matrix4x4 matrix)
{
	float determinant =
		matrix.m[0][0] * (
			matrix.m[1][1] * (matrix.m[2][2] * matrix.m[3][3] - matrix.m[2][3] * matrix.m[3][2]) -
			matrix.m[1][2] * (matrix.m[2][1] * matrix.m[3][3] - matrix.m[2][3] * matrix.m[3][1]) +
			matrix.m[1][3] * (matrix.m[2][1] * matrix.m[3][2] - matrix.m[2][2] * matrix.m[3][1])) -
		matrix.m[0][1] * (
			matrix.m[1][0] * (matrix.m[2][2] * matrix.m[3][3] - matrix.m[2][3] * matrix.m[3][2]) -
			matrix.m[1][2] * (matrix.m[2][0] * matrix.m[3][3] - matrix.m[2][3] * matrix.m[3][0]) +
			matrix.m[1][3] * (matrix.m[2][0] * matrix.m[3][2] - matrix.m[2][2] * matrix.m[3][0])) +
		matrix.m[0][2] * (
			matrix.m[1][0] * (matrix.m[2][1] * matrix.m[3][3] - matrix.m[2][3] * matrix.m[3][1]) -
			matrix.m[1][1] * (matrix.m[2][0] * matrix.m[3][3] - matrix.m[2][3] * matrix.m[3][0]) +
			matrix.m[1][3] * (matrix.m[2][0] * matrix.m[3][1] - matrix.m[2][1] * matrix.m[3][0])) -
		matrix.m[0][3] * (
			matrix.m[1][0] * (matrix.m[2][1] * matrix.m[3][2] - matrix.m[2][2] * matrix.m[3][1]) -
			matrix.m[1][1] * (matrix.m[2][0] * matrix.m[3][2] - matrix.m[2][2] * matrix.m[3][0]) +
			matrix.m[1][2] * (matrix.m[2][0] * matrix.m[3][1] - matrix.m[2][1] * matrix.m[3][0]));

	assert(determinant != 0.0f);
	float invDet = 1.0f / determinant;
	return matrix =
	{
		 (matrix.m[1][1] * (matrix.m[2][2] * matrix.m[3][3] - matrix.m[2][3] * matrix.m[3][2]) - matrix.m[1][2] * (matrix.m[2][1] * matrix.m[3][3] - matrix.m[2][3] * matrix.m[3][1]) + matrix.m[1][3] * (matrix.m[2][1] * matrix.m[3][2] - matrix.m[2][2] * matrix.m[3][1])) * invDet,
		-(matrix.m[0][1] * (matrix.m[2][2] * matrix.m[3][3] - matrix.m[2][3] * matrix.m[3][2]) - matrix.m[0][2] * (matrix.m[2][1] * matrix.m[3][3] - matrix.m[2][3] * matrix.m[3][1]) + matrix.m[0][3] * (matrix.m[2][1] * matrix.m[3][2] - matrix.m[2][2] * matrix.m[3][1])) * invDet,
		 (matrix.m[0][1] * (matrix.m[1][2] * matrix.m[3][3] - matrix.m[1][3] * matrix.m[3][2]) - matrix.m[0][2] * (matrix.m[1][1] * matrix.m[3][3] - matrix.m[1][3] * matrix.m[3][1]) + matrix.m[0][3] * (matrix.m[1][1] * matrix.m[3][2] - matrix.m[1][2] * matrix.m[3][1])) * invDet,
		-(matrix.m[0][1] * (matrix.m[1][2] * matrix.m[2][3] - matrix.m[1][3] * matrix.m[2][2]) - matrix.m[0][2] * (matrix.m[1][1] * matrix.m[2][3] - matrix.m[1][3] * matrix.m[2][1]) + matrix.m[0][3] * (matrix.m[1][1] * matrix.m[2][2] - matrix.m[1][2] * matrix.m[2][1])) * invDet,

		-(matrix.m[1][0] * (matrix.m[2][2] * matrix.m[3][3] - matrix.m[2][3] * matrix.m[3][2]) - matrix.m[1][2] * (matrix.m[2][0] * matrix.m[3][3] - matrix.m[2][3] * matrix.m[3][0]) + matrix.m[1][3] * (matrix.m[2][0] * matrix.m[3][2] - matrix.m[2][2] * matrix.m[3][0])) * invDet,
		 (matrix.m[0][0] * (matrix.m[2][2] * matrix.m[3][3] - matrix.m[2][3] * matrix.m[3][2]) - matrix.m[0][2] * (matrix.m[2][0] * matrix.m[3][3] - matrix.m[2][3] * matrix.m[3][0]) + matrix.m[0][3] * (matrix.m[2][0] * matrix.m[3][2] - matrix.m[2][2] * matrix.m[3][0])) * invDet,
		-(matrix.m[0][0] * (matrix.m[1][2] * matrix.m[3][3] - matrix.m[1][3] * matrix.m[3][2]) - matrix.m[0][2] * (matrix.m[1][0] * matrix.m[3][3] - matrix.m[1][3] * matrix.m[3][0]) + matrix.m[0][3] * (matrix.m[1][0] * matrix.m[3][2] - matrix.m[1][2] * matrix.m[3][0])) * invDet,
		 (matrix.m[0][0] * (matrix.m[1][2] * matrix.m[2][3] - matrix.m[1][3] * matrix.m[2][2]) - matrix.m[0][2] * (matrix.m[1][0] * matrix.m[2][3] - matrix.m[1][3] * matrix.m[2][0]) + matrix.m[0][3] * (matrix.m[1][0] * matrix.m[2][2] - matrix.m[1][2] * matrix.m[2][0])) * invDet,

		 (matrix.m[1][0] * (matrix.m[2][1] * matrix.m[3][3] - matrix.m[2][3] * matrix.m[3][1]) - matrix.m[1][1] * (matrix.m[2][0] * matrix.m[3][3] - matrix.m[2][3] * matrix.m[3][0]) + matrix.m[1][3] * (matrix.m[2][0] * matrix.m[3][1] - matrix.m[2][1] * matrix.m[3][0])) * invDet,
		-(matrix.m[0][0] * (matrix.m[2][1] * matrix.m[3][3] - matrix.m[2][3] * matrix.m[3][1]) - matrix.m[0][1] * (matrix.m[2][0] * matrix.m[3][3] - matrix.m[2][3] * matrix.m[3][0]) + matrix.m[0][3] * (matrix.m[2][0] * matrix.m[3][1] - matrix.m[2][1] * matrix.m[3][0])) * invDet,
		 (matrix.m[0][0] * (matrix.m[1][1] * matrix.m[3][3] - matrix.m[1][3] * matrix.m[3][1]) - matrix.m[0][1] * (matrix.m[1][0] * matrix.m[3][3] - matrix.m[1][3] * matrix.m[3][0]) + matrix.m[0][3] * (matrix.m[1][0] * matrix.m[3][1] - matrix.m[1][1] * matrix.m[3][0])) * invDet,
		-(matrix.m[0][0] * (matrix.m[1][1] * matrix.m[2][3] - matrix.m[1][3] * matrix.m[2][1]) - matrix.m[0][1] * (matrix.m[1][0] * matrix.m[2][3] - matrix.m[1][3] * matrix.m[2][0]) + matrix.m[0][3] * (matrix.m[1][0] * matrix.m[2][1] - matrix.m[1][1] * matrix.m[2][0])) * invDet,

		-(matrix.m[1][0] * (matrix.m[2][1] * matrix.m[3][2] - matrix.m[2][2] * matrix.m[3][1]) - matrix.m[1][1] * (matrix.m[2][0] * matrix.m[3][2] - matrix.m[2][2] * matrix.m[3][0]) + matrix.m[1][2] * (matrix.m[2][0] * matrix.m[3][1] - matrix.m[2][1] * matrix.m[3][0])) * invDet,
		 (matrix.m[0][0] * (matrix.m[2][1] * matrix.m[3][2] - matrix.m[2][2] * matrix.m[3][1]) - matrix.m[0][1] * (matrix.m[2][0] * matrix.m[3][2] - matrix.m[2][2] * matrix.m[3][0]) + matrix.m[0][2] * (matrix.m[2][0] * matrix.m[3][1] - matrix.m[2][1] * matrix.m[3][0])) * invDet,
		-(matrix.m[0][0] * (matrix.m[1][1] * matrix.m[3][2] - matrix.m[1][2] * matrix.m[3][1]) - matrix.m[0][1] * (matrix.m[1][0] * matrix.m[3][2] - matrix.m[1][2] * matrix.m[3][0]) + matrix.m[0][2] * (matrix.m[1][0] * matrix.m[3][1] - matrix.m[1][1] * matrix.m[3][0])) * invDet,
		 (matrix.m[0][0] * (matrix.m[1][1] * matrix.m[2][2] - matrix.m[1][2] * matrix.m[2][1]) - matrix.m[0][1] * (matrix.m[1][0] * matrix.m[2][2] - matrix.m[1][2] * matrix.m[2][0]) + matrix.m[0][2] * (matrix.m[1][0] * matrix.m[2][1] - matrix.m[1][1] * matrix.m[2][0])) * invDet,
	};
}

Vector3 Perpendicular(const Vector3& vector)
{
	if (vector.x != 0.0f || vector.y != 0.0f)
	{
		return { -vector.y, vector.x, 0.0f };
	}
	return { 0.0f, -vector.z, vector.y };
}

// 単位行列の生成
Matrix4x4 MakeIdentityMatrix() {
	Matrix4x4 result = {};
	for (int i = 0; i < 4; ++i) {
		result.m[i][i] = 1.0f; // 単位行列
	}
	return result;
}
Matrix4x4 MakeTranslateMatrix(const Vector3& translate) {
	Matrix4x4 result = MakeIdentityMatrix();
	result.m[3][0] = translate.x;
	result.m[3][1] = translate.y;
	result.m[3][2] = translate.z;
	return result;
}

Matrix4x4 MakeScaleMatrix(const Vector3& scale) {
	Matrix4x4 result;
	result = {
		scale.x, 0.0f, 0.0f, 0.0f,
		0.0f, scale.y, 0.0f, 0.0f,
		0.0f, 0.0f, scale.z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};
	return result;
}
// 1. X軸回転行列
Matrix4x4 MakeRotateXMatrix(float radian) {
	Matrix4x4 result = {
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, cosf(radian), sinf(radian), 0.0f,
		0.0f, -sinf(radian), cosf(radian), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};
	return result;
}
// 2. Y軸回転行列
Matrix4x4 MakeRotateYMatrix(float radian) {
	Matrix4x4 result = {
		cosf(radian), 0.0f, -sinf(radian), 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		sinf(radian), 0.0f, cosf(radian), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};
	return result;
}
// 3. Z軸回転行列
Matrix4x4 MakeRotateZMatrix(float radian) {
	Matrix4x4 result = {
		cosf(radian), sinf(radian), 0.0f, 0.0f,
		-sinf(radian), cosf(radian), 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};
	return result;
}

Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate) {
	Matrix4x4 scaleMatrix = MakeScaleMatrix(scale);
	Matrix4x4 rotateXMatrix = MakeRotateXMatrix(rotate.x);
	Matrix4x4 rotateYMatrix = MakeRotateYMatrix(rotate.y);
	Matrix4x4 rotateZMatrix = MakeRotateZMatrix(rotate.z);
	Matrix4x4 translateMatrix = MakeTranslateMatrix(translate);

	Matrix4x4 rotateXYZMatrix = Multiply(rotateXMatrix, Multiply(rotateYMatrix, rotateZMatrix));
	Matrix4x4 worldMatrix = Multiply(scaleMatrix, Multiply(rotateXYZMatrix, translateMatrix));
	//worldMatrix = Multiply(worldMatrix, translateMatrix);

	return worldMatrix;
}

//1. 透視投影行列
Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip)
{
	Matrix4x4 result =
	{
	(1.0f / aspectRatio) * cotf(fovY / 2.0f), 0, 0, 0,
	0, cotf(fovY / 2), 0, 0,
	0, 0, farClip / (farClip - nearClip), 1.0f,
	0, 0, -nearClip * farClip / (farClip - nearClip), 0
	};
	return result;

};
//2. 正射影行列
Matrix4x4 MakeOrthoGraphicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip)
{
	Matrix4x4 result =
	{
		2.0f / (right - left), 0, 0, 0,
		0, 2.0f / (top - bottom), 0, 0,
		0, 0, 1.0f / (farClip - nearClip), 0,
		(left + right) / (left - right), (top + bottom) / (bottom - top), nearClip / (nearClip - farClip), 1.0f
	};
	return result;
};
//3. ビューポート変換行列
Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth)
{
	Matrix4x4 result =
	{
		width / 2.0f, 0, 0, 0,
		0, -height / 2.0f, 0, 0,
		0, 0, maxDepth - minDepth, 0,
		left + (width / 2.0f), top + (height / 2.0f), minDepth, 1.0f
	};
	return result;
};

void MatrixScreenPrint(int x, int y, const Matrix4x4& matrix, const char* label) {
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			Novice::ScreenPrintf(x + kColumnWidth * j, y + kRowHeight * i, "%.02f", matrix.m[i][j]);
		}
	}
	Novice::ScreenPrintf(x + kColumnWidth * 5, y, "%s", label);
}

void VectorScreenPrint(int x, int y, const Vector3& vector, const char* label) {
	Novice::ScreenPrintf(x, y, "%.02f", vector.x);
	Novice::ScreenPrintf(x + kColumnWidth, y, "%.02f", vector.y);
	Novice::ScreenPrintf(x + kColumnWidth * 2, y, "%.02f", vector.z);
	Novice::ScreenPrintf(x + kColumnWidth * 3, y, "%s", label);
}

Vector3 Cross(const Vector3& v1, const Vector3& v2)
{
	Vector3 result;
	return result = {
		v1.y * v2.z - v1.z * v2.y,
		v1.z * v2.x - v1.x * v2.z,
		v1.x * v2.y - v1.y * v2.x
	};
}

void DrawGrid(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix)
{
	const float kGridHalfWidth = 2.0f; // Gridの半分の幅
	const uint32_t kSubdivision = 10; //分割数
	const float kGridEvery = (kGridHalfWidth * 2.0f) / float(kSubdivision); //1つ分の長さ
	// 奥から手前への線を順々に引いていく
	for (uint32_t zIndex = 0; zIndex <= kSubdivision; ++zIndex)
	{
		// 上の情報を使ってワールド座標系上の始点と終点を求める
		Vector3 start = { -kGridHalfWidth + kGridEvery * zIndex, 0.0f, -kGridHalfWidth };
		Vector3 end = { -kGridHalfWidth + kGridEvery * zIndex, 0.0f,  kGridHalfWidth };
		//　スクリーン座標系まで変換かける
		//変換した座標を使って表示。色は薄い灰色(0xAAAAAAFF)、原点は黒ぐらいが良いが、何でもよい

		// Normalized Device coordinates
		//X ranges from -1 (left) to +1 (right)	
		//Y ranges from - 1 (bottom)to + 1 (top)
		//Z ranges from 0 (near)to 1 (far)

		Vector3 ndcStart = Transform(start, viewProjectionMatrix);
		Vector3 screenStart = Transform(ndcStart, viewportMatrix);

		Vector3 ndcEnd = Transform(end, viewProjectionMatrix);
		Vector3 screenEnd = Transform(ndcEnd, viewportMatrix);
		unsigned int color = zIndex == 5 ? 0xAAAAAAFF : WHITE;

		Novice::DrawLine(int(screenStart.x), int(screenStart.y), int(screenEnd.x), int(screenEnd.y), color);
	}
	//左か右も同じように順々に引いていく
	for (uint32_t xIndex = 0; xIndex <= kSubdivision; ++xIndex)
	{
		// 上の情報を使ってワールド座標系上の始点と終点を求める
		Vector3 start = { -kGridHalfWidth, 0.0f, -kGridHalfWidth + kGridEvery * xIndex };
		Vector3 end = { kGridHalfWidth, 0.0f, -kGridHalfWidth + kGridEvery * xIndex };
		//　スクリーン座標系まで変換かける
		//変換した座標を使って表示。色は薄い灰色(0xAAAAAAFF)、原点は黒ぐらいが良いが、何でもよい
		Vector3 ndcStart = Transform(start, viewProjectionMatrix);
		Vector3 screenStart = Transform(ndcStart, viewportMatrix);

		Vector3 ndcEnd = Transform(end, viewProjectionMatrix);
		Vector3 screenEnd = Transform(ndcEnd, viewportMatrix);

		unsigned int color = xIndex == 5 ? 0xAAAAAAFF : WHITE;

		Novice::DrawLine(int(screenStart.x), int(screenStart.y), int(screenEnd.x), int(screenEnd.y), color);
	}
}

void DrawSphere(const Sphere& sphere, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewPortMatrix, uint32_t color)
{
	const uint32_t kSubDivision = 16; //分割数
	const float kLonEvery = (2.0f * float(M_PI)) / kSubDivision; //経度分割１つ分の角度
	const float KLatEvery = float(M_PI) / kSubDivision; //緯度分割１つ分の角度
	// 緯度の方向に分割 -π/2 ~ π/2
	for (uint32_t latIndex = 0; latIndex < kSubDivision; ++latIndex)
	{
		float lat = float(-M_PI) / 2.0f + KLatEvery * latIndex;//現在の緯度
		// 経度の方向に分割 0 ~ 2π
		for (uint32_t lonIndex = 0; lonIndex < kSubDivision; ++lonIndex)
		{
			float lon = lonIndex * kLonEvery;//現在の経度

			//world座標系でのa,b,cを求める
			Vector3 a, b, c;
			a = { cosf(lat) * cosf(lon), sinf(lat), cosf(lat) * sinf(lon) };
			b = { cosf(lat + KLatEvery) * cosf(lon), sinf(lat + KLatEvery), cosf(lat + KLatEvery) * sinf(lon) };
			c = { cosf(lat) * cosf(lon + kLonEvery), sinf(lat), cosf(lat) * sinf(lon + kLonEvery) };

			// // ab, bcで線を引く
			Vector3 abStart = { sphere.center.x + sphere.radius * a.x, sphere.center.y + sphere.radius * a.y, sphere.center.z + sphere.radius * a.z };
			Vector3 abEnd = { sphere.center.x + sphere.radius * b.x, sphere.center.y + sphere.radius * b.y, sphere.center.z + sphere.radius * b.z };
			Vector3 bcEnd = { sphere.center.x + sphere.radius * c.x, sphere.center.y + sphere.radius * c.y, sphere.center.z + sphere.radius * c.z };

			// a, b ,cをScreen座標系まで変換
			Vector3 ndcStartAB = Transform(abStart, viewProjectionMatrix);
			Vector3 screenStartAB = Transform(ndcStartAB, viewPortMatrix);
			Vector3 ndcEndAB = Transform(abEnd, viewProjectionMatrix);
			Vector3 screenEndAB = Transform(ndcEndAB, viewPortMatrix);
			Vector3 ndcEndBC = Transform(bcEnd, viewProjectionMatrix);
			Vector3 screenEndBC = Transform(ndcEndBC, viewPortMatrix);

			//ab
			Novice::DrawLine(int(screenStartAB.x), int(screenStartAB.y), int(screenEndAB.x), int(screenEndAB.y), color);
			//bc
			Novice::DrawLine(int(screenStartAB.x), int(screenStartAB.y), int(screenEndBC.x), int(screenEndBC.y), color);
		}
	}
}

void DrawPlane(const Plane& plane, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewPortMatrix, uint32_t color)
{
	Vector3 center = Multiply(plane.distance, plane.normal); // 1
	Vector3 perpendiculars[4];
	perpendiculars[0] = Normalize(Perpendicular(plane.normal)); // 2
	perpendiculars[1] = { -perpendiculars[0].x, -perpendiculars[0].y, -perpendiculars[0].z }; // 3
	perpendiculars[2] = Cross(plane.normal, perpendiculars[0]); // 4
	perpendiculars[3] = { -perpendiculars[2].x, -perpendiculars[2].y, -perpendiculars[2].z }; // 5
	// 0
	Vector3 points[4];
	for (int32_t index = 0; index < 4; ++index)
	{
		Vector3 extend = Multiply(2.0f, perpendiculars[index]);
		Vector3 point = Add(center, extend);
		points[index] = Transform(Transform(point, viewProjectionMatrix), viewPortMatrix);
	}
	Novice::DrawLine(int(points[0].x), int(points[0].y), int(points[2].x), int(points[2].y), color);
	Novice::DrawLine(int(points[2].x), int(points[2].y), int(points[1].x), int(points[1].y), color);
	Novice::DrawLine(int(points[1].x), int(points[1].y), int(points[3].x), int(points[3].y), color);
	Novice::DrawLine(int(points[3].x), int(points[3].y), int(points[0].x), int(points[0].y), color);
}

void DrawTriangle(const Triangle& triangle, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewPortMatrix, uint32_t color)
{
	// Screen空間へと頂点を変換する
	Vector3 screenVertices[3];
	for (uint32_t i = 0; i < 3; ++i)
	{
		// NDCまで変換。Transformを使うと同次座標系ー>デカルト座標系の処理が行われ、結果的にZDivideが行われることになる
		Vector3 ndcVertex = Transform(triangle.vertices[i], viewProjectionMatrix);
		// Viewport変換を行ってScreen空間へ
		screenVertices[i] = Transform(ndcVertex, viewPortMatrix);
	}
	Vector3 top = screenVertices[0];
	Vector3 left = screenVertices[1];
	Vector3 right = screenVertices[2];
	//描画
	Novice::DrawTriangle(int(top.x), int(top.y), int(left.x), int(left.y), int(right.x), int(right.y), color, kFillModeSolid);
}

void DrawAABB(const AABB& aabb, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) 
{
	// 8頂点のAABB
	Vector3 vertices[8] =
	{
		{aabb.min.x, aabb.min.y, aabb.min.z}, // 0
		{aabb.max.x, aabb.min.y, aabb.min.z}, // 1
		{aabb.max.x, aabb.max.y, aabb.min.z}, // 2
		{aabb.min.x, aabb.max.y, aabb.min.z}, // 3

		{aabb.min.x, aabb.min.y, aabb.max.z}, // 4
		{aabb.max.x, aabb.min.y, aabb.max.z}, // 5
		{aabb.max.x, aabb.max.y, aabb.max.z}, // 6
		{aabb.min.x, aabb.max.y, aabb.max.z}  // 7
	};

	// スクリーン座標をトランスフォム変換
	Vector3 screen[8];

	for (int i = 0; i < 8; i++)
	{
		Vector3 ndc = Transform(vertices[i], viewProjectionMatrix);
		screen[i] = Transform(ndc, viewportMatrix);
	}

	// 下面
	Novice::DrawLine((int)screen[0].x, (int)screen[0].y, (int)screen[1].x, (int)screen[1].y, color);
	Novice::DrawLine((int)screen[1].x, (int)screen[1].y, (int)screen[2].x, (int)screen[2].y, color);
	Novice::DrawLine((int)screen[2].x, (int)screen[2].y, (int)screen[3].x, (int)screen[3].y, color);
	Novice::DrawLine((int)screen[3].x, (int)screen[3].y, (int)screen[0].x, (int)screen[0].y, color);

	// 上面
	Novice::DrawLine((int)screen[4].x, (int)screen[4].y, (int)screen[5].x, (int)screen[5].y, color);
	Novice::DrawLine((int)screen[5].x, (int)screen[5].y, (int)screen[6].x, (int)screen[6].y, color);
	Novice::DrawLine((int)screen[6].x, (int)screen[6].y, (int)screen[7].x, (int)screen[7].y, color);
	Novice::DrawLine((int)screen[7].x, (int)screen[7].y, (int)screen[4].x, (int)screen[4].y, color);

	// 垂直
	Novice::DrawLine((int)screen[0].x, (int)screen[0].y, (int)screen[4].x, (int)screen[4].y, color);
	Novice::DrawLine((int)screen[1].x, (int)screen[1].y, (int)screen[5].x, (int)screen[5].y, color);
	Novice::DrawLine((int)screen[2].x, (int)screen[2].y, (int)screen[6].x, (int)screen[6].y, color);
	Novice::DrawLine((int)screen[3].x, (int)screen[3].y, (int)screen[7].x, (int)screen[7].y, color);
}

Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t)
{
	Vector3 result;
	result.x = v1.x + (v2.x - v1.x) * t;
	result.y = v1.y + (v2.y - v1.y) * t;
	result.z = v1.z + (v2.z - v1.z) * t;
	return result;
}

Vector3 CreateBezier(const Vector3& controlPoint0, const Vector3& controlPoint1, const Vector3& controlPoint2, float t)
{
	//制御点p0,p1を線形補間
	Vector3 p0p1 = Lerp(controlPoint0, controlPoint1, t);
	//制御点p1,p2を線形補間
	Vector3 p1p2 = Lerp(controlPoint1, controlPoint2, t);
	//制御点p0p1,p1p2をさらに線形補間
	return Lerp(p0p1, p1p2, t);
}


void DrawBezier(const Vector3& controlPoint0, const Vector3& controlPoint1, const Vector3& controlPoint2, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, float pointSize, uint32_t color)
{
	const int curveDivision = 16;

	// Transform control points to screen space
	Sphere screenP0 = { controlPoint0 , pointSize / 2.0f };

	Sphere screenP1 = { controlPoint1 , pointSize };

	Sphere screenP2 = { controlPoint2 , pointSize };

	// Draw the Bézier curve
	for (int i = 0; i < curveDivision; ++i)
	{
		float t0 = float(i) / curveDivision;
		float t1 = float(i + 1) / curveDivision;

		Vector3 bezier0 = CreateBezier(controlPoint0, controlPoint1, controlPoint2, t0);
		Vector3 bezier1 = CreateBezier(controlPoint0, controlPoint1, controlPoint2, t1);

		Vector3 screenBezier0 =
			Transform(Transform(bezier0, viewProjectionMatrix), viewportMatrix);

		Vector3 screenBezier1 =
			Transform(Transform(bezier1, viewProjectionMatrix), viewportMatrix);

		// Old Bézier sample point visualization
		/*
		Novice::DrawEllipse(
			static_cast<int>(screenBezier0.x),
			static_cast<int>(screenBezier0.y),
			4, 4, 0.0f,
			color,
			kFillModeSolid);

		Novice::DrawEllipse(
			static_cast<int>(screenBezier1.x),
			static_cast<int>(screenBezier1.y),
			4, 4, 0.0f,
			color,
			kFillModeSolid);
		*/

		Novice::DrawLine(
			static_cast<int>(screenBezier0.x),
			static_cast<int>(screenBezier0.y),
			static_cast<int>(screenBezier1.x),
			static_cast<int>(screenBezier1.y),
			color);
	}

	//コントロールポイント
	DrawSphere(screenP0, viewProjectionMatrix, viewportMatrix, GREEN);

	DrawSphere(screenP1, viewProjectionMatrix, viewportMatrix, GREEN);

	DrawSphere(screenP2, viewProjectionMatrix, viewportMatrix, GREEN);
}