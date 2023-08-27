#include "Enemy.h"
#include "Player.h"
#include "EnemyStateApproach.h"
#include "EnemyStateLeave.h"
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

	// 最初の状態
	state = new StateApproach();
	//ApproachInitialize();

}

// 更新
void Enemy::Update(){

	state->Update(this);

	// 現在フェーズの関数を実行
	//(this->*phaseFuncTable[static_cast<size_t>(phase_)])();

    // 行列の更新
	worldTransform_.UpdateMatrix();

	/*
	// フェーズ
	switch (phase_) { 
	case Phase::Approach:
	default:
		ApproachUpdate(move);
		break;

    case Phase::Leave:
		Leave(move);
		break;

	}*/
	
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
    velocity_ = Vec3Multiply(kBulletSpeed, diff); // ベクトルの速度

	// 弾を生成して初期化
	EnemyBullet* newBullet = new EnemyBullet();
	newBullet->Initialize(model_, worldTransform_.translation_ , velocity_);
	// 弾をセット
	gameScene_->AddEnemyBullet(newBullet);
}

// 描画
void Enemy::Draw(ViewProjection viewProjection) {
	
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

void Enemy::changeState(BaseEnemyState* newState) { 
	state = newState;
}

// 移動
void Enemy::Move() {
	worldTransform_.translation_ = Vec3Add(worldTransform_.translation_, velocity_);
}

// setter
void Enemy::SetVelocity(Vector3 velocity) { 
	velocity_.x = velocity.x;
	velocity_.y = velocity.y;
	velocity_.z = velocity.z;
}

// フェーズの関数テーブル
/* void (Enemy::*Enemy::phaseFuncTable[])(){
    &Enemy::ApproachUpdate, // 接近
    &Enemy::Leave           // 離脱
};*/

/*
// 接近フェーズの初期化
void Enemy::ApproachInitialize() {
	// 発射タイマーを初期化
	shotTimer = kFireInterval;
	
}

// フェーズの更新
void Enemy::ApproachUpdate() {

	

}

void Enemy::Leave() {

}
*/

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