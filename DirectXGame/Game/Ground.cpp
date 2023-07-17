#include "Ground.h"

Ground::Ground()
{
}

Ground::~Ground()
{
}

void Ground::Initialize(std::shared_ptr<Model> model, const Vector3& position)
{
	this->model_ = model;
	this->worldTransform_.Initialize();
	this->worldTransform_.translation_ = position;
	this->worldTransform_.scale_ = { 500.0f,500.0f,500.0f };
}

void Ground::Update()
{
	//	行列の更新
	worldTransform_.UpdateMatrix();
}

void Ground::Draw(const ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection);
}