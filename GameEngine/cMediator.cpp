#include "cMediator.h"

#include "cMeshObject.h"
#include "cBall.h"

cMediator::cMediator()
{}


cMediator::~cMediator()
{}

void cMediator::LoadEquipment(iEquipment* equipment)
{
	this->vecEquipments.push_back(equipment);
}

iEquipment * cMediator::findEquipmentByName(std::string name)
{
	size_t numEquipments = this->vecEquipments.size();

	for (size_t i = 0; i < numEquipments; i++)
	{
		cMeshObject* mesh = (cMeshObject*) vecEquipments[i]->getMesh();
		if (mesh->friendlyName == name)
		{
			return vecEquipments[i];
		}
	}
	return NULL;
}

void cMediator::sendMessageToEquipment(std::vector<std::string> vecMessage)
{
	size_t numMessages = vecMessage.size();
	if (vecMessage[0] == "hit")
	{
		iEquipment* equipment = this->findEquipmentByName(vecMessage[1]);

		cBall* cueBall = (cBall*)equipment;
		cMeshObject* ballMesh = (cMeshObject*) cueBall->getMesh();
		ballMesh->position.x += 1.0f;
	}
}

void cMediator::sendMessageToEquipment(nlohmann::json message)
{
	iEquipment* equipment = this->findEquipmentByName(message["to"].get<std::string>());

	std::string action = message["action"].get<std::string>();

	if (action == "hit")
	{
		cBall* ball = (cBall*) equipment;
		ball->hit();
	}
}


