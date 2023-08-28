#include "CollisionManager.h"

// 衝突判定
void CollisionManager::CheckAllCollisions() {

	// 自弾リストの取得
	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();
	// 敵弾リストの取得
	const std::list<EnemyBullet*>& enemyBullets = gameScene_->GetBullets();

	// 敵リストの取得
	const std::list<Enemy*>& enemys = enemys_;

	
	// コライダーをリストに登録
	colliders_.push_back(player_);
	for (Enemy* enemy : enemys) {
		colliders_.push_back(enemy);
	}
	// 自弾について
	for (PlayerBullet* bullet : playerBullets) {
		colliders_.push_back(bullet);
	}
	// 敵弾について
	for (EnemyBullet* enemyBullet : enemyBullets) {
		colliders_.push_back(enemyBullet);
	}

	// リスト内のペアを総当たり
	std::list<Collider*>::iterator itrA = colliders_.begin();
	for (; itrA != colliders_.end(); ++itrA) {

		// イテレーターBはイテレーターAの次の要素から回す(重複判定を回避)
		std::list<Collider*>::iterator itrB = itrA;
		itrB++;

		for (; itrB != colliders_.end(); ++itrB) {

			// ペアの当たり判定
			CheckCollisionPair(*itrA, *itrB);
		}
	}
}

// コライダーで衝突判定
void CollisionManager::CheckCollisionPair(Collider* colliderA, Collider* colliderB) {
	// 座標と半径
	Vector3 posA, posB;
	float RA, RB;

	// コライダーA、Bのワールド座標取得
	posA = colliderA->GetWorldPosition();
	RA = colliderA->GetRadius();

	posB = colliderB->GetWorldPosition();
	RB = colliderB->GetRadius();

	// AとBの距離
	float distance = (posB.x - posA.x) * (posB.x - posA.x) + (posB.y - posA.y) * (posB.y - posA.y) +
	                 (posB.z - posA.z) * (posB.z - posA.z);

	// 球と球の交差判定
	if (distance <= (RA + RB) * (RA + RB)) {
		// 衝突フィルタリング
		if (colliderA->GetcollisionAttribute() != colliderB->GetCollisionMask() ||
		    colliderB->GetcollisionAttribute() != colliderA->GetCollisionMask()) {
			return;
		}

		colliderA->OnCollision();
		colliderB->OnCollision();
	}
}