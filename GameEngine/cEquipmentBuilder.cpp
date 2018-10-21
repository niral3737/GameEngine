#include "cEquipmentBuilder.h"

#include "cCueStick.h"
#include "iCueRackBuilder.h"

cEquipmentBuilder::cEquipmentBuilder()
{}


cEquipmentBuilder::~cEquipmentBuilder()
{}

void cEquipmentBuilder::buildEquipment(int equipmentType, iEquipment* &equipment)
{
	if (equipmentType == 3) //cueRack
	{
		iCueRackBuilder* cueRack = dynamic_cast<iCueRackBuilder*> (equipment);

		cueRack->addCue(new cCueStick());
		cueRack->addCue(new cCueStick());
	}
}
