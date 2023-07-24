#include "Game/Player.h"
#include <cassert>
#include "ImGuiManager.h"
#include <numbers>
#include <cmath>

void Player::Initialize(const std::vector<std::shared_ptr<Model>>& models, Vector3 pos)
{
	//	NULLポインタチェック
	for (auto& i : models)
	{
		assert(i);
	}
	//	シングルトンインスタンスを取得する
	input_ = Input::GetInstance();

	//	受け取ったデータをメンバ変数に記録する
	this->models_ = models;
	//	ワールド変換の初期化
	this->worldTransform_.Initialize();
	this->worldTransform_.translation_ = pos;

	parts_.resize(models_.size());
	for (auto& i : parts_)
	{
		i.Initialize();
		i.parent_ = &parts_[0];
	}
	parts_[0].parent_ = &worldTransform_;
	//	パーツ毎の設定
	parts_[0].translation_ = Vector3{ 0.0f,0.0f,0.0f };
	parts_[1].translation_ = Vector3{ 0.0f,1.5f,0.0f };
	parts_[2].translation_ = Vector3{ -0.2f,1.7f,0.0f };
	parts_[3].translation_ = Vector3{ 0.2f,1.7f,0.0f };

	InitializeFloatingGimmick();
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
	
	//	animation
	UpdataFloatingGimmick();

	//	行列更新
	worldTransform_.UpdateMatrix();
	//	partsに親座標の足して更新
	for (auto& i : parts_)
	{
		i.UpdateMatrix();
	}

	//	キャラクターの座標を画面表示する処理
	ImGui::Begin("Player");
	ImGui::Text(
	    "player : %0.2f,%0.2f,%0.2f", worldTransform_.translation_.x, worldTransform_.translation_.y,
	    worldTransform_.translation_.z);
	ImGui::DragFloat3("te", &worldTransform_.rotation_.x, 0.1f);
	ImGui::DragFloat3("tu", &parts_[2].rotation_.x, 0.1f);
	//ImGui::DragFloat3("to", &parts[2].translation_.x, 0.1f);
	
	ImGui::End();

}

void Player::InitializeFloatingGimmick() {
	floatingParameter_ = 0.0f;
}

void Player::UpdataFloatingGimmick() {
	//	浮遊移動のサイクル<frame>
	const uint16_t cycle = 60;
	//	1フレームでの加算値
	const float step = 2.0f * std::numbers::pi_v<float> / cycle;
	//	パラメーターを1ステップずつ加算
	floatingParameter_ += step;
	//	2πを超えたら0に戻す
	floatingParameter_ = std::fmodf(floatingParameter_, 2.0f * std::numbers::pi_v<float>);

	//	浮遊の振幅<m>
	const float swingWidth = 0.2f;
	//	浮遊を座標に更新反映
	parts_[0].translation_.y = std::sinf(floatingParameter_) * swingWidth;

	//	腕振り
	parts_[2].rotation_.x = std::sinf(floatingParameter_) *  0.5f;
	parts_[3].rotation_.x = std::sinf(floatingParameter_) * -0.5f;

}
