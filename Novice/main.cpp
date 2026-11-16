#define NOMINMAX
#define _USE_MATH_DEFINES

#include <Novice.h>
#include <algorithm>
#include <assert.h>
#include <cmath>
#include <cstring>
#include <numbers>
#include <limits>

#include "CollisionDetection.h"
#include "MatrixFunc.h"

#ifdef USE_IMGUI
#include <imgui.h>
#endif // USE_IMGUI

struct Spherical {
	float radius; // 動径 r
	float theta;  // 垂直角 θ
	float phi;    // 水平角 φ
};

// 球面座標 → 直交座標へ変換
Vector3 ToCartesian(const Spherical& s) {
	float rho = s.radius * std::cos(s.theta);

	return {rho * std::cos(s.phi), s.radius * std::sin(s.theta), rho * std::sin(s.phi)};
}

// 直交座標 → 球面座標へ変換
Spherical ToSpherical(const Vector3& p) {
	float r = std::sqrt(p.x * p.x + p.y * p.y + p.z * p.z);

	if (r == 0.0f) {
		return {0.0f, 0.0f, 0.0f};
	}

	float sinTheta = std::clamp(p.y / r, -1.0f, 1.0f);

	// atan2ならxまたはzが0でも正しく角度を求められる
	float phi = std::atan2(p.z, p.x);

	return {r, std::asin(sinTheta), phi};
}

struct Quaternion {
	float x;
	float y;
	float z;
	float w;
};

// Quaternionの積
Quaternion Multiply(const Quaternion& q1, const Quaternion& q2) {
	return {
	    q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y,
	    q1.w * q2.y - q1.x * q2.z + q1.y * q2.w + q1.z * q2.x,
	    q1.w * q2.z + q1.x * q2.y - q1.y * q2.x + q1.z * q2.w,
	    q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z,
	};
}

 // Quaternionの共役
Quaternion Conjugate(const Quaternion& q) { return {-q.x, -q.y, -q.z, q.w}; }

// Quaternionの単位元を返す
Quaternion IdentityQuaternion() { return {0.0f, 0.0f, 0.0f, 1.0f}; }

// Quaternionのノルム（大きさ）を返す
float Norm(const Quaternion& quaternion) { return std::sqrtf(quaternion.x * quaternion.x + quaternion.y * quaternion.y + quaternion.z * quaternion.z + quaternion.w * quaternion.w); }

// Quaternionを正規化する
Quaternion Normalize(const Quaternion& quaternion) {
	float norm = Norm(quaternion);

	// ゼロ除算を防ぐ
	if (norm <= std::numeric_limits<float>::epsilon()) {
		return IdentityQuaternion();
	}

	return {quaternion.x / norm, quaternion.y / norm, quaternion.z / norm, quaternion.w / norm};
}

// Quaternionの逆元を返す
Quaternion Inverse(const Quaternion& quaternion) {
	float normSquared = quaternion.x * quaternion.x + quaternion.y * quaternion.y + quaternion.z * quaternion.z + quaternion.w * quaternion.w;

	// ゼロQuaternionには逆元が存在しない
	if (normSquared == 0.0f) {
		return {0.0f, 0.0f, 0.0f, 0.0f};
	}

	Quaternion conjugate = Conjugate(quaternion);

	return {conjugate.x / normSquared, conjugate.y / normSquared, conjugate.z / normSquared, conjugate.w / normSquared};
}

//MT4_01_01
//  任意軸回転を表す Quaternionの生成
Quaternion MakeRotateAxisAngleQuaternion(const Vector3& axis, float angle) {
	// 回転軸の長さを求める
	float length = std::sqrt(axis.x * axis.x + axis.y * axis.y + axis.z * axis.z);

	// 回転軸がゼロベクトルなら単位Quaternionを返す
	if (length == 0.0f) {
		return IdentityQuaternion();
	}

	// 回転軸を正規化する
	Vector3 normalizedAxis = {axis.x / length, axis.y / length, axis.z / length};

	float halfAngle = angle * 0.5f;
	float sinHalfAngle = std::sin(halfAngle);

	return {normalizedAxis.x * sinHalfAngle, normalizedAxis.y * sinHalfAngle, normalizedAxis.z * sinHalfAngle, std::cos(halfAngle)};
}

// ベクトルをQuaternionで回転させた結果のベクトルを求める
Vector3 RotateVector(const Vector3& vector, const Quaternion& quaternion) {
	// Quaternionを正規化して回転量を整える
	Quaternion q = Normalize(quaternion);

	// ベクトルをQuaternionに変換する
	Quaternion p = {vector.x, vector.y, vector.z, 0.0f};

	// q * p * qの共役でベクトルを回転させる
	Quaternion result = Multiply(Multiply(q, p), Conjugate(q));

	return {result.x, result.y, result.z};
}

