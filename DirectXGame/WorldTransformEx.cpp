#include "WorldTransform.h"


void WorldTransform::UpdateMatrix() {
	
	// スケール、回転、平行移動を合成して行列を計算
	matWorld_ = utility_->MakeAffineMatrix(scale_, rotation_, translation_);

	if (parent_) {
		matWorld_ = utility_->Multiply(matWorld_, parent_->matWorld_);
	}

	TransferMatrix();
}