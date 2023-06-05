#pragma once

#include <Model.h>
#include <WorldTransform.h>
#include <ViewProjection.h>
#include <memory>
#include "Input.h"
#include "MyMatrix4x4/MyMatrix4x4.h"
#include "Game/EnemyBullet.h"
#include <list>

//	クラスの前方宣言
class Player;

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

	void Reset();

private:
	//	ワールド変換データ
	WorldTransform worldTransform_;
	//	モデル
	std::shared_ptr<Model> model_;
	//	テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	//	行動フェーズ
	enum class Phase
	{
		Apprpach,	//	接近する
		Leave,		//	離脱する
	};
	Phase phase_ = Phase::Apprpach;

	//	弾
	std::list<std::unique_ptr<EnemyBullet>> bullets_;
	//	発射速度
	static const int32_t kFireInterval = 60;
	int32_t timer = 0;

	//	自キャラ
	Player* player_ = nullptr;
public:
	//	アドレスのセッター
	void SetPlayer(Player* player) { player_ = player; };

public:
	//	自作メンバ関数
	void Move1();
	void Move2();
private:
	//	メンバ関数ポインタ
	static void (Enemy::* pPhaseTable[])();

	//	移動量
	Vector3 move;

public:
	/// <summary>
	/// 弾発射
	/// </summary>
	void Fire();

public:
	/// <summary>
	/// ワールド座標を取得
	/// </summary>
	/// <returns>Vector3</returns>
	Vector3 GetWorldPosition();

	/// <summary>
	/// 衝突を検出したら呼び出されるコールバック関数
	/// </summary>
	void OnCollision();

	/// <summary>
	/// 弾リストを取得
	/// </summary>
	/// <returns>bullet->List</returns>
	const std::list<std::unique_ptr<EnemyBullet>>& GetBullets() { return bullets_; };
};

