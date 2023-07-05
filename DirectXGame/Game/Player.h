#pragma once
#include <Model.h>
#include <WorldTransform.h>
#include <ViewProjection.h>
#include <memory>
#include "Input.h"
#include "MyMatrix4x4/MyMatrix4x4.h"
#include "Game/PlayerBullet.h"
#include <list>
#include <Sprite.h>

class Player {
public:
	//	コンストラクタ
	Player();
	//	デストラクタ
	~Player();
	//	初期化
	void Initialize(std::shared_ptr<Model> model, uint32_t textureHandle, Vector3 pos);
	
	//	更新
	void Update(ViewProjection& viewProjection);

	//	描画
	void Draw(ViewProjection& viewProjection);

	void DrawUI();

private:
	//	キーボード入力
	Input* input_ = nullptr;

	//	ワールド変換データ
	WorldTransform worldTransform_;
	//	モデル
	std::shared_ptr<Model> model_;
	//	テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	//	弾
	std::list<std::unique_ptr<PlayerBullet>> bullets_;

	//	3Dレティクル用ワールドトランスフォーム
	WorldTransform worldTransform3DReticle_;

	//	2Dレティクル用スプライト
	std::shared_ptr<Sprite> sprite2DRetecle_ = nullptr;

	//	ゲームパッドの状態を得る変数
	XINPUT_STATE joyState{};

	int32_t timer = 0;

private:
	//	自機の回転
	void Rotate();

	/// <summary>
	/// 攻撃
	/// </summary>
	void Attack();

	/// <summary>
	/// マウスカーソルのスクリーン座標からワールド座標を取得して3Dレティクル配置
	/// </summary>
	void Scope(ViewProjection& viewProjection);

public:
	/// <summary>
	/// ワールド座標を取得
	/// </summary>
	/// <returns>Vector3</returns>
	Vector3 GetWorldPosition() const;

	Vector3 GetReticleWorldPosition() const;

	/// <summary>
	/// 衝突を検出したら呼び出されるコールバック関数
	/// </summary>
	void OnCollision();

	/// <summary>
	/// 弾リストを取得
	/// </summary>
	/// <returns>bullet->List</returns>
	const std::list<std::unique_ptr<PlayerBullet>>& GetBullets() const { return bullets_; };

	/// <summary>
	/// 親子関係を結ぶ
	/// </summary>
	/// <param name="parent">親となるワールドトランスフォーム</param>
	void SetParent(const WorldTransform* parent);

};
