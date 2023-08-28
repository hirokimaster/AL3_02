#pragma once
#include <stdlib.h>
#include "Vector3.h"

/// <summary>
/// 衝突判定オブジェクト
/// </summary>
class Collider {
public:
	// 半径を取得
	float GetRadius() { return radius_; }
    // 半径を設定
	void SetRadius(float radius) { radius_ = radius; }

	//衝突時に呼ばれる関数
	virtual void OnCollision() = 0;

	// ワールド座標を取得
	virtual Vector3 GetWorldPosition() const = 0;


private:

	// 半径
	float radius_ = 1.0f;

};

