#pragma once

class Vector {
private:

public:
	Vector operator+(Vector vector);
	void operator+=(Vector vector);
	Vector operator-(Vector vector);
	void operator-=(Vector vector);
	Vector operator*(double k);
	Vector operator/(double k);

	double x;
	double y;

	Vector();
	Vector(double X, double Y);
	double AbsValue();
	static double AbsValue(Vector vec);
	void ZeroVec();
	void Normalize();
	void Rotate(double angle);
	static double InnerProduct(Vector vec1, Vector vec2);
};

