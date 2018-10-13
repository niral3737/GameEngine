#include "cMeshObject.h"

unsigned int cMeshObject::mNextID = cMeshObject::STARTING_ID_VALUE;

cMeshObject::cMeshObject()
{
	this->position = glm::vec3(0.0f);
	this->postRotation = glm::vec3(0.0f);
	//this->objectColor = glm::vec3(1.0f);
	this->materialDiffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	this->materialSpecular = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	
	this->scale = 1.0f;

	this->isVisible = true;
	this->isWireFrame = false;
	this->useVertexColor = false;
	this->dontLight = false;

	this->isUpdatedByPhysics = true;
	this->velocity = glm::vec3(0.0f);
	this->acceleration = glm::vec3(0.0f);

	this->mUniqueID = cMeshObject::mNextID;
	cMeshObject::mNextID++;

	return;
}

cMeshObject::~cMeshObject()
{}

iMeshObject* cMeshObject::create(void)
{
	return new cMeshObject();
}

void cMeshObject::setDiffuseColour(glm::vec3 newDiffuse)
{
	this->materialDiffuse = glm::vec4(newDiffuse, this->materialDiffuse.a);
	return;
}

void cMeshObject::setAlphaTransparency(float newAlpha)
{
	this->materialDiffuse.a = newAlpha;
	return;
}

void cMeshObject::setSpecularColour(glm::vec3 colourRGB)
{
	this->materialSpecular = glm::vec4(colourRGB, this->materialSpecular.a);
	return;
}

void cMeshObject::setSpecularPower(float specPower)
{
	this->materialSpecular.a = specPower;
	return;
}

float cMeshObject::getSpecularPower()
{
	return this->materialSpecular.a;
}
