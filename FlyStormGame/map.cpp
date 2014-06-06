#include "map.h"

stMapClass::stMapClass()
{
	INT rawPlan[20][15] =
	{
  {1,0,2,0,2,0,0,1,1,1,1,1,1,1,1},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {0,0,0,1,1,0,0,0,1,1,0,0,1,0,1},
  {0,0,0,1,1,0,0,0,1,0,0,0,1,0,1},
  {0,0,0,0,1,1,0,1,1,0,0,0,0,0,1},
  {0,0,0,0,0,0,0,0,0,0,0,1,0,0,1},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,1,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,1,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,1,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,1,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,1,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,1,0,0,0,1,0,0,0,0,1},
  {1,0,1,0,0,0,0,0,0,1,0,0,0,0,1},
  {1,0,1,1,0,0,0,0,1,1,0,0,0,1,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,1,1},
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	};
	szHeight = 20;
	szWidth = 15;
	floorPlan = new INT[szHeight * szWidth];
	memcpy(floorPlan, rawPlan, sizeof(INT) * szHeight * szWidth );
}

stMapClass::stMapClass(DWORD szH, DWORD szW)
{
}

DWORD stMapClass::GetSizeX() {return szWidth;}

DWORD stMapClass::GetSizeY() {return szHeight;}

DWORD stMapClass::GetHeight(DWORD x, DWORD y)
{
	if (x > szWidth || x < 0 || y > szHeight || y < 0)
		return 0;
	else
		return floorPlan[y*szWidth + x];
}