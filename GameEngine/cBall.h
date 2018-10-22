#ifndef _C_EQUIPMENT_HG_
#define _C_EQUIPMENT_HG_

#include <iostream>
#include <glm/glm.hpp>

#include "iEquipment.h"
#include "iMeshObject.h"
#include "iMediatorEquipment.h"

class cBall : public iEquipment
{
public:
	cBall();
	virtual ~cBall();

	virtual void move(void);
	void hit(void);
	virtual iMeshObject* getMesh();
	virtual void setMesh(iMeshObject* mesh);
	virtual void setMediator(iMediatorEquipment* mediator);

	iMeshObject* mesh;
	int ballNumber;
	glm::vec3 color;
	bool isSolid;

private:
	iMediatorEquipment* mediator;
};


#endif // !_C_EQUIPMENT_HG_


