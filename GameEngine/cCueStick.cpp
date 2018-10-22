#include "cCueStick.h"
#include "json.hpp"


cCueStick::cCueStick()
{
}


cCueStick::~cCueStick()
{}

void cCueStick::move(void)
{
	std::cout << "hit" << std::endl;
}

iMeshObject* cCueStick::getMesh()
{
	return this->mesh;
}

void cCueStick::setMesh(iMeshObject * mesh)
{
	this->mesh = mesh;
}

void cCueStick::setMediator(iMediatorEquipment * mediator)
{
	this->mediator = mediator;
}

void cCueStick::hitCueBall()
{
	nlohmann::json message;
	message["to"] = "cueBall";
	message["action"] = "hit";

	this->mediator->sendMessageToEquipment(message);
}
