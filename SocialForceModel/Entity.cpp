#include "Entity.h"
#include "Dxlib.h"
#include "Vector.h"

Entity::Entity() {
	pos = new Vector;
}

Entity::Entity(double posX, double posY) {
	pos = new Vector(posX, posY);
}

Entity::~Entity() {
	delete pos;
}


void Entity::SetPos(double posX, double posY) {
	pos->x = posX;
	pos->y = posY;
}

Vector Entity::GetPos() { return *pos; }