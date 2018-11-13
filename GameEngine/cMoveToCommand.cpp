#include "cMoveToCommand.h"

#include <glm/glm.hpp>

cMoveToCommand::cMoveToCommand()
{}


cMoveToCommand::~cMoveToCommand()
{}

void cMoveToCommand::initialize(nlohmann::json values)
{
	this->from.x = values["from"]["x"].get<float>();
	this->from.y = values["from"]["y"].get<float>();
	this->from.z = values["from"]["z"].get<float>();
	this->currentLocation = this->from;

	this->to.x = values["to"]["x"].get<float>();
	this->to.y = values["to"]["y"].get<float>();
	this->to.z = values["to"]["z"].get<float>();

	this->time = values["time"].get<double>();
	this->minDistance = 1.0f;
	this->bIsDone = false;
	this->timeElapsed = 0.0;
	this->direction = this->to - this->from;
}

void cMoveToCommand::update(double deltaTime)
{
	double offset = deltaTime / this->time;
	glm::vec3 stepInDirection = glm::vec3( this->direction.x * offset,
		this->direction.y * offset,
		this->direction.z * offset);
	this->currentLocation += stepInDirection;
	this->timeElapsed += deltaTime;
	float currentDistance = glm::distance(this->to, this->from);

	if (timeElapsed >= this->time)
	{
		// Yes, like Lady Gaga, I have arrived...
		this->bIsDone = true;
		this->currentLocation = this->to;
		return;
	}
}

bool cMoveToCommand::isFinished(void)
{
	return this->bIsDone;
}
