#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Player.h"
#include "Enemy.h"
//#include "DebugCamera.h"
#include "Skydome.h"
#include "RailCamera.h"
#include <list>
#include <sstream>

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 衝突判定と応答
	/// </summary>
	void CheckAllCollisions();

	/// <summary>
	/// 敵弾を追加する
	/// </summary>
	/// <param name="enemyBullet">敵弾</param>
	void AddEnemyBullet(EnemyBullet* enemyBullet);

	// 敵発生コマンド
	std::stringstream enemyPopCommands;

    /// <summary>
    /// 敵発生データの読み込み
    /// </summary>
	void LoadEnemyPopData();

	/// <summary>
	/// 敵発生コマンドの更新
	/// </summary>
	void UpdateEnemyPopCommands();

	/// <summary>
	/// 敵発生処理
	/// </summary>
	void EnemyPop(const Vector3& position);

    /// <summary>
    /// game開始前初期化
    /// </summary>
	void GameStart();

	// スコア数値
	void DrawScore();


private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	// デバッグカメラ有効
	//bool isDebugCameraActive_ = false;
	// デバックカメラ
	//DebugCamera* debugCamera_ = nullptr;

	WorldTransform worldTransform_;
	ViewProjection viewProjection_;

	// プレイヤー
	Player* player_ = nullptr;
	uint32_t textureHandlePlayer_ = 0;
	Model* modelPlayer_ = nullptr;

	// エネミー
	std::list<Enemy*> enemys_;
	uint32_t texturehandleEnemy_ = 0;
	Model* modelEnemy_ = nullptr;

	// 天球
	Skydome* skydome_ = nullptr;
	Model* modelSkydome_ = nullptr;

	// レールカメラ
	RailCamera* railCamera_ = nullptr;

	// 敵弾
	std::list<EnemyBullet*> enemyBullets_;

	// 待機中フラグ
	bool wait = false;
	// 待機中タイマー
	uint32_t waitTimer = 0;

	// タイトル（スプライト）
	uint32_t textureHandleTitle_ = 0;
	Sprite* spriteTitle_ = nullptr;
	// シーン
	int32_t sceneMode_ = 1;

	// ゲームオーバー
	uint32_t textureHandleGameOver_ = 0;
	Sprite* spriteGameOver_ = nullptr;
	bool gameOver_ = false;

	// スコア
	uint32_t textureHandleNumber_ = 0;
	Sprite* spriteNumber_[5];
	uint32_t textureHandleScore_ = 0;
	Sprite* spriteScore_ = nullptr;
	int32_t gameScore_ = 0;
	Sprite* spriteLife_[3];
	int32_t playerLife_ = 3;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
