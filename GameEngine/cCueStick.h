#ifndef _CUE_STICK_HG_
#define _CUE_STICK_HG_

#include <iostream>

#include "iEquipment.h"
#include "iMeshObject.h"
#include "iMediatorEquipment.h"

class cCueStick : public iEquipment
{
public:
	cCueStick();
	virtual ~cCueStick();

	virtual void move(void);
	virtual iMeshObject* getMesh();
	virtual void setMesh(iMeshObject* mesh);
	virtual void setMediator(iMediatorEquipment* mediator);
	void hitCueBall();

	iMeshObject* mesh;

private:
	iMediatorEquipment* mediator;
};

#endif // !_CUE_STICK_HG_



