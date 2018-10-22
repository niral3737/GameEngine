#include "cCueRack.h"

#include <iostream>


cCueRack::cCueRack()
{}


cCueRack::~cCueRack()
{}

void cCueRack::move()
{
	std::cout << "movec cue rack" << std::endl;
}

void cCueRack::addCue(iEquipment* cue)
{
	this->cues.push_back(cue);
}

iMeshObject * cCueRack::getMesh()
{
	return this->mesh;
}

void cCueRack::setMesh(iMeshObject * mesh)
{
	this->mesh = mesh;
}

void cCueRack::setMediator(iMediatorEquipment * mediator)
{
	this->mediator = mediator;
}
