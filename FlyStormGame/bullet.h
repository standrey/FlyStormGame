#ifndef BULLET_H
#define BULLET_H

#include "global.h"

class Bullet
{
public:
	Bullet();
	Bullet(IDirect3DDevice9 * d3ddev, D3DXVECTOR3 faceToCam, D3DXVECTOR3 moveDir, D3DXVECTOR3 worldPos);
	~Bullet();
	HRESULT Draw(IDirect3DDevice9 * d3ddev);
	void addRotation(float Angle, D3DXVECTOR3 vec);
	void moveForward(float speed);
	int getDistanceFromOriginal();
	D3DXQUATERNION		localRotation;
	D3DXVECTOR3			worldPosition;
	D3DXVECTOR3			moveDirection;
	//one texture for every bullet
private:
	static IDirect3DTexture9*			texture;
	static IDirect3DVertexBuffer9*		squareBullet;
};

#endif