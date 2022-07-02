#pragma once
#include "Entity.h"

//障害物の形状は矩形とする
class Object : public Entity {
private:
	Vector *posUpperLeft;//矩形の左上の頂点の座標
	Vector* posBottomRight;//矩形の右下の頂点の座標

public:
	Object(double x, double y, Vector *upL, Vector *boR);
	void Draw(int originX, int originY, double drawMagnification);
	Vector GetPosUL();
	Vector GetPosBR();
};

