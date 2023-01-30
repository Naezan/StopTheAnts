#pragma once

#include "../Util/Node.h"

class Object
{
public:
	Object(ID2D1SolidColorBrush* brushType, std::pair<int, int>&& pos);
	virtual ~Object();

public:
	ID2D1SolidColorBrush* m_BrushType;
	std::pair<int, int> position;
};

class Wall : public Object
{
public:
	Wall(ID2D1SolidColorBrush* brushType, std::pair<int, int>&& pos);
	~Wall();

private:

};

class AntHouse : public Object
{
public:
	AntHouse(ID2D1SolidColorBrush* brushType, std::pair<int, int>&& pos);
	~AntHouse();

private:

};

class Leaf : public Object
{
public:
	Leaf(ID2D1SolidColorBrush* brushType, std::pair<int, int>&& pos);
	~Leaf();

private:

};

class Ant: public Object
{
public:
	Ant(ID2D1SolidColorBrush* brushType, std::pair<int, int>&& pos);
	~Ant();

	void UpdatePath(std::vector<std::vector<Node*>>& nodes);
	void MoveBasedOnVelocity();

private:
	//FSM
	Node* m_House;
	Node* m_Leaf;
};
