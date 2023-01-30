#pragma once

class Input
{
public:
	Input();
	~Input();

	bool Init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight);
	bool Update();
	void ShutDown();

	bool IsKeyPressed(const unsigned char keycode);
	bool IsLeftMouseButtonPressed();
	bool IsRightMouseButtonPressed();
	bool IsEscapePressed();
	void GetMouseLocation(int& outx, int& outy);

private:
	bool ReadKeyboard();
	bool ReadMouse();
	void ProcessInput();
private:
	HWND m_inputhwnd;

	IDirectInput8* pInput;
	IDirectInputDevice8* m_keyboard;
	IDirectInputDevice8* m_mouse;

	unsigned char m_keyboardState[256] = { 0, };
	DIMOUSESTATE m_mouseState;

	int m_screenWidth = 0;
	int m_screenHeight = 0;
	POINT ptMouse;
};
