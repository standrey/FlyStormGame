#pragma comment(lib, "user32.lib")
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9d.lib")
//#pragma comment(lib, "dinput8.lib")
//#pragma comment(lib, "dxguid.lib")

#include <windows.h>
//#include <dinput.h>
#include <list>

#include "building.h"
#include "debug.h"

/*Globals*/
DebugClass * oneDebugObject = NULL;
Building * oneBuilding = NULL;

/*directx*/
LPDIRECT3DDEVICE9 gp_dx_OutputDevice;
/*
LPDIRECTINPUT8 gp_dx_InputObject;
LPDIRECTINPUTDEVICE8 gp_dx_MouseDevice;
LPDIRECTINPUTDEVICE8 gp_dx_KeybDevice;
*/
LPDIRECT3DTEXTURE9 gp_dx_Texture;
LPDIRECT3DVERTEXBUFFER9 gp_dx_VertexBuffer1;
LPDIRECT3DVERTEXBUFFER9 gp_dx_VertexAxes;
IDirect3DTexture9 * gpTextureBullet;
D3DXQUATERNION xwingRotation;
D3DXQUATERNION cameraRotation;
D3DXVECTOR3 xwingPosition(10,10,10);

IDirect3DVertexBuffer9 * gp_dx_Bullet;

//---> skybox variables
unsigned long NumMaterialsSkybox;
LPD3DXMESH MeshSkybox;
LPDIRECT3DTEXTURE9 * TexturesSkybox;
D3DMATERIAL9 * MaterialsSkybox;
//<---

DWORD numMaterials;
LPD3DXMESH gp_dx_MeshShip;
LPD3DXMESH gp_dx_MeshTarget;
D3DMATERIAL9 MeshMatTarget;
D3DMATERIAL9 *meshMaterials;
LPDIRECT3DTEXTURE9 *meshTextures;

class Bullet
{
	public :
		D3DXVECTOR3 position;
		D3DXQUATERNION rotation;
		bool operator == (Bullet const& x) {
			return (position == x.position && rotation == x.rotation);
		}
};

/*windows api*/
HWND g_han_Window;
bool AppRunning = true;
/*game logic*/
int vb_count = 0;
int maxTargets = 50;
std::list<D3DXVECTOR3> targetList;
std::list<Bullet> bulletList;
double lastBulletTime = 0;

