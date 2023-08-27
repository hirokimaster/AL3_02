#include "EnemyStateLeave.h"
#include "BaseEnemyState.h"

void StateLeave::Update(Enemy* pEnemy) {

	// 移動量
	pEnemy->SetVelocity({-0.2f, 0.2f, 0.2f});
	// 移動処理
	pEnemy->Move();
}