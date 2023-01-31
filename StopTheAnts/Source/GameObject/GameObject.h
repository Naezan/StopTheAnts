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

	void Update();
	void UpdateAntPath(std::vector<std::vector<std::shared_ptr<Node>>>& nodes);

	float m_Distance = INFINITY;
	std::weak_ptr<Node> m_DestLeaf;
private:
	//TODO 개미 생성은 여기서 합니다. 이때 개미의 House와 Leaf를 지정해줍니다.
	std::vector<class Ant*> m_Ants;
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

	void Update();
	void UpdatePath(std::vector<std::vector<std::shared_ptr<Node>>>& nodes);
	void MoveBasedOnVelocity();

private:
	//FSM
	std::weak_ptr<Node> m_House;
	std::weak_ptr<Node> m_Leaf;
};
