#ifndef BUILDING_H
#define BUILDING_H

#include "global.h"
#include <vector>

class Building
{
	LPDIRECT3DTEXTURE9 buildingTexture;
	std::vector<IDirect3DVertexBuffer9 *> pdxVertexBuildings;
	IDirect3DVertexBuffer9 * pdxVertexFloor;
public:
	Building();
	Building(IDirect3DDevice9 * pdxDevice);
	IDirect3DVertexBuffer9 * CreateBuilding(IDirect3DDevice9 * d3ddev, DWORD hightBuilding);
	void Draw(IDirect3DDevice9 * pdxDevice, D3DXVECTOR3 worldPos, float height);
	~Building();
};

#endif