#ifndef _EQUIPMENT_FACTORY_HG_
#define _EQUIPMENT_FACTORY_HG_

#include "iEquipment.h"

class cEquipmentFactoryImpl;

class cEquipmentFactory
{
public:
	cEquipmentFactory();
	~cEquipmentFactory();

	bool isEquipmentTypeValid(int equipmentType);

	iEquipment* createEquipment(int equipmentType);

private:
	cEquipmentFactoryImpl* impl;
};

#endif // !_EQUIPMENT_FACTORY_HG_



