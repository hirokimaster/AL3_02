#include "EnemyBullet.h"
#include "Mathfunction.h"
#include <cassert>

// 初期化
void EnemyBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {
	// NULLポインタチェック
	assert(model);

	model_ = model;
	// テクスチャ読み込み
	textureHandle_ = TextureManager::Load("block2.png");

	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	// 初期座標をセット
	worldTransform_.translation_ = position;
    // 速度
	velocity_ = velocity;

}

// 更新
void EnemyBullet::Update() {
	
	worldTransform_.translation_ = Vec3Add(worldTransform_.translation_, velocity_);
	// ワールドトランスフォームの更新
	worldTransform_.UpdateMatrix();

	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}

}

// 描画
void EnemyBullet::Draw(const ViewProjection& viewProjection) {
	// モデルの描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);	

}