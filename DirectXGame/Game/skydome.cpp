#include <Game/skydome.h>

Skydome::Skydome()
{
}

Skydome::~Skydome()
{
}

void Skydome::Initialize(std::shared_ptr<Model> model, const Vector3& position, const Vector3& velocity)
{
}

void Skydome::Update()
{
}

void Skydome::Draw(const ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}
