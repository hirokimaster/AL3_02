#pragma once
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "EnemyBullet.h"
#include "BaseEnemyState.h"
#include <list>

// 自機クラスの前方宣言
class Player;

// GameSceneの前方宣言
class GameScene;

/*
// 行動フェーズ
enum class Phase {
	Approach, // 接近する
	Leave,    // 離脱する

};*/

// 敵
class Enemy {
public:
	// コンストラクタ
	Enemy();

	// デストラクタ
	~Enemy();

	// 初期化
	void Initialize(Model* model, uint32_t textureHandle, const Vector3& position);

	// 更新
	void Update();

	// 描画
	void Draw(ViewProjection viewProjection);

	// 攻撃
	void Fire();

	void changeState(BaseEnemyState* newState);

	void Move();

	// getter
	Vector3 GetVelocity() { return velocity_; }
	Vector3 GetPos(){ return worldTransform_.translation_;}

	// setter
	void SetVelocity(Vector3 velocity);

	// 接近フェーズ初期化	
	//void ApproachInitialize();

	// フェーズごとの更新
	//void ApproachUpdate();
	//void Leave();

	// 発射間隔
	//static const int kFireInterval = 60;

	// Playerを借りる
	void SetPlayer(Player* player) { player_ = player; }

	//ワールド座標を取得
	Vector3 GetWorldPosition();

	// 衝突判定
	void OnCollision();

	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }

	// 敵のデスフラグ
	bool IsDead() const { return isDead_; }

private:

	// ワールドトランスフォーム
	WorldTransform worldTransform_;
	// モデル
	Model* model_;
	// テクスチャ
	uint32_t textureHandle_ = 0u;
	// 発射タイマー
	//int32_t shotTimer = 0;
	// 自キャラ
	Player* player_ = nullptr;
    // ゲームシーン
	GameScene* gameScene_ = nullptr;
	// デスフラグ
	bool isDead_ = false;
	// フェーズ
	//Phase phase_ = Phase::Approach;
	// メンバ関数ポインタ
	//static void (Enemy::*phaseFuncTable[])();

	// 速度
	Vector3 velocity_;

	BaseEnemyState* state;
};
