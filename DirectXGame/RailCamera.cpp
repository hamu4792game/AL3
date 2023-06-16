#include "RailCamera.h"
#include "MyMatrix4x4/MyMatrix4x4.h"
#include <imgui.h>

RailCamera::RailCamera()
{

}

RailCamera::~RailCamera()
{

}

void RailCamera::Initialize(ViewProjection viewProjection)
{
	//ワールドトランスフォームの初期設定
	//	引数でワールド座標を受け取ってワールドトランスフォームに設定
	worldTransform_.translation_ = viewProjection.translation_;
	//	引数で回転角を受け取ってワールドトランスフォームに設定
	worldTransform_.rotation_ = viewProjection.rotation_;
	
	//	ビュープロジェクションの初期化
	viewProjection_.farZ = 2000.0f;
	viewProjection_.Initialize();

}

void RailCamera::Updata()
{
	//	カメラの移動
	//worldTransform_.rotation_.y -= 0.005f;

	//	アフィン変換
	worldTransform_.myMatWorld_ = MakeAffineMatrix(
		worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	
	//	カメラオブジェクトのワールド行列からビュー行列を計算する
	viewProjection_.myMatView = Inverse(worldTransform_.myMatWorld_);

	//	ImGui
	ImGui::Begin("Camera");
	//ImGui::Text("cameraPos : %0.2f,%0.2f,%0.2f", worldTransform_.translation_.x, worldTransform_.translation_.y, worldTransform_.translation_.z);
	//ImGui::Text("cameraRot : %0.2f,%0.2f,%0.2f", worldTransform_.rotation_.x, worldTransform_.rotation_.y, worldTransform_.rotation_.z);
	ImGui::DragFloat3("cameraPos", &worldTransform_.translation_.x, 0.01f);
	ImGui::DragFloat3("cameraRot", &worldTransform_.rotation_.x, 0.01f);
	ImGui::End();
}

