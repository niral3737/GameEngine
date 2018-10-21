#ifndef _EQUIPMENT_FACTORY_IMPL_HG_
#define _EQUIPMENT_FACTORY_IMPL_HG_

#include "iEquipment.h"
#include "cEquipmentBuilder.h"

class cEquipmentFactoryImpl
{
public:
	cEquipmentFactoryImpl();
	~cEquipmentFactoryImpl();

	bool isEquipmentTypeValid(int equipmentType);

	iEquipment* createEquipment(int equipmentType);
private:
	cEquipmentBuilder* builder;
};

#endif // !_EQUIPMENT_FACTORY_IMPL_HG_



