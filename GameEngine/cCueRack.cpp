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
