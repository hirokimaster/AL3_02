#pragma once
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"


// 自キャラの弾
class PlayerBullet {
public:
	// 初期化
	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);

	// 更新
	void Update();

	// 描画
	void Draw(const ViewProjection& viewProjection);	

	// デスフラグ
	bool IsDead() const { return isDead_; }

	// 衝突判定
	void OnCollision();

	// ワールド座標を取得
	Vector3 GetWorldPosition();

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	// 速度
	Vector3 velocity_;
	// 寿命<frm>
	static const int32_t kLifeTime = 60 * 5;
	// デスタイマー
	int32_t deathTimer_ = kLifeTime;
	// デスフラグ
	bool isDead_ = false;

};
