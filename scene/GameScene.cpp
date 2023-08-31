#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "Player.h"
#include "Enemy.h"
#include "AxisIndicator.h"
#include <fstream>

GameScene::GameScene() {}

GameScene::~GameScene() {
	// プレイヤー
	delete player_;
	// エネミー
	for (Enemy* enemy : enemys_) {
		delete enemy;
	}

	// デバックカメラ
	delete debugCamera_;
	// 天球
	delete modelSkydome_;
	// レールカメラ
	delete railCamera_;
	// 敵弾
	for (EnemyBullet* bullet : enemyBullets_) {
		delete bullet;
	}

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

	// レティクルのテクスチャ
	TextureManager::Load("2DReticle.png");

	// タイトル（2Dスプライト)
	textureHandleTitle_ = TextureManager::Load("title.png");
	spriteTitle_ = Sprite::Create(textureHandleTitle_, {0, 0});

	// ゲームオーバー
	textureHandleGameOver_ = TextureManager::Load("gameover.png");
	spriteGameOver_ = Sprite::Create(textureHandleGameOver_, {0, 0});

	// プレイヤー
	textureHandlePlayer_ = TextureManager::Load("player.jpg");
	modelPlayer_ = Model::Create();
	player_ = new Player();
	Vector3 playerPosition(0, 0, 50);
	player_->Initialize(modelPlayer_, textureHandlePlayer_, playerPosition);

	// エネミー
	texturehandleEnemy_ = TextureManager::Load("naitou5.jpg");
	modelEnemy_ = Model::Create();
	LoadEnemyPopData();

	// 天球
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);
	skydome_ = new Skydome();
	skydome_->Initialize(modelSkydome_);

	// レールカメラ
	railCamera_ = new RailCamera();
	railCamera_->Initialize({0,0,0}, worldTransform_.rotation_);
	// 自キャラとレールカメラの親子関係を結ぶ
	player_->SetParent(&railCamera_->GetWorldTransform());

	// スコア数値
	textureHandleNumber_ = TextureManager::Load("number.png");
	for (int i = 0; i < 5; i++) {
		spriteNumber_[i] = Sprite::Create(textureHandleNumber_, {300.0f + i * 26, 0});
	}

	// スコア
	textureHandleScore_ = TextureManager::Load("score.png");
	spriteScore_ = Sprite::Create(textureHandleScore_, {150.0f, 0});

	// ライフ
	for (int i = 0; i < 3; i++) {
		spriteLife_[i] = Sprite::Create(textureHandlePlayer_, {800.0f + i * 60, 0});
		spriteLife_[i]->SetSize({40, 40});
	}




	// デバックカメラ生成
	debugCamera_ = new DebugCamera(1920, 1080);

}
// スコア表示
void GameScene::DrawScore() {

	int eachNumber[5] = {};
	int number = gameScore_;
	int keta = 10000;
	for (int i = 0; i < 5; i++) {
		eachNumber[i] = number / keta;
		number = number % keta;
		keta = keta / 10;
	}

	// 描画
	for (int i = 0; i < 5; i++) {
		spriteNumber_[i]->SetSize({32, 64});
		spriteNumber_[i]->SetTextureRect({32.0f * eachNumber[i], 0}, {32, 64});
		spriteNumber_[i]->Draw();
	}

	for (int i = 0; i < playerLife_; i++) {
		spriteLife_[i]->Draw();
	}

	spriteScore_->Draw();
}

