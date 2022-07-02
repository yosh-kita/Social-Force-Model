#pragma once
#include "Entity.h"

//��Q���̌`��͋�`�Ƃ���
class Object : public Entity {
private:
	Vector *posUpperLeft;//��`�̍���̒��_�̍��W
	Vector* posBottomRight;//��`�̉E���̒��_�̍��W

public:
	Object(double x, double y, Vector *upL, Vector *boR);
	void Draw(int originX, int originY, double drawMagnification);
	Vector GetPosUL();
	Vector GetPosBR();
};

