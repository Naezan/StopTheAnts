#include "pch.h"
#include "Game.h"
#include "Application.h"

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

	return hr;
}

void Game::Update(float deltaTime)
{
	Application::GetInstance().lock()->GetMousePosition(m_MousePositionX, m_MousePositionY);

	{
		FILE* fp;

		AllocConsole();
		freopen_s(&fp, "CONIN$", "r", stdin);
		freopen_s(&fp, "CONOUT$", "w", stdout);
		freopen_s(&fp, "CONOUT$", "w", stderr);
		std::cout << m_MousePositionX << " " << m_MousePositionY << std::endl;
	}
}

void Game::Render()
{
	//TODO 오브젝트들을 렌더링합니다.

	m_MousePositionX /= 20;
	m_MousePositionY /= 20;
	D2D1_RECT_F rectangle1 = D2D1::RectF(
		m_MousePositionX * 20,
		m_MousePositionY * 20,
		m_MousePositionX * 20 + 20,
		m_MousePositionY * 20 + 20
	);

	// Draw a filled rectangle.
	m_d2dhWnd->FillRectangle(&rectangle1, m_pBlackBrush);
}

void Game::ShutDown()
{
	SafeRelease(&m_pBlackBrush);
	SafeRelease(&m_pBrownBrush);
	SafeRelease(&m_pGreenBrush);
	SafeRelease(&m_pBlueBrush);
}
