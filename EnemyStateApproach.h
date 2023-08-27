#pragma once
#include "BaseEnemyState.h"
#include "Enemy.h"

class EnemyStateLeave;
class StateApproach : public BaseEnemyState {
public:
	void Update(Enemy* pEnemy);

	// 発射間隔
	static const int kFireInterval = 60;

private:
	int32_t shotTimer = 60;
};