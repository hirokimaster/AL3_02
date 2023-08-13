﻿#pragma once
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "EnemyBullet.h"
#include <list>

// 自機クラスの前方宣言
class Player;

// 行動フェーズ
enum class Phase {
    Approach, // 接近する
    Leave,    // 離脱する

};

// 敵
class Enemy {
public:
	// コンストラクタ
	Enemy();

	// デストラクタ
	~Enemy();

	// 初期化
	void Initialize(Model* model, uint32_t textureHandle);

	// 更新
	void Update();

	// 描画
	void Draw(ViewProjection viewProjection);

	// 攻撃
	void Fire();

	// 接近フェーズ初期化
	void ApproachInitialize();

	// swich　フェーズごとの更新
	void ApproachUpdate(Vector3& move);
	void Leave(Vector3& move);

	// 発射間隔
	static const int kFireInterval = 60;

	// Playerを借りる
	void SetPlayer(Player* player) { player_ = player; }

	//ワールド座標を取得
	Vector3 GetWorldPosition();

	// 衝突判定
	void OnCollision();

	// 弾リストを取得
	const std::list<EnemyBullet*>& GetBullets() const { return bullets_; }

private:

	// ワールドトランスフォーム
	WorldTransform worldTransform_;
	// モデル
	Model* model_;
	// テクスチャ
	uint32_t textureHandle_ = 0u;
	// フェーズ
	Phase phase_ = Phase::Approach;
	// 弾
	std::list<EnemyBullet*> bullets_;
	// 発射タイマー
	int32_t shotTimer = 0;
	// 自キャラ
	Player* player_ = nullptr;

};