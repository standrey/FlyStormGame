#ifndef MAP_H
#define MAP_H

#include "global.h"

class stMapClass
{
public:
	stMapClass();
	stMapClass(DWORD szH, DWORD szW);
	DWORD GetHeight(DWORD x, DWORD y);
	DWORD GetSizeX();
	DWORD GetSizeY();
private:
	DWORD szHeight;
	DWORD szWidth;
	INT * floorPlan;
};

#endif