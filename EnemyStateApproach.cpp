#include "EnemyStateApproach.h"
#include "EnemyStateLeave.h"

void StateApproach::Update(){

	Vector3 move = {0, 0, 0};
	// 攻撃
	// 発射タイマーをデクリメント
	--shotTimer;


	// 指定時間に達した
	if (shotTimer <= 0) {
		// 弾を発射
		enemy_->Fire();
		// 発射タイマーの初期化
		shotTimer = kFireInterval;
	}

	// 移動速度
	const float enemySpeed = 0.2f;
	// 移動
	move.z -= enemySpeed;
	
	worldTransform_.translation_ = enemy_->GetPos();
	worldTransform_.translation_ = Vec3Add(worldTransform_.translation_, move);

	// 規定の位置に到達したら離脱
	if (worldTransform_.translation_.z < 50.0f) {
		enemy_->changeState(new StateLeave);
	}

}