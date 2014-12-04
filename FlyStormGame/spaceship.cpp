#include "spaceship.h"

SpaceshipClass::SpaceshipClass(IDirect3DDevice9 * d3ddev) {
	numMaterials = 0;
	worldPosition = D3DXVECTOR3(0,0,0);
	moveVector = D3DXVECTOR3(0,0,-1);
	D3DXQuaternionIdentity(&localRotation);

	D3DXMATERIAL * d3dxMaterials;

	ID3DXBuffer * materialBuffer;
	ID3DXMesh	* tmpMesh;
	HRESULT hr = D3DXLoadMeshFromX(	
						L"gameresources/ship.x", 
						D3DXMESH_DYNAMIC, 
						d3ddev, 
						0, 
						&materialBuffer, 
						0, 
						&numMaterials, 
						&tmpMesh);
	if (FAILED(hr)) return;
	d3dxMaterials = (D3DXMATERIAL*)materialBuffer->GetBufferPointer();
	meshMaterials = new D3DMATERIAL9[numMaterials];

	for (DWORD i=0; i<numMaterials; i++)
	{
		// Copy a material
		meshMaterials[i] = d3dxMaterials[i].MatD3D;
		// Set the ambient color for the material (D3DX does not do this)
		meshMaterials[i].Ambient = meshMaterials[i].Diffuse;
		// Create the texture if it exists - it may not
	}
	materialBuffer->Release();
	ZeroMemory(&baseBodyMaterial, sizeof(D3DMATERIAL9));
	baseBodyMaterial.Diffuse = baseBodyMaterial.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	baseBodyMaterial.Power = 10.0f;
	tmpMesh->CloneMeshFVF(D3DXMESH_MANAGED, D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE, d3ddev, &m_GeneralMesh);
	tmpMesh->Release();
	hr = D3DXComputeNormals(m_GeneralMesh, 0);
	InitObbFromMesh();
}

SpaceshipClass::~SpaceshipClass() {
	delete [] meshMaterials;
	meshMaterials = NULL;

	if (m_GeneralMesh)
	{
		m_GeneralMesh->Release();
		m_GeneralMesh = NULL;
	}
}

D3DXVECTOR3 SpaceshipClass::GetMoveVector() const  {
	D3DXVECTOR3 vecTmp;
	D3DXMATRIX m_Rot;
	D3DXMatrixRotationQuaternion(&m_Rot, &localRotation);
	D3DXVec3TransformCoord(&vecTmp, &moveVector, &m_Rot);
	return vecTmp;
}

D3DXVECTOR3 SpaceshipClass::GetWorldPos() const {
	return worldPosition;
}

void SpaceshipClass::addPitch(float angle) {
	D3DXMATRIX m_Rot;
	D3DXVECTOR3 pitchVector(1,0,0);
	D3DXMatrixRotationQuaternion(&m_Rot, &localRotation);
	D3DXVec3TransformCoord(&pitchVector, &pitchVector, &m_Rot);
	addRotation(angle, pitchVector);
}

void SpaceshipClass::addRoll(float angle) {
	D3DXMATRIX m_Rot;
	D3DXVECTOR3 rollVector(0,0,1);
	D3DXMatrixRotationQuaternion(&m_Rot, &localRotation);
	D3DXVec3TransformCoord(&rollVector, &rollVector, &m_Rot);
	addRotation(angle, rollVector);
}

void SpaceshipClass::addRotation(float Angle, D3DXVECTOR3 vec) {
	float halfangle = Angle/2.0f;
	float sine_halfangle = sin(halfangle);
	D3DXQUATERNION tmp;
	tmp.x = sine_halfangle * vec.x;
	tmp.y = sine_halfangle * vec.y;
	tmp.z = sine_halfangle * vec.z;
	tmp.w = cos(halfangle);
	localRotation *= tmp;
}

