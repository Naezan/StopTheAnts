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
	//���̸� ���� �ֱ⸦ ������ ������ݴϴ�.
	static float duration = 0.f;

	if (m_Ants.size() >= m_MaxAntCount)
		return;

	duration += deltaTime;
	if (duration > 1.f)
	{
		duration = 0.f;

		//TODO ���� ����
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

	//�극���� ���� ���� -> �극���� ���н� AStar ����
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
	//TODO �����ġ���� �ϳ��� ������� �����մϴ�. ���߿� �ǵ��ƿö� pop_back�� �̿��ؼ� �ٽ� �ǵ��ƿɴϴ�.
	//���࿡ �ٽ� ����ؼ� ��� ��ġ�� �ٲ�ٸ� ������ ��ġ���� ó������ �����մϴ�.
	m_ParentHouse.lock()->nodePosition;
}
