#pragma once
#include "Vector3.h"
#include <array>

class MyMatrix4x4 {
public:
	MyMatrix4x4();
	~MyMatrix4x4();

public:
	//	mの多重配列 固定長配列
	std::array<std::array<float, 4>, 4> m;

private:
public:
	//	加算
	MyMatrix4x4 operator+(const MyMatrix4x4& mat);
	//	減算
	MyMatrix4x4 operator-(const MyMatrix4x4& mat);
	//	行列の積
	MyMatrix4x4 operator*(const MyMatrix4x4& mat);
	//	行列の代入
	MyMatrix4x4& operator=(const MyMatrix4x4& mat);
};

//	逆行列
MyMatrix4x4 Inverse(const MyMatrix4x4& m);
//	転置行列
MyMatrix4x4 Transpose(const MyMatrix4x4& m);
//	単位行列の作成
MyMatrix4x4 MakeIdentity4x4();
//	平行移動行列
MyMatrix4x4 MakeTranslateMatrix(const Vector3& translate);
//	拡大縮小行列
MyMatrix4x4 MakeScaleMatrix(const Vector3& scale);
//	座標変換
Vector3 Transform(const Vector3& vector, const MyMatrix4x4& matrix);
//	X軸回転行列
MyMatrix4x4 MakeRotateXMatrix(float radian);
//	Y軸回転行列
MyMatrix4x4 MakeRotateYMatrix(float radian);
//	Z軸回転行列
MyMatrix4x4 MakeRotateZMatrix(float radian);
//	3次元アフィン変換行列
MyMatrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);

//	透視投影行列
MyMatrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip);
//	正射影行列
MyMatrix4x4 MakeOrthographicMatrix(
	float left, float top, float right, float bottom, float nearClip, float farClip);
//	ビューポート変換行列
MyMatrix4x4 MakeViewportMatrix(
	float left, float top, float width, float height, float minDepth, float maxDepth);

//	クロス積
Vector3 Cross(const Vector3& v1, const Vector3& v2);