int floorPlan[20][15] =
{
  {1,0,0,0,0,0,0,1,1,1,1,1,1,1,1},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {0,0,0,1,1,0,0,0,1,1,0,0,1,0,1},
  {0,0,0,1,1,0,0,0,1,0,0,0,1,0,1},
  {0,0,0,0,1,1,0,1,1,0,0,0,0,0,1},
  {0,0,0,0,0,0,0,0,0,0,0,1,0,0,1},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,1,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,1,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,1,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,1,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,1,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,1,0,0,0,1,0,0,0,0,1},
  {1,0,1,0,0,0,0,0,0,1,0,0,0,0,1},
  {1,0,1,1,0,0,0,0,1,1,0,0,0,1,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,1,1},
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
};
/*
void UpdateKeybAndMouse()
{
	char chr_KeybState[256];	
	gp_dx_KeybDevice->Acquire();
	gp_dx_KeybDevice->GetDeviceState(sizeof(chr_KeybState),(LPVOID)&chr_KeybState);

	float leftRightRot = 0;
	float upDownRot = 0;
	float speed = 0.002f;
	D3DXQUATERNION additionalRot;

	//update bullet position
	for(std::list<Bullet>::iterator i=bulletList.begin(); i != bulletList.end(); ++i) {
		Bullet curBullet = (Bullet)*i;
		D3DXVECTOR3 t3(0, -1, 0);
		D3DXMATRIX fromQ;
		D3DXMatrixRotationQuaternion(&fromQ, &curBullet.rotation);
		D3DXVec3TransformCoord(&t3, &t3, &fromQ);
		t3 *= speed*4;
		D3DXVec3Add(&curBullet.position, &curBullet.position, &t3);
		*i = curBullet;
    }
	if(chr_KeybState[DIK_SPACE]/128) {
		SYSTEMTIME st;
		GetSystemTime(&st);
		if (st.wMilliseconds - lastBulletTime > 100)
		{
			Bullet tmp;
			tmp.position = xwingPosition;
			tmp.rotation = xwingRotation;
			bulletList.push_back(tmp);
		}
		lastBulletTime = st.wMilliseconds;
	}
	if (chr_KeybState[DIK_LEFT]/128)
		leftRightRot -= 0.003f;
	if (chr_KeybState[DIK_RIGHT]/128)
		leftRightRot += 0.003f;
	D3DXVECTOR3 t1(0, 0, -1);
	D3DXQUATERNION additionalRot1;
	D3DXQuaternionRotationAxis(&additionalRot1, &t1,  leftRightRot);

	if (chr_KeybState[DIK_DOWN]/128)
		upDownRot += 0.003f;
	if (chr_KeybState[DIK_UP]/128)
		upDownRot -= 0.003f;
	D3DXVECTOR3 t2(-1, 0, 0);
	D3DXQUATERNION additionalRot2;
	D3DXQuaternionRotationAxis(&additionalRot2, &t2, upDownRot);
	
	D3DXQuaternionMultiply(&additionalRot1, &additionalRot2, &additionalRot1);
	D3DXQuaternionMultiply(&xwingRotation, &xwingRotation, &additionalRot1);

	D3DXVECTOR3 t3(0, -1, 0);
	D3DXMATRIX fromQ;
	D3DXMatrixRotationQuaternion(&fromQ, &xwingRotation);
	D3DXVec3TransformCoord(&t3, &t3, &fromQ);
	t3 *= speed;
	D3DXVec3Add(&xwingPosition, &xwingPosition, &t3);
}

void UpdateCamera()
{
	D3DXQuaternionSlerp(&cameraRotation, &cameraRotation, &xwingRotation, 0.1f);

	//set campos behind spaceship
	D3DXVECTOR3 campos(0, 3.0f, -1.0f);
	D3DXMATRIX fromQ;
	D3DXMatrixRotationQuaternion(&fromQ, &cameraRotation);
	D3DXVec3TransformCoord(&campos, &campos, &fromQ);
	campos +=xwingPosition;

	//set up cam behind spacheship in world coordinates
	D3DXVECTOR3 camup(0, 0, -1);
	D3DXVec3TransformCoord(&camup, &camup, &fromQ);

	//create new matrices proj and view
	D3DXMATRIXA16 m_View;
	D3DXMatrixLookAtLH(&m_View, &campos, &xwingPosition, &camup);
	gp_dx_OutputDevice->SetTransform(D3DTS_VIEW, &m_View);

	D3DXMATRIX matProj;
	D3DXMatrixPerspectiveFovLH( &matProj, D3DXToRadian( 45.0f ), 640.0f / 480.0f, 0.1f, 100.0f );
	gp_dx_OutputDevice->SetTransform(D3DTS_PROJECTION, &matProj );
}
*/
void InitializeLight()
{
	D3DLIGHT9 light;
	ZeroMemory( &light, sizeof(D3DLIGHT9) );
	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Diffuse.r  = 1.0f;
	light.Diffuse.g  = 1.0f;
	light.Diffuse.b  = 1.0f;
	light.Diffuse.a  = 1.0f;
	light.Range      = 100.0f;
	light.Ambient.r = 0.8f;
	light.Ambient.g = 0.8f;
	light.Ambient.b = 0.8f;
	D3DXVECTOR3 dx_vecDir(0, 2, 1);
	D3DXVec3Normalize( (D3DXVECTOR3*)&light.Direction, &dx_vecDir );
	gp_dx_OutputDevice->SetLight( 0, &light );
	gp_dx_OutputDevice->LightEnable( 0, TRUE );
	gp_dx_OutputDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

void InitializeSkybox()
{
	LPD3DXBUFFER MaterialBuffer;
	D3DXMATERIAL * Materials;
	LPD3DXBUFFER MeshBuffer;
	D3DXLoadMeshFromX(L"skybox.x", D3DXMESH_DYNAMIC, gp_dx_OutputDevice, &MeshBuffer, &MaterialBuffer, 0, &NumMaterialsSkybox, &MeshSkybox);
	Materials = (D3DXMATERIAL*)MaterialBuffer->GetBufferPointer();
	MaterialsSkybox = new D3DMATERIAL9[NumMaterialsSkybox];
	TexturesSkybox  = new LPDIRECT3DTEXTURE9[NumMaterialsSkybox];

	for (DWORD i=0; i<NumMaterialsSkybox; i++)
	{
		MaterialsSkybox[i] = Materials[i].MatD3D;
		MaterialsSkybox[i].Ambient = MaterialsSkybox[i].Diffuse;
		TexturesSkybox[i] = 0;
		if (Materials[i].pTextureFilename)
		{
			char * pString = new char[255];
			memcpy(pString,Materials[i].pTextureFilename,sizeof(char)*255);
			wchar_t pWideString[255];
			MultiByteToWideChar(CP_ACP,0,pString,-1,pWideString,255);
			D3DXCreateTextureFromFile(gp_dx_OutputDevice, pWideString, &TexturesSkybox[i]);
		}
	}
}

void InitializeShip()
{
	LPD3DXBUFFER lp_dx_materialBuffer;
	D3DXMATERIAL* d3dxMaterials;
	ID3DXBuffer* lp_dx_MeshBuffer  = 0;
	D3DXLoadMeshFromX(L"bigship1.x", D3DXMESH_DYNAMIC, gp_dx_OutputDevice, &lp_dx_MeshBuffer, &lp_dx_materialBuffer, 0, &numMaterials, &gp_dx_MeshShip);
	//gp_dx_MeshShip->OptimizeInplace( D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_COMPACT | D3DXMESHOPT_VERTEXCACHE, (DWORD*)lp_dx_MeshBuffer->GetBufferPointer(), 0, 0, 0);
	//D3DXComputeNormals(gp_dx_MeshShip, 0);
	d3dxMaterials = (D3DXMATERIAL*)lp_dx_materialBuffer->GetBufferPointer();
	meshMaterials = new D3DMATERIAL9[numMaterials];
	meshTextures  = new LPDIRECT3DTEXTURE9[numMaterials];

	for (DWORD i=0; i<numMaterials; i++)
	{
		// Copy the material
		meshMaterials[i] = d3dxMaterials[i].MatD3D;
		// Set the ambient color for the material (D3DX does not do this)
		meshMaterials[i].Ambient = meshMaterials[i].Diffuse;
		// Create the texture if it exists - it may not
		meshTextures[i] = 0;
		if (d3dxMaterials[i].pTextureFilename)
		{
			char * pString = new char[255];
			memcpy(pString,d3dxMaterials[i].pTextureFilename,sizeof(char)*255);
			wchar_t pWideString[255];
			MultiByteToWideChar(CP_ACP,0,pString,-1,pWideString,255);
			D3DXCreateTextureFromFile(gp_dx_OutputDevice, pWideString, &meshTextures[i]);
		}
	}
}

void InitializeMap()
{
	for (int x = 0; x < 20; x++)
		for (int y = 0; y < 15; y++)
			if (floorPlan[x][y] == 1)
				floorPlan[x][y] = rand()%6 + 1;
}
/*
void ReadMouse()
{
	DIMOUSESTATE mousestate;
	gp_dx_MouseDevice->Acquire();
	gp_dx_MouseDevice->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&mousestate);
}
*/
void Render()
{
    gp_dx_OutputDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
                         D3DCOLOR_COLORVALUE(.0f,.5f,.5f,1.0f), 1.0f, 0 );

	/*
	for (int x = 0; x < 20; x++)
		for (int y = 0; y < 15; y++)
			if (floorPlan[x][y] == 1)
				floorPlan[x][y] = rand()%6 + 1;
	*/
	gp_dx_OutputDevice->BeginScene();

    D3DXMATRIX matWorld;
    D3DXMatrixTranslation( &matWorld, 0.0f, 0.0f, 0.0f );
    gp_dx_OutputDevice->SetTransform( D3DTS_WORLD, &matWorld );

		for (int x = 0; x < 20; x++)
		for (int y = 0; y < 15; y++)
		{
			if (floorPlan[x][y] == 1)
				oneBuilding->Draw(gp_dx_OutputDevice, D3DXVECTOR3((float)x, float(0), float(y)), 1);
			else
				oneBuilding->Draw(gp_dx_OutputDevice, D3DXVECTOR3((float)x, float(0), float(y)), 0);
		}

	/*draw skybox*/
	/*
	D3DXMatrixIdentity(&matWorld);
	gp_dx_OutputDevice->SetTransform(D3DTS_WORLD, &matWorld);

	for (DWORD i=0; i<NumMaterialsSkybox; i++)
	{
		gp_dx_OutputDevice->SetMaterial(&MaterialsSkybox[i]);
		gp_dx_OutputDevice->SetTexture(0,TexturesSkybox[i]);
		MeshSkybox->DrawSubset(i);
	}
	*/
	/*draw buildings*/
	/*
	D3DMATERIAL9 material;
	ZeroMemory(&material, sizeof(D3DMATERIAL9));    
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);    
	material.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);   
	material.Specular= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	gp_dx_OutputDevice->SetMaterial(&material);
    gp_dx_OutputDevice->SetTexture( 0, gp_dx_Texture );
    gp_dx_OutputDevice->SetStreamSource( 0, gp_dx_VertexBuffer1, 0, sizeof(Vertex) );
	gp_dx_OutputDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, vb_count/3 );
	*/

	/*draw bullets*/
	/*
	gp_dx_OutputDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE); 
	gp_dx_OutputDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE); 
	gp_dx_OutputDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	gp_dx_OutputDevice->SetRenderState(D3DRS_SRCBLEND , D3DBLEND_ONE);
	gp_dx_OutputDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	gp_dx_OutputDevice->SetTexture( 0, gpTextureBullet );
	gp_dx_OutputDevice->SetFVF( D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_NORMAL);
	gp_dx_OutputDevice->SetStreamSource( 0, gp_dx_Bullet , 0, sizeof(Vertex)*4 );

	for(std::list<Bullet>::iterator i=bulletList.begin(); i != bulletList.end(); ++i) {
		D3DXMatrixIdentity(&matWorld);
		D3DXMATRIX fromQ;
		D3DXMatrixRotationQuaternion(&fromQ, &xwingRotation);
		D3DXMatrixMultiply(&matWorld, &matWorld, &fromQ);
		D3DXMatrixScaling(&fromQ, 1.5f, 1.5f, 1.5f);
		D3DXMatrixMultiply(&matWorld, &matWorld, &fromQ);
		D3DXMatrixTranslation(&fromQ, ((Bullet)*i).position.x, ((Bullet)*i).position.y, ((Bullet)*i).position.z);
		D3DXMatrixMultiply(&matWorld, &matWorld, &fromQ);
		gp_dx_OutputDevice->SetTransform(D3DTS_WORLD, &matWorld);
		gp_dx_OutputDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP , 0, 2);
	}
	gp_dx_OutputDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE); 
	*/

	/*draw spaceship*/	
	D3DXMatrixIdentity(&matWorld);
	D3DXMATRIX m_Scale, m_Rotation;
	D3DXMatrixScaling(&m_Scale, 0.04f, 0.04f, 0.04f);
	D3DXMatrixMultiply(&matWorld, &matWorld, &m_Scale);
	//rotate model before draw
	D3DXMatrixRotationX(&m_Rotation, -1.5f);
	D3DXMatrixMultiply(&matWorld, &matWorld, &m_Rotation);

	D3DXMATRIX fromQ;
	D3DXMatrixRotationQuaternion(&fromQ, &xwingRotation);
	D3DXMatrixMultiply(&matWorld, &matWorld, &fromQ);
	D3DXMatrixTranslation(&fromQ, xwingPosition.x, xwingPosition.y, xwingPosition.z);
	D3DXMatrixMultiply(&matWorld, &matWorld, &fromQ);

	gp_dx_OutputDevice->SetTransform(D3DTS_WORLD, &matWorld);

	for (DWORD i=0; i<numMaterials; i++)
	{
		gp_dx_OutputDevice->SetMaterial(&meshMaterials[i]);
		gp_dx_OutputDevice->SetTexture(0, meshTextures[i]);
		gp_dx_MeshShip->DrawSubset(i);
	}

	/*draw targets*/
	/*
	std::list<D3DXVECTOR3>::iterator i;
	for(i=targetList.begin(); i != targetList.end(); ++i) {
		D3DXMatrixIdentity(&matWorld);
		D3DXMATRIX m_Scale;
		D3DXMatrixScaling(&m_Scale, 0.1f, 0.1f, 0.1f);
		D3DXMatrixMultiply(&matWorld, &matWorld, &m_Scale);
		D3DXMATRIX m_Translate;
		D3DXMatrixTranslation(&m_Translate, ((D3DXVECTOR3)*i).x, ((D3DXVECTOR3)*i).y, ((D3DXVECTOR3)*i).z);
		D3DXMatrixMultiply(&matWorld, &matWorld, &m_Translate);
		gp_dx_OutputDevice->SetTransform(D3DTS_WORLD, &matWorld);
		gp_dx_OutputDevice->SetMaterial(&MeshMatTarget);
		gp_dx_MeshTarget->DrawSubset(0);
	}
	*/

	gp_dx_OutputDevice->EndScene();
    gp_dx_OutputDevice->Present( NULL, NULL, NULL, NULL );
}

