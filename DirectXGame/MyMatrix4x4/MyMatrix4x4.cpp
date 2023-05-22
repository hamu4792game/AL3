#include "MyMatrix4x4/MyMatrix4x4.h"
#include <cassert>
#include <cmath>

MyMatrix4x4::MyMatrix4x4():num(4) {
	for (int y = 0; y < num; y++)
	{
		for (int x = 0; x < num; x++)
		{
			result.m[y][x] = 0.0f;
		}
	}
	resultVec = { 0.0f,0.0f,0.0f };
}

MyMatrix4x4::~MyMatrix4x4() {

}

void MyMatrix4x4::Reset() {
	for (int y = 0; y < num; y++)
	{
		for (int x = 0; x < num; x++)
		{
			result.m[y][x] = 0.0f;
		}
	}
	resultVec = { 0.0f,0.0f,0.0f };
}

//	加算
Matrix4x4 MyMatrix4x4::Add(const Matrix4x4& m1, const Matrix4x4& m2) {
	for (int y = 0; y < num; y++)
	{
		for (int x = 0; x < num; x++)
		{
			result.m[y][x] = m1.m[y][x] + m2.m[y][x];
		}
	}

	return result;
}
//	減算
Matrix4x4 MyMatrix4x4::Subtract(const Matrix4x4& m1, const Matrix4x4& m2) {
	for (int y = 0; y < num; y++)
	{
		for (int x = 0; x < num; x++)
		{
			result.m[y][x] = m1.m[y][x] - m2.m[y][x];
		}
	}

	return result;
}
//	行列の積
Matrix4x4 MyMatrix4x4::Multiply(const Matrix4x4& m1, const Matrix4x4& m2) {
	Reset();

	for (int z = 0; z < num; z++)
	{
		for (int y = 0; y < num; y++)
		{
			for (int x = 0; x < num; x++)
			{
				result.m[z][y] += m1.m[z][x] * m2.m[x][y];
			}
		}
	}

	return result;
}
//	逆行列
Matrix4x4 MyMatrix4x4::Inverse(const Matrix4x4& m) {
	float A = 0.0f;
	A = m.m[0][0] * m.m[1][1] * m.m[2][2] * m.m[3][3] +
		m.m[0][0] * m.m[1][2] * m.m[2][3] * m.m[3][1] +
		m.m[0][0] * m.m[1][3] * m.m[2][1] * m.m[3][2] -
		m.m[0][0] * m.m[1][3] * m.m[2][2] * m.m[3][1] -
		m.m[0][0] * m.m[1][2] * m.m[2][1] * m.m[3][3] -
		m.m[0][0] * m.m[1][1] * m.m[2][3] * m.m[3][2] -

		m.m[0][1] * m.m[1][0] * m.m[2][2] * m.m[3][3] -
		m.m[0][2] * m.m[1][0] * m.m[2][3] * m.m[3][1] -
		m.m[0][3] * m.m[1][0] * m.m[2][1] * m.m[3][2] +
		m.m[0][3] * m.m[1][0] * m.m[2][2] * m.m[3][1] +
		m.m[0][2] * m.m[1][0] * m.m[2][1] * m.m[3][3] +
		m.m[0][1] * m.m[1][0] * m.m[2][3] * m.m[3][2] +

		m.m[0][1] * m.m[1][2] * m.m[2][0] * m.m[3][3] +
		m.m[0][2] * m.m[1][3] * m.m[2][0] * m.m[3][1] +
		m.m[0][3] * m.m[1][1] * m.m[2][0] * m.m[3][2] -
		m.m[0][3] * m.m[1][2] * m.m[2][0] * m.m[3][1] -
		m.m[0][2] * m.m[1][1] * m.m[2][0] * m.m[3][3] -
		m.m[0][1] * m.m[1][3] * m.m[2][0] * m.m[3][2] -

		m.m[0][1] * m.m[1][2] * m.m[2][3] * m.m[3][0] -
		m.m[0][2] * m.m[1][3] * m.m[2][1] * m.m[3][0] -
		m.m[0][3] * m.m[1][1] * m.m[2][2] * m.m[3][0] +
		m.m[0][3] * m.m[1][2] * m.m[2][1] * m.m[3][0] +
		m.m[0][2] * m.m[1][1] * m.m[2][3] * m.m[3][0] +
		m.m[0][1] * m.m[1][3] * m.m[2][2] * m.m[3][0];

	result.m[0][0] = (m.m[1][1] * m.m[2][2] * m.m[3][3] +
		m.m[1][2] * m.m[2][3] * m.m[3][1] +
		m.m[1][3] * m.m[2][1] * m.m[3][2] -
		m.m[1][3] * m.m[2][2] * m.m[3][1] -
		m.m[1][2] * m.m[2][1] * m.m[3][3] -
		m.m[1][1] * m.m[2][3] * m.m[3][2]) / A;
	result.m[0][1] = (-m.m[0][1] * m.m[2][2] * m.m[3][3] -
		m.m[0][2] * m.m[2][3] * m.m[3][1] -
		m.m[0][3] * m.m[2][1] * m.m[3][2] +
		m.m[0][3] * m.m[2][2] * m.m[3][1] +
		m.m[0][2] * m.m[2][1] * m.m[3][3] +
		m.m[0][1] * m.m[2][3] * m.m[3][2]) / A;
	result.m[0][2] = (m.m[0][1] * m.m[1][2] * m.m[3][3] +
		m.m[0][2] * m.m[1][3] * m.m[3][1] +
		m.m[0][3] * m.m[1][1] * m.m[3][2] -
		m.m[0][3] * m.m[1][2] * m.m[3][1] -
		m.m[0][2] * m.m[1][1] * m.m[3][3] -
		m.m[0][1] * m.m[1][3] * m.m[2][2]) / A;
	result.m[0][3] = (-m.m[0][1] * m.m[1][2] * m.m[2][3] -
		m.m[0][2] * m.m[1][3] * m.m[2][1] -
		m.m[0][3] * m.m[1][1] * m.m[2][2] +
		m.m[0][3] * m.m[1][2] * m.m[2][1] +
		m.m[0][2] * m.m[1][1] * m.m[2][3] +
		m.m[0][1] * m.m[1][3] * m.m[2][2]) / A;

	result.m[1][0] = (-m.m[1][0] * m.m[2][2] * m.m[3][3] -
		m.m[1][2] * m.m[2][3] * m.m[3][0] -
		m.m[1][3] * m.m[2][0] * m.m[3][2] +
		m.m[1][3] * m.m[2][2] * m.m[3][0] +
		m.m[1][2] * m.m[1][1] * m.m[3][2] +
		m.m[1][0] * m.m[2][3] * m.m[3][2]) / A;
	result.m[1][1] = (m.m[0][0] * m.m[2][2] * m.m[3][3] +
		m.m[0][2] * m.m[2][3] * m.m[3][0] +
		m.m[0][3] * m.m[2][0] * m.m[0][3] -
		m.m[0][3] * m.m[2][2] * m.m[3][0] -
		m.m[0][2] * m.m[2][0] * m.m[3][3] -
		m.m[0][0] * m.m[2][3] * m.m[3][2]) / A;
	result.m[1][2] = (-m.m[0][0] * m.m[1][2] * m.m[3][3] -
		m.m[0][2] * m.m[1][3] * m.m[3][0] -
		m.m[0][3] * m.m[1][0] * m.m[3][2] +
		m.m[0][3] * m.m[1][2] * m.m[3][0] +
		m.m[0][2] * m.m[1][0] * m.m[3][3] +
		m.m[0][0] * m.m[1][3] * m.m[3][2]) / A;
	result.m[1][3] = (m.m[0][0] * m.m[1][2] * m.m[2][3] +
		m.m[0][2] * m.m[1][3] * m.m[2][0] +
		m.m[0][3] * m.m[1][0] * m.m[2][2] -
		m.m[0][3] * m.m[1][2] * m.m[2][0] -
		m.m[0][2] * m.m[1][0] * m.m[2][3] -
		m.m[0][0] * m.m[1][3] * m.m[2][2]) / A;

	result.m[2][0] = (m.m[1][0] * m.m[2][1] * m.m[3][3] +
		m.m[1][1] * m.m[2][3] * m.m[3][0] +
		m.m[1][3] * m.m[2][0] * m.m[3][1] -
		m.m[1][3] * m.m[2][1] * m.m[3][0] -
		m.m[1][1] * m.m[2][0] * m.m[3][3] -
		m.m[1][0] * m.m[2][3] * m.m[3][1]) / A;
	result.m[2][1] = (-m.m[0][0] * m.m[2][1] * m.m[3][3] -
		m.m[0][1] * m.m[2][3] * m.m[3][0] -
		m.m[0][3] * m.m[2][0] * m.m[3][1] +
		m.m[0][3] * m.m[2][1] * m.m[3][0] +
		m.m[0][1] * m.m[2][0] * m.m[3][3] +
		m.m[0][0] * m.m[2][3] * m.m[3][1]) / A;
	result.m[2][2] = (m.m[0][0] * m.m[1][1] * m.m[3][3] +
		m.m[0][1] * m.m[1][3] * m.m[3][0] +
		m.m[0][3] * m.m[1][0] * m.m[3][1] -
		m.m[0][3] * m.m[1][1] * m.m[2][0] -
		m.m[0][1] * m.m[1][0] * m.m[3][3] -
		m.m[0][0] * m.m[1][3] * m.m[3][1]) / A;
	result.m[2][3] = (-m.m[0][0] * m.m[1][1] * m.m[2][3] -
		m.m[0][1] * m.m[1][3] * m.m[2][0] -
		m.m[0][3] * m.m[1][0] * m.m[2][1] +
		m.m[0][3] * m.m[1][1] * m.m[2][0] +
		m.m[0][1] * m.m[1][0] * m.m[2][3] +
		m.m[0][0] * m.m[1][3] * m.m[2][1]) / A;

	result.m[3][0] = (-m.m[1][0] * m.m[2][1] * m.m[3][2] -
		m.m[1][1] * m.m[2][2] * m.m[3][0] -
		m.m[1][2] * m.m[2][0] * m.m[3][1] +
		m.m[1][2] * m.m[2][1] * m.m[3][0] +
		m.m[1][1] * m.m[2][0] * m.m[3][2] +
		m.m[1][0] * m.m[2][2] * m.m[3][1]) / A;
	result.m[3][1] = (m.m[0][0] * m.m[2][1] * m.m[3][2] +
		m.m[0][1] * m.m[2][2] * m.m[3][0] +
		m.m[0][2] * m.m[2][0] * m.m[3][1] -
		m.m[0][2] * m.m[2][1] * m.m[3][0] -
		m.m[0][1] * m.m[2][0] * m.m[3][2] -
		m.m[0][0] * m.m[2][2] * m.m[3][1]) / A;
	result.m[3][2] = (-m.m[0][0] * m.m[1][1] * m.m[3][2] - 
		m.m[0][1] * m.m[1][2] * m.m[3][0] -
		m.m[0][2] * m.m[1][0] * m.m[3][1] +
		m.m[0][2] * m.m[1][1] * m.m[3][0] +
		m.m[0][1] * m.m[1][0] * m.m[3][2] +
		m.m[0][0] * m.m[1][2] * m.m[3][1]) / A;
	result.m[3][3] = (m.m[0][0] * m.m[1][1] * m.m[2][2] +
		m.m[0][1] * m.m[1][2] * m.m[2][0] +
		m.m[0][2] * m.m[1][0] * m.m[2][1] -
		m.m[0][2] * m.m[1][1] * m.m[2][0] -
		m.m[0][1] * m.m[1][0] * m.m[2][2] -
		m.m[0][0] * m.m[1][2] * m.m[2][1]) / A;

	return result;
}
//	転置行列
Matrix4x4 MyMatrix4x4::Transpose(const Matrix4x4& m) {
	for (int y = 0; y < num; y++) {
		for (int x = 0; x < num; x++) {
			result.m[y][x] = m.m[x][y];
		}
	}

	return result;
}
//	単位行列の作成
Matrix4x4 MyMatrix4x4::MakeIdentity4x4() {
	for (int y = 0; y < 4; y++) {
		for (int x = 0; x < 4; x++) {
			if (y < 1 && x < 1) {
				result.m[y][x] = 1.0f;
			}
			else if (y == x) {
				result.m[y][x] = 1.0f;
			}
			else {
				result.m[y][x] = 0.0f;
			}
		}
	}

	return result;
}
//	平行移動行列
Matrix4x4 MyMatrix4x4::MakeTranslateMatrix(const Vector3& translate)
{
	for (int y = 0; y < num; y++)
	{
		for (int x = 0; x < num; x++)
		{
			if (y == x)
			{
				result.m[y][x] = 1.0f;
			}
			else
			{
				result.m[y][x] = 0.0f;
			}
		}
	}
	result.m[3][0] = translate.x;
	result.m[3][1] = translate.y;
	result.m[3][2] = translate.z;

	return result;
}
//	拡大縮小行列
Matrix4x4 MyMatrix4x4::MakeScaleMatrix(const Vector3& scale)
{
	Reset();
	result.m[0][0] = scale.x;
	result.m[1][1] = scale.y;
	result.m[2][2] = scale.z;
	result.m[3][3] = 1.0f;

	return result;
}
//	座標変換
Vector3 MyMatrix4x4::Transform(const Vector3& vector, const Matrix4x4& matrix)
{
	resultVec.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] + 1.0f * matrix.m[3][0];
	resultVec.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] + 1.0f * matrix.m[3][1];
	resultVec.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] + 1.0f * matrix.m[3][2];
	float w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] + 1.0f * matrix.m[3][3];
	assert(w != 0);
	resultVec.x /= w;
	resultVec.y /= w;
	resultVec.z /= w;

	return resultVec;
}

