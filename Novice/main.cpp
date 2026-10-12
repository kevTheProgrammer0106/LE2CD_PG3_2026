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
	
	Vector2 targetPosition = {0.0f, 0.0f};
	float targetRadius = 12.0f;
	uint32_t targetColor = RED;

	Vector2 interpolationPosition = {640.0f, 360.0f};
	float interpolationRadius = 20.0f;
	uint32_t interpolationColor = GREEN;

	int mousePosX = 0;
	int mousePosY = 0;

	float speed = 1.0f;

	float framesPerSecond = 60.0f;
	// deltatTime秒での追従割合tを計算
	float deltaTime = 1.0f / framesPerSecond;

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

		Novice::GetMousePosition(&mousePosX, &mousePosY);
		targetPosition = {static_cast<float>(mousePosX), static_cast<float>(mousePosY)};
		interpolationPosition += {
		    (speed * deltaTime) * (targetPosition.x - interpolationPosition.x),
		    (speed * deltaTime) * (targetPosition.y - interpolationPosition.y),
		};
		float distanceToTarget = Length({targetPosition.x - interpolationPosition.x, targetPosition.y - interpolationPosition.y, 0.0f});

		ImGui::Begin("Interpolation Controller");
		ImGui::Text("Target: Mouse Position (Red Circle)");
		
		ImGui::Separator();

		ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "[Linear Interpolation - Student Mode]");
		ImGui::DragFloat("Speed", &speed, 0.01f);
		ImGui::Text("Mouse Pos: (%.2f, %.2f)", static_cast<float>(mousePosX), static_cast<float>(mousePosY));
		ImGui::Text("Interpolation Position: (%.2f, %.2f)", targetPosition.x, targetPosition.y);
		ImGui::Text("Distance to target: %.2f px", distanceToTarget);

		ImGui::Separator();

		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "[Teacher Comparison Option]");
		/*ImGui::Checkbox("Show Exponetial Decay");*/
		if (ImGui::Button("Snap To Target")) {
			interpolationPosition = targetPosition;
		}
		ImGui::SameLine();
		if (ImGui::Button("Reset To Center")) {
			interpolationPosition = {640.0f, 360.0f};
		}



		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///
		
		Novice::DrawEllipse(static_cast<int>(interpolationPosition.x), static_cast<int>(interpolationPosition.y), static_cast<int>(interpolationRadius), static_cast<int>(interpolationRadius), 0.0f,interpolationColor, kFillModeSolid);
		Novice::DrawEllipse(static_cast<int>(targetPosition.x), static_cast<int>(targetPosition.y), static_cast<int>(targetRadius), static_cast<int>(targetRadius), 0.0f, targetColor, kFillModeSolid);

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