void InitializeGameObjects()
{
	oneBuilding = new Building();
	oneBuilding->Init(gp_dx_OutputDevice);

	oneDebugObject = new DebugClass();
	oneDebugObject->Init(gp_dx_OutputDevice);
}

void LoadTexture()
{
	D3DXCreateTextureFromFile( gp_dx_OutputDevice, L"bullet.jpg", &gpTextureBullet );
	gp_dx_OutputDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	gp_dx_OutputDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
}
/*
void InitializeInputDevices()
{
   DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&gp_dx_InputObject, NULL);

   gp_dx_InputObject->CreateDevice(GUID_SysKeyboard, &gp_dx_KeybDevice, NULL);
   gp_dx_KeybDevice->SetDataFormat(&c_dfDIKeyboard);
   gp_dx_KeybDevice->SetCooperativeLevel(g_han_Window, DISCL_FOREGROUND|DISCL_NONEXCLUSIVE);

   gp_dx_InputObject->CreateDevice(GUID_SysMouse, &gp_dx_MouseDevice, NULL);
   gp_dx_MouseDevice->SetDataFormat(&c_dfDIMouse);
   gp_dx_MouseDevice->SetCooperativeLevel(g_han_Window, DISCL_EXCLUSIVE | DISCL_FOREGROUND);
}
*/
INT InitializeOutputDevice()
{
	HRESULT hr;
	
	LPDIRECT3D9 lp_dx_Object;
    lp_dx_Object = Direct3DCreate9(D3D_SDK_VERSION);
    if (lp_dx_Object == NULL)
    {
        MessageBox(g_han_Window,L"DirectX Runtime library not installed!",L"InitializeDevice()",MB_OK);
		return GetLastError();
    }
	/*
    D3DDISPLAYMODE d3ddm;
	lp_dx_Object->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &d3ddm );
	
    D3DPRESENT_PARAMETERS dx_PresParams;
    ZeroMemory( &dx_PresParams, sizeof(dx_PresParams) );
	dx_PresParams.hDeviceWindow = g_han_Window;
    dx_PresParams.Windowed = TRUE;
    dx_PresParams.SwapEffect = D3DSWAPEFFECT_DISCARD;
	dx_PresParams.BackBufferCount       = 1;
    dx_PresParams.BackBufferFormat       = d3ddm.Format;
    dx_PresParams.EnableAutoDepthStencil = TRUE;
    dx_PresParams.AutoDepthStencilFormat = D3DFMT_D16;
    dx_PresParams.PresentationInterval   = D3DPRESENT_INTERVAL_IMMEDIATE;
	
	hr = lp_dx_Object->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, g_han_Window, D3DCREATE_HARDWARE_VERTEXPROCESSING, &dx_PresParams, &gp_dx_OutputDevice);
    if (FAILED(hr))
    {
		MessageBox(g_han_Window,L"Failed to create device!",L"InitializeDevice()",MB_OK);
		return GetLastError();
    }
	*/
	return 0;
}

