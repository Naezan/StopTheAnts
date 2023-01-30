#include "pch.h"
#include "Game.h"
#include "Application.h"
#include "SubSystem/Input.h"
#include "GameObject/GameObject.h"

Game::Game()
{
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

	if (Application::GetInstance().lock()->GetInput()->IsKeyPressed(DIK_1))
	{
		//장애물
		m_pDrawBrush = m_pBlackBrush;
	}
	if (Application::GetInstance().lock()->GetInput()->IsKeyPressed(DIK_2))
	{
		//개미집
		m_pDrawBrush = m_pBrownBrush;
	}
	if (Application::GetInstance().lock()->GetInput()->IsKeyPressed(DIK_3))
	{
		//나뭇잎
		m_pDrawBrush = m_pGreenBrush;
	}
	if (Application::GetInstance().lock()->GetInput()->IsKeyPressed(DIK_4))
	{
		//마우스
		//m_pDrawBrush = m_pBlueBrush;
	}

	if (Application::GetInstance().lock()->GetInput()->IsLeftMouseButtonPressed())
	{
		qaudObject.emplace_back(m_pDrawBrush, std::make_pair(m_MousePositionX, m_MousePositionY));
	}
	{
		/*FILE* fp;

		AllocConsole();
		freopen_s(&fp, "CONIN$", "r", stdin);
		freopen_s(&fp, "CONOUT$", "w", stdout);
		freopen_s(&fp, "CONOUT$", "w", stderr);
		std::cout << m_MousePositionX << " " << m_MousePositionY << std::endl;*/
	}
}

void Game::Render()
{
	//오브젝트들을 렌더링합니다.
	RenderQuad(m_MousePositionX, m_MousePositionY, m_pDrawBrush);

	for (auto& obj : qaudObject)
	{
		RenderQuad(obj.position.first, obj.position.second, obj.m_BrushType);
	}
}

void Game::RenderQuad(int x, int y, ID2D1SolidColorBrush* colorBrush)
{
	if(x < 0 || x >= (SCREEN_WIDTH / 20) || y < 0 || y >= (SCREEN_HEIGHT / 20))
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
