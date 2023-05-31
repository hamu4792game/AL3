#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"

GameScene::GameScene() {}

GameScene::~GameScene() { 
	delete debugCamera_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	//	playerモデルの生成
	playerTexture = TextureManager::Load("Player/player.png");
	playerModel.reset(Model::Create());
	playerObsever = playerModel;
	
	//	enemyモデルの生成
	enemyTexture = TextureManager::Load("Player/enemy.png");
	enemyModel.reset(Model::Create());
	enemyObsever = enemyModel;


	viewProjection.Initialize();

	//	自キャラの生成
	player = std::make_unique<Player>();
	//	敵キャラの生成
	enemy = std::make_unique<Enemy>();


	//	自キャラの初期化
	player->Initialize(playerModel, playerTexture);
	//	敵キャラの初期化
	enemy->Initialize(enemyModel, enemyTexture);
	//	敵キャラに自キャラのアドレスを渡す
	enemy->SetPlayer(player.get());

	//	デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);
	//	軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	//	軸方向表示が参照するビュープロジェクションを指定する（アドレス渡し）
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection);
}

void GameScene::Update() { 
	//	自キャラの更新
	player->Update();
	//	敵キャラの更新
	enemy->Update();

	if (input_->PushKey(DIK_R))
	{
		enemy->Reset();
	}
	CheckAllCollisions();

#ifdef _DEBUG
	if (input_->TriggerKey(DIK_5)) {
		isDebugCameraActive_ = true;
	}
	//	カメラの処理
	if (isDebugCameraActive_) {
		//	デバッグカメラの更新
		debugCamera_->Update();
		viewProjection.matView = debugCamera_->GetViewProjection().matView;
		viewProjection.matProjection = debugCamera_->GetViewProjection().matProjection;
		//	ビュープロジェクション行列の転送
		viewProjection.TransferMatrix();
	} else {
		//	ビュープロジェクション行列の更新と転送
		viewProjection.UpdateMatrix();
	}
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

	//	敵キャラの描画
	enemy->Draw(viewProjection);

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

void GameScene::CheckAllCollisions()
{
	//	判定対象AとBの座標
	Vector3 posA, posB;

	//	自弾リストの取得
	const std::list<std::unique_ptr<PlayerBullet>>& playerBullets = player->GetBullets();
	//	敵弾リストの取得
	const std::list<std::unique_ptr<EnemyBullet>>& enemyBullets = enemy->GetBullets();
#pragma region 自キャラと敵弾の当たり判定
	//	自キャラの座標
	posA = player->GetWorldPosition();

	for (const auto& bullet : enemyBullets)
	{
		//	敵弾の座標
		posB = bullet->GetWorldPosition();
		//	座標Aと座標Bの距離を求める
		float distance = powf(posB.x - posA.x, 2.0f) + powf(posB.y - posA.y, 2.0f) + powf(posB.z - posA.z, 2.0f);
		if (distance <= powf(1.0f + 1.0f, 2.0f))
		{
			player->OnCollision();
			bullet->OnCollision();
		}
	}

#pragma endregion
#pragma region 自弾と敵キャラの当たり判定
	//	敵キャラの座標
	posA = enemy->GetWorldPosition();

	for (const auto& bullet : playerBullets)
	{
		//	自弾の座標
		posB = bullet->GetWorldPosition();
		//	座標Aと座標Bの距離を求める
		float distance = powf(posB.x - posA.x, 2.0f) + powf(posB.y - posA.y, 2.0f) + powf(posB.z - posA.z, 2.0f);
		if (distance <= powf(1.0f + 1.0f, 2.0f))
		{
			enemy->OnCollision();
			bullet->OnCollision();
		}
	}

#pragma endregion
#pragma region 自弾と敵弾の当たり判定
	for (const auto& pBullet : playerBullets)
	{
		//	自弾の座標
		posA = pBullet->GetWorldPosition();
		for (const auto& eBullet : enemyBullets)
		{
			//	自弾の座標
			posB = eBullet->GetWorldPosition();
			//	座標Aと座標Bの距離を求める
			float distance = powf(posB.x - posA.x, 2.0f) + powf(posB.y - posA.y, 2.0f) + powf(posB.z - posA.z, 2.0f);
			if (distance <= powf(1.0f + 1.0f, 2.0f))
			{
				pBullet->OnCollision();
				eBullet->OnCollision();
			}
		}
	}

#pragma endregion


}