void ReleaseObjects()
{
	/*
	gp_dx_KeybDevice->Unacquire();
	gp_dx_MouseDevice->Unacquire();
	gp_dx_InputObject->Release();
	*/
    gp_dx_OutputDevice->Release();
	oneBuilding->~Building();
	oneDebugObject->~DebugClass();
}

LRESULT WINAPI OurWindowProcedure(HWND han_Wind,UINT uint_Message,WPARAM wParam,LPARAM lParam)
{
    switch(uint_Message)
    {
        case WM_KEYDOWN:
        {
			switch (wParam)
			{
			case VK_ESCAPE:
				AppRunning = false;
				break;
			}
			break;
        }
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
    }
    return CallWindowProc((WNDPROC)DefWindowProc,han_Wind,uint_Message,wParam,lParam);
}

INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmd, int nCmdShow)
{
	MSG msg_Message;
	
	LPCWSTR wndClassName = L"WindowClassName";
	LPCWSTR wndTitle = L"FlyStorm 1.0";
	DWORD dwWindowStile = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE;
	DWORD dwWindowHeight = 700;
	DWORD dwWindowWidth = 700;

	WNDCLASS wnd_Structure;
    wnd_Structure.style = CS_HREDRAW | CS_VREDRAW;
    wnd_Structure.lpfnWndProc = (WNDPROC)OurWindowProcedure;
    wnd_Structure.cbClsExtra = 0;
    wnd_Structure.cbWndExtra = 0;
    wnd_Structure.hInstance = hInst;
    wnd_Structure.hIcon = NULL;
    wnd_Structure.hCursor = LoadCursor(NULL, IDC_ARROW);
    wnd_Structure.hbrBackground = (HBRUSH)(COLOR_WINDOW);
    wnd_Structure.lpszMenuName = NULL;
    wnd_Structure.lpszClassName = wndClassName;

    if (!RegisterClass(&wnd_Structure))
	{
		MessageBox(NULL, L"Window class registration failed!", L"Error!", MB_OK | MB_ICONEXCLAMATION);
		return GetLastError();
	}

	g_han_Window = CreateWindow(wndClassName, 
								wndTitle,  
								dwWindowStile, 
								100, 100, 
								dwWindowHeight, dwWindowWidth, 
								NULL, NULL, 
								hInst, 
								NULL);

	ShowWindow(g_han_Window, nCmdShow);
	UpdateWindow(g_han_Window);

    InitializeOutputDevice();
	//InitializeInputDevices();
	//D3DXQuaternionIdentity(&xwingRotation);
	//D3DXQuaternionIdentity(&cameraRotation);
	//LoadTexture();
	//InitializeSkybox();
	//InitializeShip();
	//InitializeLight();

	//InitializeGameObjects();
	/*
	MSG mssg;
	PeekMessage( &mssg, NULL, 0, 0, PM_NOREMOVE);
	while (mssg.message!=WM_QUIT) 
	{
		if (PeekMessage(&mssg, NULL, 0, 0, PM_REMOVE)) 
		{
			TranslateMessage(&mssg);
			DispatchMessage(&mssg);
		} 
		else 
		{
			Sleep(1);
		}
	}
	*/
	/*
    while(AppRunning)
    {
        if(PeekMessage(&msg_Message,g_han_Window,0,0,PM_REMOVE))
        {
			TranslateMessage(&msg_Message);
            DispatchMessage(&msg_Message);
        }
		else
		{
			//UpdateCamera();
			//UpdateKeybAndMouse();
			//CheckCollision();
			//Render();
			Sleep(1);
		}
	}
	*/
	//ReleaseObjects();
    return 0;
}