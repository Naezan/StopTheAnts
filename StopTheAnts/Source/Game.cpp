#include "pch.h"
#include "Game.h"
#include "Application.h"
#include "SubSystem/Input.h"
#include "GameObject/GameObject.h"

Game::Game(): nodes(BOARD_HEIGHT, std::vector<Node*>(BOARD_WIDTH, nullptr))
{
	std::fill(&m_Board[0][0], &m_Board[0][0] + 1600, QuadType::None);
	m_DrawQuadType = QuadType::Wall;

	//�ܼ�â ����
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
		//TODO ��Ȳ�� ���� Ŭ������ Ÿ���� �޶�� �Ѵ�
		//TODO �������̶� �������� ������ 1�����̸� �Ѿ �� ����.
		//�������� �������� 1��1�� ��Ī�Ǽ� AStar�� ���ȴ�.
		qaudObject.emplace_back(m_pDrawBrush, std::make_pair(m_MousePositionX, m_MousePositionY));
		m_Board[m_MousePositionY][m_MousePositionX] = m_DrawQuadType;
		
		//����� obstacle����
		SetNodeCollision(m_MousePositionX, m_MousePositionY);

		//TODO Ant�� UpdatePath����
	}

	//TODO Ant�� MoveBasedOnVelocity����
}

void Game::Render()
{
	//������Ʈ���� �������մϴ�.
	RenderQuad(m_MousePositionX, m_MousePositionY, m_pDrawBrush);

	for (auto& obj : qaudObject)
	{
		RenderQuad(obj.position.first, obj.position.second, obj.m_BrushType);
	}
}

void Game::RenderQuad(int x, int y, ID2D1SolidColorBrush* colorBrush)
{
	if (x < 0 || x >= (SCREEN_WIDTH / 20) || y < 0 || y >= (SCREEN_HEIGHT / 20))
		return;

	D2D1_RECT_F rect = D2D1::RectF(
		x * 20,
		y * 20,
		x * 20 + 20,
		y * 20 + 20
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
		//��ֹ�
		m_pDrawBrush = m_pBlackBrush;
		m_DrawQuadType = QuadType::Wall;
	}
	if (Application::GetInstance().lock()->GetInput()->IsKeyPressed(DIK_2))
	{
		//������
		m_pDrawBrush = m_pBrownBrush;
		m_DrawQuadType = QuadType::AntHouse;
	}
	if (Application::GetInstance().lock()->GetInput()->IsKeyPressed(DIK_3))
	{
		//������
		m_pDrawBrush = m_pGreenBrush;
		m_DrawQuadType = QuadType::Leaf;
	}
	if (Application::GetInstance().lock()->GetInput()->IsKeyPressed(DIK_4))
	{
		//���콺
		//m_pDrawBrush = m_pBlueBrush;
	}
}

void Game::SetNodeCollision(int x, int y)
{
	switch (m_DrawQuadType)
	{
	case QuadType::None:
		nodes[y][x]->bObstacle = false;
	case QuadType::Wall:
		nodes[y][x]->bObstacle = true;
		break;
	case QuadType::AntHouse:
		nodes[y][x]->bObstacle = true;
		break;
	case QuadType::Leaf:
		nodes[y][x]->bObstacle = true;
		break;
	default:
		nodes[y][x]->bObstacle = false;
		break;
	}
}
