#include "Enemy.h"
#include "Player.h"
#include "Mathfunction.h"
#include "GameScene.h"
#include <cassert>
#include <list>

// コンストラクタ
Enemy::Enemy(){};

// デストラクタ
Enemy::~Enemy(){};

// 初期化
void Enemy::Initialize(Model* model, uint32_t textureHandle, const Vector3& position) {
	// NULLポインタチェック
	assert(model);

	// メンバ変数に引数を記録
	model_ = model;
	textureHandle_ = textureHandle;
	
	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;

	ApproachInitialize();

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

// 攻撃
void Enemy::Fire() {
	
	assert(player_);
	assert(gameScene_);

	// 弾の速度
	const float kBulletSpeed = 0.05f;

	Vector3 playerWorldPos = player_->GetWorldPosition(); // 自キャラのワールド座標を取得
	Vector3 enemyWorldPos = GetWorldPosition(); // 敵キャラのワールド座標を取得
	Vector3 diff = Vec3Subtract(playerWorldPos, enemyWorldPos); // 差分ベクトルを求める
	Normalize(diff); // 正規化
	Vector3 velocity = Vec3Multiply(kBulletSpeed, diff); // ベクトルの速度

	// 弾を生成して初期化
	EnemyBullet* newBullet = new EnemyBullet();
	newBullet->Initialize(model_, worldTransform_.translation_ , velocity);
	// 弾をセット
	gameScene_->AddEnemyBullet(newBullet);
}

// 描画
void Enemy::Draw(ViewProjection viewProjection) {
	
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
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
	worldTransform_.translation_ = Vec3Add(worldTransform_.translation_, move);
	// 規定の位置に到達したら離脱
	/* if (worldTransform_.translation_.z < 0.0f) {
		phase_ = Phase::Leave;
	}*/

}

void Enemy::Leave(Vector3& move) {
	// 移動速度
	const float enemySpeed = 0.2f;
	// 移動
	move.x -= enemySpeed;
	move.y += enemySpeed;
	worldTransform_.translation_ = Vec3Add(worldTransform_.translation_, move);
}

// ワールド座標を取得
Vector3 Enemy::GetWorldPosition() {
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

// 衝突判定
void Enemy::OnCollision() { 
	isDead_ = true;
}