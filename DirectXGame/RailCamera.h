#pragma once
#include <WorldTransform.h>
#include <ViewProjection.h>
class RailCamera
{
public:
	RailCamera();
	~RailCamera();
public:
	//	初期化
	void Initialize(ViewProjection viewProjection);
	//	更新
	void Updata();

private:
	//	ワールド変換データ
	WorldTransform worldTransform_;
	//	ビュープロジェクション
	ViewProjection viewProjection_;

public:
	/// <summary>
	/// ビュープロジェクションを取得
	/// </summary>
	/// <returns>ビュープロジェクション</returns>
	const ViewProjection& GetViewProjection() const { return viewProjection_; }

	const WorldTransform& GetWorldTransform() const { return worldTransform_; };

};

