#pragma once
#include <Model.h>
#include <WorldTransform.h>
#include <ViewProjection.h>
#include "Vector3.h"

/// <summary>
/// 自キャラの弾
/// </summary>
class PlayerBullet {
public:
	//	コンストラクタ
	PlayerBullet();
	//	デストラクタ
	~PlayerBullet();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="position">初期座標</param>
	/// <param name="velocity">速度</param>
	void Initialize(std::shared_ptr<Model> model, const Vector3& position,const Vector3& velocity);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション</param>
	void Draw(const ViewProjection& viewProjection);

private:

	//	ワールド変換データ
	WorldTransform worldTransform_;
	//	モデル
	std::shared_ptr<Model> model_;
	//	テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	//	速度
	Vector3 velocity_;
	//	寿命<frm>
	static const int32_t kLifeTime = 60 * 5;
	//	デスタイマー
	int32_t deathTimer_ = kLifeTime;
	//	デスフラグ
	bool isDead_ = false;

public:
	bool IsDead() const { return isDead_; }

	/// <summary>
	/// 衝突を検出したら呼び出されるコールバック関数
	/// </summary>
	void OnCollision();

	Vector3 GetWorldPosition() const;
};
