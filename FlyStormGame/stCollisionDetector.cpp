#include "stCollisionDetector.h"

void stCollisionDetector::CheckAndMark(	
	std::vector<stIGameObject *>	& dynamicObjects, 
	std::vector<stITerrainObject *> & terrains, 
	std::vector<stIGameObject *>	& buildings) {
	for (auto i = dynamicObjects.begin(); i != dynamicObjects.end(); ++i) {
		D3DXVECTOR3 minB_Object1 = (*i)->GetMinBound();
		D3DXVECTOR3 maxB_Object1 = (*i)->GetMaxBound();
		D3DXVECTOR3 minB_Object2;
		D3DXVECTOR3 maxB_Object2;
		//check for intersection with every object
		for (auto j = dynamicObjects.begin(); j != dynamicObjects.end(); ++j) {
			if (j == i) continue;
			minB_Object2 = (*j)->GetMinBound();
			maxB_Object2 = (*j)->GetMaxBound();
			if ((minB_Object1.x < maxB_Object2.x || minB_Object2.x > maxB_Object1.x)&&
				(minB_Object1.y < maxB_Object2.y || minB_Object2.y > maxB_Object1.y)&&
				(minB_Object1.z < maxB_Object2.z || minB_Object2.z > maxB_Object1.z))
				(*i)->AddCollisionObject(*j);
		}
	}
}