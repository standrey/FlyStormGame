#include "bullet.h"

IDirect3DTexture9*			Bullet::texture = NULL;
IDirect3DVertexBuffer9*		Bullet::squareBullet = NULL;

Bullet::Bullet()
{
}

Bullet::Bullet(IDirect3DDevice9 * d3ddev, D3DXVECTOR3 faceToCam, D3DXVECTOR3 moveDir, D3DXVECTOR3 worldPos)
{
	worldPosition = worldPos;
	moveDirection = moveDir;

	HRESULT hr = D3DXCreateTextureFromFile(d3ddev, L"gameresources/bullet.jpg", &(Bullet::texture));

	// try to eval perpendicular vector
	D3DXVECTOR3 firstVector(1,1,0);
	firstVector.z = -(faceToCam.x + faceToCam.y)/faceToCam.z; 
	D3DXVec3Normalize(&firstVector, &firstVector);

	D3DXVECTOR3 secondVector;
	D3DXVec3Cross(&secondVector, &faceToCam, &firstVector);
	D3DXVec3Normalize(&secondVector, &secondVector);

	void * lock = NULL;
	
	float scaleBullet = 0.2f;
	gameVertex rawVertices[] =
	{
		gameVertex(-secondVector*scaleBullet, (float)0, (float)0, -faceToCam),
		gameVertex(-firstVector*scaleBullet, (float)1, (float)0, -faceToCam),
		gameVertex(firstVector*scaleBullet, (float)0, (float)1, -faceToCam),
		gameVertex(secondVector*scaleBullet, (float)1, (float)1, -faceToCam)
	};

	int vbSize = sizeof(rawVertices);
	hr = d3ddev->CreateVertexBuffer(vbSize, D3DUSAGE_WRITEONLY, CUSTOMFVF, D3DPOOL_DEFAULT, &Bullet::squareBullet, 0);
	if (FAILED(hr))
		return;
	squareBullet->Lock(0, vbSize, &lock, 0);
	memcpy(lock, rawVertices, vbSize);
	squareBullet->Unlock();
}

Bullet::~Bullet()
{
	if (Bullet::squareBullet)
	{
		Bullet::squareBullet->Release();
		Bullet::squareBullet = NULL;
	}
	if (Bullet::texture)
	{
		Bullet::texture->Release();
	}

}

HRESULT Bullet::Draw(IDirect3DDevice9 * d3ddev)
{
	D3DXMATRIX matWorld;
	D3DXMatrixTranslation( &matWorld, worldPosition.x, worldPosition.y, worldPosition.z);
	d3ddev->SetFVF(CUSTOMFVF);
	d3ddev->SetTexture( 0, Bullet::texture );
	d3ddev->SetTransform( D3DTS_WORLD, &matWorld );
	d3ddev->SetStreamSource( 0, Bullet::squareBullet, 0, sizeof(gameVertex) );
	d3ddev->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2);
	return S_OK;
}

void Bullet::addRotation(float Angle, D3DXVECTOR3 vec)
{
}

void Bullet::moveForward(float speed)
{
	worldPosition += moveDirection * speed;
}

int Bullet::getDistanceFromOriginal()
{
	return sqrt(worldPosition.x * worldPosition.x + worldPosition.y * worldPosition.y + worldPosition.z * worldPosition.z);
}