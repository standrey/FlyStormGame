#include "cinput.h"

CInput::CInput(void)
{
	m_pDIObject = NULL;
	m_pDIKeyboardDevice = NULL;

	if(!InitKeyboard()) return;
} 

CInput::~CInput(void)
{
	if(m_pDIKeyboardDevice != NULL)
	{
		m_pDIKeyboardDevice->Unacquire();
		m_pDIKeyboardDevice->Release();
		m_pDIKeyboardDevice = NULL;
	}
	
	if(m_pDIObject != NULL)
	{
		m_pDIObject->Release();
		m_pDIObject = NULL;
	}
}

bool CInput::InitKeyboard(void)
{
	HRESULT hr = DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID **)&m_pDIObject, NULL);
    if (FAILED(hr))
    {
		MessageBox(NULL, L"Failed to create input device!",L"InitializeDevice()", MB_OK | MB_ICONEXCLAMATION);
		return false;
    }

	if(FAILED(m_pDIObject->CreateDevice(GUID_SysKeyboard, &m_pDIKeyboardDevice, NULL)))
	{
		MessageBox(NULL, L"CreateDevice() failed!", L"InitKeyboard()", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}
	
	if(FAILED(m_pDIKeyboardDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		MessageBox(NULL, L"SetDataFormat() failed!", L"InitKeyboard()", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}

	if (FAILED(m_pDIKeyboardDevice->SetCooperativeLevel(g_hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE)))
	{
		MessageBox(NULL, L"SetCooperativeLevel() failed!", L"InitKeyboard()", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}
	
	if(FAILED(m_pDIKeyboardDevice->Acquire()))
	{
		MessageBox(NULL, L"Acquire() failed!", L"InitKeyboard()", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}
	return true;
}

bool CInput::Update(void)
{
	if(FAILED(m_pDIKeyboardDevice->GetDeviceState(sizeof(m_KeyBuffer), (LPVOID)&m_KeyBuffer)))
	{
		MessageBox(NULL, L"GetDeviceState() failed!", L"Update()", MB_OK);
		return false;
	}
	if (m_KeyBuffer[DIK_SPACE] & 0x80)
	{
		OutputDebugString(L"CInput. DIK_SPACE\n");
	}
	if (m_KeyBuffer[DIK_UP] & 0x80)
	{
		OutputDebugString(L"CInput. DIK_UP\n");
	}
	return true;
}

bool CInput::KeyPressed(CInput::MyKeys pressedKey)
{
	switch (pressedKey)
	{
	case CInput::DOWN:
		return bool(m_KeyBuffer[DIK_S] & 0x80);
	case CInput::UP:
		return bool(m_KeyBuffer[DIK_W] & 0x80);
	case CInput::LEFT:
		return bool(m_KeyBuffer[DIK_A] & 0x80);
	case CInput::RIGHT:
		return bool(m_KeyBuffer[DIK_D] & 0x80);
	case CInput::SPACE:
			return bool(m_KeyBuffer[DIK_SPACE] & 0x80);
	}
}

bool CInput::MouseEvent(MSG* msg)
{
	return false;
}

bool CInput::KeyboardEvent(MSG* msg)
{
	switch (msg->wParam)
	{
	case VK_SPACE:
		m_KeyBuffer[DIK_SPACE] = 0x80;
		return true;
	case VK_UP:
		m_KeyBuffer[DIK_UP] = 0x80;
		return true;
	case VK_DOWN:
		m_KeyBuffer[DIK_DOWN] = 0x80;
		return true;
	case VK_LEFT:
		m_KeyBuffer[DIK_LEFT] = 0x80;
		return true;
	case VK_RIGHT:
		m_KeyBuffer[DIK_RIGHT] = 0x80;
		return true;
	}
	return false;
}