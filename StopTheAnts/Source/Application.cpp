#include "pch.h"
#include "Application.h"
#include "Game.h"
#include "Util/Timer.h"
#include "SubSystem/Input.h"

std::shared_ptr<Application> Application::instance;

Application::Application() :
	m_hwnd(NULL),
	m_pDirect2dFactory(NULL),
	m_pRenderTarget(NULL),
	m_pLightSlateGrayBrush(NULL),
	m_pCornflowerBlueBrush(NULL)
{
	game = std::make_unique<Game>();
	inputSystem = std::make_unique<Input>();
}

Application::~Application()
{
	SafeRelease(&m_pDirect2dFactory);
	SafeRelease(&m_pRenderTarget);
	SafeRelease(&m_pLightSlateGrayBrush);
	SafeRelease(&m_pCornflowerBlueBrush);

	// 창을 제거합니다
	DestroyWindow(m_hwnd);
	m_hwnd = nullptr;

	// 프로그램 인스턴스를 제거합니다
	UnregisterClass(szWindowClass, m_hinstance);
	m_hinstance = nullptr;
}

HRESULT Application::Initialize(HINSTANCE hInstance)
{
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_STOPTHEANTS, szWindowClass, MAX_LOADSTRING);

	HRESULT hr;

	// Initialize device-indpendent resources, such
	// as the Direct2D factory.
	hr = CreateDeviceIndependentResources();

	if (SUCCEEDED(hr))
	{
		// 이 프로그램의 인스턴스를 가져옵니다
		m_hinstance = GetModuleHandle(NULL);

		// Register the window class.
		WNDCLASSEX wcex = { sizeof(WNDCLASSEX) };
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = Application::WndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = sizeof(LONG_PTR);
		wcex.hInstance = m_hinstance;
		wcex.hbrBackground = NULL;
		wcex.lpszMenuName = NULL;
		wcex.hCursor = LoadCursor(NULL, IDI_APPLICATION);
		wcex.lpszClassName = szWindowClass;

		// windows class를 등록합니다
		RegisterClassEx(&wcex);

		RECT window_rect;
		window_rect.left = 0;
		window_rect.top = 0;
		window_rect.right = SCREEN_WIDTH;
		window_rect.bottom = SCREEN_HEIGHT;

		AdjustWindowRectEx(&window_rect,
			WS_OVERLAPPEDWINDOW,
			0, 0
		);

		// 모니터 화면의 해상도를 읽어옵니다
		// 윈도우 창 가로, 세로 넓이 변수 초기화

		// 윈도우 모드의 경우 800 * 800 크기를 지정합니다.
		int screenWidth = window_rect.right - window_rect.left;
		int screenHeight = window_rect.bottom - window_rect.top;
		// 윈도우 창을 가로, 세로의 정 가운데 오도록 합니다.
		int posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		int posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;

		m_hwnd = CreateWindow(
			szWindowClass,
			szTitle,
			WS_OVERLAPPEDWINDOW,
			posX, posY, screenWidth, screenHeight,
			NULL,
			NULL,
			m_hinstance,
			this);

		if (m_hwnd)
		{
			// 윈도우를 화면에 표시하고 포커스를 지정합니다
			ShowWindow(m_hwnd, SW_SHOW);
			SetForegroundWindow(m_hwnd);
			SetFocus(m_hwnd);

			//여기에 기본적인 초기화를 호출합니다.
			if (!inputSystem->Init(m_hinstance, m_hwnd, screenWidth, screenHeight))
			{
				MessageBox(m_hwnd, L"Could not initialize the input object.", L"Error", MB_OK);
			}
		}
	}

	return hr;
}

void Application::RunMessageLoop()
{
	MSG msg;
	Timer timer;
	while (true)
	{
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (msg.message == WM_QUIT)
			break;

		// 여기서 게임 코드 실행
		timer.UpdateTimer();
		timer.RenderTimer(m_hwnd, szTitle);

		//Update
		inputSystem->Update();

		//Print Mouse Position
		{
			/*FILE* fp;
			
			AllocConsole();
			freopen_s(&fp, "CONIN$", "r", stdin);
			freopen_s(&fp, "CONOUT$", "w", stdout);
			freopen_s(&fp, "CONOUT$", "w", stderr);
			std::cout << mouseX << " " << mouseY << std::endl;*/
		}

		game->Update(timer.GetDeltaTime());

		//Render
		Render();
	}
}

void Application::GetMousePosition(int& outX, int& outY)
{
	inputSystem->GetMouseLocation(outX, outY);
}

HRESULT Application::CreateDeviceIndependentResources()
{
	HRESULT hr = S_OK;

	// Create a Direct2D factory.
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pDirect2dFactory);

	return hr;
}

