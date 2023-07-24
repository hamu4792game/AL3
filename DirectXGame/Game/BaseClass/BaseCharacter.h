#pragma once
#include <vector>
#include <Model.h>
#include <WorldTransform.h>
#include <ViewProjection.h>

class BaseCharacter
{
public:
	BaseCharacter() = default;
	~BaseCharacter() = default;

protected:
	//	モデルデータ配列
	std::vector<std::shared_ptr < Model>> models_;
	//	パーツ用データ
	std::vector<WorldTransform> parts_;
	//	ワールド変換データ
	WorldTransform worldTransform_;

public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="models">モデルデータ配列</param>
	/// <param name="pos">初期位置</param>
	virtual void Initialize(const std::vector<std::shared_ptr<Model>>& models, Vector3 pos);

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション(参照渡し)</param>
	virtual void Draw(const ViewProjection& viewProjection);

public:
	//	ゲッターセッター
	
	/// <summary>
	/// ワールド変換データを取得
	/// </summary>
	/// <returns>ワールド変換データ</returns>
	const WorldTransform& GetWorldTransform() { return worldTransform_; };


};
