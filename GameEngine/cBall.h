#ifndef _C_EQUIPMENT_HG_
#define _C_EQUIPMENT_HG_

#include <iostream>
#include <glm/glm.hpp>

#include "iEquipment.h"
#include "iMeshObject.h"

class cBall : public iEquipment
{
public:
	cBall();
	virtual ~cBall();

	virtual void move(void);
	virtual iMeshObject* getMesh();

	iMeshObject* mesh;
	int ballNumber;
	glm::vec3 color;
	bool isSolid;
};


#endif // !_C_EQUIPMENT_HG_


