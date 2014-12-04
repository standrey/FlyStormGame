#ifndef IGAMEOBJECT_H
#define IGAMEOBJECT_H

#include "global.h"
#include <vector>

class stIGameObject {
private:
	std::vector<stIGameObject*>		collideWithThis;

protected:
	//AABB in object space
	D3DXVECTOR3						minB;
	D3DXVECTOR3						maxB;

	//OBB in object space
	D3DXVECTOR3						minBounds;
	D3DXVECTOR3						maxBounds;

	ID3DXMesh*						m_GeneralMesh;
	D3DXVECTOR3						obb[8];
	
	// object bounding box calculation
	void TranslateObbFromMesh(const D3DXMATRIX * p_matWorld) {
		D3DXVECTOR3 m_objectBounds[8];
		m_objectBounds[0] = D3DXVECTOR3(minBounds.x, minBounds.y, minBounds.z);
		m_objectBounds[1] = D3DXVECTOR3(maxBounds.x, minBounds.y, minBounds.z);
		m_objectBounds[2] = D3DXVECTOR3(maxBounds.x, maxBounds.y, minBounds.z);
		m_objectBounds[3] = D3DXVECTOR3(minBounds.x, maxBounds.y, minBounds.z);

		m_objectBounds[4] = D3DXVECTOR3(minBounds.x, minBounds.y, maxBounds.z);
		m_objectBounds[5] = D3DXVECTOR3(maxBounds.x, minBounds.y, maxBounds.z);
		m_objectBounds[6] = D3DXVECTOR3(maxBounds.x, maxBounds.y, maxBounds.z);
		m_objectBounds[7] = D3DXVECTOR3(minBounds.x, maxBounds.y, maxBounds.z);

		for( int i = 0; i < 8; i++ )
			D3DXVec3TransformCoord( &obb[i], &m_objectBounds[i], p_matWorld );
	}

	void InitObbFromMesh() {
		BYTE* pVertices = NULL;
		HRESULT hr = m_GeneralMesh->LockVertexBuffer(D3DLOCK_READONLY, (LPVOID*)&pVertices);
		if (FAILED(hr)) {
			MessageBox(NULL, L"Can't lock mesh structure", L"Error", MB_ICONEXCLAMATION);
			return;
		}
		D3DXComputeBoundingBox((D3DXVECTOR3*)pVertices, m_GeneralMesh->GetNumVertices(), D3DXGetFVFVertexSize(m_GeneralMesh->GetFVF()), &minBounds, &maxBounds);
		m_GeneralMesh->UnlockVertexBuffer();
	}

	void CalcAABBFromOBB() {
		minB.x = maxB.x = obb[0].x;
		minB.y = maxB.y = obb[0].y;
		minB.z = maxB.z = obb[0].z;
		for (int i=1;i<8;++i) {
			if (obb[i].x < minB.x) minB.x = obb[i].x;
			if (obb[i].x > maxB.x) maxB.x = obb[i].x;
			if (obb[i].y < minB.y) minB.y = obb[i].y;
			if (obb[i].y > maxB.y) maxB.y = obb[i].y;
			if (obb[i].z < minB.x) minB.z = obb[i].z;
			if (obb[i].z > maxB.x) maxB.z = obb[i].z;
		}
	}
public:
	D3DXVECTOR3			GetMaxBound() const {return maxB;} 
	D3DXVECTOR3			GetMinBound() const {return minB;}
	void				SetOob(const D3DXVECTOR3 * obb) {if (obb == nullptr) return; memcpy(this->obb, obb, sizeof(D3DXVECTOR3) * 8);}
	void				SetCollisionObjects(std::vector<stIGameObject*> & collideObjects) {collideWithThis = collideObjects;}
	void				AddCollisionObject(stIGameObject* obj) {collideWithThis.push_back(obj);}
	virtual void		DrawDebug(IDirect3DDevice9 *) = 0;
	virtual				~stIGameObject() {};
};

class stITerrainObject {
public:
	D3DXVECTOR3 * GetOBB() const;
};

#endif