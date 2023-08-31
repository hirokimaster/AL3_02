#include "Player.h"
#include <cassert>
#include "Vector3.h"
#include "Mathfunction.h"
#include "ImGuiManager.h"

Player::Player() {}

Player::~Player() { 
	// bulletの解放
	for (PlayerBullet* bullet : bullets_) {
		 delete bullet;
	}

	delete sprite2DReticle_;

}

// 初期化
void Player::Initialize(Model* model, uint32_t textureHandle, Vector3& position) {
	// NULLポインタチェック
	assert(model);

	// レティクル用テクスチャ取得
	uint32_t textureReticle = TextureManager::Load("2DReticle.png");

	// スプライト生成
	sprite2DReticle_ = Sprite::Create(textureReticle, {640, 360}, {1, 1, 1, 1}, {0.5, 0.5});

	// メンバ変数に引数を記録
	model_ = model;
	textureHandle_ = textureHandle;
	worldTransform_.translation_ = position;

	// ワールド変換の初期化
	worldTransform_.Initialize();

	// シングルトンインスタンスを取得する
	input_ = Input::GetInstance();

	// 3Dレティクルのワールドトランスフォーム初期化
	worldTransform3DReticle_.Initialize();
}

// 更新
void Player::Update(const ViewProjection& viewProjection) {

	// 行列を定数バッファに転送
	worldTransform_.UpdateMatrix();

	// 移動ベクトル
	Vector3 move = {0, 0, 0};

	// 移動速度
	const float kCharacterSpeed = 0.2f;

	// デスフラグが立った弾を削除
	bullets_.remove_if([](PlayerBullet* bullet){
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	// ゲームパッドの状態を得る変数(XINPUT)
	XINPUT_STATE joyState;

	// ゲームパッド状態取得
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		 move.x += (float)joyState.Gamepad.sThumbLX / SHRT_MAX * kCharacterSpeed;
		 move.y += (float)joyState.Gamepad.sThumbLY / SHRT_MAX * kCharacterSpeed;
	}


	// 自機回転
	Rotate();

	// 移動処理
	worldTransform_.translation_ = Vec3Add(worldTransform_.translation_, move);

	// 攻撃
	Attack();

	// 弾更新
	for (PlayerBullet* bullet : bullets_) {
		bullet->Update();
	
	}

	/*
	// 移動限界
	const float kMoveLimitX = 34.0f;
	const float kMoveLimitY = 18.0f;

	// 範囲を超えない処理
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, kMoveLimitY);
	*/


	POINT mousePosition;
	// マウス座標(スクリーン座標)を取得する
	GetCursorPos(&mousePosition);

	// クライアントエリア座標に変換する
	HWND hwnd = WinApp::GetInstance()->GetHwnd();
	ScreenToClient(hwnd, &mousePosition);

	Vector2 spritePosition = sprite2DReticle_->GetPosition();

	// ジョイスティック状態取得
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		spritePosition.x += (float)joyState.Gamepad.sThumbRX / SHRT_MAX * 5.0f;
		spritePosition.y -= (float)joyState.Gamepad.sThumbRY / SHRT_MAX * 5.0f;
        // スプライトの座標変更を反映
		sprite2DReticle_->SetPosition(spritePosition);
	
	}

	// レティクル
	Reticle(viewProjection, Vector2((float)spritePosition.x, (float)spritePosition.y));

	
	// デバック
	/* ImGui::Begin("Player");
	ImGui::Text(
	    "Player X[%.03f].Y[%.03f].Z[%.03f]", worldTransform_.translation_.x,
	    worldTransform_.translation_.y, worldTransform_.translation_.z);
	ImGui::End();*/

}

// 描画
void Player::Draw(ViewProjection viewProjection) {

	// 3Dモデルを描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);


	// 弾描画
	for (PlayerBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	
	}

}

void Player::DrawUI() {

	sprite2DReticle_->Draw();
}

