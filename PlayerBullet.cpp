#include "PlayerBullet.h"
#include <cassert>
#include "WorldTransform.h"

// 初期化
void PlayerBullet::Initialize(Model* model, const Vector3& position) {
	// NULLポインタチェック
	assert(model);

	// メンバ変数に引数を記録
	model_ = model;
	// テクスチャ読み込み
	textureHandle_ = TextureManager::Load("black2.png");

	// ワールドトランスフォームの初期化
	// ワールド変換の初期化
	worldTransform_.Initialize();

	// 初期座標をセット
	worldTransform_.translation_ = position;

}

// 更新
void PlayerBullet::Update() {
	// 行列を定数バッファに転送
	worldTransform_.UpdateMatrix();

}

// 描画
void PlayerBullet::Draw(const ViewProjection& viewProjection) {
	// モデルの描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);



}