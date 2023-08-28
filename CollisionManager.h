#pragma once

#include "Collider.h"
#include <list>
#include "Enemy.h"
#include "Player.h"
#include "GameScene.h"

// 前方宣言
class GameScene;
class Player;
class Enemy;

class CollisionManager {
public:

	/// <summary>
	/// 衝突判定と応答
	/// </summary>
	void CheckAllCollisions();
	
	// playerのsetter
	void SetPlayer(Player* player) { player_ = player; }
	// enemyのsetter
	void SetEnemy(std::list<Enemy*> enemys) { enemys_ = enemys; }
	// gameSceneのsetter
	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }


private:

	// コライダー二つの衝突判定
	void CheckCollisionPair(Collider* colliderA, Collider* colliderB);

private:

	// コライダーリスト
	std::list<Collider*> colliders_;
	// プレイヤー
	Player* player_ = nullptr;
	// エネミー
	std::list<Enemy*> enemys_;
	// ゲームシーン
	GameScene* gameScene_  =  nullptr;
};
