#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "Player.h"
#include "Enemy.h"
#include "AxisIndicator.h"

GameScene::GameScene() {
	// ビュープロジェクションの初期化
	viewProjection_.Initialize();

	// プレイヤー
	textureHandlePlayer_ = TextureManager::Load("Player.png");
	modelPlayer_ = Model::Create();
	player_ = new Player();
	player_->Initialize(modelPlayer_, textureHandlePlayer_);

	// エネミー
	texturehandleEnemy_ = TextureManager::Load("naitou5.jpg");
	modelEnemy_ = Model::Create();
	enemy_ = new Enemy();
	enemy_->Initialize(modelEnemy_, texturehandleEnemy_);
}

GameScene::~GameScene() {
	// プレイヤー
	delete player_;
	// エネミー
	delete enemy_;
	// デバックカメラ
	delete debugCamera_;

}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// デバックカメラ生成
	debugCamera_ = new DebugCamera(1920, 1080);
	
	// 敵キャラに自キャラのアドレスを渡す
	enemy_->SetPlayer(player_);

	// 軸方向の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	// 軸方向表示が参照するビュープロジェクションを指定する（アドレス渡し）
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

}

void GameScene::Update() { 

	player_->Update(); 

	if (enemy_ != nullptr) {
		enemy_->Update();
	}

	#ifdef _DEBUG
	// デバックカメラ有効フラグ
	if (input_->TriggerKey(DIK_F)) {
		isDebugCameraActive_ = true;
	}
#endif // DEBUG

	// カメラの処理
	if (isDebugCameraActive_) {
		// デバックカメラ更新
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		// ビュープロジェクション行列の転送
		viewProjection_.TransferMatrix();

	} else {
		// ビュープロジェクション行列の更新と転送
		viewProjection_.UpdateMatrix();
	}

}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>

	player_->Draw(viewProjection_);

	if (enemy_ != nullptr) {
		enemy_->Draw(viewProjection_);
	}
	   
	

	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::CheckAllCollisions() {
	// 判定対象AとBの座標
	Vector3 posA, posB;

	// 自弾リストの取得
	const std::list<PlayerBullet*>& playerBullets = player_->GetBullet();
	// 敵弾リストの取得
	const std::list<EnemyBullet*>& enemyBullets = enemy_->GetBullets();

#pragma region 自キャラと敵弾の当たり判定
	// 自キャラの座標
	posA = player_->GetWorldPosition();

	// 自キャラと敵弾すべての当たり判定
	for (EnemyBullet* bullet : enemyBullets) {
	    // 敵弾の座標
		posB = enemy_->GetWorldPosition();

		// AとBの距離
		
		// 球と球の交差判定
		
	}

#pragma endregion

#pragma region 自弾と敵キャラの当たり判定
#pragma endregion

#pragma region 自弾と敵弾の当たり判定
#pragma endregion

}
