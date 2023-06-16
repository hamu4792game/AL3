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
#include "Game/skydome.h"
#include "RailCamera.h"
#include <sstream>

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
	std::shared_ptr<Model> playerModel;
	std::weak_ptr<Model> playerObsever;
	std::shared_ptr<Model> enemyModel;
	std::weak_ptr<Model> enemyObsever;
	std::shared_ptr<Model> skydomeModel;
	std::weak_ptr<Model> skydomeObsever;

	std::shared_ptr<Model> enemyBulletModel;

	//	ビュープロジェクション
	ViewProjection viewProjection;

	//	自キャラ
	std::unique_ptr<Player> player;
	//	敵キャラ
	//std::unique_ptr<Enemy> enemy;
	std::list<std::unique_ptr<Enemy>> enemys_;
	//	敵弾
	std::list<std::unique_ptr<EnemyBullet>> enemyBullets_;
	//	天球
	std::unique_ptr<Skydome> skydome;
	//	レールカメラ
	std::unique_ptr<RailCamera> railCamera;
	//	デバッグカメラ有効
	bool isDebugCameraActive_ = false;
	//	デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;

	//	敵発生コマンド
	std::stringstream enemyPopCommands;

	//	待機フラグ
	bool isWait = false;
	//	待機時間
	int32_t waitCount = 0;

public:
	/// <summary>
	/// 衝突判定と応答
	/// </summary>
	void CheckAllCollisions();

	/// <summary>
	/// 敵弾を追加する
	/// </summary>
	/// <param name="enemyBullet">敵弾</param>
	void AddEnemyBullet(const std::unique_ptr<Enemy>& enemyBullet);

	/// <summary>
	/// 敵の生成
	/// </summary>
	void CreateEnemy(const Vector3& position);

	/// <summary>
	/// 敵発生データの読み込み
	/// </summary>
	void LoadEnemyPopData();

	/// <summary>
	/// 敵発生データの更新
	/// </summary>
	void UpdataEnemyPopCommands();

};
