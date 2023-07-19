#include "Game/Player.h"
#include <cassert>
#include "ImGuiManager.h"

Player::Player() {

}

Player::~Player() {

}

void Player::Initialize(std::array < std::shared_ptr < Model>, 4> model, Vector3 pos) {
	//	NULLポインタチェック
	for (auto& i : model)
	{
		assert(i);
	}
	//	シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
	
	//	受け取ったデータをメンバ変数に記録する
	this->model_ = model;
	//	ワールド変換の初期化
	this->worldTransform_.Initialize();
	this->worldTransform_.translation_ = pos;

	for (auto& i : parts)
	{
		i.Initialize();
		i.parent_ = &worldTransform_;
	}
	//	パーツ毎の設定
	parts[0].translation_ = Vector3{ 0.0f,1.5f,0.0f };
	parts[1].translation_ = Vector3{ 0.0f,0.0f,0.0f };
	parts[2].translation_ = Vector3{ -0.2f,1.7f,0.0f };
	parts[3].translation_ = Vector3{ 0.2f,1.7f,0.0f };

}

void Player::Update() {
	
	
	//	キャラクターの移動ベクトル
	Vector3 move = {0.0f, 0.0f, 0.0f};
	//	キャラクターの移動速さ
	const float kCharacterSpeed = 0.2f;
	
	//	ジョイスティック状態取得
	if (Input::GetInstance()->GetJoystickState(0,joyState))
	{
		if (joyState.Gamepad.sThumbLX || joyState.Gamepad.sThumbLY)
		{
			move.x = static_cast<float>(joyState.Gamepad.sThumbLX);
			move.z = static_cast<float>(joyState.Gamepad.sThumbLY);
		}
	}
	//	押した方向で移動ベクトルを変更（左右）
	if (input_->PushKey(DIK_A)) {
		move.x -= kCharacterSpeed;
	}
	else if (input_->PushKey(DIK_D)) {
		move.x += kCharacterSpeed;
	};
	//	上下
	if (input_->PushKey(DIK_W)) {
		move.z += kCharacterSpeed;
	}
	else if (input_->PushKey(DIK_S)) {
		move.z -= kCharacterSpeed;
	};

	//	移動があれば更新
	if (move.x != 0.0f || move.y != 0.0f || move.z != 0.0f)
	{
		//	移動量の正規化
		move = Normalize(move) * kCharacterSpeed;
		//	移動ベクトルをカメラの角度だけ回転させる
		move = TransformNormal(move, MakeRotateMatrix(viewProjection_->rotation_));

		//	移動方向に見た目を合わせる
		worldTransform_.rotation_.y = std::atan2f(move.x, move.z);
	}
	//	座標移動（ベクトルの加算）
	worldTransform_.translation_ += move;
	

	//	行列更新
	worldTransform_.UpdateMatrix();
	//	partsに親座標の足して更新
	for (auto& i : parts)
	{
		i.UpdateMatrix();
	}

	//	キャラクターの座標を画面表示する処理
	ImGui::Begin("Player");
	ImGui::Text(
	    "player : %0.2f,%0.2f,%0.2f", worldTransform_.translation_.x, worldTransform_.translation_.y,
	    worldTransform_.translation_.z);
	ImGui::DragFloat3("te", &worldTransform_.rotation_.x, 0.1f);
	//ImGui::DragFloat3("tu", &parts[2].rotation_.x, 0.1f);
	
	ImGui::End();

}

void Player::Draw(ViewProjection& viewProjection) {

	for (uint16_t i = 0u; i < 4u; i++)
	{
		model_[i]->Draw(parts[i], viewProjection);
	}
}

