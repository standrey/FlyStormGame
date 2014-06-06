#ifndef QUAT_H
#define QUAT_H

#include "global.h"

class quaternion
{
public:
	float x, y, z, w;
	quaternion();
	quaternion(float xv, float yv, float zv, float wv): x(xv), y(yv), z(zv), w(wv) {}
	void			addAngle(double Angle, D3DXVECTOR3 & v);
	D3DXMATRIX		toMatrix();
	D3DXVECTOR3 rotateAbout(double Angle, D3DXVECTOR3 & Axes, D3DXVECTOR3 & View);
	D3DXVECTOR3 getInvVector();
	D3DXVECTOR3 transformVector(D3DXVECTOR3 pVec);
};

#endif