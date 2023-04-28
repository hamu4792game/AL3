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
	void Initialize(std::shared_ptr<Model> model, const Vector3& position);

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
	

};
