#pragma once
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Mathfunction.h"

/// <summary>
/// レールカメラ
/// </summary>
class RailCamera {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(const Vector3& position, const Vector3& rotate);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

    /// <summary>
    /// viewProjection取得関数
    /// </summary>
	const ViewProjection& GetViewProjection() { return viewProjection_; }

    /// <summary>
    /// worldTransform取得
    /// </summary>
	const WorldTransform& GetWorldTransform() { return worldTransform_; }
    
private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// ビュープロジェクション
	ViewProjection viewProjection_;
	// 移動速度
	Vector3 velocity_ = {0.0f, 0.0f, 0.0f};
	// 角度
	Vector3 rotate_ = {0.0f, 0.0f, 0.0f};

};