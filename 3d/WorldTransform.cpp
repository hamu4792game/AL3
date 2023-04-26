#include "3d/WorldTransform.h"

void WorldTransform::UpdateMatrix() {
	//	スケール、回転、平行移動を合成して行列を計算する
	matWorld_ = myMatWorld_.MakeAffineMatrix(scale_, rotation_, translation_);
	//	定数バッファに定数する
	TransferMatrix();
	
}
