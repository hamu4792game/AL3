#include <Game/skydome.h>

Skydome::Skydome()
{
}

Skydome::~Skydome()
{
}

void Skydome::Initialize(std::shared_ptr<Model> model, const Vector3& position)
{
	this->model_ = model;
	this->worldTransform_.Initialize();
	this->worldTransform_.translation_ = position;
	this->worldTransform_.scale_ = { 100.0f,100.0f,100.0f };
}

void Skydome::Update()
{
	////	アフィン変換
	//worldTransform_.myMatWorld_ = MakeAffineMatrix(
	//	worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	////	行列を定数バッファに転送
	//worldTransform_.TransferMatrix();
	worldTransform_.UpdateMatrix();
}

void Skydome::Draw(const ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection);
}
