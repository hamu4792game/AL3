#pragma once
#include <Model.h>
#include <WorldTransform.h>
#include <ViewProjection.h>
#include <memory>
#include "Input.h"
#include "MyMatrix4x4/MyMatrix4x4.h"

class Player {
public:
	//	コンストラクタ
	Player();
	//	デストラクタ
	~Player();
	//	初期化
	void Initialize(std::array < std::shared_ptr < Model>, 4> model, Vector3 pos);
	
	//	更新
	void Update();

	//	描画
	void Draw(ViewProjection& viewProjection);


private:
	//	キーボード入力
	Input* input_ = nullptr;

	//	ワールド変換データ
	WorldTransform worldTransform_;
	//	モデル
	std::array < std::shared_ptr < Model>, 4> model_;
	//	個々のパーツ body(親),head,RArm,LArm
	std::array<WorldTransform, 4> parts;

	//	ゲームパッドの状態を得る変数
	XINPUT_STATE joyState{};

	//	カメラ
	const ViewProjection* viewProjection_ = nullptr;

public:
	//	
	const WorldTransform& GetWorldTransform() { return worldTransform_; };

	//	
	void SetViewProjection(const ViewProjection* view) { viewProjection_ = view; };

private:	//	animation
	//	浮遊ギミックの媒介変数
	float floatingParameter_ = 0.0f;

	//	浮遊ギミック初期化
	void InitializeFloatingGimmick();
	//	浮遊ギミック更新処置
	void UpdataFloatingGimmick();
};
