#pragma once

enum QuadType : uint8_t
{
	None,
	Wall,
	AntHouse,
	Leaf,
	QuadMAX
};

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

	void ShutDown();

	void KeyInputProcess();

	inline const QuadType& GetBoardQuadType(int x, int y) { return m_Board[y][x]; }
	void SetNodeCollision(int x, int y);

private:
	ID2D1HwndRenderTarget* m_d2dhWnd;

	ID2D1SolidColorBrush* m_pDrawBrush;
	ID2D1SolidColorBrush* m_pBlackBrush;
	ID2D1SolidColorBrush* m_pBrownBrush;
	ID2D1SolidColorBrush* m_pGreenBrush;
	ID2D1SolidColorBrush* m_pBlueBrush;

	std::array<std::array<QuadType, BOARD_WIDTH>, BOARD_HEIGHT> m_Board;

	QuadType m_DrawQuadType;

	std::vector<class Object> qaudObject;

	int m_MousePositionX;
	int m_MousePositionY;

	std::vector<std::vector<std::shared_ptr<struct Node>>> nodes;
	//개미집을 생성할때 가장 가까운 leaf위치를 찾을때 사용합니다.
	std::vector<class Leaf> leafs;
};
