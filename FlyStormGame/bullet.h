#ifndef BULLET_H
#define BULLET_H

#include "global.h"

class Bullet
{
public:
	Bullet();
	Bullet(IDirect3DDevice9 * , D3DXQUATERNION , D3DXVECTOR3 );
	~Bullet();
	HRESULT Draw(IDirect3DDevice9 * d3ddev);
	void setRotation(D3DXQUATERNION);
	void moveForward(float speed);
	int getDistanceFromOriginal();
	D3DXQUATERNION		localRotation;
	D3DXVECTOR3			worldPosition;
	D3DXVECTOR3			moveDirection;
	//one texture for every bullet
private:
	static IDirect3DTexture9*			texture;
	IDirect3DVertexBuffer9*				squareBullet;
	IDirect3DVertexDeclaration9*		bulletVertexDecl;
};

#endif