void SpaceshipClass::moveForward(float speed) {
	D3DXVECTOR3 vecTmp;
	D3DXMATRIX m_Rot;
	D3DXMatrixRotationQuaternion(&m_Rot, &localRotation);
	D3DXVec3TransformCoord(&vecTmp, &moveVector, &m_Rot);
	worldPosition += vecTmp * speed;
}

HRESULT SpaceshipClass::Draw(IDirect3DDevice9 * d3ddev) {
	HRESULT hr = S_OK;
	DWORD AmbientColorState;
	d3ddev->GetRenderState(D3DRS_AMBIENT, &AmbientColorState);
	d3ddev->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_COLORVALUE( 0.5f, 0.5f, 0.5f, 1 ));

	D3DXMATRIX matWorld, m_Scale, m_Rotation, m_Translation;
	//уменьшаем модель
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixScaling(&m_Scale, 0.005f, 0.005f, 0.005f);
	D3DXMatrixMultiply(&matWorld, &matWorld, &m_Scale);
	//поворачиваем
	D3DXMatrixRotationQuaternion(&m_Rotation, &localRotation);
	D3DXMatrixMultiply(&matWorld, &matWorld, &m_Rotation);
	//перемещаем
	D3DXMatrixTranslation(&m_Translation, worldPosition.x, worldPosition.y, worldPosition.z);
	D3DXMatrixMultiply(&matWorld, &matWorld, &m_Translation);
	//устанавливаем матрицу
	d3ddev->SetTransform(D3DTS_WORLD, &matWorld);
	TranslateObbFromMesh(&matWorld);

	//проходим по материалам и расуем
	for (DWORD i=0; i<numMaterials; i++)
	{
		d3ddev->SetMaterial(&meshMaterials[i]);
		m_GeneralMesh->DrawSubset(i);
	}
	//возвращаем значение фонового цвета
	d3ddev->SetRenderState(D3DRS_AMBIENT, AmbientColorState);

	return S_OK;
}

/*
 * Отрисовка примитивов для отладки игры
 * IDirect3DDevice9 * d3ddev - указатель на девайс
*/
void SpaceshipClass::DrawDebug(IDirect3DDevice9 * d3ddev) {
	
	HRESULT hr;
	//создаём буфер для отправки в конвеер
	IDirect3DVertexBuffer9 * debugLineBuffer;
	UINT vbSize = 8 * D3DXGetFVFVertexSize(D3DFVF_XYZ);
	hr  = d3ddev->CreateVertexBuffer(vbSize, D3DUSAGE_WRITEONLY, D3DFVF_XYZ, D3DPOOL_DEFAULT, &debugLineBuffer, NULL);
	if (FAILED(hr))
		return;
	
	BYTE * pLock = nullptr;
	
	hr = debugLineBuffer->Lock(0, vbSize, (LPVOID *)&pLock, 0);
	
	if (FAILED(hr)) {
		debugLineBuffer->Release();
		return;
	}
	memcpy(pLock, obb, sizeof(D3DXVECTOR3) * 8);
	debugLineBuffer->Unlock();

	D3DXMATRIX matWorld;
	D3DXMatrixIdentity(&matWorld);
	d3ddev->SetTransform(D3DTS_WORLD, &matWorld);
	D3DMATERIAL9 debugMaterial;
	ZeroMemory(&debugMaterial, sizeof(D3DMATERIAL9));
	
	debugMaterial.Ambient.r = 1.0f;
	debugMaterial.Ambient.g = 1.0f;
	debugMaterial.Ambient.b = 1.0f;

	debugMaterial.Diffuse.r = 1.0f;
	debugMaterial.Diffuse.g = 1.0f;
	debugMaterial.Diffuse.b = 1.0f;
	//d3ddev->SetMaterial(&debugMaterial);

	d3ddev->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_COLORVALUE( 1.0f, 1.0f, 1.0f, 1 ));

	d3ddev->SetStreamSource( 0, debugLineBuffer, 0, sizeof(D3DXVECTOR3) );
	//рисуем две замкнутые линии
	d3ddev->DrawPrimitive(D3DPT_LINELIST, 0, 4);
	debugLineBuffer->Release();
}