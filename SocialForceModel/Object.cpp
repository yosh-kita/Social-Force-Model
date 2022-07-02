#include "Object.h"
#include "Vector.h"
#include "Dxlib.h"

Object::Object(double x, double y, Vector *upL, Vector *boR) : Entity(x, y) {
	posUpperLeft = new Vector;
	*posUpperLeft = *upL;
	posBottomRight = new Vector;
	*posBottomRight = *boR;
}

//ï`é ä÷êî
void Object::Draw(int originX, int originY, double drawMagnification) {
	int x1, y1, x2, y2;
	x1 = static_cast<int>(posUpperLeft->x * drawMagnification);
	y1 = static_cast<int>(posUpperLeft->y * drawMagnification);
	x2 = static_cast<int>(posBottomRight->x * drawMagnification);
	y2 = static_cast<int>(posBottomRight->y * drawMagnification);
	DrawBox(originX + x1, originY + y1, originX + x2, originY + y2, GetColor(0, 0, 0), TRUE);
}

Vector Object::GetPosUL() { return *posUpperLeft; }
Vector Object::GetPosBR() { return *posBottomRight; }