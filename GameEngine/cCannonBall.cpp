#include "cCannonBall.h"



cCannonBall::cCannonBall()
{
	this->isHit = false;
	this->lastInitialVelocity = glm::vec3(0.0f);
}


cCannonBall::~cCannonBall()
{}

cMeshObject* cCannonBall::getMesh()
{
	return (cMeshObject*)mesh;
}

void cCannonBall::setMesh(iMeshObject * mesh)
{
	this->mesh = mesh;

}

void cCannonBall::increaseShootingBaseIndex(size_t start, size_t end)
{
	if (shootingBaseIndex >= end)
	{
		shootingBaseIndex = start;
	}
	else
	{
		shootingBaseIndex++;
	}
}

std::string cCannonBall::getName()
{
	if (this->mesh != NULL)
	{
		return this->getMesh()->friendlyName;
	}
	return "";
}