// 自機回転
void Player::Rotate() {
	// 回転速さ[ラジアン/frame]
	const float kRotSpeed = 0.02f;

	// 押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_LEFT)) {
		worldTransform_.rotation_.y -= kRotSpeed;

	} else if (input_->PushKey(DIK_RIGHT)) {
		worldTransform_.rotation_.y += kRotSpeed;
	}
}

// 攻撃
void Player::Attack() {

		XINPUT_STATE joyState;

		// ゲームパッド未接続なら何もせず抜ける
		if (!Input::GetInstance()->GetJoystickState(0, joyState)) {
			return;
		}

		// Rトリガーを押していたら
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {

		    // 弾を生成し、初期化
		    const float kBulletSpeed = 0.2f;
		    Vector3 velocity;
		    PlayerBullet* newBullet = new PlayerBullet();
		    Vector3 WorldPos;
		    WorldPos = GetWorldPosition();
		    // 自機から照準オブジェクトのベクトル
		    Vector3 ReticleWorldPos = GetWorldPosition3DReticle();
		    velocity = Vec3Subtract(ReticleWorldPos, WorldPos);
		    velocity = Normalize(velocity);
		    velocity = Vec3Multiply(kBulletSpeed, velocity);
		    newBullet->Initialize(model_, WorldPos, velocity);

		    // 速度ベクトルを自機の向きに合わせて回転させる
		    velocity = TransformNormal(velocity, worldTransform_.matWorld_);

		    // 弾をセット
		    bullets_.push_back(newBullet);
		}

}

// ワールド座標を取得
Vector3 Player::GetWorldPosition() {
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

// 3Dレティクルのワールド座標
Vector3 Player::GetWorldPosition3DReticle() {
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldTransform3DReticle_.matWorld_.m[3][0];
	worldPos.y = worldTransform3DReticle_.matWorld_.m[3][1];
	worldPos.z = worldTransform3DReticle_.matWorld_.m[3][2];

	return worldPos;

}

// 衝突判定
void Player::OnCollision() {}

// 親子関係を結ぶ
void Player::SetParent(const WorldTransform* parent) { 
	worldTransform_.parent_ = parent;
}

// レティクル
void Player::Reticle(const ViewProjection& viewProjection, const Vector2& position) {

	// スプライトのレティクルに座標設定
	sprite2DReticle_->SetPosition(position);
	
	// ビューポート行列
	Matrix4x4 matViewport =
	    MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);

	// ビュープロジェクションビューポート合成行列
	Matrix4x4 matVPN =
	    Multiply(viewProjection.matView, Multiply(viewProjection.matProjection, matViewport));
	// 逆行列を計算
	Matrix4x4 matInverseVPN = Inverse(matVPN);

	// スクリーン座標
	Vector3 posNear = Vector3((float)position.x, (float)position.y, 1);
	Vector3 posFar = Vector3((float)position.x, (float)position.y, 0);

	// スクリーン座標からワールド座標系へ
	posNear = Transform(posNear, matInverseVPN);
	posFar = Transform(posFar, matInverseVPN);

	// マウスレイの方向
	Vector3 mouseDirection = Vec3Subtract(posNear, posFar);
	mouseDirection = Normalize(mouseDirection);
	// カメラから照準オブジェクトの距離
	const float kDistanceTestObject = 100.0f;
	worldTransform3DReticle_.translation_ = Vec3Multiply(kDistanceTestObject, mouseDirection);
	worldTransform3DReticle_.UpdateMatrix();

	/*
	ImGui::Begin("Player");
	ImGui::Text("2DReticle:(%f,%f)", position.x, position.y);
	ImGui::Text("Near:(%+.2f,%+.2f,%+.2f", posNear.x, posNear.y, posNear.z);
	ImGui::Text("Far:(%+.2f,%+.2f,%+.2f", posFar.x, posFar.y, posFar.z);
	ImGui::Text(
	    "3DReticle:(%+.2f,%+.2f,%+.2f", worldTransform3DReticle_.translation_.x,
	    worldTransform3DReticle_.translation_.y, worldTransform3DReticle_.translation_.z);
	ImGui::End();
	*/

}