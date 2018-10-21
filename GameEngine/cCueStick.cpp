#include "cCueStick.h"



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
