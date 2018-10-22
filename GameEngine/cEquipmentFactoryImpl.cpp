#include "cEquipmentFactoryImpl.h"

#include "iEquipment.h"
#include "cBall.h"
#include "cCueStick.h"
#include "cCueRack.h"

cEquipmentFactoryImpl::cEquipmentFactoryImpl()
{}


cEquipmentFactoryImpl::~cEquipmentFactoryImpl()
{
	delete this->builder;
}

bool cEquipmentFactoryImpl::isEquipmentTypeValid(int equipmentType)
{
	// 1 - cBall
	// 2 - cCueStick
	// 3 - cCueRack
	return (equipmentType > 0 && equipmentType <= 3);
}

iEquipment* cEquipmentFactoryImpl::createEquipment(int equipmentType)
{
	iEquipment* equipment = NULL;
	if (equipmentType == 1)
	{
		cBall* ball = new cBall();
		ball->setMediator(mediator);
		equipment = ball;
	}
	else if (equipmentType == 2)
	{
		cCueStick* cueStick = new cCueStick();
		cueStick->setMediator(mediator);
		equipment = cueStick;
	}
	else if (equipmentType == 3)
	{
		cCueRack* cueRack = new cCueRack();
		cueRack->setMediator(mediator);
		equipment = cueRack;
	}

	return equipment;
}

void cEquipmentFactoryImpl::setMediator(iMediatorEquipment* mediator)
{
	this->mediator = mediator;
}
