#pragma once

class Vector;

class Entity {
protected:
	Vector* pos;//ˆÊ’u
	int graphHandle;

public:
	Entity();
	Entity(double posX, double posY);
	~Entity();
	void SetPos(double posX, double posY);
	Vector GetPos();
};