HRESULT Application::CreateDeviceResources()
{
	HRESULT hr = S_OK;

	if (!m_pRenderTarget)
	{
		RECT rc;
		GetClientRect(m_hwnd, &rc);

		D2D1_SIZE_U size = D2D1::SizeU(
			rc.right - rc.left,
			rc.bottom - rc.top
		);

		// Create a Direct2D render target.
		hr = m_pDirect2dFactory->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(m_hwnd, size),
			&m_pRenderTarget
		);

		if (SUCCEEDED(hr))
		{
			// Create a gray brush.
			hr = m_pRenderTarget->CreateSolidColorBrush(
				D2D1::ColorF(D2D1::ColorF::LightSlateGray),
				&m_pLightSlateGrayBrush
			);
		}
		if (SUCCEEDED(hr))
		{
			// Create a blue brush.
			hr = m_pRenderTarget->CreateSolidColorBrush(
				D2D1::ColorF(D2D1::ColorF::CornflowerBlue),
				&m_pCornflowerBlueBrush
			);
		}

		//게임의 초기화를 호출합니다.
		if (SUCCEEDED(hr))
		{
			hr = game->InitColor(m_pRenderTarget);
		}
	}

	return hr;
}

void Application::DiscardDeviceResources()
{
	SafeRelease(&m_pRenderTarget);
	SafeRelease(&m_pLightSlateGrayBrush);
	SafeRelease(&m_pCornflowerBlueBrush);
}

HRESULT Application::Render()
{
	HRESULT hr = S_OK;

	hr = CreateDeviceResources();

	if (SUCCEEDED(hr))
	{
		m_pRenderTarget->BeginDraw();
		m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
		m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));

		D2D1_SIZE_F rtSize = m_pRenderTarget->GetSize();

		// Draw a grid background.
		int width = static_cast<int>(rtSize.width);
		int height = static_cast<int>(rtSize.height);

		for (int x = 0; x < width; x += 20)
		{
			m_pRenderTarget->DrawLine(
				D2D1::Point2F(static_cast<FLOAT>(x), 0.0f),
				D2D1::Point2F(static_cast<FLOAT>(x), rtSize.height),
				m_pLightSlateGrayBrush,
				1.0f
			);
		}

		for (int y = 0; y < height; y += 20)
		{
			m_pRenderTarget->DrawLine(
				D2D1::Point2F(0.0f, static_cast<FLOAT>(y)),
				D2D1::Point2F(rtSize.width, static_cast<FLOAT>(y)),
				m_pLightSlateGrayBrush,
				1.0f
			);
		}

		//여기에서 게임을 렌더링합니다.
		game->Render();

		hr = m_pRenderTarget->EndDraw();

		if (hr == D2DERR_RECREATE_TARGET)
		{
			hr = S_OK;
			DiscardDeviceResources();

			//여기서 ShutDown함수를 호출합니다.
			game->ShutDown();
		}
	}

	return hr;
}

void Application::OnResize(UINT width, UINT height)
{
	if (m_pRenderTarget)
	{
		// Note: This method can fail, but it's okay to ignore the
		// error here, because the error will be returned again
		// the next time EndDraw is called.
		m_pRenderTarget->Resize(D2D1::SizeU(width, height));
	}
}

LRESULT Application::WndProc(HWND _hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = 0;

	if (message == WM_CREATE)
	{
		LPCREATESTRUCT pcs = (LPCREATESTRUCT)lParam;
		Application* pDemoApp = (Application*)pcs->lpCreateParams;

		::SetWindowLongPtrW(
			_hWnd,
			GWLP_USERDATA,
			reinterpret_cast<LONG_PTR>(pDemoApp)
		);

		result = 1;
	}
	else
	{
		Application* pDemoApp = reinterpret_cast<Application*>(static_cast<LONG_PTR>(
			::GetWindowLongPtrW(
				_hWnd,
				GWLP_USERDATA
			)));

		bool wasHandled = false;

		if (pDemoApp)
		{
			switch (message)
			{
			case WM_SIZE:
			{
				UINT width = LOWORD(lParam);
				UINT height = HIWORD(lParam);
				pDemoApp->OnResize(width, height);
			}
			result = 0;
			wasHandled = true;
			break;

			case WM_DISPLAYCHANGE:
			{
				InvalidateRect(_hWnd, NULL, FALSE);
			}
			result = 0;
			wasHandled = true;
			break;

			case WM_PAINT:
			{
				ValidateRect(_hWnd, NULL);
			}
			result = 0;
			wasHandled = true;
			break;

			case WM_DESTROY:
			{
				PostQuitMessage(0);
			}
			result = 1;
			wasHandled = true;
			break;
			}
		}

		if (!wasHandled)
		{
			result = DefWindowProc(_hWnd, message, wParam, lParam);
		}
	}

	return result;
}
