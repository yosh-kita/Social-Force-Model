#pragma once
#include <list>

class Map;
class Agent;
class Object;
class Vector;
enum class Direction;

class Manager {
private:
	Map* map;
	std::list<Agent> lAgent;
	std::list<Object> lObject;

public:
	Manager();
	~Manager();
	void Process();
	void CreateAgt();
	void CreateObj(double x1, double y1, double x2, double y2);
};