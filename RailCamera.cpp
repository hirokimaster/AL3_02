#include "RailCamera.h"

/// <summary>
/// 初期化
/// </summary>
void RailCamera::Initialize(const Vector3& position, const Vector3& rotate) {
	// ワールドトランスフォーム
	worldTransform_.translation_ = position;
	worldTransform_.rotation_ = rotate;
	worldTransform_.scale_ = {1.0f, 1.0f, 1.0f};

	viewProjection_.farZ = 2000;
	// ビュープロジェクションの初期化
	viewProjection_.Initialize();
}

/// <summary>
/// 更新
/// </summary>
void RailCamera::Update() {

	// 移動
	//const float cameraSpeed = -0.5f; // 移動スピード
	//velocity_.z = cameraSpeed;
	worldTransform_.translation_ = Vec3Add(worldTransform_.translation_, velocity_);

	// 回転
	//const float kRotSpeed = 0.001f; // 回転速さ[ラジアン/frame]
	//rotate_.y = kRotSpeed;
	worldTransform_.rotation_ = Vec3Add(worldTransform_.rotation_, rotate_);

	// ワールドトランスフォームのワールド行列再計算
	worldTransform_.matWorld_ = MakeAffineMatrix(
	    worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	// カメラオブジェクトのワールド行列からビュー行列を計算する
	viewProjection_.matView = Inverse(worldTransform_.matWorld_);

	// カメラの座標を画面表示する処理
	ImGui::Begin("Camera");
	ImGui::SliderFloat3("CmeraTranslation ", &worldTransform_.translation_.x, -50.0f,50.0f);
	ImGui::SliderFloat3("CmeraRotate ", &worldTransform_.rotation_.x, 0.0f, 10.0f);
	ImGui::End();	

}
