#include "global.h"
#include "quaternion.h"

class SpaceshipClass
{
public:
	SpaceshipClass();
	SpaceshipClass(IDirect3DDevice9 * d3ddev);
	~SpaceshipClass();
	HRESULT Draw(IDirect3DDevice9 * d3ddev);
	void addRotation(float Angle, D3DXVECTOR3 vec);
	void moveForward(float speed);
	D3DXQUATERNION		localRotation;
	D3DXVECTOR3			worldPosition;
	void				addPitch(float);
	void				addRoll(float angle);
	D3DXVECTOR3			GetMoveVector();
	D3DXVECTOR3			GetWorldPos();
private:
	D3DXVECTOR3			moveVector;
	ID3DXMesh*			MeshShip;
	D3DMATERIAL9*		meshMaterials;
	DWORD				numMaterials;
	D3DMATERIAL9		baseBodyMaterial;
};