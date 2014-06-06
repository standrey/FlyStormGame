#include "scene.h"

stSceneClass::stSceneClass() {};

stSceneClass::stSceneClass(IDirect3DDevice9 * d3ddev)
{
	device = d3ddev;
	DebugObject = new DebugClass(d3ddev);
	BuildingObject = new Building(d3ddev);
	MapObject = new stMapClass();
	Camera = new CameraClass();
	Ship = new SpaceshipClass(d3ddev);

	//set render states
	d3ddev->SetRenderState(D3DRS_LIGHTING, TRUE);
	d3ddev->SetRenderState(D3DRS_ZENABLE, TRUE);
	d3ddev->SetRenderState(D3DRS_COLORVERTEX, FALSE);
	d3ddev->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_COLORVALUE( 0.1f, 0.1f, 0.1f, 1.0f ));
	d3ddev->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_MATERIAL);

	//init simple lighting
	ZeroMemory( &light, sizeof(D3DLIGHT9) );
	light.Type       = D3DLIGHT_DIRECTIONAL;
	
	light.Diffuse.r  = 1.0f;
	light.Diffuse.g  = 1.0f;
	light.Diffuse.b  = 1.0f;

	D3DXVECTOR3 tmpDir = D3DXVECTOR3(-1.0f, -1.0f, -1.0f);
	D3DXVec3Normalize(&tmpDir, &tmpDir);
	light.Direction = tmpDir;
	light.Range      = 100.0f;
	
	d3ddev->SetLight( 0, &light );
	d3ddev->LightEnable( 0, TRUE );

}

HRESULT stSceneClass::PrepareObjects(IDirect3DDevice9 * d3ddev)
{
	d3ddev->SetRenderState(D3DRS_LIGHTING, FALSE);

	d3ddev->SetTransform(D3DTS_VIEW, &(Camera->GetViewMatix()));

	IDirect3DSurface9* pRenderTarget=NULL;
	d3ddev->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pRenderTarget);
	D3DSURFACE_DESC pDesc;
	pRenderTarget->GetDesc(&pDesc);
	pRenderTarget->Release();

	D3DXMATRIX matProj;
	D3DXMatrixPerspectiveFovLH( &matProj, D3DXToRadian( 45.0f ), pDesc.Width / pDesc.Height, 0.1f, 100.0f );
	d3ddev->SetTransform(D3DTS_PROJECTION, &matProj );
	return S_OK;
}

HRESULT stSceneClass::DrawObjects()
{
	PrepareObjects(device);

	device->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_COLORVALUE(.5f,.5f,.5f,1.0f), 1.0f, 0 );
	device->BeginScene();

    D3DXMATRIX matWorld;
    D3DXMatrixTranslation( &matWorld, 0.0f, 0.0f, 0.0f );
    device->SetTransform( D3DTS_WORLD, &matWorld );

	DebugObject->Draw(device);
	
	for (int x = 0; x < MapObject->GetSizeY(); x++)
		for (int y = 0; y < MapObject->GetSizeX(); y++)
			BuildingObject->Draw(device, D3DXVECTOR3((float)x, float(0), float(y)), MapObject->GetHeight(x,y));
	
	Ship->Draw(device);

	for(std::deque<Bullet *>::iterator iter = bulletList.begin(); iter != bulletList.end(); iter++) (*iter)->Draw(device);

	device->EndScene();
    device->Present( NULL, NULL, NULL, NULL );
	return S_OK;
}

void stSceneClass::KeyboardEvent(CInput * keybState)
{
	if (keybState == NULL)
		return;

	// firestarter
	if (keybState->KeyPressed(CInput::SPACE))
	{
		bulletList.push_back(new Bullet(device, Camera->GetLookAt(), Ship->GetMoveVector(), Ship->GetWorldPos()));
		OutputDebugString(L"Bullet added\n");
	}

	if (keybState->KeyPressed(CInput::UP))
	{
		Ship->addPitch(-0.01f);
		Camera->setRotation(Ship->localRotation);
	}
	if (keybState->KeyPressed(CInput::DOWN))
	{
		Ship->addPitch(0.01f);
		Camera->setRotation(Ship->localRotation);
	}
	if (keybState->KeyPressed(CInput::LEFT))
	{
		Ship->addRoll(-0.01f);
		Camera->setRotation(Ship->localRotation);
	}
	if (keybState->KeyPressed(CInput::RIGHT))
	{
		Ship->addRoll(0.01f);
		Camera->setRotation(Ship->localRotation);
	}
}

void stSceneClass::UpdateScene()
{
	float speed = 0.05f;
	
	//двигаем сферу в заданном направлении
	for(std::deque<Bullet *>::iterator iter = bulletList.begin();
		iter != bulletList.end();
		iter++)
		(*iter)->moveForward(speed * 2);

	//удаляем сферу из очереди
	if (!bulletList.empty())
	{
		Bullet * firstBullet =*(bulletList.begin());
		float distance = firstBullet->getDistanceFromOriginal();
		if (distance > 50)
			bulletList.pop_front();
	}
	//двигаем корабль с заданной скоростью
	Ship->moveForward(speed);

	//переставляем позицию корабля
	Camera->setPosition(Ship->GetWorldPos());
	Camera->UpdateCamera();
}

stSceneClass::~stSceneClass()
{
	DebugObject->~DebugClass();
	BuildingObject->~Building();
	MapObject->~stMapClass();
	Camera->~CameraClass();
	Ship->~SpaceshipClass();
}