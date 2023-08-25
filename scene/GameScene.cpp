#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "Player.h"
#include "Enemy.h"
#include "AxisIndicator.h"

GameScene::GameScene() {}

GameScene::~GameScene() {
	// プレイヤー
	delete player_;
	// エネミー
	delete enemy_;
	// デバックカメラ
	delete debugCamera_;
	// 天球
	delete modelSkydome_;
	// レールカメラ
	delete railCamera_;

}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// 軸方向の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	// 軸方向表示が参照するビュープロジェクションを指定する（アドレス渡し）
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	// ビュープロジェクションの初期化
	viewProjection_.Initialize();
	viewProjection_.farZ = 500.0f;

	// プレイヤー
	textureHandlePlayer_ = TextureManager::Load("Player.png");
	modelPlayer_ = Model::Create();
	player_ = new Player();
	Vector3 playerPosition(0, 0, 50);
	player_->Initialize(modelPlayer_, textureHandlePlayer_, playerPosition);

	// エネミー
	texturehandleEnemy_ = TextureManager::Load("naitou5.jpg");
	modelEnemy_ = Model::Create();
	enemy_ = new Enemy();
	enemy_->Initialize(modelEnemy_, texturehandleEnemy_);

	// 天球
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);
	skydome_ = new Skydome();
	skydome_->Initialize(modelSkydome_);

	// レールカメラ
	railCamera_ = new RailCamera();
	railCamera_->Initialize({0,0,0}, worldTransform_.rotation_);
	// 自キャラとレールカメラの親子関係を結ぶ
	player_->SetParent(&railCamera_->GetWorldTransform());
	
	// デバックカメラ生成
	debugCamera_ = new DebugCamera(1920, 1080);

	// 敵キャラに自キャラのアドレスを渡す
	enemy_->SetPlayer(player_);


}

void GameScene::Update() { 

	player_->Update(); 

	if (enemy_ != nullptr) {
		enemy_->Update();
	}

	CheckAllCollisions();

	skydome_->Update();

	railCamera_->Update();

	viewProjection_.matView = railCamera_->GetViewProjection().matView;
	viewProjection_.matProjection = railCamera_->GetViewProjection().matProjection;
	// ビュープロジェクション行列の転送
	viewProjection_.TransferMatrix();
	
	

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
	   
	skydome_->Draw(viewProjection_);

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
	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();
	// 敵弾リストの取得
	const std::list<EnemyBullet*>& enemyBullets = enemy_->GetBullets();

	

#pragma region 自キャラと敵弾の当たり判定
	// 自キャラの座標
	posA = player_->GetWorldPosition();

	// 自キャラと敵弾すべての当たり判定
	for (EnemyBullet* bullet : enemyBullets) {
	    // 敵弾の座標
		posB = bullet->GetWorldPosition();

		// AとBの距離
		float distance = (posB.x - posA.x) * (posB.x - posA.x) +
		                 (posB.y - posA.y) * (posB.y - posA.y) +
		                 (posB.z - posA.z) * (posB.z - posA.z);

		// 半径
	    float R1 = 1.0f;
	    float R2 = 1.0f;

		
		// 球と球の交差判定
		if (distance <= (R1 + R2) * (R1 + R2)) {
		    // 自キャラの衝突時コールバックを呼び出す
			player_->OnCollision();
			// 敵弾の衝突時コールバックを呼び出す
			bullet->OnCollision();
		}
	}

#pragma endregion

#pragma region 自弾と敵キャラの当たり判定
	// 自キャラの座標
	posA = enemy_->GetWorldPosition();

	// 自弾と敵すべての当たり判定
	for (PlayerBullet* bullet : playerBullets) {
		// 自弾の座標
		posB = bullet->GetWorldPosition();

		// AとBの距離
		float distance = (posB.x - posA.x) * (posB.x - posA.x) +
		                 (posB.y - posA.y) * (posB.y - posA.y) +
		                 (posB.z - posA.z) * (posB.z - posA.z);

		// 半径
		float R1 = 1.0f;
		float R2 = 1.0f;

		// 球と球の交差判定
		if (distance <= (R1 + R2) * (R1 + R2)) {
			// 敵キャラの衝突時コールバックを呼び出す
			enemy_->OnCollision();
			// 自弾の衝突時コールバックを呼び出す
			bullet->OnCollision();
		}
	}

#pragma endregion

#pragma region 自弾と敵弾の当たり判定

	// 自弾と敵すべての当たり判定
	for (PlayerBullet* bullet : playerBullets) {
		// 自キャラの座標
		posA = bullet->GetWorldPosition();

		for (EnemyBullet* enemyBullet : enemyBullets) {
			// 敵弾の座標
			posB = enemyBullet->GetWorldPosition();

			// AとBの距離
			float distance = (posB.x - posA.x) * (posB.x - posA.x) +
			                 (posB.y - posA.y) * (posB.y - posA.y) +
			                 (posB.z - posA.z) * (posB.z - posA.z);

			// 半径
			float R1 = 1.0f;
			float R2 = 1.0f;

			// 球と球の交差判定
			if (distance <= (R1 + R2) * (R1 + R2)) {
				// 自キャラの衝突時コールバックを呼び出す
				bullet->OnCollision();
				// 敵弾の衝突時コールバックを呼び出す
				enemyBullet->OnCollision();
			}
		}
	}
	

#pragma endregion

}
