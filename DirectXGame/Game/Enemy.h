#pragma once
#include "BaseClass/BaseCharacter.h"

class Enemy : public BaseCharacter
{
public:
	//	初期化
	void Initialize(const std::vector<std::shared_ptr<Model>>& models, Vector3 pos) override;
	//	更新
	void Update() override;

private:


};