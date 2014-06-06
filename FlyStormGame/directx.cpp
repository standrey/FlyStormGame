#include "global.h"

static IDirect3D9			* pDxObject;
static IDirect3DDevice9		* pDxDevice;

IDirect3DDevice9 * InitD3D(HWND hWnd, DWORD height, DWORD width)
{
	HRESULT hr;
	pDxObject = Direct3DCreate9(D3D_SDK_VERSION);
	if (pDxObject == NULL)
    {
        MessageBox( NULL ,L"DirectX Runtime library not installed!",L"InitializeDevice()",MB_OK | MB_ICONEXCLAMATION);
		//return GetLastError();
		return NULL;
    }
	
    D3DDISPLAYMODE d3ddm;
	pDxObject->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &d3ddm );
	
    D3DPRESENT_PARAMETERS dx_PresParams;
    ZeroMemory( &dx_PresParams, sizeof(dx_PresParams) );
	dx_PresParams.hDeviceWindow = hWnd;
    dx_PresParams.Windowed = TRUE;
    dx_PresParams.SwapEffect = D3DSWAPEFFECT_DISCARD;
	dx_PresParams.BackBufferCount       = 1;
    dx_PresParams.BackBufferFormat       = d3ddm.Format;
    dx_PresParams.EnableAutoDepthStencil = TRUE;
    dx_PresParams.AutoDepthStencilFormat = D3DFMT_D16;
    dx_PresParams.PresentationInterval   = D3DPRESENT_INTERVAL_IMMEDIATE;
	
	hr = pDxObject->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &dx_PresParams, &pDxDevice);
    if (FAILED(hr))
    {
		MessageBox(NULL,L"Failed to create device!",L"InitializeDevice()",MB_OK | MB_ICONEXCLAMATION);
		//return GetLastError();
		pDxObject->Release();
		return NULL;
    }

	return pDxDevice;
}

void ReleaseD3D()
{
	if (pDxDevice != NULL)
		pDxDevice->Release();
	if (pDxObject != NULL)
		pDxObject->Release();
}