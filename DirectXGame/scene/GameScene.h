#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <memory>
#include "DebugCamera.h"
#include "Game/Player.h"
#include <sstream>
#include "Game/Skydome.h"
#include "Game/Ground.h"
#include <array>

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;


	/// <summary>
	/// ゲームシーン用
	/// </summary>
	//	テクスチャハンドル
	uint32_t playerTexture = 0;
	uint32_t enemyTexture = 0;
	//	3Dモデル
	std::shared_ptr<Model> skydomeModel;
	std::shared_ptr<Model> groundModel;
	//	playerのモデル
	std::array < std::shared_ptr < Model>, 4> playerModels;

	//	自キャラ
	std::unique_ptr<Player> player;
	//	天球
	std::unique_ptr<Skydome> skydome;
	//	地面
	std::unique_ptr<Ground> ground;

	//	ビュープロジェクション
	ViewProjection viewProjection;
	
	//	デバッグカメラ有効
	bool isDebugCameraActive_ = false;
	//	デバッグカメラ
	std::unique_ptr<DebugCamera> debugCamera_;

};
