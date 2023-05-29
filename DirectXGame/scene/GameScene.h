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
#include "Game/Enemy.h"

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
	std::weak_ptr<Model> playerObsever;
	std::shared_ptr<Model> playerModel;
	std::weak_ptr<Model> enemyObsever;
	std::shared_ptr<Model> enemyModel;
	//	ビュープロジェクション
	ViewProjection viewProjection;
	//	自キャラ
	std::unique_ptr<Player> player;
	//	敵キャラ
	std::unique_ptr<Enemy> enemy;
	//	デバッグカメラ有効
	bool isDebugCameraActive_ = false;
	//	デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;

};
