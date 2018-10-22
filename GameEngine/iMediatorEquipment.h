#ifndef _MEDIATOR_EQUIPMENT_HG_
#define _MEDIATOR_EQUIPMENT_HG_

#include "iEquipment.h"
#include <string>
#include <vector>
#include "json.hpp"

class iMediatorEquipment
{
public:
	virtual ~iMediatorEquipment() {};

	virtual iEquipment* findEquipmentByName(std::string name) = 0;

	virtual void sendMessageToEquipment(std::vector<std::string> vecMessage) = 0;

	virtual void sendMessageToEquipment(nlohmann::json message) = 0;
};

#endif // !_MEDIATOR_EQUIPMENT_HG_

