#include "Player.h"
#include <cassert>

Player::Player() {

}

Player::~Player() {

}

void Player::Initialize(Model* model, uint32_t textureHandle) {
	//	NULLポインタチェック
	assert(model);
	//	受け取ったデータをメンバ変数に記録する
	this->model_ = model;
	this->textureHandle_ = textureHandle;
	//	ワールド変換の初期化
	this->worldTransform_.Initialize();
}

void Player::Update() { 
	worldTransform_.TransferMatrix();

}

void Player::Draw(ViewProjection viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}