Matrix4x4 MyMatrix4x4::MakeRotateXMatrix(float radian)
{
	Reset();
	result.m[0][0] = 1.0f;
	result.m[3][3] = 1.0f;

	result.m[1][1] = std::cosf(radian);
	result.m[2][2] = std::cosf(radian);

	result.m[2][1] = -std::sinf(radian);
	result.m[1][2] = std::sinf(radian);

	return result;
}

Matrix4x4 MyMatrix4x4::MakeRotateYMatrix(float radian)
{
	Reset();
	result.m[1][1] = 1.0f;
	result.m[3][3] = 1.0f;

	result.m[0][0] = std::cosf(radian);
	result.m[2][2] = std::cosf(radian);

	result.m[0][2] = -std::sinf(radian);
	result.m[2][0] = std::sinf(radian);

	return result;
}

Matrix4x4 MyMatrix4x4::MakeRotateZMatrix(float radian)
{
	Reset();
	result.m[2][2] = 1.0f;
	result.m[3][3] = 1.0f;

	result.m[0][0] = std::cosf(radian);
	result.m[1][1] = std::cosf(radian);

	result.m[1][0] = -std::sinf(radian);
	result.m[0][1] = std::sinf(radian);

	return result;
}

Matrix4x4 MyMatrix4x4::MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate)
{
	Reset();
	Matrix4x4 rotateMatrix;
	rotateMatrix = Multiply(MakeRotateXMatrix(rotate.x), Multiply(MakeRotateYMatrix(rotate.y), MakeRotateZMatrix(rotate.z)));
	result = rotateMatrix;

	
	for (int i = 0; i < 3; i++)
	{
		result.m[0][i] = scale.x * rotateMatrix.m[0][i];
		result.m[1][i] = scale.y * rotateMatrix.m[1][i];
		result.m[2][i] = scale.z * rotateMatrix.m[2][i];
	}
	result.m[3][0] = translate.x;
	result.m[3][1] = translate.y;
	result.m[3][2] = translate.z;

	return result;
}
