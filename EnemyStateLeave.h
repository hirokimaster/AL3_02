#pragma once

#include "BaseEnemyState.h"
#include "Enemy.h"

class StateLeave : public BaseEnemyState {
public:

	void Update(Enemy* pEnemy);
};