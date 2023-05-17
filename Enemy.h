#pragma once

#include <Model.h>
#include <WorldTransform.h>
#include <ViewProjection.h>
#include <memory>
#include "Input.h"
#include "MyMatrix4x4/MyMatrix4x4.h"

class Enemy
{
public:
	Enemy();
	~Enemy();

public:
	//	初期化
	void Initialize(std::shared_ptr<Model> model, uint32_t textureHandle);

	//	更新
	void Update();

	//	描画
	void Draw(ViewProjection& viewProjection);

private:
	//	ワールド変換データ
	WorldTransform worldTransform_;
	//	モデル
	std::shared_ptr<Model> model_;
	//	テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	//	マトリックス
	MyMatrix4x4 matrix;
};

