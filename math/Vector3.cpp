#include "math/Vector3.h"

Vector3::Vector3() {
	this->x = 0.0f;
	this->y = 0.0f;
	this->z = 0.0f;
}

Vector3::Vector3(const float& X, const float& Y, const float& Z) {
	this->x = X;
	this->y = Y;
	this->z = Z;
}

Vector3 Vector3::operator+(const Vector3& num) const {
	Vector3 result;
	result.x = this->x + num.x;
	result.y = this->y + num.y;
	result.z = this->z + num.z;
	return result;
}

Vector3 Vector3::operator-(const Vector3& num) const {
	Vector3 result;
	result.x = this->x - num.x;
	result.y = this->y - num.y;
	result.z = this->z - num.z;
	return result;
}

Vector3& Vector3::operator+=(const Vector3& num) {
	*this = *this + num;

	return *this;
}

Vector3& Vector3::operator-=(const Vector3& num) {
	*this = *this - num;

	return *this;
}