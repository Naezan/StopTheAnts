#include "../pch.h"
#include "Input.h"

Input::Input() : pInput(nullptr), m_keyboard(nullptr), m_mouse(nullptr)
{
}

Input::~Input()
{

}

bool Input::Init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight)
{
	m_inputhwnd = hwnd;
	// 마우스 커서의 위치 지정에 사용될 화면 크기를 설정합니다.
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	// Direct Input 인터페이스를 초기화 합니다.
	if (FAILED(DirectInput8Create(hinstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&pInput, nullptr)))
	{
		return false;
	}
	// 키보드의 Direct Input 인터페이스를 생성합니다
	if (FAILED(pInput->CreateDevice(GUID_SysKeyboard, &m_keyboard, nullptr)))
	{
		return false;
	}
	// 데이터 형식을 설정하십시오. 이 경우 키보드이므로 사전 정의 된 데이터 형식을 사용할 수 있습니다.
	if (FAILED(m_keyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		return false;
	}
	// 다른 프로그램과 공유하지 않도록 키보드의 협조 수준을 설정합니다
	if (FAILED(m_keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE)))
	{
		return false;
	}
	// 키보드를 할당받는다
	HRESULT result = m_keyboard->Acquire();
	if (FAILED(result))
	{
		MessageBoxW(hwnd, L"DirectInputDevice8::Acquire failed", L"Error", MB_ICONERROR | MB_OK);
		return false;
	}

	// 마우스 Direct Input 인터페이스를 생성합니다.
	if (FAILED(pInput->CreateDevice(GUID_SysMouse, &m_mouse, NULL)))
	{
		return false;
	}

	// 미리 정의 된 마우스 데이터 형식을 사용하여 마우스의 데이터 형식을 설정합니다.
	if (FAILED(m_mouse->SetDataFormat(&c_dfDIMouse)))
	{
		return false;
	}

	// 다른 프로그램과 공유 할 수 있도록 마우스의 협력 수준을 설정합니다.
	if (FAILED(m_mouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)))
	{
		return false;
	}

	// 마우스를 할당받는다
	result = m_mouse->Acquire();
	if (FAILED(result))
	{
		MessageBoxW(hwnd, L"DirectInputDevice8::Acquire failed", L"Error", MB_ICONERROR | MB_OK);
		return false;
	}

	return true;
}

bool Input::Update()
{
	// 키보드의 현재 상태를 읽는다.
	if (!ReadKeyboard())
	{
		return false;
	}

	// 마우스의 현재 상태를 읽는다.
	if (!ReadMouse())
	{
		return false;
	}

	// 키보드와 마우스의 변경상태를 처리합니다.
	ProcessInput();

	return true;
}

void Input::ShutDown()
{
	// 마우스를 반환합니다.
	if (m_mouse)
	{
		m_mouse->Unacquire();
		m_mouse->Release();
		m_mouse = nullptr;
	}

	// 키보드를 반환합니다.
	if (m_keyboard)
	{
		m_keyboard->Unacquire();
		m_keyboard->Release();
		m_keyboard = nullptr;
	}

	// m_directInput 객체를 반환합니다.
	if (pInput)
	{
		pInput->Release();
		pInput = nullptr;
	}
}

bool Input::IsKeyPressed(const unsigned char keycode)
{
	return m_keyboardState[keycode] & 0x80;
}

bool Input::IsLeftMouseButtonPressed()
{
	return m_mouseState.rgbButtons[0] & 0x80;
}

bool Input::IsRightMouseButtonPressed()
{
	return m_mouseState.rgbButtons[1] & 0x80;
}

bool Input::IsEscapePressed()
{
	// escape 키가 현재 눌려지고 있는지 bit값을 계산하여 확인한다.
	if (m_keyboardState[DIK_ESCAPE] & 0x80)
	{
		return true;
	}

	return false;
}

void Input::GetMouseLocation(int& outx, int& outy)
{
	outx = ptMouse.x;
	outy = ptMouse.y;
}

bool Input::ReadKeyboard()
{
	// 키보드 디바이스를 얻는다.
	HRESULT result = m_keyboard->GetDeviceState(sizeof(m_keyboardState), (LPVOID)&m_keyboardState);
	if (FAILED(result))
	{
		// 키보드가 포커스를 잃었거나 획득되지 않은 경우 컨트롤을 다시 가져 온다
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			m_keyboard->Acquire();
		}
		else
		{
			return false;
		}
	}

	return true;
}

bool Input::ReadMouse()
{
	// 마우스 디바이스를 얻는다.
	HRESULT result = m_mouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&m_mouseState);
	if (FAILED(result))
	{
		// 마우스가 포커스를 잃었거나 획득되지 않은 경우 컨트롤을 다시 가져 온다
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			m_mouse->Acquire();
		}
		else
		{
			return false;
		}
	}

	return true;
}

void Input::ProcessInput()
{
	GetCursorPos(&ptMouse);
	ScreenToClient(m_inputhwnd, &ptMouse);
}
