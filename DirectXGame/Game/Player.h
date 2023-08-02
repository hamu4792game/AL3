#pragma once
#include "Input.h"
#include "BaseClass/BaseCharacter.h"
#include <optional>

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
	enum class Behavior {
		kRoot,	// 通常攻撃
		kAttack,// 攻撃中
	};
	Behavior behavior_ = Behavior::kRoot;
	//	次の振る舞いリクエスト
	std::optional<Behavior> behaviorRequest_ = std::nullopt;

public:
	//	カメラのセット
	void SetViewProjection(const ViewProjection* view) { viewProjection_ = view; };

private:	//	animation
	//	浮遊ギミックの媒介変数
	float floatingParameter_ = 0.0f;
	//	攻撃フレーム
	uint16_t attackFrame = 0;


	//	浮遊ギミック初期化
	void InitializeFloatingGimmick();
	//	浮遊ギミック更新処置
	void UpdataFloatingGimmick();
	//	攻撃フレーム初期化
	void InitializeAttack();

	//	通常行動更新
	void BehaviorRootUpdate();
	//	攻撃行動更新
	void BehaviorAttackUpdate();


public:
	//	調整項目の適用
	void ApplyGlobalVariables();

};
