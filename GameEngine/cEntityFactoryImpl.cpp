#include "cEntityFactoryImpl.h"

#include "cPort.h"
#include "cShip.h"
#include "cNormalShip.h"
#include "cSuperShip.h"
#include "cGoldCoins.h"
#include "cGemStones.h"

cEntityFactoryImpl::cEntityFactoryImpl()
{}


cEntityFactoryImpl::~cEntityFactoryImpl()
{}

bool cEntityFactoryImpl::isEntityTypeValid(std::string entityType)
{
	return entityType == "port" 
		|| entityType == "normalShip" 
		|| entityType == "superShip"
		|| entityType == "goldCoins" 
		|| entityType == "gemStones" ;
}

iEntity * cEntityFactoryImpl::createEntity(std::string entityType)
{
	iEntity* entity = NULL;

	if (!isEntityTypeValid(entityType))
	{
		return entity;
	}

	if (entityType == "port")
	{
		cPort* port = new cPort();
		port->setMediator(mediator);
		entity = port;
	}
	else if (entityType == "normalShip")
	{
		cShip* ship = new cNormalShip();
		ship->setMediator(mediator);
		entity = ship;
	}
	else if (entityType == "superShip")
	{
		cShip* ship = new cSuperShip();
		ship->setMediator(mediator);
		entity = ship;
	}
	else if (entityType == "goldCoins")
	{
		cTreasure* goldCoins = new cGoldCoins();
		//ship->setMediator(mediator);
		entity = goldCoins;
	}
	else if (entityType == "gemStones")
	{
		cTreasure* gemStones = new cGemStones();
		//ship->setMediator(mediator);
		entity = gemStones;
	}

	return entity;
}

void cEntityFactoryImpl::setMediator(iMediatorEntity * mediator)
{
	this->mediator = mediator;
}
