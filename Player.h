﻿#pragma once
#include <Model.h>
#include <WorldTransform.h>
#include <ViewProjection.h>
#include <memory>
#include "Input.h"

class Player {
public:
	//	コンストラクタ
	Player();
	//	デストラクタ
	~Player();
	//	初期化
	void Initialize(std::shared_ptr<Model> model, uint32_t textureHandle);
	
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
	std::shared_ptr<Model> model_;
	//	テクスチャハンドル
	uint32_t textureHandle_ = 0u;


};
