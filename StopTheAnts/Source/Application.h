#pragma once

#include "Resource.h"

class Application
{
public:
	Application();
	~Application();

	static std::weak_ptr<Application> GetInstance()
	{
		if (instance == nullptr)
		{
			instance = std::make_shared<Application>();
		}

		return instance;
	}

private:
	static std::shared_ptr<Application> instance;

public:
	// Register the window class and call methods for instantiating drawing resources
	HRESULT Initialize(HINSTANCE hInstance);

	// Process and dispatch messages
	void RunMessageLoop();

	void GetMousePosition(int& outX, int& outY);

private:
	// Initialize device-independent resources.
	HRESULT CreateDeviceIndependentResources();

	// Initialize device-dependent resources.
	HRESULT CreateDeviceResources();

	// Release device-dependent resource.
	void DiscardDeviceResources();

	// Draw content.
	HRESULT Render();

	// Resize the render target.
	void OnResize(
		UINT width,
		UINT height
	);

	// The windows procedure.
	static LRESULT CALLBACK WndProc(
		HWND _hWnd,
		UINT message,
		WPARAM wParam,
		LPARAM lParam
	);

private:
	WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
	WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
	HINSTANCE m_hinstance;
	HWND m_hwnd;
	ID2D1Factory* m_pDirect2dFactory;
	ID2D1HwndRenderTarget* m_pRenderTarget;
	ID2D1SolidColorBrush* m_pLightSlateGrayBrush;
	ID2D1SolidColorBrush* m_pCornflowerBlueBrush;

	std::unique_ptr<class Game> game;
	std::unique_ptr<class Input> inputSystem;
};