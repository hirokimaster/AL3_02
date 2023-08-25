#include "Skydome.h"
#include <cassert>

/// <summary>
/// 初期化
/// </summary>
void Skydome::Initialize(Model* model){
	assert(model);
	model_ = model;

	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();

	// 敵の初期座標
	worldTransform_.translation_ = {0.0f, 0.0f, 0.0f};
	worldTransform_.scale_ = {100.0f, 100.0f, 100.0f};
	
};


/// <summary>
/// 更新
/// </summary>
void Skydome::Update(){

	// 行列の更新
	worldTransform_.UpdateMatrix();
};

/// <summary>
/// 描画
/// </summary>
void Skydome::Draw(ViewProjection viewProjection) { 
	model_->Draw(worldTransform_, viewProjection);
};
