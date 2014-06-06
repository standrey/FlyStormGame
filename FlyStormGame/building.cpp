#include "building.h"

Building::Building() {};

Building::Building(IDirect3DDevice9 * pdxDevice)
{
	//init texture
	HRESULT hRes = D3DXCreateTextureFromFile( pdxDevice, L"gameresources/texturemap.bmp", &buildingTexture );

	//create floor
	D3DXVECTOR3 normal(0.0f,1.0f,0.0f);
	gameVertex * vertexBufferFloor = new gameVertex[4];
	vertexBufferFloor[0] = gameVertex(D3DXVECTOR3(0,0,1), (float)0, (float)0, normal);
	vertexBufferFloor[1] = gameVertex(D3DXVECTOR3(1,0,1), (float)1/11, (float)0, normal);
	vertexBufferFloor[2] = gameVertex(D3DXVECTOR3(0,0,0), (float)0, (float)1, normal);
	vertexBufferFloor[3] = gameVertex(D3DXVECTOR3(1,0,0), (float)1/11, (float)1, normal);

	void * pLock = NULL;
	int sizeDxVertexBuffer = 4 * sizeof(gameVertex);
	pdxDevice->CreateVertexBuffer( sizeDxVertexBuffer, D3DUSAGE_WRITEONLY , CUSTOMFVF, D3DPOOL_DEFAULT, &pdxVertexFloor, NULL);
	pdxVertexFloor->Lock(0, sizeDxVertexBuffer, (void**)&pLock,0);
	memcpy(pLock, vertexBufferFloor, sizeDxVertexBuffer);
	pdxVertexFloor->Unlock();

	delete [] vertexBufferFloor;

	for (int i = 1; i < 11; i++)
		pdxVertexBuildings.push_back(CreateBuilding(pdxDevice, i));
}

//generate buildings as 
IDirect3DVertexBuffer9 * Building::CreateBuilding(IDirect3DDevice9 * d3ddev, DWORD hightBuilding)
{
	IDirect3DVertexBuffer9 * resVertexBuilding = NULL;
	DWORD h = hightBuilding;

	gameVertex * vertexBufferBuilding = new gameVertex[4 * 5];

	D3DXVECTOR3 normal;

	normal.x=0.0f; normal.y=0.0f; normal.z=1.0f;	
	vertexBufferBuilding[0] = gameVertex(D3DXVECTOR3(1,h,1), (float)h/11, (float)0, normal);
	vertexBufferBuilding[1] = gameVertex(D3DXVECTOR3(0,h,1), (float)(h+1)/11, (float)0, normal);
	vertexBufferBuilding[2] = gameVertex(D3DXVECTOR3(1,0,1), (float)h/11, (float)1, normal);
	vertexBufferBuilding[3] = gameVertex(D3DXVECTOR3(0,0,1), (float)(h+1)/11, (float)1, normal);

	normal.x=1.0f; normal.y=0.0f; normal.z=0.0f;
	vertexBufferBuilding[4] = gameVertex(D3DXVECTOR3(1,h,0), (float)h/11, (float)0, normal);
	vertexBufferBuilding[5] = gameVertex(D3DXVECTOR3(1,h,1), (float)(h+1)/11, (float)0, normal);
	vertexBufferBuilding[6] = gameVertex(D3DXVECTOR3(1,0,0), (float)h/11, (float)1, normal);
	vertexBufferBuilding[7] = gameVertex(D3DXVECTOR3(1,0,1), (float)(h+1)/11, (float)1, normal);

	normal.x=0.0f; normal.y=0.0f; normal.z=-1.0f;
	vertexBufferBuilding[8] = gameVertex(D3DXVECTOR3(0,h,0), (float)h/11, (float)0, normal);
	vertexBufferBuilding[9] = gameVertex(D3DXVECTOR3(1,h,0), (float)(h+1)/11, (float)0, normal);
	vertexBufferBuilding[10]= gameVertex(D3DXVECTOR3(0,0,0), (float)h/11, (float)1, normal);
	vertexBufferBuilding[11]= gameVertex(D3DXVECTOR3(1,0,0), (float)(h+1)/11, (float)1, normal);

	normal.x=-1.0f; normal.y=0.0f; normal.z=0.0f;
	vertexBufferBuilding[12]= gameVertex(D3DXVECTOR3(0,h,1), (float)h/11, (float)0, normal);
	vertexBufferBuilding[13]= gameVertex(D3DXVECTOR3(0,h,0), (float)(h+1)/11, (float)0, normal);
	vertexBufferBuilding[14]= gameVertex(D3DXVECTOR3(0,0,1), (float)h/11, (float)1, normal);
	vertexBufferBuilding[15]= gameVertex(D3DXVECTOR3(0,0,0), (float)(h+1)/11, (float)1, normal);

	//roof
	vertexBufferBuilding[16]= gameVertex(D3DXVECTOR3(0,h,1), (float)6/11, (float)0, normal);
	vertexBufferBuilding[17]= gameVertex(D3DXVECTOR3(1,h,1), (float)7/11, (float)0, normal);
	vertexBufferBuilding[18]= gameVertex(D3DXVECTOR3(0,h,0), (float)6/11, (float)1, normal);
	vertexBufferBuilding[19]= gameVertex(D3DXVECTOR3(1,h,0), (float)7/11, (float)1, normal);

	void * pLock = NULL;
	int sizeDxVertexBuffer = 4 * 5 * sizeof(gameVertex);
	d3ddev->CreateVertexBuffer( sizeDxVertexBuffer, D3DUSAGE_WRITEONLY, CUSTOMFVF, D3DPOOL_DEFAULT, &resVertexBuilding, NULL);
	resVertexBuilding->Lock(0, sizeDxVertexBuffer, (void**)&pLock,0);
	memcpy(pLock, vertexBufferBuilding, sizeDxVertexBuffer);
	resVertexBuilding->Unlock();
	delete [] vertexBufferBuilding;

	return resVertexBuilding;
}

void Building::Draw(IDirect3DDevice9 * pdxDevice, D3DXVECTOR3 worldPos, float height)
{
	D3DXMATRIX matWorld;
    D3DXMatrixTranslation( &matWorld, worldPos.x, worldPos.y  + height, worldPos.z);
    pdxDevice->SetTransform( D3DTS_WORLD, &matWorld );

	pdxDevice->SetFVF(CUSTOMFVF);
	pdxDevice->SetTexture( 0, buildingTexture );

	if (height > 0)
	{
		D3DXMatrixTranslation( &matWorld, worldPos.x, worldPos.y, worldPos.z);
		pdxDevice->SetTransform( D3DTS_WORLD, &matWorld );
		if (height <= 11 && height > 0)
		{
			pdxDevice->SetStreamSource( 0, pdxVertexBuildings[height - 1], 0, sizeof(gameVertex) );
			//sides of building
			for (int i = 0; i < 20; i+=4)
				pdxDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, i, 2 );
		}
	}

	//floor or roof 
	pdxDevice->SetStreamSource( 0, pdxVertexFloor, 0, sizeof(gameVertex) );
	pdxDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );
	
}

Building::~Building()
{
	if (buildingTexture)
	{
		buildingTexture->Release();
		buildingTexture = NULL;
	}

	if (pdxVertexFloor != NULL)
	{
		pdxVertexFloor->Release();
		pdxVertexFloor = NULL;
	}
	for (std::vector<IDirect3DVertexBuffer9 * >::iterator it = pdxVertexBuildings.begin();
		it != pdxVertexBuildings.end();
		it++)
	{
		(*it)->Release();
	}
	pdxVertexBuildings.clear();
}