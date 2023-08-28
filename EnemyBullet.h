#pragma once
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Collider.h"

// 敵の弾
class EnemyBullet : public Collider {
public:

	// 初期化
	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);

	// 更新
	void Update();

	// 描画
	void Draw(const ViewProjection& viewProjection);	

	// 弾のデスフラグ
	bool IsDead() const { return isDead_; }

	// 衝突判定
	//void OnCollision();
	void OnCollision() override;

	// ワールド座標を取得
	//Vector3 GetWorldPosition();
	Vector3 GetWorldPosition() const override;

private:
	// ワールド変換
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	//速度
	Vector3 velocity_;
	// 弾の寿命
	static const int32_t kLifeTime = 60 * 5;
	// デスタイマー
	int32_t deathTimer_ = kLifeTime;
	// デスフラグ
	bool isDead_ = false;
};