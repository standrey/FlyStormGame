#include "debug.h"

DebugClass::DebugClass() {};

void DebugClass::Update(D3DXVECTOR3 endPos)
{
	D3DXVECTOR3 normal(0.0f,0.0f,1.0f);
	rawVertexBuffer = new gameVertex[2];
	rawVertexBuffer[0] = gameVertex(D3DXVECTOR3(0,0,0), 0, 0, normal);
	rawVertexBuffer[1] = gameVertex(endPos, 0, 0, normal);

	void * pLock = NULL;
	int sizeDxVertexBuffer = 2 * sizeof(gameVertex);
	pdxVertexBuffer2->Lock(0, sizeDxVertexBuffer, (void**)&pLock,0);
	memcpy(pLock, rawVertexBuffer, sizeDxVertexBuffer);
	pdxVertexBuffer2->Unlock();

	delete [] rawVertexBuffer;
}

DebugClass::DebugClass(IDirect3DDevice9 * pdxDevice)
{
	{
	int sizeDxVertexBuffer = 2 * sizeof(gameVertex);
	pdxDevice->CreateVertexBuffer( sizeDxVertexBuffer, D3DUSAGE_WRITEONLY , CUSTOMFVF, D3DPOOL_DEFAULT, &pdxVertexBuffer2, NULL);
	}

	//create floor
	D3DXVECTOR3 normal(0.0f,0.0f,1.0f);
	rawVertexBuffer = new gameVertex[6];
	rawVertexBuffer[0] = gameVertex(D3DXVECTOR3(0,0,0), 0, 0, normal);
	rawVertexBuffer[1] = gameVertex(D3DXVECTOR3(20.0f,0,0), 0, 0, normal);

	rawVertexBuffer[2] = gameVertex(D3DXVECTOR3(0,0,0), 0, 0, normal);
	rawVertexBuffer[3] = gameVertex(D3DXVECTOR3(0,20.0f,0), 0, 0, normal);

	rawVertexBuffer[4] = gameVertex(D3DXVECTOR3(0,0,0), 0, 0, normal);
	rawVertexBuffer[5] = gameVertex(D3DXVECTOR3(0,0,20.0f), 0, 0, normal);

	void * pLock = NULL;
	int sizeDxVertexBuffer = 6 * sizeof(gameVertex);
	pdxDevice->CreateVertexBuffer( sizeDxVertexBuffer, D3DUSAGE_WRITEONLY , CUSTOMFVF, D3DPOOL_DEFAULT, &pdxVertexBuffer, NULL);
	pdxVertexBuffer->Lock(0, sizeDxVertexBuffer, (void**)&pLock,0);
	memcpy(pLock, rawVertexBuffer, sizeDxVertexBuffer);
	pdxVertexBuffer->Unlock();

	delete [] rawVertexBuffer;
}

void DebugClass::Draw(IDirect3DDevice9 * pdxDevice)
{
	pdxDevice->SetFVF(CUSTOMFVF);
    pdxDevice->SetStreamSource( 0, pdxVertexBuffer2, 0, sizeof(gameVertex) );
	pdxDevice->DrawPrimitive( D3DPT_LINELIST, 0, 1 );
}

DebugClass::~DebugClass()
{
	if (pdxVertexBuffer2 != NULL)
	{
		pdxVertexBuffer2->Release();
		pdxVertexBuffer2 = NULL;
	}

	if (pdxVertexBuffer != NULL)
	{
		pdxVertexBuffer->Release();
		pdxVertexBuffer = NULL;
	}
}

