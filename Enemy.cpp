#include "Enemy.h"
#include "Mathfunction.h"
#include <cassert>
#include <list>

// コンストラクタ
Enemy::Enemy(){};

// デストラクタ
Enemy::~Enemy(){ 
	for (EnemyBullet* bullet : bullets_) {
		delete bullet;
	}
};

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
	worldTransform_.translation_ = {10.0f, 5.0f, 50.0f};

	ApproachInitialize();

}

// 更新
void Enemy::Update(){
    // 行列の更新
	worldTransform_.UpdateMatrix();

	// デスフラグが立ったら弾を削除
	bullets_.remove_if([](EnemyBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

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
	
	for (EnemyBullet* bullet : bullets_) {
		bullet->Update();
	}
}

// 攻撃
void Enemy::Fire() {
	
	// 弾の速度
	const float kBulletSpeed = -1.0f;
	Vector3 velocity(0, 0, kBulletSpeed);

	// 弾を生成して初期化
	EnemyBullet* newBullet = new EnemyBullet();
	newBullet->Initialize(model_, worldTransform_.translation_ , velocity);
	// 弾をセット
	bullets_.push_back(newBullet);
}

// 描画
void Enemy::Draw(ViewProjection viewProjection) {
	
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	// 弾
	for (EnemyBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}

// 接近フェーズの初期化
void Enemy::ApproachInitialize() {
	// 発射タイマーを初期化
	shotTimer = kFireInterval;
	
}

// フェーズの更新
void Enemy::ApproachUpdate(Vector3& move) {

	// 攻撃
	// 発射タイマーをデクリメント
	--shotTimer;

	// 指定時間に達した
	if (shotTimer <= 0) {
		// 弾を発射
		Fire();
		// 発射タイマーの初期化
		shotTimer = kFireInterval;
	}

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