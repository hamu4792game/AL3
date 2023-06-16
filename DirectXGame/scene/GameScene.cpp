#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"
#include <fstream>

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

	//	skydomeモデルの生成
	skydomeModel.reset(Model::CreateFromOBJ("skydome", true));
	skydomeObsever = skydomeModel;

	//	敵弾モデルの生成
	enemyBulletModel.reset(Model::Create());

	viewProjection.Initialize();
	//	自キャラの生成
	player = std::make_unique<Player>();
	//	自キャラの初期化
	Vector3 playerPos{ 0.0f,0.0f,20.0f };
	player->Initialize(playerModel, playerTexture, playerPos);
	//	天球の生成と初期化
	skydome = std::make_unique<Skydome>();
	skydome->Initialize(skydomeModel, { 0.0f,0.0f,0.0f });
	//	レールカメラの生成と初期化
	railCamera = std::make_unique<RailCamera>();
	railCamera->Initialize(viewProjection);

	//	自キャラとレールカメラの親子関係を結ぶ
	player->SetParent(&railCamera->GetWorldTransform());

	//	敵データの取得
	LoadEnemyPopData();

	//	デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);
	//	軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	//	軸方向表示が参照するビュープロジェクションを指定する（アドレス渡し）
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection);
}

void GameScene::Update() { 
	//	天球の更新
	skydome->Update();
	//	レールカメラの更新
	railCamera->Updata();
	//	自キャラの更新
	player->Update(viewProjection);
	//	敵キャラの更新
	UpdataEnemyPopCommands();
	for (auto i = enemys_.begin(); i != enemys_.end(); i++) {
		(*i)->Update();
		//	ゲームシーンに敵弾を代入
		if (!(*i)->GetCoolDawn())
		{
			AddEnemyBullet((*i));
		}
	}
	
	//	敵の弾の更新
	for (auto i = enemyBullets_.begin(); i != enemyBullets_.end(); i++)
	{
		(*i)->Update();
	}
	//	デスフラグの立った弾を削除
	if (enemyBullets_.remove_if([](const std::unique_ptr<EnemyBullet>& bullet)
		{
			return bullet->IsDead();
		})
		)

	/*if (input_->PushKey(DIK_R))
	{
		enemy->Reset();
	}*/
	CheckAllCollisions();

	
#ifdef _DEBUG
	if (input_->TriggerKey(DIK_5)) {
		isDebugCameraActive_ = true;
	}
	//	カメラの処理
	if (isDebugCameraActive_) {
		//	デバッグカメラの更新
		debugCamera_->Update();
		viewProjection.myMatView = debugCamera_->GetViewProjection().myMatView;
		viewProjection.myMatProjection = debugCamera_->GetViewProjection().myMatProjection;
		//	ビュープロジェクション行列の転送
		viewProjection.TransferMatrix();
	} else {

		viewProjection.myMatView = railCamera->GetViewProjection().myMatView;
		viewProjection.myMatProjection = railCamera->GetViewProjection().myMatProjection;

		//	ビュープロジェクション行列の転送
		viewProjection.TransferMatrix();

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
	skydome->Draw(viewProjection);

	//	自キャラの描画
	player->Draw(viewProjection);

	//	敵キャラの描画
	for (auto i = enemys_.begin(); i != enemys_.end(); i++)	{
		(*i)->Draw(viewProjection);
	}

	//	敵の弾の更新
	for (auto i = enemyBullets_.begin(); i != enemyBullets_.end(); i++)
	{
		(*i)->Draw(viewProjection);
	}

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	player->DrawUI();

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
	const std::list<std::unique_ptr<EnemyBullet>>& enemyBullets = enemyBullets_;
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
	
	for (const auto& enemy : enemys_)
	{
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

void GameScene::AddEnemyBullet(const std::unique_ptr<Enemy>& enemyBullet)
{
	//	リストに登録する
	enemyBullets_.push_back(std::make_unique<EnemyBullet>());
	(*enemyBullets_.rbegin())->Initialize(enemyBulletModel, enemyBullet->GetTranslation(), enemyBullet->GetVelocity());
	
}

void GameScene::CreateEnemy(const Vector3& position)
{
	//	敵を発生させる
	enemys_.push_back(std::make_unique<Enemy>());
	(*enemys_.rbegin())->Initialize(enemyModel, enemyTexture, position);
	//	敵キャラにゲームシーンを渡す
	(*enemys_.rbegin())->SetGameScene(this);
	//	敵キャラに自キャラのアドレスを渡す
	(*enemys_.rbegin())->SetPlayer(player.get());
}

void GameScene::LoadEnemyPopData()
{
	//	ファイルを開く
	std::ifstream file;
	file.open("Resources/data/enemyPop.csv");
	assert(file.is_open());
	//	ファイルの内容を文字列ストリームにコピー
	enemyPopCommands << file.rdbuf();
	//	ファイルを閉じる
	file.close();
}

void GameScene::UpdataEnemyPopCommands()
{
	//	待機処理 早期リターン
	if (isWait)
	{
		waitCount--;
		if (waitCount <= 0)
		{
			//	待機完了
			isWait = false;
		}
		return;
	}

	//	1行分の文字列を入れる変数
	std::string line;

	//	コマンド実行用ループ
	while (std::getline(enemyPopCommands, line))
	{
		//	1行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		std::string word;
		//	,区切りで行の先頭文字列を取得
		std::getline(line_stream, word, ',');

		//	"//"から始まる行はコメント
		if (word.find("//") == 0)
		{
			//	コメント行を飛ばす
			continue;
		}

		//	Popコマンドの実行
		if (word.find("POP") == 0)
		{
			//	x座標
			std::getline(line_stream, word, ',');
			float x = static_cast<float>(std::atof(word.c_str()));

			//	y座標
			std::getline(line_stream, word, ',');
			float y = static_cast<float>(std::atof(word.c_str()));

			//	z座標
			std::getline(line_stream, word, ',');
			float z = static_cast<float>(std::atof(word.c_str()));

			//	敵を発生させる
			CreateEnemy({ x,y,z });
		}
		else if (word.find("WAIT") == 0)
		{
			std::getline(line_stream, word, ',');
			//	待ち時間
			int32_t waitTime = atoi(word.c_str());

			//	待機開始
			isWait = true;
			waitCount = waitTime;
			//	コマンドループを抜ける
			break;
		}

	}

}
