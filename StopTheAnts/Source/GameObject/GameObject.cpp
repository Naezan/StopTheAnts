#include "../pch.h"
#include "GameObject.h"
#include "../Math/AStar.h"
#include "../Math/Bresenham.h"

Object::Object(ID2D1SolidColorBrush* brushType, std::pair<int, int>&& pos) : m_BrushType(brushType), position(std::move(pos))
{
}

Object::~Object()
{
}

Wall::Wall(ID2D1SolidColorBrush* brushType, std::pair<int, int>&& pos) :
	Object(brushType, std::move(pos))
{
}

Wall::~Wall()
{
}

AntHouse::AntHouse(ID2D1SolidColorBrush* brushType, std::pair<int, int>&& pos) :
	Object(brushType, std::move(pos))
{
}

AntHouse::~AntHouse()
{
}

void AntHouse::Update()
{
	for (auto& ant : m_Ants)
	{
		ant->Update();
	}
}

void AntHouse::UpdateAntPath(std::vector<std::vector<std::shared_ptr<Node>>>& nodes)
{
	for (auto& ant : m_Ants)
	{
		ant->UpdatePath(nodes);
	}
}

Leaf::Leaf(ID2D1SolidColorBrush* brushType, std::pair<int, int>&& pos) :
	Object(brushType, std::move(pos))
{
}

Leaf::~Leaf()
{
}

Ant::Ant(ID2D1SolidColorBrush* brushType, std::pair<int, int>&& pos) :
	Object(brushType, std::move(pos))
{
}

Ant::~Ant()
{
}

void Ant::Update()
{
	MoveBasedOnVelocity();
}

void Ant::UpdatePath(std::vector<std::vector<std::shared_ptr<Node>>>& nodes)
{
	if (m_House.lock() == nullptr || m_Leaf.lock() == nullptr)
		return;
	
	//브레젠험 먼저 수행 -> 브레젠험 실패시 AStar 수행
	if (!Bresenham::FindStraightPath(nodes, m_House.lock()->position, m_Leaf.lock()->position))
	{
		AStar::InitNodes(nodes);
		AStar::FindPath(nodes, m_House, m_Leaf);
	}
}

void Ant::MoveBasedOnVelocity()
{
}
