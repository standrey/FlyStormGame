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

class stSceneClass
{
public:
	stSceneClass();
	stSceneClass(IDirect3DDevice9 * d3ddev);
	~stSceneClass();
	HRESULT			DrawObjects();
	void			KeyboardEvent(CInput * keybState);
	void			UpdateScene();
private:
	IDirect3DDevice9*		device;
	D3DLIGHT9				light;
	DebugClass*				DebugObject;
	Building*				BuildingObject;
	stMapClass*				MapObject;
	CameraClass*			Camera;
	SpaceshipClass*			Ship;
	BulletFabric*			Bullets;
	DWORD					lastFireTime;
	HRESULT					PrepareObjects(IDirect3DDevice9 * d3ddev);
};

#endif