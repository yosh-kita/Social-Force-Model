#include "Vector.h"
#include <math.h>

Vector Vector::operator+(Vector vector) {
	Vector ansVector;
	ansVector.x = x + vector.x;
	ansVector.y = y + vector.y;
	return ansVector;
}
void Vector::operator+=(Vector vector) {
	x = x + vector.x;
	y = y + vector.y;
}
Vector Vector::operator-(Vector vector) {
	Vector ansVector;
	ansVector.x = x - vector.x;
	ansVector.y = y - vector.y;
	return ansVector;
}
void Vector::operator-=(Vector vector) {
	x = x - vector.x;
	y = y - vector.y;
}
Vector Vector::operator*(double k) {
	Vector ansVector;
	ansVector.x = x * k;
	ansVector.y = y * k;
	return ansVector;
}
Vector Vector::operator/(double k) {
	Vector ansVector;
	ansVector.x = x / k;
	ansVector.y = y / k;
	return ansVector;
}

Vector::Vector() {
	x = 0.0;
	y = 0.0;
}

Vector::Vector(double X, double Y) {
	x = X;
	y = Y;
}

//ベクトルの大きさ
double Vector::AbsValue() { return sqrt(pow(x, 2.0) + pow(y, 2.0)); }
double Vector::AbsValue(Vector vec) { return sqrt(pow(vec.x, 2.0) + pow(vec.y, 2.0)); }

//ゼロベクトル化
void Vector::ZeroVec() {
	x = 0.0;
	y = 0.0;
}

//正規化
void Vector::Normalize() {
	double abs = AbsValue();
	if (abs == 0) return;
	x = x / abs;
	y = y / abs;
}

//ベクトルの回転
void Vector::Rotate(double angle) {
	double newX, newY;
	//回転行列を用いて反時計回りに回転させる
	newX = x * cos(angle) - y * sin(angle);
	newY = x * sin(angle) + y * cos(angle);
	x = newX;
	y = newY;
}

//ベクトルの内積
double Vector::InnerProduct(Vector vec1, Vector vec2) { return (vec1.x * vec2.x) + (vec1.y * vec2.y); }