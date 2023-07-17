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
	playerTexture = TextureManager::Load("Player/player.png");
	playerModel.reset(Model::Create());
	playerObsever = playerModel;

	viewProjection.Initialize();
	//	自キャラの生成
	player = std::make_unique<Player>();
	//	自キャラの初期化
	Vector3 playerPos{ 0.0f,0.0f,20.0f };
	player->Initialize(playerModel, playerTexture, playerPos);

	//	デバッグカメラの生成
	debugCamera_ = std::make_unique<DebugCamera>(1280, 720);
	//	軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	//	軸方向表示が参照するビュープロジェクションを指定する（アドレス渡し）
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection);
}

void GameScene::Update() { 
	
	//	自キャラの更新
	player->Update(viewProjection);
	
	
#ifdef _DEBUG
	if (input_->TriggerKey(DIK_5)) {
		isDebugCameraActive_ = true;
	}
	//	カメラの処理
	if (isDebugCameraActive_) {
		
	}
	//	デバッグカメラの更新
	debugCamera_->Update();
	viewProjection.myMatView = debugCamera_->GetViewProjection().myMatView;
	viewProjection.myMatProjection = debugCamera_->GetViewProjection().myMatProjection;
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
