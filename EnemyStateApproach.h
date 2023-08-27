#pragma once

#include "BaseEnemyState.h"


class StateApproach : public BaseEnemyState {
	void Update();

	// 発射間隔
	static const int kFireInterval = 60;
	
private:
	// 発射タイマー
	int32_t shotTimer = 0;

};