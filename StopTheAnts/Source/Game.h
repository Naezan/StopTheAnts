#pragma once

class Game
{
public:
	Game();
	~Game();

public:
	HRESULT InitColor(ID2D1HwndRenderTarget* d2dhWnd);
	void Update(float deltaTime);
	void Render();
	void RenderQuad(int x, int y, ID2D1SolidColorBrush* colorBrush);

	void ShutDown();         // closes Direct3D and releases memory
private:
	ID2D1HwndRenderTarget* m_d2dhWnd;

	ID2D1SolidColorBrush* m_pDrawBrush;
	ID2D1SolidColorBrush* m_pBlackBrush;
	ID2D1SolidColorBrush* m_pBrownBrush;
	ID2D1SolidColorBrush* m_pGreenBrush;
	ID2D1SolidColorBrush* m_pBlueBrush;

	//오브젝트 구조체(타입, 위치)
	std::vector<class Object> qaudObject;

	int m_MousePositionX;
	int m_MousePositionY;
	//ID3D11Device* dev;                     // the pointer to our Direct3D device interface
	//ID3D11DeviceContext* devcon;           // the pointer to our Direct3D device context
};
