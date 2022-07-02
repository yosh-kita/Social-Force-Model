#pragma once

class Map {
private:
	double w;
	double h;
	int graphHandle;

public:
	Map();
	Map(double width, double height);
	void Draw(int originX, int originY, double drawMagnification);

	double getW() { return w; }
	double getH() { return h; }
};