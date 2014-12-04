#ifndef COLLISION_DETECTOR_H
#define COLLISION_DETECTOR_H

#include "global.h"
#include "stIGameObject.h"
#include <vector>

class stCollisionDetector
{
public:
	void CheckAndMark(	std::vector<stIGameObject *>	& dynamicObjects, 
						std::vector<stITerrainObject *> & terrains, 
						std::vector<stIGameObject *>	& buildings);
private:

};

#endif