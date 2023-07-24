#include "BaseCharacter.h"
#include <cassert>

void BaseCharacter::Initialize(const std::vector<std::shared_ptr<Model>>& models, Vector3 pos)
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
}

void BaseCharacter::Update()
{
	//	行列更新
	this->worldTransform_.UpdateMatrix();
	//	partsに親座標の足して更新
	for (auto& i : this->parts_)
	{
		i.UpdateMatrix();
	}
}

void BaseCharacter::Draw(const ViewProjection& viewProjection)
{
	for (uint16_t i = 0u; i < parts_.size(); i++)
	{
		models_[i]->Draw(parts_[i], viewProjection);
	}
}
