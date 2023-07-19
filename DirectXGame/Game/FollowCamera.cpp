#include "FollowCamera.h"

void FollowCamera::Initialize(ViewProjection viewProjection)
{
	this->viewProjection_ = viewProjection;
}

void FollowCamera::Update()
{
	//	ジョイスティック状態取得
	if (Input::GetInstance()->GetJoystickState(0, joyState))
	{
		float rotateSpeed = 0.1f;

		viewProjection_.rotation_.y += static_cast<float>(joyState.Gamepad.sThumbRX / static_cast<float>(SHRT_MAX) * rotateSpeed);
	}

	if (target_)
	{
		//	追従対象からカメラまでのオフセット
		Vector3 offset = { 0.0f,2.0f,-10.0f };

		//	カメラの角度から回転行列を計算する
		MyMatrix4x4 rotate = MakeRotateMatrix(viewProjection_.rotation_);

		offset = TransformNormal(offset, rotate);

		//	座標をコピーしてオフセット分ずらす
		viewProjection_.translation_ = target_->translation_ + offset;
	}
	//	行列更新
	viewProjection_.UpdateMatrix();
}

void FollowCamera::SetTarget(const WorldTransform* target) { target_ = target; }
