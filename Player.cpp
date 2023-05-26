#include "Player.h"
#include <cassert>

// 初期化
void Player::Initialize(Model* model, uint32_t textureHandle) {
	// NULLポインタチェック
	assert(model);

	// メンバ変数に引数を記録
	model_ = model;
	textureHandle_ = textureHandle;

	// ワールド変換の初期化
	worldTransform_.Initialize();
}

// 更新
void Player::Update() {
	// 行列を定数バッファに転送
	worldTransform_.TransferMatrix();
}

// 描画
void Player::Draw(ViewProjection viewProjection) {

	// 3Dモデルを描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}