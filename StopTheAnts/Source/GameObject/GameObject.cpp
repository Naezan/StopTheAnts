#include "../pch.h"
#include "GameObject.h"
#include "../Math/AStar.h"

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

Leaf::Leaf(ID2D1SolidColorBrush* brushType, std::pair<int, int>&& pos) :
	Object(brushType, std::move(pos))
{
}

Leaf::~Leaf()
{
}

Ant::Ant(ID2D1SolidColorBrush* brushType, std::pair<int, int>&& pos) :
	Object(brushType, std::move(pos)), m_House(nullptr), m_Leaf(nullptr)
{
}

Ant::~Ant()
{
}

void Ant::UpdatePath(std::vector<std::vector<Node*>>& nodes)
{
	if (m_House == nullptr && m_Leaf == nullptr)
		return;

	AStar::InitNodes(nodes);
	AStar::FindPath(nodes, m_House, m_Leaf);
}

void Ant::MoveBasedOnVelocity()
{
}
