#include "Player.h"
#include <cassert>
#include "Mathfunction.h"
#include "ImGuiManager.h"

// 初期化
void Player::Initialize(Model* model, uint32_t textureHandle) {
	// NULLポインタチェック
	assert(model);

	// メンバ変数に引数を記録
	model_ = model;
	textureHandle_ = textureHandle;

	// ワールド変換の初期化
	worldTransform_.Initialize();

	// シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
}

// 更新
void Player::Update() {

	// 行列を定数バッファに転送
	worldTransform_.matWorld_ = MakeAffineMatrix(
	    worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix();

	// 移動ベクトル
	Vector3 move = {0, 0, 0};

	// 移動速度
	const float kCharacterSpeed = 0.2f;

	// 入力した方向で移動ベクトルを変更

	// 左
	if (input_->PushKey(DIK_A)) {
		move.x -= kCharacterSpeed;

		// 右
	} else if (input_->PushKey(DIK_D)) {
		move.x += kCharacterSpeed;

	}
	// 上
	if (input_->PushKey(DIK_W)) {
		move.y += kCharacterSpeed;

		// 下
	} else if (input_->PushKey(DIK_S)) {
		move.y -= kCharacterSpeed;
	}

	// 移動処理
	worldTransform_.translation_ = Add(worldTransform_.translation_, move);

	// 移動限界
	const float kMoveLimitX = 34.0f;
	const float kMoveLimitY = 18.0f;

	// 範囲を超えない処理
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, kMoveLimitY);

	// デバック
	ImGui::Begin("Debug1");
	ImGui::Text(
	    "Player X[%.03f].Y[%.03f].Z[%.03f]", worldTransform_.translation_.x,
	    worldTransform_.translation_.y, worldTransform_.translation_.z);
	ImGui::End();

}

// 描画
void Player::Draw(ViewProjection viewProjection) {

	// 3Dモデルを描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}