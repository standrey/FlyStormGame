#ifndef DEBUGCLASS_H
#define DEBUGCLASS_H

#include "global.h"

class DebugClass
{
	IDirect3DVertexBuffer9 * pdxVertexBuffer;
	gameVertex * rawVertexBuffer;
	IDirect3DVertexBuffer9 * pdxVertexBuffer2;
public:
	DebugClass();
	DebugClass(IDirect3DDevice9 * pdxDevice);
	void Update(D3DXVECTOR3 endPos);
	void Draw(IDirect3DDevice9 * pdxDevice);
	~DebugClass();
};

#endif