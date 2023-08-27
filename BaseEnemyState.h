#pragma once
#include "Enemy.h"
#include "Mathfunction.h"
#include "WorldTransform.h"

class Enemy;

class BaseEnemyState {
public:
    void Update();


	WorldTransform worldTransform_;
	Enemy* enemy_;
};
