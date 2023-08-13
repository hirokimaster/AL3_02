﻿#pragma once
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Input.h"
#include "PlayerBullet.h"
#include <list>

// 自キャラ
class Player {

public:
	// コンストラクタ
	Player();
	// デストラクタ
	~Player();
	// 初期化
	void Initialize(Model* model, uint32_t textureHandle);

	// 更新
	void Update();

	// 描画
	void Draw(ViewProjection viewProjection);

	// 回転
	void Rotate();

	// 攻撃
	void Attack();

	// ワールド座標を取得
	Vector3 GetWorldPosition();

	// 衝突判定
	void OnCollision();

	// 弾リストを取得
	const std::list<PlayerBullet*>& GetBullet() const { return bullets_; }

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	// キーボード入力
	Input* input_ = nullptr;
	// 弾
	std::list<PlayerBullet*> bullets_;

};
