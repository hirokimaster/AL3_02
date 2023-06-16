#include "Enemy.h"
#include <cassert>

// コンストラクタ
Enemy::Enemy(){};

// デストラクタ
Enemy::~Enemy(){};

// 初期化
void Enemy::Initialize(Model* model, uint32_t textureHandle) {
	// NULLポインタチェック
	assert(model);

	// メンバ変数に引数を記録
	model_ = model;
	textureHandle_ = textureHandle;

	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();

	// 敵の初期座標
	worldTransform_.translation_ = {0.0f, 5.0f, 50.0f};

}

// 更新
void Enemy::Update(){
    // 行列の更新
	worldTransform_.UpdateMatrix();

	worldTransform_.translation_.z -= 0.2f;


}

// 描画
void Enemy::Draw(ViewProjection viewProjection) {
	
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

}