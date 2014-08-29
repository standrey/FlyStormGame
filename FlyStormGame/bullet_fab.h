#ifndef BULLETFAB_H
#define BULLETFAB_H

#include "global.h"

#include <deque>
#include <algorithm>

	struct Bullet {
		D3DXVECTOR3 worldPosition;
		D3DXQUATERNION localQuat;
		D3DXVECTOR3 moveDir;
		Bullet(D3DXVECTOR3 pos, D3DXQUATERNION quat) : worldPosition(pos), localQuat(quat) {
			// change move direction according quaternion
			D3DXMATRIX rotationMatrix;
			D3DXMatrixRotationQuaternion(&rotationMatrix, &localQuat);
			D3DXVECTOR3 normal = D3DXVECTOR3(0, 0, -1);
			D3DXVec3TransformCoord(&moveDir, &normal, &rotationMatrix);
		}
	};

class BulletFabric
{
public:
						BulletFabric();
						BulletFabric(IDirect3DDevice9 * );
						~BulletFabric();
	HRESULT				Draw();
	void				setRotationBullets(D3DXQUATERNION);
	void				moveForward(float speed);
	int					getDistanceFromOriginal();
	void				AddBullet(D3DXQUATERNION, D3DXVECTOR3);
	void				RemoveFirstBullet();
	D3DXQUATERNION		localRotation;
	D3DXVECTOR3			worldPosition;
	D3DXVECTOR3			moveDirection;

private:
	IDirect3DTexture9*					texture;
	IDirect3DVertexBuffer9*				squareBullet;
	IDirect3DVertexDeclaration9*		bulletVertexDecl;
	std::deque<Bullet *>				bulletList;
	D3DLIGHT9							bulletLight;
	IDirect3DDevice9 *					l_d3ddev;
	int									maxLights;
};

#endif