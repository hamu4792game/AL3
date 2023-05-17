﻿#include "Enemy.h"
#include <cassert>
#include "ImGuiManager.h"

Enemy::Enemy()
{
	move = { 0.0f,0.0f,0.0f };
}

Enemy::~Enemy()
{
}

void Enemy::Initialize(std::shared_ptr<Model> model, uint32_t textureHandle)
{
	//	NULLポインタチェック
	assert(model);

	//	受け取ったデータをメンバ変数に記録する
	this->model_ = model;
	this->textureHandle_ = textureHandle;
	//	ワールド変換の初期化
	this->worldTransform_.Initialize();
	//	初期座標の設定
	this->worldTransform_.translation_ = { 0.0f,2.0f,50.0f };
}

void Enemy::Update()
{
	switch (phase_)
	{
	case Phase::Apprpach:
		move = { 0.0f,0.0f,-0.5f };
		if (worldTransform_.translation_.z <= 0.0f)
		{
			phase_ = Phase::Leave;
		}
		break;
	case Phase::Leave:
		move = { -0.5f,0.2f,0.0f };
		break;
	}

	//	座標移動
	worldTransform_.translation_ += move;




	//	アフィン変換
	worldTransform_.matWorld_ = matrix.MakeAffineMatrix(
		worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	//	行列を定数バッファに転送
	worldTransform_.TransferMatrix();

	//	キャラクターの座標を画面表示する処理
	ImGui::Begin("Enemy");
	ImGui::Text(
		"enemy : %0.2f,%0.2f,%0.2f", worldTransform_.translation_.x, worldTransform_.translation_.y,
		worldTransform_.translation_.z);
	ImGui::End();
}

void Enemy::Draw(ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}
