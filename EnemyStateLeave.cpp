#include "EnemyStateLeave.h"

void StateLeave::Update() {

	Vector3 move = {0, 0, 0};
	// 移動速度
	const float enemySpeed = 0.2f;
	// 移動
	move.x -= enemySpeed;
	move.y += enemySpeed;
	worldTransform_.translation_ = enemy_->GetPos();
	worldTransform_.translation_ = Vec3Add(worldTransform_.translation_, move);

}