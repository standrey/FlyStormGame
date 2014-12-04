#ifndef SCENE_H
#define SCENE_H

#include <deque>
#include <algorithm>

#include "building.h"
#include "map.h"
#include "debug.h"
#include "camera.h"
#include "spaceship.h"
#include "bullet_fab.h"
#include "cinput.h"
#include "stIGameObject.h"
#include "stCollisionDetector.h"

class stSceneClass
{
public:
						stSceneClass(IDirect3DDevice9 * d3ddev);
						~stSceneClass();
	HRESULT				DrawObjects();
	void				KeyboardEvent(CInput * keybState);
	void				UpdateScene();
private:
	std::vector<stIGameObject* >			m_DynamicObjects;
	std::vector<stITerrainObject* >			m_Terrain;
	std::vector<stIGameObject* >			m_Buildings;
	IDirect3DDevice9*						device;
	D3DLIGHT9								light;
	DebugClass*								DebugObject;
	Building*								BuildingObject;
	stMapClass*								MapObject;
	CameraClass*							Camera;
	SpaceshipClass*							Ship;
	BulletFabric*							Bullets;
	DWORD									lastFireTime;
	HRESULT									PrepareObjects(IDirect3DDevice9 * d3ddev);
	std::unique_ptr<stCollisionDetector>	CollisionDetect;
};

#endif