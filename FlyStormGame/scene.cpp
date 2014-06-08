#include "scene.h"

stSceneClass::stSceneClass() {};

stSceneClass::stSceneClass(IDirect3DDevice9 * d3ddev)
{
	lastFireTime = GetTickCount();

	device = d3ddev;
	DebugObject = new DebugClass(d3ddev);
	BuildingObject = new Building(d3ddev);
	MapObject = new stMapClass();
	Camera = new CameraClass();
	Ship = new SpaceshipClass(d3ddev);
	Bullets = new BulletFabric(d3ddev);

	//set render states
	d3ddev->SetRenderState(D3DRS_ZENABLE, TRUE);
	d3ddev->SetRenderState(D3DRS_LIGHTING, TRUE);
	d3ddev->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_COLORVALUE( 1, 1, 1, 1 ));
	
	//init simple lighting
	/*
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
	*/
}

HRESULT stSceneClass::PrepareObjects(IDirect3DDevice9 * d3ddev)
{
	d3ddev->SetTransform(D3DTS_VIEW, &(Camera->GetViewMatix()));
	
	// определяем разрешение backbuffer
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

//
// Отображение объектов
//
HRESULT stSceneClass::DrawObjects()
{
	PrepareObjects(device);

	device->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_COLORVALUE(0.35f, 0.53f, 0.2, 1.0f), 1.0f, 0 );
	device->BeginScene();

    D3DXMATRIX matWorld;
    D3DXMatrixTranslation( &matWorld, 0.0f, 0.0f, 0.0f );
    device->SetTransform( D3DTS_WORLD, &matWorld );

	for (int x = 0; x < MapObject->GetSizeY(); x++)
		for (int y = 0; y < MapObject->GetSizeX(); y++)
			BuildingObject->Draw(device, D3DXVECTOR3((float)x, float(0), float(y)), MapObject->GetHeight(x,y));
	
	Ship->Draw(device);
	Bullets->Draw();

	device->EndScene();
    device->Present( NULL, NULL, NULL, NULL );
	return S_OK;
}

//
// Обрабатываем нажатие клавиш
//
void stSceneClass::KeyboardEvent(CInput * keybState)
{
	if (keybState == NULL)
		return;

	// add bullet to scene
	if (keybState->KeyPressed(CInput::SPACE) && (GetTickCount() - lastFireTime > 500) )
	{
		//make bullet forwarded regarded ship
		Bullets->AddBullet(Ship->localRotation, Ship->GetWorldPos() + Ship->GetMoveVector()*0.6f );
		lastFireTime= GetTickCount();
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

	//rotate bullet according point of view
	Bullets->setRotationBullets(Ship->localRotation);

}

void stSceneClass::UpdateScene()
{
	float speed = 0.02f;
	
	//двигаем пулю в заданном направлении
	Bullets->moveForward(speed * 2.0f);

	//удаляем сферу/пулю из очереди
	static DWORD lastTimeToDel = GetTickCount();
	if (GetTickCount() - lastTimeToDel > 10000) {
		Bullets->RemoveFirstBullet();
		lastTimeToDel = GetTickCount();
	}

	//двигаем корабль с заданной скоростью
	Ship->moveForward(speed);

	//переставляем позицию камеры
	//относительно мировой системы координат
	Camera->setPosition(Ship->GetWorldPos());
}

stSceneClass::~stSceneClass()
{
	delete DebugObject;
	delete BuildingObject;
	delete MapObject;
	delete Camera;
	delete Ship;
	delete Bullets;
}