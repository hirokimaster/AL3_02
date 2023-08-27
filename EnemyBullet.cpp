#include "EnemyBullet.h"
#include "Mathfunction.h"
#include <cassert>
#include "Player.h"

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
	// スケール
	worldTransform_.scale_ = {0.5f, 0.5f, 3.0f};
    // 速度
	velocity_ = velocity;

	worldTransform_.rotation_.y = std::atan2(velocity_.x, velocity_.z);

	Vector3 velocityXZ = {velocity_.x, 0.0f, velocity_.z};

	float veloXZ = Length(velocityXZ);

	worldTransform_.rotation_.z = std::atan2(-velocity_.y, veloXZ);

}

// 更新
void EnemyBullet::Update() {

	// ワールドトランスフォームの更新
	worldTransform_.UpdateMatrix();
	
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}

	Vector3 toPlayer;
	toPlayer = Vec3Subtract(player_->GetWorldPosition(), GetWorldPosition());

	float t = 0.01f;

	// 正規化
	Normalize(toPlayer);
	Normalize(velocity_);

	velocity_ = SLerp(toPlayer, GetWorldPosition(), t);

	worldTransform_.rotation_.y = std::atan2(velocity_.x, velocity_.z);

	Vector3 velocityXZ = {velocity_.x, 0.0f, velocity_.z};

	float veloXZ = Length(velocityXZ);

	worldTransform_.rotation_.z = std::atan2(-velocity_.y, veloXZ);

	worldTransform_.translation_ = Vec3Add(worldTransform_.translation_, velocity_);

	
}

// 描画
void EnemyBullet::Draw(const ViewProjection& viewProjection) {
	// モデルの描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);	

}

// 衝突判定
void EnemyBullet::OnCollision() { 
	isDead_ = true;
}

// ワールド座標を取得
Vector3 EnemyBullet::GetWorldPosition() {
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}