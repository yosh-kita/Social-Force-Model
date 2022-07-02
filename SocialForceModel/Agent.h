#pragma once
#include "Entity.h"
#include <list>

class Vector;
class Object;
enum class Direction;

class Agent : public Entity {
private:
	Vector* vel;
	Vector* goalPos;
	Vector* drivingForce;
	Vector* repulsiveForce;
	double targetSpeed;
	int id;//エージェントの識別番号
	static int n;//エージェントの総数

public:
	Agent(std::list<Agent>& lAgent);
	~Agent();
	void CalDrivingForce();
	void CalRepulsiveForce(std::list<Agent>& lAgent, std::list<Object>& lObject);
	Vector CalOneAgtReplusive(Agent& agent, double ri, double rj);
	Vector CalOneObjReplusive(Object& object, double ri);
	void CalVelocity();
	void Move(std::list<Object>& lObject);
	bool CheckArrived();
	static Direction GetRandDir();
	static Vector GetPosDir(Direction dir);
	void ShiftPos(Direction dir, Vector& pos, double l);
	void SetGoalPos(double posX, double posY);
	bool CheckExistAgt(Vector pos, double r, std::list<Agent>& lAgent);
	void Draw(int originX, int originY, double drawMagnification);
	Vector posLineToPoint(double x, double y, double x1, double y1, double x2, double y2);
};