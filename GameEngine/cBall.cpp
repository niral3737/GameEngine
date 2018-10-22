#include "cBall.h"
#include "cMeshObject.h"
#include <glm/vec3.hpp>

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
	
}

void cBall::hit(void)
{
	cMeshObject* ballMesh = (cMeshObject*) this->mesh;
	ballMesh->position.x += 1.0f;
	float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	float g = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	float b = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	ballMesh->setDiffuseColour(glm::vec3(r, g, b));
}

iMeshObject* cBall::getMesh()
{
	return this->mesh;
}

void cBall::setMesh(iMeshObject * mesh)
{
	this->mesh = mesh;
}

void cBall::setMediator(iMediatorEquipment * mediator)
{
	this->mediator = mediator;
}
