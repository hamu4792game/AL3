#include "3d/WorldTransform.h"

void WorldTransform::UpdateMatrix() {
	//	スケール、回転、平行移動を合成して行列を計算する
	myMatWorld_ = MakeAffineMatrix(scale_, rotation_, translation_);

	//	親があれば親のワールド行列を掛ける
	if (parent_)
	{
		myMatWorld_ = myMatWorld_ * parent_->myMatWorld_;
	}

	//	定数バッファに定数する
	TransferMatrix();
	
}
