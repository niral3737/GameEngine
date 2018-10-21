#include "cBall.h"



cBall::cBall()
{
	this->ballNumber = 0;
	this->color = glm::vec3(0.0f); //make it black by default
	this->isSolid = true;          //solid by default
}


cBall::~cBall()
{}

void cBall::move(void)
{
	std::cout << "change position" << std::endl;
}

iMeshObject* cBall::getMesh()
{
	return this->mesh;
}
