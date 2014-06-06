#include "camera.h"

CameraClass::CameraClass()
{
	lookAt = D3DXVECTOR3(0,0,0);
	upVector = D3DXVECTOR3(0,1,0);
	cameraPosition = D3DXVECTOR3(0,2,10);
	D3DXVec3Normalize(&cameraPosition, &cameraPosition);
	D3DXQuaternionIdentity(&cameraRotation);
	D3DXQuaternionIdentity(&rotationQuat);
}

CameraClass::~CameraClass()
{
}

// get real lookAt (just vector in local space coords)
D3DXVECTOR3 CameraClass::GetLookAt()
{
	return lookAt - cameraPosition * 3;
}

// set world position of Ship ant set it to lookAt
void CameraClass::setPosition(D3DXVECTOR3 newPos)
{
	lookAt = newPos;
	UpdateCamera();
}

void CameraClass::setRotation(D3DXQUATERNION rotQuat)
{
	rotationQuat = rotQuat;
}

void CameraClass::UpdateCamera()
{

	cameraPosition = D3DXVECTOR3(0,2,10);
	upVector = D3DXVECTOR3(0,1,0);

	D3DXVec3Normalize(&cameraPosition, &cameraPosition);

	D3DXQuaternionSlerp(&cameraRotation, &cameraRotation, &rotationQuat, 0.1f);

	D3DXMATRIX m_Rot;
	D3DXMatrixRotationQuaternion(&m_Rot, &cameraRotation);

	D3DXVec3TransformCoord(&cameraPosition, &cameraPosition, &m_Rot);
	D3DXVec3Normalize(&cameraPosition, &cameraPosition);
	D3DXVec3TransformCoord(&upVector, &upVector, &m_Rot);
	D3DXVec3Normalize(&upVector, &upVector);

	D3DXVECTOR3 realCameraPosition = cameraPosition * 3;
	realCameraPosition += lookAt;
	D3DXMatrixLookAtLH(&viewMatrix, &realCameraPosition, &lookAt, &upVector);
}

D3DXMATRIX CameraClass::GetViewMatix()
{
	return viewMatrix;
}