// Quaternionから回転行列を求める
Matrix4x4 MakeRotateMatrix(const Quaternion& quaternion) {
	// Quaternionを正規化する
	Quaternion q = Normalize(quaternion);

	float x = q.x;
	float y = q.y;
	float z = q.z;
	float w = q.w;

	Matrix4x4 result{};

	// Quaternionから回転行列を計算する
	result.m[0][0] = 1.0f - 2.0f * (y * y + z * z);
	result.m[0][1] = 2.0f * (x * y + z * w);
	result.m[0][2] = 2.0f * (x * z - y * w);
	result.m[0][3] = 0.0f;

	result.m[1][0] = 2.0f * (x * y - z * w);
	result.m[1][1] = 1.0f - 2.0f * (x * x + z * z);
	result.m[1][2] = 2.0f * (y * z + x * w);
	result.m[1][3] = 0.0f;

	result.m[2][0] = 2.0f * (x * z + y * w);
	result.m[2][1] = 2.0f * (y * z - x * w);
	result.m[2][2] = 1.0f - 2.0f * (x * x + y * y);
	result.m[2][3] = 0.0f;

	result.m[3][0] = 0.0f;
	result.m[3][1] = 0.0f;
	result.m[3][2] = 0.0f;
	result.m[3][3] = 1.0f;

	return result;
}
void QuaternionScreenPrint(int x, int y, const Quaternion& quaternion, const char* label) {
	Novice::ScreenPrintf(x, y, "%.2f", quaternion.x);
	Novice::ScreenPrintf(x + kColumnWidth, y, "%.2f", quaternion.y);
	Novice::ScreenPrintf(x + kColumnWidth * 2, y, "%.2f", quaternion.z);
	Novice::ScreenPrintf(x + kColumnWidth * 3, y, "%.2f", quaternion.w);
	Novice::ScreenPrintf(x + kColumnWidth * 5, y, "%s", label);
}


// あるベクトルと直交する単位ベクトルを求める
Vector3 MakeOrthogonalVector(const Vector3& v) {
	Vector3 reference;

	if (std::abs(v.x) <= std::abs(v.y) && std::abs(v.x) <= std::abs(v.z)) {
		reference = {1.0f, 0.0f, 0.0f};
	} else if (std::abs(v.y) < std::abs(v.z)) {
		reference = {0.0f, 1.0f, 0.0f};
	} else {
		reference = {0.0f, 0.0f, 1.0f};
	}

	Vector3 result = Cross(v, reference);

	return Normalize(result);
}

// 方向ベクトルから方向ベクトルへの回転Quaternionを求める
Quaternion DirectionToDirectionQuaternion(const Vector3& from, const Vector3& to) {
	// 方向ベクトルを正規化
	Vector3 fromNormalized = Normalize(from);
	Vector3 toNormalized = Normalize(to);

	// 内積から回転角度を求める
	float dotProduct = fromNormalized.x * toNormalized.x + fromNormalized.y * toNormalized.y + fromNormalized.z * toNormalized.z;

	// 浮動小数点誤差を防ぐ
	dotProduct = std::clamp(dotProduct, -1.0f, 1.0f);

	// 同じ方向なら単位Quaternionを返す
	if (dotProduct >= 1.0f - 0.000001f) {
		return IdentityQuaternion();
	}

	// 正反対の方向なら直交する回転軸を使う
	if (dotProduct <= -1.0f + 0.000001f) {
		Vector3 orthogonalAxis = MakeOrthogonalVector(fromNormalized);

		return MakeRotateAxisAngleQuaternion(orthogonalAxis, std::numbers::pi_v<float>);
	}

	// 外積から回転軸を求める
	Vector3 rotationAxis = Cross(fromNormalized, toNormalized);

	// 内積から回転角度を求める
	float rotationAngle = std::acos(dotProduct);

	return MakeRotateAxisAngleQuaternion(rotationAxis, rotationAngle);
}

// 方向ベクトルから方向ベクトルへの回転Matrix4x4を求める
Matrix4x4 DirectionToDirectionMatrix(const Vector3& from, const Vector3& to) {
	// Quaternionから回転行列へ変換
	Quaternion quaternion = DirectionToDirectionQuaternion(from, to);

	return MakeRotateMatrix(quaternion);
}
const char kWindowTitle[] = "LE2C_29_メンドーザ_ケビン_ラルフ_パレルモ";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);
	
	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {

		// フレームの開始
		Novice::BeginFrame();
		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///

Vector3 from0 = Normalize(Vector3{1.0f, 0.7f, 0.5f});
Vector3 to0 = -from0;
Vector3 from1 = Normalize(Vector3{-0.6f, 0.9f, 0.2f});
Vector3 to1 = Normalize(Vector3{0.4f, 0.7f, -0.5f});
Matrix4x4 rotationMatrix0 = DirectionToDirectionMatrix(Normalize(Vector3{1.0f, 0.0f, 0.0f}), Normalize(Vector3{-1.0f, 0.0f, 0.0f}));
Matrix4x4 rotationMatrix1 = DirectionToDirectionMatrix(from0, to0);
Matrix4x4 rotationMatrix2 = DirectionToDirectionMatrix(from1, to1);

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

MatrixScreenPrint(0, 0, rotationMatrix0, "rotationMatrix0");
MatrixScreenPrint(0, kRowHeight * 5, rotationMatrix1, "rotationMatrix1");
MatrixScreenPrint(0, kRowHeight * 10, rotationMatrix2, "rotationMatrix2");


		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();

	return 0;
}
