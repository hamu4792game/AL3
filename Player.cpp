#include "Player.h"
#include <cassert>
#include "ImGuiManager.h"

Player::Player() {

}

Player::~Player() {

}

void Player::Initialize(std::shared_ptr<Model> model, uint32_t textureHandle) {
	//	NULLポインタチェック
	assert(model);
	//	シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
	
	//	受け取ったデータをメンバ変数に記録する
	this->model_ = model;
	this->textureHandle_ = textureHandle;
	//	ワールド変換の初期化
	this->worldTransform_.Initialize();
}

void Player::Update() { 
	worldTransform_.TransferMatrix();
	//	キャラクターの移動ベクトル
	Vector3 move = {0, 0, 0};
	//	キャラクターの移動速さ
	const float kCharacterSpeed = 0.2f;
	//	押した方向で移動ベクトルを変更（左右）
	if (input_->PushKey(DIK_LEFT)) {
		move.x -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_RIGHT)) {
		move.x += kCharacterSpeed;
	};
	//	上下
	if (input_->PushKey(DIK_UP)) {
		move.y += kCharacterSpeed;
	} else if (input_->PushKey(DIK_DOWN)) {
		move.y -= kCharacterSpeed;
	};

	//	座標移動（ベクトルの加算）
	worldTransform_.translation_ += move;

	//	アフィン変換
	worldTransform_.matWorld_ = matrix.MakeAffineMatrix(
	    worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	//	行列を定数バッファに転送
	worldTransform_.TransferMatrix();

	//	キャラクターの座標を画面表示する処理
	ImGui::Begin("Player");
	ImGui::Text(
	    "player : %0.2f,%0.2f,%0.2f", worldTransform_.translation_.x, worldTransform_.translation_.y,
	    worldTransform_.translation_.z);
	ImGui::End();

}

void Player::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}