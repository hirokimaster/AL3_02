#include "EnemyStateApproach.h"
#include "EnemyStateLeave.h"

void StateApproach::Update(Enemy* pEnemy){

	// 移動量
	pEnemy->SetVelocity({0.0f, 0.0f, -0.2f});
	// 移動処理
	pEnemy->Move();

	// 発射タイマーをデクリメント
	--shotTimer;

	if (shotTimer <= 0) {
	    // 弾を発射
		pEnemy->Fire();
		// 発射タイマーの初期化
		shotTimer = kFireInterval;
	}

	// 規定の位置に到達したら離脱
	if (pEnemy->GetPos().z < 50.0f) {
		pEnemy->changeState(new StateLeave);
	}

}