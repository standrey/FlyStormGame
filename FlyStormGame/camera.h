#include "global.h"
#include "quaternion.h"

class CameraClass
{
private:
	D3DXVECTOR3 lookAt;
	D3DXVECTOR3 upVector;
	D3DXQUATERNION cameraRotation;
	D3DXQUATERNION rotationQuat;
	D3DXVECTOR3 cameraPosition;
	D3DXMATRIX viewMatrix;
	quaternion q;
public:
	CameraClass();
	~CameraClass();
	D3DXVECTOR3 GetLookAt();
	D3DXMATRIX GetViewMatix();
	void UpdateCamera();
	void setRotation(D3DXQUATERNION);
	void setPosition(D3DXVECTOR3);
};