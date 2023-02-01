#include "pch.h"
#include "Game.h"
#include "Application.h"
#include "SubSystem/Input.h"
#include "GameObject/GameObject.h"
#include "Math/QeadTree.h"

Game::Game() : nodes(BOARD_HEIGHT, std::vector<std::shared_ptr<Node>>(BOARD_WIDTH))
{

	std::fill(&m_Board[0][0], &m_Board[0][0] + 1600, QuadType::Q_None);
	m_MouseQuadType = QuadType::Q_Wall;

	InitNode();

	//콘솔창 생성
	/*FILE* fp;

	AllocConsole();
	freopen_s(&fp, "CONIN$", "r", stdin);
	freopen_s(&fp, "CONOUT$", "w", stdout);
	freopen_s(&fp, "CONOUT$", "w", stderr);*/
}

Game::~Game()
{
	ShutDown();
}

void Game::InitNode()
{
	for (int x = 0; x < BOARD_WIDTH; ++x)
	{
		for (int y = 0; y < BOARD_HEIGHT; ++y)
		{
			nodes[y][x] = std::make_shared<Node>();
			nodes[y][x]->position.x = x;
			nodes[y][x]->position.y = y;
			nodes[y][x]->bObstacle = false;
			nodes[y][x]->parent.reset();
			nodes[y][x]->bVisited = false;
		}
	}

	for (int x = 0; x < BOARD_WIDTH; ++x)
	{
		for (int y = 0; y < BOARD_HEIGHT; ++y)
		{
			if (y > 0)
				nodes[y][x]->vecNeighbours.push_back(nodes[y - 1][x]);
			if (y < BOARD_HEIGHT - 1)
				nodes[y][x]->vecNeighbours.push_back(nodes[y + 1][x]);
			if (x > 0)
				nodes[y][x]->vecNeighbours.push_back(nodes[y][x - 1]);
			if (x < BOARD_WIDTH - 1)
				nodes[y][x]->vecNeighbours.push_back(nodes[y][x + 1]);
		}
	}
}

HRESULT Game::InitColor(ID2D1HwndRenderTarget* d2dhWnd)
{
	m_d2dhWnd = d2dhWnd;

	HRESULT hr = S_OK;

	if (SUCCEEDED(hr))
	{
		hr = d2dhWnd->CreateSolidColorBrush(
			D2D1::ColorF(D2D1::ColorF::Black),
			&m_pBlackBrush
		);
	}
	if (SUCCEEDED(hr))
	{
		hr = d2dhWnd->CreateSolidColorBrush(
			D2D1::ColorF(D2D1::ColorF::Brown),
			&m_pBrownBrush
		);
	}
	if (SUCCEEDED(hr))
	{
		hr = d2dhWnd->CreateSolidColorBrush(
			D2D1::ColorF(D2D1::ColorF::Green),
			&m_pGreenBrush
		);
	}
	if (SUCCEEDED(hr))
	{
		hr = d2dhWnd->CreateSolidColorBrush(
			D2D1::ColorF(D2D1::ColorF::Blue),
			&m_pBlueBrush
		);
	}

	m_pDrawBrush = m_pBlackBrush;

	return hr;
}

void Game::Update(float deltaTime)
{
	Application::GetInstance().lock()->GetMousePosition(m_MousePositionX, m_MousePositionY);
	m_MousePositionX /= 20;
	m_MousePositionY /= 20;

	KeyInputProcess();

	if (Application::GetInstance().lock()->GetInput()->IsLeftMouseButtonPressed())
	{
		if (m_MousePositionX < 0 || m_MousePositionX > 40 || m_MousePositionY < 0 || m_MousePositionY > 40)
			return;

		//이미 있는곳에는 칠하지 않습니다.
		if (m_Board[m_MousePositionY][m_MousePositionX] != QuadType::Q_None)
			return;

		m_Board[m_MousePositionY][m_MousePositionX] = m_MouseQuadType;

		AddQuadObjectByQuadType(m_MouseQuadType);

		//노드의 obstacle설정
		SetNodeCollision(m_MousePositionX, m_MousePositionY);

		//AntHouse들 UpdateAntPath실행
		for (auto& house : antHouses)
		{
			house.lock()->UpdateAntPath(nodes);
		}
	}

	//AntHouse들 Update실행
	for (auto& house : antHouses)
	{
		house.lock()->Update(deltaTime);
	}
}

void Game::Render()
{
	//오브젝트들을 렌더링합니다.
	RenderQuad(m_MousePositionX, m_MousePositionY, m_pDrawBrush);

	for (auto& obj : qaudObject)
	{
		RenderQuad(obj->position.first, obj->position.second, obj->m_BrushType);
	}

	//개미를 렌더링합니다.
	for (auto& house : antHouses)
	{
		house.lock()->Render(m_d2dhWnd);
	}
}

