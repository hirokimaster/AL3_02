#pragma once
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Input.h"
#include "PlayerBullet.h"
#include "Sprite.h"
#include <list>

// 自キャラ
class Player {

public:
	// コンストラクタ
	Player();
	// デストラクタ
	~Player();
	// 初期化
	void Initialize(Model* model, uint32_t textureHandle, Vector3& position);

	// 更新
	void Update(const ViewProjection& viewProjection);

	// 描画
	void Draw(ViewProjection viewProjection);

	// 回転
	void Rotate();

	// 攻撃
	void Attack();

	// ワールド座標を取得
	Vector3 GetWorldPosition();

	// 衝突判定
	void OnCollision();

	// 弾リストを取得
	const std::list<PlayerBullet*>& GetBullets() const { return bullets_; }

    /// <summary>
    /// 親となるワールドトランスフォームをセット
    /// </summary>
	/// <param name="parent">親となるワールドトランスフォーム</param>
	void SetParent(const WorldTransform* parent);

	Vector3 GetWorldPosition3DReticle();

	// レティクルの処理
	void Reticle(const ViewProjection& viewProjection, const Vector2& position);

    /// <summary>
    /// UI描画
    /// </summary>
	void DrawUI();


private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	// キーボード入力
	Input* input_ = nullptr;
	// 弾
	std::list<PlayerBullet*> bullets_;
    // 3Dレティクル用ワールドトランスフォーム
	WorldTransform worldTransform3DReticle_;
	// 2Dレティクル用スプライト
	Sprite* sprite2DReticle_ = nullptr;
	
};
