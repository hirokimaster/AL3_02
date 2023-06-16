#pragma once
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

///敵
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

private:

	// ワールドトランスフォーム
	WorldTransform worldTransform_;
	// モデル
	Model* model_;
	// テクスチャ
	uint32_t textureHandle_ = 0u;

};
