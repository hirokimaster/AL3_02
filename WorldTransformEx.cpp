#include "WorldTransform.h"
#include "Mathfunction.h"


void WorldTransform::UpdateMatrix() {
	// Scale, Rotate, translateを合成して行列を計算
	matWorld_ = MakeAffineMatrix(scale_, rotation_, translation_);

	// 親があれば親のワールド行列を掛ける
	if (parent_) {
		matWorld_ = Multiply(parent_->matWorld_, matWorld_);
	}

	// 定数バッファの転送
	TransferMatrix();

}
    
