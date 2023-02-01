#include "../pch.h"
#include "GameObject.h"
#include "../Application.h"
#include "../Game.h"
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
	m_AntBrush = Application::GetInstance().lock()->GetGameInstance()->m_pBlueBrush;
}

AntHouse::~AntHouse()
{
}

void AntHouse::Update(float deltaTime)
{
	//개미를 일정 주기를 가지고 만들어줍니다.
	static float duration = 0.f;

	if (m_Ants.size() >= m_MaxAntCount)
		return;

	duration += deltaTime;
	if (duration > 1.f)
	{
		duration = 0.f;

		//TODO 개미 생성
		Assert(m_AntBrush != nullptr);
		if (!m_StartNode.expired() && !m_DestNode.expired())
		{
			auto& ant = m_Ants.emplace_back(std::make_unique<Ant>(m_AntBrush, position, m_StartNode, m_DestNode));
			ant->m_ParentHouse = shared_from_this();
		}
	}

	for (auto& ant : m_Ants)
	{
		ant->Update(deltaTime);
	}
}

void AntHouse::Render(ID2D1HwndRenderTarget* d2dhWnd)
{
	for (auto& ant : m_Ants)
	{
		ant->Render(d2dhWnd, m_AntBrush);
	}
}

void AntHouse::UpdateAntPath(std::vector<std::vector<std::shared_ptr<Node>>>& nodes)
{
	if (m_StartNode.expired() || m_DestNode.expired())
	{
		return;
	}

	//브레젠험 먼저 수행 -> 브레젠험 실패시 AStar 수행
	nodePosition.clear();
	if (!Bresenham::FindStraightPath(nodes, m_StartNode.lock()->position, m_DestNode.lock()->position, nodePosition))
	{
		nodePosition.clear();
		AStar::FindPath(nodes, m_StartNode, m_DestNode, nodePosition);
	}
}

Leaf::Leaf(ID2D1SolidColorBrush* brushType, std::pair<int, int>&& pos) :
	Object(brushType, std::move(pos))
{
}

Leaf::~Leaf()
{
}

Ant::Ant(ID2D1SolidColorBrush* brushType, std::pair<int, int> pos, std::weak_ptr<Node> houseNode, std::weak_ptr<Node> leafNode) :
	Object(brushType, std::move(pos)), m_House(houseNode), m_Leaf(leafNode)
{
}

Ant::~Ant()
{
}

void Ant::Update(float deltaTime)
{
	if (m_House.expired() || m_Leaf.expired())
	{
		return;
	}

	MoveBasedOnVelocity(deltaTime);
}

void Ant::Render(ID2D1HwndRenderTarget* d2dhWnd, ID2D1SolidColorBrush* colorBrush)
{
	if (position.first < 0 || position.first >= SCREEN_WIDTH || position.second < 0 || position.second >= SCREEN_HEIGHT)
		return;

	D2D1_RECT_F rect = D2D1::RectF(
		static_cast<float>(position.first * 20),
		static_cast<float>(position.second * 20),
		static_cast<float>(position.first * 20 + 5),
		static_cast<float>(position.second * 20 + 5)
	);

	// Draw a filled rectangle.
	d2dhWnd->FillRectangle(&rect, colorBrush);
}

void Ant::MoveBasedOnVelocity(float deltaTime)
{
	//TODO 노드위치에서 하나씩 순서대로 진행합니다. 나중에 되돌아올때 pop_back을 이용해서 다시 되돌아옵니다.
	//만약에 다시 계산해서 노드 위치가 바뀐다면 개미의 위치에서 처음부터 시행합니다.
	m_ParentHouse.lock()->nodePosition;
}
