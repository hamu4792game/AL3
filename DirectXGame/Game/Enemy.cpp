#include "Enemy.h"
#include <cassert>
#include <imgui.h>
#include "math/Matrix4x4.h"
#include <numbers>
#include <cmath>

void Enemy::Initialize(const std::vector<std::shared_ptr<Model>>& models, Vector3 pos)
{
	//	NULLポインタチェック
	for (auto& i : models)
	{
		assert(i);
	}
	this->models_ = models;
	for (auto& i : this->parts_) {
		i.Initialize();
	}
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
	parts_[1].translation_ = Vector3{ 1.7f,1.1f,0.5f };
	parts_[1].rotation_ = Vector3{ 4.75f,0.0f,0.0f };


}

void Enemy::Update()
{
	ImGui::Begin("enemy");
	ImGui::DragFloat3("to", &worldTransform_.translation_.x, 0.1f);
	ImGui::DragFloat3("ro", &worldTransform_.rotation_.x, 0.1f);
	ImGui::End();

	if (true)
	{
		Vector3 move{ 0.0f,0.0f,0.1f };
		move = TransformNormal(move, worldTransform_.myMatWorld_);
		
		worldTransform_.rotation_.y += 5.0f * std::numbers::pi_v<float> / 180.0f;

		if (worldTransform_.rotation_.y >= std::numbers::pi_v<float> *2.0f)	{
			worldTransform_.rotation_.y -= std::numbers::pi_v<float> *2.0f;
		}

		worldTransform_.translation_ += move;

	}

	//	行列更新
	this->worldTransform_.UpdateMatrix();
	//	partsに親座標の足して更新
	for (auto& i : this->parts_)
	{
		i.UpdateMatrix();
	}

}
