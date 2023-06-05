#pragma once
#include <Model.h>
#include <WorldTransform.h>
#include <ViewProjection.h>
#include "Vector3.h"

/// <summary>
/// 天球
/// </summary>
class Skydome
{
public:
	Skydome();
	~Skydome();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="position">初期座標</param>
	/// <param name="velocity">速度</param>
	void Initialize(std::shared_ptr<Model> model, const Vector3& position, const Vector3& velocity);

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



};