#include "Enemy.h"
#include "Mathfunction.h"
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

	// 移動ベクトル
	Vector3 move = {0, 0, 0};

	// フェーズ
	switch (phase_) { 
	case Phase::Approach:
	default:
		ApproachUpdate(move);
		break;

    case Phase::Leave:
		Leave(move);
		break;

	}
}

// 描画
void Enemy::Draw(ViewProjection viewProjection) {
	
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

}

// フェーズの更新
void Enemy::ApproachUpdate(Vector3& move) {
	// 移動速度
	const float enemySpeed = 0.2f;
	// 移動
	move.z -= enemySpeed;
	worldTransform_.translation_ = VectorAdd(worldTransform_.translation_, move);
	// 規定の位置に到達したら離脱
	if (worldTransform_.translation_.z < 0.0f) {
		phase_ = Phase::Leave;
	}

}

void Enemy::Leave(Vector3& move) {
	// 移動速度
	const float enemySpeed = 0.2f;
	// 移動
	move.x -= enemySpeed;
	move.y += enemySpeed;
	worldTransform_.translation_ = VectorAdd(worldTransform_.translation_, move);
}