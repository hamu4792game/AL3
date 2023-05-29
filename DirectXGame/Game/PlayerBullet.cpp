#include "Game/PlayerBullet.h"
#include <cassert>

PlayerBullet::PlayerBullet() {}

PlayerBullet::~PlayerBullet() {}

void PlayerBullet::Initialize(
    std::shared_ptr<Model> model, const Vector3& position, const Vector3& velocity)
{
	//	NULLポインタチェック
	assert(model);

	model_ = model;
	//	テクスチャ読み込み
	textureHandle_ = TextureManager::Load("white1x1.png");
	//	ワールド変換の初期化
	worldTransform_.Initialize();
	//	引数で受け取った初期座標をリセット
	worldTransform_.translation_ = position;
	//	velocityのset
	velocity_ = velocity;

}

void PlayerBullet::Update() { 
	//	座標を更新
	worldTransform_.translation_ += velocity_;

	//	行列の更新
	worldTransform_.UpdateMatrix();

	//	時間経過でデス
	if (--deathTimer_ <= 0)
	{
		isDead_ = true;
	}

}

void PlayerBullet::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}
