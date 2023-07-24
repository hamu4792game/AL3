#pragma once
#include "Input.h"
#include "BaseClass/BaseCharacter.h"

class Player :public BaseCharacter {
public:

	//	初期化
	void Initialize(const std::vector<std::shared_ptr<Model>>& models, Vector3 pos) override;
	
	//	更新
	void Update() override;

private:
	//	キーボード入力
	Input* input_ = nullptr;

	//	ゲームパッドの状態を得る変数
	XINPUT_STATE joyState{};

	//	カメラ
	const ViewProjection* viewProjection_ = nullptr;

public:
	//	カメラのセット
	void SetViewProjection(const ViewProjection* view) { viewProjection_ = view; };

private:	//	animation
	//	浮遊ギミックの媒介変数
	float floatingParameter_ = 0.0f;

	//	浮遊ギミック初期化
	void InitializeFloatingGimmick();
	//	浮遊ギミック更新処置
	void UpdataFloatingGimmick();
};
