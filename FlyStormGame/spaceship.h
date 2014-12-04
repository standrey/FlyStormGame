#include "global.h"
#include "quaternion.h"
#include "stCollisionDetector.h"

class SpaceshipClass : public stIGameObject
{
public:
						SpaceshipClass(IDirect3DDevice9 * d3ddev);
	virtual				~SpaceshipClass();
	HRESULT				Draw(IDirect3DDevice9 * d3ddev);
	void				addRotation(float Angle, D3DXVECTOR3 vec);
	void				moveForward(float speed);
	D3DXQUATERNION		localRotation;
	D3DXVECTOR3			worldPosition;
	void				addPitch(float);
	void				addRoll(float angle);
	D3DXVECTOR3			GetMoveVector() const;
	D3DXVECTOR3			GetWorldPos() const;
	virtual void		DrawDebug(IDirect3DDevice9 *);
private:
	D3DXVECTOR3			moveVector;
	D3DMATERIAL9*		meshMaterials;
	DWORD				numMaterials;
	D3DMATERIAL9		baseBodyMaterial;
};