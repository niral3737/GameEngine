#ifndef _MEDIATOR_HG_
#define _MEDIATOR_HG_

#include "iMediatorEquipment.h"
#include "json.hpp"

class cMediator : public iMediatorEquipment
{
public:
	cMediator();
	~cMediator();

	void LoadEquipment(iEquipment* equipment);
	
	iEquipment* findEquipmentByName(std::string name);

	virtual void sendMessageToEquipment(std::vector<std::string> vecMessage);
	virtual void sendMessageToEquipment(nlohmann::json message);

private:
	std::vector<iEquipment*> vecEquipments;
};


#endif // !_MEDIATOR_HG_