void GameScene::Update() { 

	XINPUT_STATE joyState;

	// ゲームパッド未接続なら何もせず抜ける
	if (!Input::GetInstance()->GetJoystickState(0, joyState)) {
		return;
	}

	// シーン切り替え
	switch (sceneMode_) {

	case 0:
		
	player_->Update(viewProjection_);

		// 敵更新
		for (Enemy* enemy : enemys_) {
			enemy->Update();
		}

		UpdateEnemyPopCommands();

		// デスフラグが立ったら敵を削除
		enemys_.remove_if([](Enemy* enemy) {
			if (enemy->IsDead()) {
				delete enemy;
				return true;
			}
			return false;
		});

		// 敵弾更新
		for (EnemyBullet* bullet : enemyBullets_) {
			bullet->Update();
		}

		// デスフラグが立ったら敵弾を削除
		enemyBullets_.remove_if([](EnemyBullet* bullet) {
			if (bullet->IsDead()) {
				delete bullet;
				return true;
			}
			return false;
		});

		CheckAllCollisions();

		skydome_->Update();

		railCamera_->Update();

		viewProjection_.matView = railCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = railCamera_->GetViewProjection().matProjection;
		// ビュープロジェクション行列の転送
		viewProjection_.TransferMatrix();

		for (Enemy* enemy : enemys_) {
			if (enemy->IsDead()) {
				gameScore_ += 100;
			}
		}

		if (playerLife_ <= 0) {
			gameOver_ = true;
		}

		if (gameOver_) {
			sceneMode_ = 2;
			

		}

		break;

	case 1:

		// エンターキー押したとき　(タイトルからプレイ）
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A) {

			sceneMode_ = 0;
	
		}
		
		break;

	case 2:
		if (gameOver_) {

			// 自弾リストの取得
			const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();

			for (Enemy* enemy : enemys_) {
				enemy->OnCollision();
			}

			for (EnemyBullet* enemyBullet : enemyBullets_) {
				enemyBullet->OnCollision();
			}

			for (PlayerBullet* playerBullet : playerBullets) {
				playerBullet->OnCollision();
			}
	
			if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A) {
				GameStart();
				sceneMode_ = 1;
			}
		}
		break;
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
	
	// シーン切り替え
	switch (sceneMode_) {

	case 1:
		// タイトル表示
		spriteTitle_->Draw();

		break;

	case 2:
		if (gameOver_) {
			// ゲームオーバー
			spriteGameOver_->Draw();
		}
		break;
	}
	

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>

	switch (sceneMode_) {

	case 0:
		
		player_->Draw(viewProjection_);

		// 敵
		for (Enemy* enemy : enemys_) {
			enemy->Draw(viewProjection_);
		}

		// 敵弾
		for (EnemyBullet* bullet : enemyBullets_) {
			bullet->Draw(viewProjection_);
		}

		skydome_->Draw(viewProjection_);

		break;

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

	switch (sceneMode_) {

	case 0:

		DrawScore();
		player_->DrawUI();

		break;
	}
	
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

// 衝突判定
void GameScene::CheckAllCollisions() {
	// 判定対象AとBの座標
	Vector3 posA, posB;

	// 自弾リストの取得
	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();
	// 敵弾リストの取得
	const std::list<EnemyBullet*>& enemyBullets = enemyBullets_;

	// 敵リストの取得
	const std::list<Enemy*>& enemys = enemys_;

	

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

			playerLife_ -= 1;
		}
	}

#pragma endregion

#pragma region 自弾とすべての敵キャラの当たり判定

	for (Enemy* enemy : enemys) {
		// 敵キャラの座標
		posA = enemy->GetWorldPosition();

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
				enemy->OnCollision();
				// 自弾の衝突時コールバックを呼び出す
				bullet->OnCollision();
			
			}
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

void GameScene::AddEnemyBullet(EnemyBullet* enemyBullet) {
	// リストに登録する
	enemyBullets_.push_back(enemyBullet);
}

// 敵発生
void GameScene::EnemyPop(const Vector3& position) {
	Enemy* enemy = new Enemy();
	enemy->Initialize(modelEnemy_, texturehandleEnemy_, position);
	// エネミーにゲームシーンを渡す
	enemy->SetGameScene(this);
	enemys_.push_back(enemy); // 登録
	// 敵キャラに自キャラのアドレスを渡す
	enemy->SetPlayer(player_);
}

// 敵発生データの読み込み
void GameScene::LoadEnemyPopData() {
	// ファイルを開く
	std::ifstream file;
	file.open("enemyPop.csv");
	assert(file.is_open());

	// ファイルの内容を文字列ストリームにコピー
	enemyPopCommands << file.rdbuf();

	// ファイルを閉じる
	file.close();
	
}

// 敵発生コマンドの更新
void GameScene::UpdateEnemyPopCommands() {
	// 待機処理
	if (wait) {
		waitTimer--;
		if (waitTimer <= 0) {
		    // 待機完了
			wait = false;
		}
		return;
	}

	// 1行分の文字列を入れる変数
	std::string line;

	// コマンド実行ループ
	while (getline(enemyPopCommands, line)) {
	    // 1行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		std::string word;
		// ,区切りで行の先頭文字列を取得
		getline(line_stream, word, ',');

		// "//"から始まる行はコメント
		if (word.find("//") == 0) {
		    // コメント行を飛ばす
			continue;
		}

		// POPコマンド
		if (word.find("POP") == 0) {
		    // x座標
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());

			// y座標
			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());

			// z座標
			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());

			//敵を発生させる
			EnemyPop(Vector3(x, y, z));

		} else if (word.find("WAIT") == 0) {
			getline(line_stream, word, ',');

			// 待ち時間
			int32_t waitTime = atoi(word.c_str());

			// 待機開始
			wait = true;
			waitTimer = waitTime;

			// コマンドループを抜ける
			break;
		}
	}
}

void GameScene::GameStart() {

	Vector3 playerPosition(0, 0, 50);
	player_->Initialize(modelPlayer_, textureHandlePlayer_, playerPosition);


	LoadEnemyPopData();

	gameOver_ = false;
	playerLife_ = 3;
	gameScore_ = 0;

	railCamera_->Initialize({0, 0, 0}, worldTransform_.rotation_);
}
