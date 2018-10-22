#ifndef _EQUIPMENT_FACTORY_IMPL_HG_
#define _EQUIPMENT_FACTORY_IMPL_HG_

#include "iEquipment.h"
#include "cEquipmentBuilder.h"
#include "iMediatorEquipment.h"

class cEquipmentFactoryImpl
{
public:
	cEquipmentFactoryImpl();
	~cEquipmentFactoryImpl();

	bool isEquipmentTypeValid(int equipmentType);

	iEquipment* createEquipment(int equipmentType);

	void setMediator(iMediatorEquipment* mediator);
private:
	cEquipmentBuilder* builder;
	iMediatorEquipment* mediator;
};

#endif // !_EQUIPMENT_FACTORY_IMPL_HG_



