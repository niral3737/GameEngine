#include "cEquipmentFactory.h"

#include "cEquipmentFactoryImpl.h"

cEquipmentFactory::cEquipmentFactory() 
	: impl(new cEquipmentFactoryImpl())
{}

cEquipmentFactory::cEquipmentFactory(iMediatorEquipment * mediator)
	:impl(new cEquipmentFactoryImpl())
{
	impl->setMediator(mediator);
}

cEquipmentFactory::~cEquipmentFactory()
{
	delete this->impl;
}

bool cEquipmentFactory::isEquipmentTypeValid(int equipmentType)
{
	return this->impl->isEquipmentTypeValid(equipmentType);
}

iEquipment* cEquipmentFactory::createEquipment(int equipmentType)
{
	return this->impl->createEquipment(equipmentType);

}

