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

class AntHouse : public Object, std::enable_shared_from_this<AntHouse>
{
public:
	AntHouse(ID2D1SolidColorBrush* brushType, std::pair<int, int>&& pos);
	~AntHouse();

	void Update(float deltaTime);
	void Render(ID2D1HwndRenderTarget* d2dhWnd);
	void UpdateAntPath(std::vector<std::vector<std::shared_ptr<Node>>>& nodes);

	float m_Distance = INFINITY;
	std::weak_ptr<Node> m_StartNode;
	std::weak_ptr<Node> m_DestNode;

	std::deque<Vec2D> nodePosition;
private:
	//TODO 개미 생성은 여기서 합니다. 이때 개미의 House와 Leaf를 지정해줍니다.
	ID2D1SolidColorBrush* m_AntBrush;
	std::vector<std::unique_ptr<class Ant>> m_Ants;

	int m_MaxAntCount = 100;
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
	Ant(ID2D1SolidColorBrush* brushType, std::pair<int, int> pos, std::weak_ptr<Node> houseNode, std::weak_ptr<Node> leafNode);
	~Ant();

	void Update(float deltaTime);
	void Render(ID2D1HwndRenderTarget* d2dhWnd, ID2D1SolidColorBrush* colorBrush);
	void MoveBasedOnVelocity(float deltaTime);

	std::weak_ptr<AntHouse> m_ParentHouse;
	std::weak_ptr<Node> m_House;
	std::weak_ptr<Node> m_Leaf;
private:
	//FSM
};
