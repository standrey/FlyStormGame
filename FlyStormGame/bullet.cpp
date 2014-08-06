#include "bullet.h"

Bullet::Bullet() {}

Bullet::Bullet(IDirect3DDevice9 * d3ddev, D3DXQUATERNION quatRot, D3DXVECTOR3 worldPos)
{
	worldPosition = worldPos;

	D3DXQuaternionIdentity(&localRotation);
	HRESULT hr;

	void * lock = NULL;
	D3DXMATRIX rotationMatrix;
	D3DXMatrixRotationQuaternion(&rotationMatrix, &quatRot);
	D3DXVECTOR3 normal = D3DXVECTOR3(0, 0, -1);
	D3DXVECTOR3 vec[4] = {	D3DXVECTOR3( 0.1f,  0.1f, 0),
							D3DXVECTOR3(-0.1f,  0.1f, 0),
							D3DXVECTOR3( 0.1f, -0.1f, 0),
							D3DXVECTOR3(-0.1f, -0.1f, 0)
						 };

	D3DXVec3TransformCoord(&moveDirection, &normal, &rotationMatrix);
	{
		WCHAR message[1000];
		swprintf(message, L"Bullet(). Move direction %f %f %f\n", moveDirection.x, moveDirection.y, moveDirection.z);
		OutputDebugString(message);
	}

	bulletVertex rawVertices[] =
	{
		bulletVertex(vec[0], (float)0, (float)0),
		bulletVertex(vec[1], (float)1, (float)0),
		bulletVertex(vec[2], (float)0, (float)1),
		bulletVertex(vec[3], (float)1, (float)1)
	};

	int vbSize = sizeof(rawVertices);
	hr = d3ddev->CreateVertexBuffer(vbSize, D3DUSAGE_WRITEONLY, CUSTOMFVF, D3DPOOL_DEFAULT, &squareBullet, 0);
	if (FAILED(hr))
		return;
	squareBullet->Lock(0, vbSize, &lock, 0);
	memcpy(lock, rawVertices, vbSize);
	squareBullet->Unlock();

	hr = d3ddev->CreateVertexDeclaration(rawBulletVertex, &bulletVertexDecl);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"CreateVertexDeclaration", L"Error", MB_ICONERROR);
		return;
	}
}

HRESULT Bullet::Draw(IDirect3DDevice9 * d3ddev)
{
	D3DXMATRIX matWorld;
	D3DXMatrixTranslation( &matWorld, worldPosition.x, worldPosition.y, worldPosition.z);
	
	D3DXMATRIX m_Rot;
	D3DXMatrixRotationQuaternion(&m_Rot, &localRotation);

	D3DXMatrixMultiply(&matWorld, &m_Rot, &matWorld);
	
	d3ddev->SetRenderState(D3DRS_LIGHTING, FALSE);
    d3ddev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	d3ddev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	d3ddev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	d3ddev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	d3ddev->SetTexture(0, Bullet::texture);
	d3ddev->SetStreamSource( 0, Bullet::squareBullet, 0, sizeof(float) * 5 );
	d3ddev->SetVertexDeclaration(bulletVertexDecl);
	
	d3ddev->SetTransform( D3DTS_WORLD, &matWorld );
	
	d3ddev->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2);

	//d3ddev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	return S_OK;
}

void Bullet::setRotation(D3DXQUATERNION rotQuat)
{
	 localRotation = rotQuat;
}

void Bullet::moveForward(float speed)
{
	worldPosition += moveDirection * speed;
}

int Bullet::getDistanceFromOriginal()
{
	return sqrt(worldPosition.x * worldPosition.x + worldPosition.y * worldPosition.y + worldPosition.z * worldPosition.z);
}


Bullet::~Bullet()
{
	if (squareBullet)
	{
		squareBullet->Release();
		squareBullet = NULL;
	}
	if (bulletVertexDecl)
	{
		bulletVertexDecl->Release();
		bulletVertexDecl = NULL;
	}
}