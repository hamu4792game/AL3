#include "Game/Enemy.h"
#include <cassert>
#include "ImGuiManager.h"
#include "Game/Player.h"

Enemy::Enemy()
{
	move = { 0.0f,0.0f,0.0f };
}

Enemy::~Enemy()
{
}

void Enemy::Initialize(std::shared_ptr<Model> model, uint32_t textureHandle, Vector3 position)
{
	//	NULLポインタチェック
	assert(model);

	//	受け取ったデータをメンバ変数に記録する
	this->model_ = model;
	this->textureHandle_ = textureHandle;
	//	ワールド変換の初期化
	this->worldTransform_.Initialize();
	//	初期座標の設定
	this->worldTransform_.translation_ = position;

}

//	staticで宣言したメンバ関数ポインタテーブルの実体
void (Enemy::* Enemy::pPhaseTable[])() = {
	&Enemy::Move1,
	&Enemy::Move2,
};


void Enemy::Update()
{
	////	デスフラグの立った弾を削除
	//bullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet) {
	//	return bullet->IsDead();
	//	});


	//	現在フェーズの関数を実行
	(this->*pPhaseTable[static_cast<size_t>(phase_)])();

	//	座標移動
	/*worldTransform_.translation_ += move;*/

	//	時間の加算
	timer--;
	//	キャラクター攻撃処理
	if (timer <= 0) {
		coolDawn = false;
		Fire();
		timer = 20;
	}
	else {
		coolDawn = true;
	}

	//	アフィン変換
	worldTransform_.myMatWorld_ = MakeAffineMatrix(
		worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	//	行列を定数バッファに転送
	worldTransform_.TransferMatrix();

	//	キャラクターの座標を画面表示する処理
	ImGui::Begin("Enemy");
	ImGui::Text(
		"enemy : %0.2f,%0.2f,%0.2f", worldTransform_.translation_.x, worldTransform_.translation_.y,
		worldTransform_.translation_.z);
	ImGui::End();
}

void Enemy::Draw(ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	////	弾更新
	//for (auto i = bullets_.begin(); i != bullets_.end(); i++) {
	//	(*i)->Draw(viewProjection);
	//}
}

void Enemy::Reset()
{
	this->worldTransform_.translation_ = { 10.0f,0.0f,50.0f };
	phase_ = Phase::Apprpach;
}

void Enemy::Move1()
{
	move = { 0.0f,0.0f,-0.5f };
}

void Enemy::Move2()
{
	move = { -0.5f,0.2f,0.0f };
}

void Enemy::Fire()
{
	assert(player_);
	//	弾の速度
	const float kBulletSpeed = 2.0f;
	velocity = { 0, 0, kBulletSpeed };

	Vector3 playerPos = player_->GetWorldPosition();
	Vector3 enemyPos = worldTransform_.translation_;
	Vector3 differencialVector = playerPos - enemyPos;
	differencialVector = Normalize(differencialVector);
	velocity = differencialVector * kBulletSpeed;

	//	弾を登録する
	//bullets_.push_back(std::make_unique<EnemyBullet>());
	//	今追加したものの初期化処理
	//	rbegin() 逆イテレーター
	//(*bullets_.rbegin())->Initialize(model_, worldTransform_.translation_, velocity);
	
}

Vector3 Enemy::GetWorldPosition()
{
	Vector3 worldPos;
	//	ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldTransform_.myMatWorld_.m[3][0];
	worldPos.y = worldTransform_.myMatWorld_.m[3][1];
	worldPos.z = worldTransform_.myMatWorld_.m[3][2];
	return worldPos;
}

void Enemy::OnCollision()
{

}
