#ifndef _EQUIPMENT_BUILDER_
#define _EQUIPMENT_BUILDER_

#include "iEquipment.h"

class cEquipmentBuilder
{
public:
	cEquipmentBuilder();
	~cEquipmentBuilder();

	void buildEquipment(int equipmentType, iEquipment* &equipment);
};

#endif // !_EQUIPMENT_BUILDER_

