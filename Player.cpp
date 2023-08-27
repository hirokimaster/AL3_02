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

	// 入力した方向で移動ベクトルを変更
	// 左
	if (input_->PushKey(DIK_A)) {
		move.x -= kCharacterSpeed;

		// 右
	} else if (input_->PushKey(DIK_D)) {
		move.x += kCharacterSpeed;

	}
	// 上
	if (input_->PushKey(DIK_W)) {
		move.y += kCharacterSpeed;

		// 下
	} else if (input_->PushKey(DIK_S)) {
		move.y -= kCharacterSpeed;
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

	// 移動限界
	const float kMoveLimitX = 34.0f;
	const float kMoveLimitY = 18.0f;

	// 範囲を超えない処理
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, kMoveLimitY);

	// 自機のワールド座標から3Dレティクルのワールド座標を計算
	// 自機から3Dレティクルへの距離
	const float kDistancePlayerTo3DReticle = 50.0f;
	// 自機から3Dレティクルへのオフセット(Z+向き)
	Vector3 offset = {0, 0, 1.0f};
	// 自機のワールド行列の回転を反映
	offset = TransformNormal(offset, worldTransform_.matWorld_);
	// ベクトルの長さを整える
	offset = Normalize(offset);
	offset = Vec3Multiply(kDistancePlayerTo3DReticle, offset);
	// 3Dレティクルの座標を設定
	worldTransform3DReticle_.translation_.x = worldTransform_.translation_.x + offset.x;
	worldTransform3DReticle_.translation_.y = worldTransform_.translation_.y + offset.y;
	worldTransform3DReticle_.translation_.z = worldTransform_.translation_.z + offset.z;
	worldTransform3DReticle_.UpdateMatrix();

	// 3Dレティクルの座標から2Dレティクルのスクリーン座標を計算
	Vector3 positionReticle = GetWorldPosition3DReticle();

	// ビューポート行列
	Matrix4x4 matViewport =
	    MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);

	// ビュー行列とプロジェクション行列、ビューポート行列を合成する
	Matrix4x4 matViewProjectionViewport =
	    Multiply(viewProjection.matView, Multiply(viewProjection.matProjection, matViewport));

	// ワールド→スクリーン座標変換(ここで3Dから2Dになる)
	positionReticle = Transform(positionReticle, matViewProjectionViewport);

	// スプライトのレティクルに座標設定
	sprite2DReticle_->SetPosition(Vector2(positionReticle.x, positionReticle.y));

	
	// デバック
	ImGui::Begin("Debug1");
	ImGui::Text(
	    "Player X[%.03f].Y[%.03f].Z[%.03f]", worldTransform_.translation_.x,
	    worldTransform_.translation_.y, worldTransform_.translation_.z);
	ImGui::End();

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
	// 処理
	if (input_->TriggerKey(DIK_SPACE)) {
		
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