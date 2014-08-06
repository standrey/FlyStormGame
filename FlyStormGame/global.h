#ifndef GLOBAL_H
#define GLOBAL_H

#include <windows.h>
#include <d3dx9.h>
#include <dxerr.h>
#include <d3dx9core.h>
#include <dinput.h>
#include <wchar.h>
#include <locale.h>

#if defined(DEBUG)|defined(_DEBUG)
	#ifndef D3D_DEBUG_INFO
		#define D3D_DEBUG_INFO
	#endif
#endif

#pragma comment(lib, "DxErr.lib")
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#if defined(DEBUG)|defined(_DEBUG)
	#pragma comment(lib,"d3dx9d.lib")
#endif

#if defined(RELEASE)|defined(_RELEASE)
	#pragma comment(lib,"d3dx9.lib")
#endif

static DWORD CUSTOMFVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

static DWORD CUSTOMFVFCOLOR = D3DFVF_XYZ | D3DFVF_DIFFUSE;

struct gameVertex
{
    D3DXVECTOR3 position;
	D3DXVECTOR3 normal;
	float tu, tv;

	gameVertex(const D3DXVECTOR3 & pos, float u, float v, const D3DXVECTOR3 & norm)
	{
		position = pos;
		tu = u;
		tv = v;
		normal = norm;
	}

	gameVertex() {};

};

void KeyboardControllerAcquire(char *);
void KeyboardControllerUnacquire();
IDirect3DDevice9		* InitD3D(HWND hWnd, DWORD height, DWORD width);
void ReleaseD3D();

static HWND g_hWnd;

#define RAD_TO_DEGREE(rad) (rad)*(180/3.14f)
#define DEGREE_TO_RAD(degree) (degree)/(180/3.14f)

#endif