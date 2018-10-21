#include "cEquipmentFactoryImpl.h"

#include "iEquipment.h"
#include "cBall.h"
#include "cCueStick.h"

cEquipmentFactoryImpl::cEquipmentFactoryImpl()
{}


cEquipmentFactoryImpl::~cEquipmentFactoryImpl()
{}

bool cEquipmentFactoryImpl::isEquipmentTypeValid(int equipmentType)
{
	// 1 - cBall
	// 2 - cCueStick
	return (equipmentType > 0 && equipmentType <= 2);
}

iEquipment* cEquipmentFactoryImpl::createEquipment(int equipmentType)
{
	iEquipment* equipment = NULL;
	if (equipmentType == 1)
	{
		equipment = new cBall();
	}
	else if (equipmentType == 2)
	{
		equipment = new cCueStick();
	}

	return equipment;
}