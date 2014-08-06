#include "global.h"
#include "scene.h"

LPCWSTR szClassName = L"FlyStormClass";
LPCWSTR szAppName = L"FlyStormApp";
LPCWSTR szTitle = L"FlyStorm";

LARGE_INTEGER countPerSecond;

LARGE_INTEGER lastCounts;

void StartTimer()
{
	QueryPerformanceFrequency(&countPerSecond);
	QueryPerformanceCounter(&lastCounts);
}

double GetTimeInSeconds()
{
	LARGE_INTEGER currCounts;
	QueryPerformanceCounter(&currCounts);
	double startTime = (double)lastCounts.QuadPart;
	double endTime = (double)currCounts.QuadPart;
	return (endTime- startTime)/double(countPerSecond.QuadPart);
}

double GetFrameTimeInSeconds()
{
	return 0;
}

LRESULT __stdcall WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return CallWindowProc((WNDPROC)DefWindowProc, hWnd, msg, wParam, lParam);
}

INT __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, INT nCmdShow)
{
	MSG			uMsg;
	WNDCLASS	wndclass;
	HWND hWnd;

	memset(&uMsg,0,sizeof(uMsg));
	
	wndclass.style         = 0;
	wndclass.lpfnWndProc   = (WNDPROC)WndProc;
	wndclass.cbClsExtra    = 0;
	wndclass.cbWndExtra    = 0;
	wndclass.hInstance     = hInstance;
	wndclass.hIcon         = LoadIcon (hInstance, L"RayTracerIcon");
	wndclass.hCursor       = LoadCursor (NULL,IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)(COLOR_WINDOW);
	wndclass.lpszMenuName  = L"RayTracerMenu";
	wndclass.lpszClassName = szClassName;

	if(!RegisterClass(&wndclass))
	{
		MessageBox(NULL, L"Window Registration Failed!", L"Error!", MB_OK | MB_ICONEXCLAMATION);
		return GetLastError();
	}

	DWORD dwStyle = WS_OVERLAPPEDWINDOW;
	DWORD nWidth = 600;
	DWORD nHeight = 600;

	hWnd = CreateWindow(szClassName, szTitle, dwStyle, 0, 0, nWidth, nHeight, 0, 0, hInstance, NULL);

	if (hWnd == NULL)
	{
		MessageBox(NULL, L"Window Creation Failed!", L"Error!", MB_ICONEXCLAMATION | MB_OK);
		return GetLastError();
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	g_hWnd = hWnd;

	IDirect3DDevice9		* localDevice = InitD3D(hWnd, nHeight, nWidth);
	MSG mssg;
	PeekMessage( &mssg, NULL, 0, 0, PM_NOREMOVE);
	stSceneClass MainSimpleScene(localDevice);
    
	//fps variables
	const int TICKS_PER_SECOND = 60;
    const int SKIP_TICKS = 1000 / TICKS_PER_SECOND;
    const int MAX_FRAMESKIP = 10;
	DWORD next_game_tick = GetTickCount();
    DWORD lastTime = GetTickCount(), frameTime = 0;
	StartTimer();
	int frameCount = 0;
	DWORD frameTickCount = 0;

	CInput inputControl;

	while (true) 
	{
		if (PeekMessage(&mssg, NULL, 0, 0, PM_REMOVE)) 
		{
			TranslateMessage(&mssg);
			DispatchMessage(&mssg);
			if (WM_QUIT == mssg.message)
				break;
		}
		else
		{
			if (GetTimeInSeconds() > 1.0f)
			{
				{
					WCHAR message[1000];
					wsprintf(message, L"MainLoop. FPS %d\n", frameCount);
					OutputDebugString(message);
				}
				frameCount = 0;
				StartTimer();
			}

			DWORD elapsedTime = GetTickCount();
			frameTime += (elapsedTime - lastTime);
			lastTime = elapsedTime;

			if (frameTime > SKIP_TICKS)
			{
				inputControl.Update();
				MainSimpleScene.KeyboardEvent(&inputControl);
				MainSimpleScene.UpdateScene();
				MainSimpleScene.DrawObjects();
				frameCount++;
				frameTime -= SKIP_TICKS;
			}
			else
				Sleep(1);
		}
	}

	ReleaseD3D();
	UnregisterClass(szClassName, wndclass.hInstance );
	return uMsg.wParam;
}