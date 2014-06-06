#include "quaternion.h"
#include <math.h>

D3DXMATRIX quaternion::toMatrix()
{
	
	D3DXMATRIX resMatrix;
	float wx, wy, wz, xx, yy, yz, xy, xz, zz, x2, y2, z2;
    float s  = 2.0f/(x*x+y*y+z*z+w*w);
    x2 = x * s;    y2 = y * s;    z2 = z * s;
    xx = x * x2;   xy = x * y2;   xz = x * z2;
    yy = y * y2;   yz = y * z2;   zz = z * z2;
    wx = w * x2;   wy = w * y2;   wz = w * z2;

	resMatrix.m[0][0] = 1.0f - (yy + zz);
    resMatrix.m[1][0] = xy - wz;
    resMatrix.m[2][0] = xz + wy;
	resMatrix.m[3][0] = 0;

    resMatrix.m[0][1] = xy + wz;
    resMatrix.m[1][1] = 1.0f - (xx + zz);
	resMatrix.m[2][1] = yz - wx;
	resMatrix.m[3][1] = 0;
	
	resMatrix.m[0][2] = xz - wy;
	resMatrix.m[1][2] = yz + wx;
    resMatrix.m[2][2] = 1.0f - (xx + yy);
	resMatrix.m[3][2] = 0;

	resMatrix.m[0][3] = 0;
	resMatrix.m[1][3] = 0;
	resMatrix.m[2][3] = 0;
	resMatrix.m[3][3] = 1;

	return resMatrix;
}

quaternion conjugate(const quaternion q)
{
	quaternion res;
	res.x = -q.x;
	res.y = -q.y;
	res.z = -q.z;
	res.w = q.w;
	return res;
}

D3DXVECTOR3 quaternion::getInvVector()
{
	D3DXVECTOR3 resVec;
	resVec.x = -x;
	resVec.y = -y;
	resVec.z = -z;
	return resVec;
}

quaternion mult(const quaternion  a, const quaternion  b)
{
	quaternion res;
	res.x = a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y;
	res.y = a.w * b.y - a.x * b.z + a.y * b.w + a.z * b.x;
	res.z = a.w * b.z + a.x * b.y - a.y * b.x + a.z * b.w;
	res.w = a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z;
	return res;
}

quaternion::quaternion()
{
	x = 0;
	y = 0;
	z = 0;
	w = 1;
}

D3DXVECTOR3 quaternion::transformVector(D3DXVECTOR3 Vec)
{
	D3DXVECTOR3 vecTransformed;
	D3DXMATRIX matRotation = toMatrix();
	vecTransformed.x = Vec.x * matRotation._11 + Vec.y * matRotation._21 + Vec.z * matRotation._31 + matRotation._41;
	vecTransformed.y = Vec.x * matRotation._12 + Vec.y * matRotation._22 + Vec.z * matRotation._32 + matRotation._42;
	vecTransformed.z = Vec.x * matRotation._13 + Vec.y * matRotation._23 + Vec.z * matRotation._33 + matRotation._43;
	return vecTransformed;
}

double length(quaternion q)
{
	return sqrt(q.x*q.x + q.y*q.y + q.z*q.z + q.w*q.w);
}

quaternion normalize(quaternion q)
{
	double len = length(q);
	q.x /= len;
	q.y /= len;
	q.z /= len;
	q.w /= len;
	return q;
}

D3DXVECTOR3 quaternion::rotateAbout(double Angle, D3DXVECTOR3 & Axes, D3DXVECTOR3 & View)
{
	D3DXVECTOR3 result;
	quaternion temp(Axes.x * sin(Angle/2), Axes.y * sin(Angle/2), Axes.z * sin(Angle/2), cos(Angle/2));
	quaternion quat_view(View.x, View.y, View.z, 0);
	quaternion res;
	res = normalize(mult(mult(temp, quat_view), conjugate(temp)));
	result.x = res.x;
	result.y = res.y;
	result.z = res.z;
	return result;
}

void quaternion::addAngle(double Angle, D3DXVECTOR3 & v)
{
	quaternion quat_view(v.x, v.y, v.z, 0);
	quaternion temp(v.x * sin(Angle/2), v.y * sin(Angle/2), v.z * sin(Angle/2), cos(Angle/2));
	quaternion res;
	res = normalize(mult(*this, temp));
	x = res.x;
	y = res.y;
	z = res.z;
	w = res.w;
}

/*
D3DXMATRIX quaternion::toMatrix()
{
	
	D3DXMATRIX resMatrix;
	float wx, wy, wz, xx, yy, yz, xy, xz, zz, x2, y2, z2;
    float s  = 2.0f/(x*x+y*y+z*z+w*w);
    x2 = x * s;    y2 = y * s;    z2 = z * s;
    xx = x * x2;   xy = x * y2;   xz = x * z2;
    yy = y * y2;   yz = y * z2;   zz = z * z2;
    wx = w * x2;   wy = w * y2;   wz = w * z2;

	resMatrix.m[0][0] = 1.0f - (yy + zz);
    resMatrix.m[1][0] = xy - wz;
    resMatrix.m[2][0] = xz + wy;
	resMatrix.m[3][0] = 0;

    resMatrix.m[0][1] = xy + wz;
    resMatrix.m[1][1] = 1.0f - (xx + zz);
	resMatrix.m[2][1] = yz - wx;
	resMatrix.m[3][1] = 0;
	
	resMatrix.m[0][2] = xz - wy;
	resMatrix.m[1][2] = yz + wx;
    resMatrix.m[2][2] = 1.0f - (xx + yy);
	resMatrix.m[3][2] = 0;

	resMatrix.m[0][3] = 0;
	resMatrix.m[1][3] = 0;
	resMatrix.m[2][3] = 0;
	resMatrix.m[3][3] = 1;

	return resMatrix;
	
//	return	 D3DXMATRIX(	1-2*y*y-2*z*z,	2*x*y-2*w*z,	2*x*z+2*w*y,	0,
//							2*x*y + 2*w*z,	1-2*x*x-2*z*z,	2*y*z-2*w*x,	0,
//							2*x*z-2*w*y,	2*y*z+2*w*x,	1-2*x*x-2*y*y,	0,
//							0,				0,				0,				1);
	
}

double quaternion::length()
{
	return sqrt(x*x + y*y + z*z + w*w);
}

void quaternion::normalize()
{
	double len = length();
	x /= len;
	y /= len;
	z /= len;
	w /= len;
}
*/