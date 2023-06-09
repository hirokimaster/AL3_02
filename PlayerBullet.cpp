#include "PlayerBullet.h"
#include <cassert>
#include "WorldTransform.h"
#include "Mathfunction.h"

// 初期化
void PlayerBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {
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

	// 引数で受け取った速度をメンバ変数に代入
	velocity_ = velocity;

}

// 更新
void PlayerBullet::Update() {
	// 座標を移動させる（1フレーム分の移動量を足しこむ）
	worldTransform_.translation_ = VectorAdd(worldTransform_.translation_, velocity_);

	// 行列を定数バッファに転送
	worldTransform_.UpdateMatrix();

	// 時間経過で消す
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}

}

// 描画
void PlayerBullet::Draw(const ViewProjection& viewProjection) {
	// モデルの描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);



}