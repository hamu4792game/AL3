#include "Game/EnemyBullet.h"
#include <cassert>

EnemyBullet::EnemyBullet() {};
EnemyBullet::~EnemyBullet() {}

void EnemyBullet::Initialize(std::shared_ptr<Model> model, const Vector3& position, const Vector3& velocity)
{
	//	NULLポインタチェック
	assert(model);

	model_ = model;
	//	テクスチャ読み込み
	textureHandle_ = TextureManager::Load("red1x1.png");
	//	ワールド変換の初期化
	worldTransform_.Initialize();
	//	引数で受け取った初期座標をリセット
	worldTransform_.translation_ = position;
	//	velocityのset
	velocity_ = velocity;

}

void EnemyBullet::Initialize(const Vector3& position, const Vector3& velocity)
{
	//	ワールド変換の初期化
	worldTransform_.Initialize();
	//	引数で受け取った初期座標をリセット
	worldTransform_.translation_ = position;
	//	velocityのset
	velocity_ = velocity;
}

void EnemyBullet::Update()
{
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

void EnemyBullet::Draw(const ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

void EnemyBullet::OnCollision()
{
	isDead_ = true;
}


