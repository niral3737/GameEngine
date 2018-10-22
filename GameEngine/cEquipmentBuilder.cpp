#include "cEquipmentBuilder.h"

#include "cCueStick.h"
#include "iCueRackBuilder.h"
#include "cEquipmentFactory.h"

cEquipmentBuilder::cEquipmentBuilder()
{}


cEquipmentBuilder::~cEquipmentBuilder()
{}

void cEquipmentBuilder::buildEquipment(int equipmentType, iEquipment* &equipment)
{
	if (equipmentType == 3) //cueRack
	{
		iCueRackBuilder* cueRack = dynamic_cast<iCueRackBuilder*> (equipment);

		cEquipmentFactory* equipmentFactory = new cEquipmentFactory();
		cueRack->addCue(equipmentFactory->createEquipment(2));
	}
}
