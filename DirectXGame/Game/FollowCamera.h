#pragma once
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Input.h"

class FollowCamera
{
public:
	//	初期化
	void Initialize(ViewProjection viewProjection);
	//	更新
	void Update();

private:
	//	ビュープロジェクション
	ViewProjection viewProjection_;
	
	//	追従対象
	const WorldTransform* target_ = nullptr;

	//	ゲームパッドの状態を得る変数
	XINPUT_STATE joyState{};

public:
	//	追従対象の設定
	void SetTarget(const WorldTransform* target);

	/// <summary>
	/// ビュープロジェクションを取得
	/// </summary>
	/// <returns>ビュープロジェクション</returns>
	const ViewProjection& GetViewProjection() { return viewProjection_; }

};

