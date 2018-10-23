#include "cPort.h"

cPort::cPort()
{
	this->treasureGold = NULL;
	this->ship = NULL;
	this->isShipDocked = true;
	this->mesh = NULL;
}


cPort::~cPort()
{}

void cPort::setMediator(iMediatorEntity * mediator)
{
	this->mediator = mediator;
}

cMeshObject * cPort::getMesh()
{
	return (cMeshObject*) this->mesh;
}

void cPort::setMesh(iMeshObject * mesh)
{
	this->mesh = mesh;
}

std::string cPort::getName()
{
	if (this->mesh != NULL)
	{
		return this->getMesh()->friendlyName;
	}
	return "";
}
