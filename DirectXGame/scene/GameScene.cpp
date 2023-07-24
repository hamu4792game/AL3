#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"
#include <fstream>

GameScene::GameScene() {}

GameScene::~GameScene() {}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	//	playerモデルの生成
	playerModels.resize(4);
	playerModels[0].reset(Model::CreateFromOBJ("body", true));
	playerModels[1].reset(Model::CreateFromOBJ("head", true));
	playerModels[2].reset(Model::CreateFromOBJ("RArm", true));
	playerModels[3].reset(Model::CreateFromOBJ("LArm", true));

	//	enemyモデルの生成
	enemyModels.resize(2);
	enemyModels[0].reset(Model::CreateFromOBJ("enemyBody", true));
	enemyModels[1].reset(Model::CreateFromOBJ("enemyWeapon", true));

	//	skydomeモデルの生成
	skydomeModel.reset(Model::CreateFromOBJ("skydome", true));
	//	groundモデルの生成
	groundModel.reset(Model::CreateFromOBJ("ground", true));

	viewProjection.farZ = 2000.0f;
	viewProjection.Initialize();
	//	自キャラの生成
	player = std::make_unique<Player>();
	//	自キャラの初期化
	Vector3 playerPos{ 0.0f,5.0f,0.0f };
	player->Initialize(playerModels, playerPos);
	//	敵キャラの生成
	enemy = std::make_unique<Enemy>();
	//	敵キャラの初期化
	Vector3 enemyPos{ 0.0f,5.0f,0.0f };
	enemy->Initialize(enemyModels, enemyPos);
	
	//	天球の生成と初期化
	skydome = std::make_unique<Skydome>();
	skydome->Initialize(skydomeModel, { 0.0f,0.0f,0.0f });
	//	地面の生成と初期化
	ground = std::make_unique<Ground>();
	ground->Initialize(groundModel, { 0.0f,0.0f,0.0f });
	//	追従カメラの生成と初期化、セット
	followCamera = std::make_unique<FollowCamera>();
	followCamera->Initialize(viewProjection);
	followCamera->SetTarget(&player->GetWorldTransform());

	//	自キャラのカメラ角度セット
	player->SetViewProjection(&followCamera->GetViewProjection());

	//	デバッグカメラの生成
	debugCamera_ = std::make_unique<DebugCamera>(1280, 720);
	//	軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	//	軸方向表示が参照するビュープロジェクションを指定する（アドレス渡し）
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection);
}

void GameScene::Update() { 
	
	//	天球の更新
	skydome->Update();
	//	地面の更新
	ground->Update();

	//	自キャラの更新
	player->Update();

	enemy->Update();

	
#ifdef _DEBUG
	if (input_->TriggerKey(DIK_5)) {
		isDebugCameraActive_ = !isDebugCameraActive_;
	}
	//	カメラの処理
	if (isDebugCameraActive_) {
		//	デバッグカメラの更新
		debugCamera_->Update();
		viewProjection.myMatView = debugCamera_->GetViewProjection().myMatView;
		viewProjection.myMatProjection = debugCamera_->GetViewProjection().myMatProjection;	
	}
	else {
		//	追従カメラの更新
		followCamera->Update();
		viewProjection.myMatView = followCamera->GetViewProjection().myMatView;
		viewProjection.myMatProjection = followCamera->GetViewProjection().myMatProjection;
	}
	//	ビュープロジェクション行列の転送
	viewProjection.TransferMatrix();
	
#endif

}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	//	天球の描画
	skydome->Draw(viewProjection);
	//	地面の描画
	ground->Draw(viewProjection);

	//	敵キャラ
	enemy->Draw(viewProjection);

	//	自キャラの描画
	player->Draw(viewProjection);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
