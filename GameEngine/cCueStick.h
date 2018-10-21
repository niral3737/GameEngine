#ifndef _CUE_STICK_HG_
#define _CUE_STICK_HG_

#include <iostream>

#include "iEquipment.h"
#include "iMeshObject.h"

class cCueStick : public iEquipment
{
public:
	cCueStick();
	virtual ~cCueStick();

	virtual void move(void);
	virtual iMeshObject* getMesh();

	iMeshObject* mesh;
};

#endif // !_CUE_STICK_HG_



