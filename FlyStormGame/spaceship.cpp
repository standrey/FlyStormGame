#include "spaceship.h"

SpaceshipClass::SpaceshipClass() {}

SpaceshipClass::SpaceshipClass(IDirect3DDevice9 * d3ddev)
{
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
	tmpMesh->CloneMeshFVF(D3DXMESH_MANAGED, D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE, d3ddev, &MeshShip);
	tmpMesh->Release();
	D3DXComputeNormals(MeshShip, 0);
}

SpaceshipClass::~SpaceshipClass()
{
	delete [] meshMaterials;
	meshMaterials = NULL;

	if (MeshShip)
	{
		MeshShip->Release();
		MeshShip = NULL;
	}
}

D3DXVECTOR3 SpaceshipClass::GetMoveVector()
{
	D3DXVECTOR3 vecTmp;
	D3DXMATRIX m_Rot;
	D3DXMatrixRotationQuaternion(&m_Rot, &localRotation);
	D3DXVec3TransformCoord(&vecTmp, &moveVector, &m_Rot);
	return vecTmp;
}

D3DXVECTOR3 SpaceshipClass::GetWorldPos()
{
	return worldPosition;
}

void SpaceshipClass::addPitch(float angle)
{
	D3DXMATRIX m_Rot;
	D3DXVECTOR3 pitchVector(1,0,0);
	D3DXMatrixRotationQuaternion(&m_Rot, &localRotation);
	D3DXVec3TransformCoord(&pitchVector, &pitchVector, &m_Rot);

	addRotation(angle, pitchVector);
}

void SpaceshipClass::addRoll(float angle)
{
	D3DXMATRIX m_Rot;
	D3DXVECTOR3 rollVector(0,0,1);
	D3DXMatrixRotationQuaternion(&m_Rot, &localRotation);
	D3DXVec3TransformCoord(&rollVector, &rollVector, &m_Rot);

	addRotation(angle, rollVector);
}

void SpaceshipClass::addRotation(float Angle, D3DXVECTOR3 vec)
{
	float halfangle = Angle/2.0f;
	float sine_halfangle = sin(halfangle);
	D3DXQUATERNION tmp;
	tmp.x = sine_halfangle * vec.x;
	tmp.y = sine_halfangle * vec.y;
	tmp.z = sine_halfangle * vec.z;
	tmp.w = cos(halfangle);
	localRotation *= tmp;
}

void SpaceshipClass::moveForward(float speed)
{
	D3DXVECTOR3 vecTmp;
	D3DXMATRIX m_Rot;
	D3DXMatrixRotationQuaternion(&m_Rot, &localRotation);
	D3DXVec3TransformCoord(&vecTmp, &moveVector, &m_Rot);
	worldPosition += vecTmp * speed;
}

HRESULT SpaceshipClass::Draw(IDirect3DDevice9 * d3ddev)
{
	HRESULT hr = S_OK;
	DWORD AmbientColorState;
	d3ddev->GetRenderState(D3DRS_AMBIENT, &AmbientColorState);
	d3ddev->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_COLORVALUE( 0.5f, 0.5f, 0.5f, 1 ));

	D3DXMATRIX matWorld, m_Scale, m_Rotation, m_Translation;

	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixScaling(&m_Scale, 0.005f, 0.005f, 0.005f);
	D3DXMatrixMultiply(&matWorld, &matWorld, &m_Scale);

	D3DXMatrixRotationQuaternion(&m_Rotation, &localRotation);
	D3DXMatrixMultiply(&matWorld, &matWorld, &m_Rotation);
	
	D3DXMatrixTranslation(&m_Translation, worldPosition.x, worldPosition.y, worldPosition.z);
	D3DXMatrixMultiply(&matWorld, &matWorld, &m_Translation);
	
	d3ddev->SetTransform(D3DTS_WORLD, &matWorld);

	for (DWORD i=0; i<numMaterials; i++)
	{
		d3ddev->SetMaterial(&meshMaterials[i]);
		MeshShip->DrawSubset(i);
	}

	d3ddev->SetRenderState(D3DRS_AMBIENT, AmbientColorState);

	return S_OK;
}