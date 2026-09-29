#define NOMINMAX
#define _USE_MATH_DEFINES

#include <Novice.h>
#include <algorithm>
#include <assert.h>
#include <cmath>
#include <cstring>
#include <numbers>

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

const char kWindowTitle[] = "LE2C_29_メンドーザ_ケビン_ラルフ_パレルモ";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// カメラの球面座標
	Spherical spherical{6.0f, 0.0f, -std::numbers::pi_v<float> / 2.0f};

	// 球面座標からカメラ位置を作る
	Vector3 cameraPosition = ToCartesian(spherical);

	// 注視点
	Vector3 target = {0.0f, 0.0f, 0.0f};

	// ワールド上方向
	Vector3 worldUp = {0.0f, 1.0f, 0.0f};

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

		// 球面座標からカメラ位置を再計算する
		cameraPosition = ToCartesian(spherical);
		// ターゲットからカメラへの前方向ベクトルを作る
		Vector3 forward = Normalize(target - cameraPosition);

		// 上方向と前方向から右方向を求める
		Vector3 right = Normalize(Cross(worldUp, forward));

		// 前方向と右方向からカメラの上方向を求める
		Vector3 up = Cross(forward, right);

		// カメラ行列を作成
		Matrix4x4 cameraMatrix{
		    {
             {right.x, right.y, right.z, 0.0f},
             {up.x, up.y, up.z, 0.0f},
             {forward.x, forward.y, forward.z, 0.0f},
             {cameraPosition.x, cameraPosition.y, cameraPosition.z, 1.0f},
		     }
        };

#ifdef USE_IMGUI
		ImGui::Begin("Spherical Coordinates");

		ImGui::Text("Target: (%f, %f, %f) / +Y up / Camera +Z forward", target.x, target.y, target.z);

		ImGui::Spacing();

		// rを変更
		ImGui::Text("%f", spherical.radius);
		ImGui::SameLine();
		ImGui::Text("Radius");
		ImGui::SameLine();

		if (ImGui::Button("-##RadiusMinus")) {
			spherical.radius = std::max(0.1f, spherical.radius - 0.1f);
		}

		ImGui::SameLine();

		if (ImGui::Button("+##RadiusPlus")) {
			spherical.radius += 0.1f;
		}

		ImGui::Spacing();

		// thetaを変更
		ImGui::Text("%f", spherical.theta);
		ImGui::SameLine();
		ImGui::Text("Theta: elevation");
		ImGui::SameLine();

		if (ImGui::Button("-##ThetaMinus")) {
			spherical.theta -= 0.1f;
		}

		ImGui::SameLine();

		if (ImGui::Button("+##ThetaPlus")) {
			spherical.theta += 0.1f;
		}

		ImGui::Spacing();

		// phiを変更
		ImGui::Text("%f", spherical.phi);
		ImGui::SameLine();
		ImGui::Text("Phi: horizontal");
		ImGui::SameLine();

		if (ImGui::Button("-##PhiMinus")) {
			spherical.phi -= 0.1f;
		}

		ImGui::SameLine();

		if (ImGui::Button("+##PhiPlus")) {
			spherical.phi += 0.1f;
		}

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();

		ImGui::Text("=== Spherical Coordinates ===");

		ImGui::Text("r = %f", spherical.radius);
		ImGui::SameLine();

		ImGui::Text("theta = %f rad", spherical.theta);
		ImGui::SameLine();

		ImGui::Text("phi = %f rad", spherical.phi);

		ImGui::Spacing();

		ImGui::Text("=== Cartesian Coordinates ===");

		ImGui::Text("x = %f", cameraPosition.x);
		ImGui::SameLine();

		ImGui::Text("y = %f", cameraPosition.y);
		ImGui::SameLine();

		ImGui::Text("z = %f", cameraPosition.z);

		ImGui::Text("cameraPosition = (%f, %f, %f)", cameraPosition.x, cameraPosition.y, cameraPosition.z);

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();

		// カメラ行列を表示
		ImGui::Text("=== Camera Matrix ===");

		ImGui::Text("[ %f  %f  %f  %f ]", cameraMatrix.m[0][0], cameraMatrix.m[0][1], cameraMatrix.m[0][2], cameraMatrix.m[0][3]);

		ImGui::Text("[ %f  %f  %f  %f ]", cameraMatrix.m[1][0], cameraMatrix.m[1][1], cameraMatrix.m[1][2], cameraMatrix.m[1][3]);

		ImGui::Text("[ %f  %f  %f  %f ]", cameraMatrix.m[2][0], cameraMatrix.m[2][1], cameraMatrix.m[2][2], cameraMatrix.m[2][3]);

		ImGui::Text("[ %f  %f  %f  %f ]", cameraMatrix.m[3][0], cameraMatrix.m[3][1], cameraMatrix.m[3][2], cameraMatrix.m[3][3]);

		ImGui::Spacing();
		ImGui::End();
#endif // USE_IMGUI

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

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