void Game::RenderQuad(int x, int y, ID2D1SolidColorBrush* colorBrush)
{
	if (x < 0 || x >= (SCREEN_WIDTH / 20) || y < 0 || y >= (SCREEN_HEIGHT / 20))
		return;

	D2D1_RECT_F rect = D2D1::RectF(
		static_cast<float>(x * 20),
		static_cast<float>(y * 20),
		static_cast<float>(x * 20 + 20),
		static_cast<float>(y * 20 + 20)
	);

	// Draw a filled rectangle.
	m_d2dhWnd->FillRectangle(&rect, colorBrush);
}

void Game::ShutDown()
{
	SafeRelease(&m_pBlackBrush);
	SafeRelease(&m_pBrownBrush);
	SafeRelease(&m_pGreenBrush);
	SafeRelease(&m_pBlueBrush);
}

void Game::KeyInputProcess()
{
	if (Application::GetInstance().lock()->GetInput()->IsKeyPressed(DIK_1))
	{
		//장애물
		m_pDrawBrush = m_pBlackBrush;
		m_MouseQuadType = QuadType::Q_Wall;
	}
	if (Application::GetInstance().lock()->GetInput()->IsKeyPressed(DIK_2))
	{
		//개미집
		m_pDrawBrush = m_pBrownBrush;
		m_MouseQuadType = QuadType::Q_AntHouse;
	}
	if (Application::GetInstance().lock()->GetInput()->IsKeyPressed(DIK_3))
	{
		//나뭇잎
		m_pDrawBrush = m_pGreenBrush;
		m_MouseQuadType = QuadType::Q_Leaf;
	}
	if (Application::GetInstance().lock()->GetInput()->IsKeyPressed(DIK_4))
	{
		//마우스
		//m_pDrawBrush = m_pBlueBrush;
	}
}

void Game::SetNodeCollision(int x, int y)
{
	switch (m_MouseQuadType)
	{
	case QuadType::Q_None:
		nodes[y][x]->bObstacle = false;
	case QuadType::Q_Wall:
		nodes[y][x]->bObstacle = true;
		break;
	case QuadType::Q_AntHouse:
		nodes[y][x]->bObstacle = true;
		break;
	case QuadType::Q_Leaf:
		nodes[y][x]->bObstacle = true;
		break;
	default:
		nodes[y][x]->bObstacle = false;
		break;
	}
}

void Game::AddQuadObjectByQuadType(QuadType type)
{
	switch (type)
	{
	case Q_Wall:
		AddQuadObject<Wall>(m_pDrawBrush, std::make_pair(m_MousePositionX, m_MousePositionY));
		break;
	case Q_AntHouse:
	{
		auto house = antHouses.emplace_back(AddQuadObject<AntHouse>(m_pDrawBrush, std::make_pair(m_MousePositionX, m_MousePositionY)));
		//개미집이 추가되었을때 leaf벡터를 순회하면서 최단거리 leaf를 갱신합니다. 시간복잡도는 N입니다.
		SetShortestLeafByLoopingLeaf(house);
	}
	break;
	case Q_Leaf:
	{
		auto leaf = leafs.emplace_back(AddQuadObject<Leaf>(m_pDrawBrush, std::make_pair(m_MousePositionX, m_MousePositionY)));
		//leaf가 추가되었을때 모든개미집들과 leaf를 비교하여 갱신합니다. 시간복잡도는 N입니다.
		SetShortestLeafByLoopingHouse(leaf);
	}
	break;
	}
}

void Game::SetShortestLeafByLoopingLeaf(std::weak_ptr<class AntHouse> house)
{
	for (auto& leaf : leafs)
	{
		float checkDist = Math::Distance(house.lock()->position.first, house.lock()->position.second, leaf.lock()->position.first, leaf.lock()->position.first);
		if (checkDist < house.lock()->m_Distance)
		{
			house.lock()->m_StartNode = nodes[house.lock()->position.second][house.lock()->position.first];
			house.lock()->m_DestNode = nodes[leaf.lock()->position.second][leaf.lock()->position.first];
			house.lock()->m_Distance = std::move(checkDist);
		}
	}
}

void Game::SetShortestLeafByLoopingHouse(std::weak_ptr<class Leaf> leaf)
{
	for (auto& house : antHouses)
	{
		float checkDist = Math::Distance(leaf.lock()->position.first, leaf.lock()->position.second, leaf.lock()->position.first, leaf.lock()->position.first);
		if (checkDist < house.lock()->m_Distance)
		{
			house.lock()->m_StartNode = nodes[house.lock()->position.second][house.lock()->position.first];
			house.lock()->m_DestNode = nodes[leaf.lock()->position.second][leaf.lock()->position.first];
			house.lock()->m_Distance = std::move(checkDist);
		}
	}
}
