#include "cLight.h"



cLight::cLight()
{}


cLight::~cLight()
{}

void cLight::setLinearAttenuation(float newLinAtten)
{
	this->atten.y = newLinAtten;
}

void cLight::setConstAttenuation(float newConstAtten)
{
	this->atten.x = newConstAtten;
}

void cLight::setQuadAttenuation(float newQuadAtten)
{
	this->atten.z = newQuadAtten;
}

void cLight::setDistanceCutOff(float distanceCutOff)
{
	this->atten.w = distanceCutOff;
}

void cLight::setDiffuse(glm::vec3 diffuse)
{
	this->diffuse = glm::vec4(diffuse, 1.0f);
}

void cLight::setPosition(glm::vec3 newPosition)
{
	this->position = glm::vec4(newPosition, 0.0f);
}

void cLight::setSpecularHighlight(glm::vec3 rgbColour, float power)
{
	this->specular = glm::vec4(rgbColour, power);
}

void cLight::setOn(bool on)
{
	this->param2.x = on ? 1.0f : 0.0f;
}

bool cLight::getOn()
{
	return this->param2.x > 0.0f;
}
