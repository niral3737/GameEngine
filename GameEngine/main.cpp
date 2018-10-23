#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr
#include <iostream>
#include <fstream>

#include "cGLFWUtils.h"
#include "cShaderUtils.h"
#include "cJsonUtils.h"
#include "cVAOMeshUtils.h"
#include "cSceneUtils.h"
#include "iMeshObject.h"
#include "cUserIO.h"
#include "cPhysics.h"
#include "cLightsManager.h"
#include "cLightHelper.h"
#include "cLight.h"
#include "iEquipment.h"
#include "cMediator.h"
#define GLM_ENABLE_EXPERIMENTAL		// To get glm quaternion stuff to compile
#include <glm/gtx/quaternion.hpp>	// Note strange folder

#include "cCamera.h"
#include "cRandomHelper.h"
#include "cPort.h"
#include "cEntityFactory.h"
#include "cGoldCoins.h"

int main(void)
{
	cGLFWUtils::setUpGLFW();
	GLuint program = cShaderUtils::setUpShaders();

	//std::cout << "Load from previously saved file? (Y/y)" << std::endl;
	char answer = 'y';
	//std::cin >> answer;

	if (answer == 'y' || answer == 'Y')
	{
		cVAOMeshUtils::loadFromSaveFile = true;
		cLightsManager::loadFromSaveFile = true;
		cSceneUtils::loadFromSaveFile = true;
	}

	cVAOMeshUtils::getInstance()->loadModels(program);

	double lastTime = glfwGetTime();

	cLightsManager* lightsManager = cLightsManager::getInstance();

	cSceneUtils::initializeCamera();
	cSceneUtils::getInstance()->loadModelsIntoScene();
	lightsManager->loadAllLights(program);

	cCamera* camera = cCamera::getInstance();
	cSceneUtils* sceneUtils = cSceneUtils::getInstance();

	cMediator* mediator = new cMediator();
	cEntityFactory* entityFactory = new cEntityFactory(mediator);
	
	const size_t NUMBER_OF_PORTS = 20;
	std::vector<cPort*> vecPorts;
	
	for (size_t i = 0; i < NUMBER_OF_PORTS; i++)
	{
		cPort* port = (cPort*) entityFactory->createEntity("port");
		mediator->LoadEntity(port);
		port->setMesh(sceneUtils->findObjectByFriendlyName("port" + std::to_string(i)));

		port->ship = (cShip*) entityFactory->createEntity("normalShip");
		port->ship->ownPortName = port->getName();
		port->ship->setMesh(sceneUtils->findObjectByFriendlyName("ship" + std::to_string(i)));
		port->ship->getMesh()->setDiffuseColour(glm::vec3(0.0f, 1.0f, 0.0f));
		mediator->LoadEntity(port->ship);
		port->treasureGold = (cTreasure*) entityFactory->createEntity("goldCoins");

		vecPorts.push_back(port);
	}

	//randomly distribute 2000 gold coins
	int goldCoins = 2000;
	while (goldCoins > 0)
	{
		unsigned int coins = cRandomHelper::generateRandomIntInRange(2, 10);
		unsigned int portIndex = cRandomHelper::generateRandomIntInRange(0, NUMBER_OF_PORTS - 1);

		if (goldCoins - coins < 0)
		{
			vecPorts[portIndex]->treasureGold->addTreasure(goldCoins);
		}else{
			vecPorts[portIndex]->treasureGold->addTreasure(coins);
		}
		goldCoins -= coins;
	}

	for (size_t i = 0; i < NUMBER_OF_PORTS; i++)
	{
		std::cout << i << ": " << vecPorts[i]->treasureGold->numberOfTreasure << std::endl;
	}

	cShaderUtils::getInstance()->getUniformVariableLocation(program, "objectColour");

	GLint matView_location = glGetUniformLocation(program, "matView");
	GLint matProj_location = glGetUniformLocation(program, "matProj");

	GLint eyeLocation_location = glGetUniformLocation(program, "eyeLocation");

	GLFWwindow* window = cGLFWUtils::getWindowInstance();

	cLightHelper* pLightHelper = new cLightHelper();
	const float SPEED_PER_TICK = 4.0f;

	for (size_t i = 0; i < vecPorts.size(); i++)
	{
		vecPorts[i]->isShipDocked = true;
		cShip* ship = vecPorts[i]->ship;
		ship->portToGoIndex = cRandomHelper::generateRandomIntInRange(0, NUMBER_OF_PORTS - 1);
		ship->timeToWait = SPEED_PER_TICK * cRandomHelper::generateRandomIntInRange(100, 200);
		
		//initial state is waiting
		ship->eShipState = WAITING;
	}

	while (!glfwWindowShouldClose(window))
	{
		float ratio;
		int width, height;
		glm::mat4x4 matProjection = glm::mat4(1.0f);
		glm::mat4x4	matView = glm::mat4(1.0f);


		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / (float)height;
		glViewport(0, 0, width, height);


		glEnable(GL_DEPTH);		// Enables the KEEPING of the depth information
		glEnable(GL_DEPTH_TEST);	// When drawing, checked the existing depth
		glEnable(GL_CULL_FACE);	// Discared "back facing" triangles

		// Colour and depth buffers are TWO DIFF THINGS.
		//glClearColor(0.4f, 0.7f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		

		//mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
		matProjection = glm::perspective(0.6f,			// FOV
			ratio,		// Aspect ratio
			0.1f,			// Near clipping plane
			10000.0f);	// Far clipping plane
		matView = glm::lookAt(cCamera::getInstance()->eye,	// Eye
			cCamera::getInstance()->getAtInWorldSpace(),		// At
			cCamera::getInstance()->getUpVector());// Up

		glUniform3f(eyeLocation_location, cCamera::getInstance()->eye.x, cCamera::getInstance()->eye.y, cCamera::getInstance()->eye.z);

		glUniformMatrix4fv(matView_location, 1, GL_FALSE, glm::value_ptr(matView));
		glUniformMatrix4fv(matProj_location, 1, GL_FALSE, glm::value_ptr(matProjection));

		lightsManager->copyLightValuesToShader();

		for (size_t i = 0; i < vecPorts.size(); i++)
		{
			cShip* ship = vecPorts[i]->ship;

			if (ship->eShipState == DEAD)
			{
				continue;
			}
			
			if (ship->eShipState == WAITING && ship->timeWaited < ship->timeToWait)
			{
				ship->timeWaited += SPEED_PER_TICK;
				continue;
			}
			else
			{
				ship->eShipState = GOING;
				ship->timeToWait = SPEED_PER_TICK * cRandomHelper::generateRandomIntInRange(100, 200);
				ship->timeWaited = 0.0f;
			}

			unsigned int waitingTimeInDistance = cRandomHelper::generateRandomIntInRange(100, 200);
			

			float deltaX = vecPorts[ship->portToGoIndex]->getMesh()->position.x - ship->getMesh()->position.x;
			float deltaZ = vecPorts[ship->portToGoIndex]->getMesh()->position.z - ship->getMesh()->position.z;
			float distance = glm::distance(vecPorts[ship->portToGoIndex]->getMesh()->position
				, ship->getMesh()->position);

			if (distance > SPEED_PER_TICK)
			{
				float ratio = SPEED_PER_TICK / distance;
				ship->getMesh()->position.x += (ratio * deltaX);
				ship->getMesh()->position.z += (ratio * deltaZ);
			}
			else
			{
				//reached the desired port
				ship->getMesh()->position.x = vecPorts[ship->portToGoIndex]->getMesh()->position.x;
				ship->getMesh()->position.z = vecPorts[ship->portToGoIndex]->getMesh()->position.z;

				if(ship->portToGoIndex != i)
				{
					ship->eShipState = ATTACKING;
				}
				else
				{
					ship->eShipState = WAITING;
				}
				
				if (ship->eShipState == ATTACKING)
				{
					//attacking logic
					if (vecPorts[ship->portToGoIndex]->isShipDocked)
					{
						//ship is there
						ship->attackPortWithShip(vecPorts[ship->portToGoIndex]->getName());
					}
					else
					{
						//no ship at the port
						ship->attackEmptyPort(vecPorts[ship->portToGoIndex]->getName());
					}
					if (vecPorts[i]->treasureGold->numberOfTreasure <= 0)
					{
						//no gold on port, must die
						//ship->getMesh()->setDiffuseColour(glm::vec3(1.0f, 0.0f, 0.0f));
						vecPorts[i]->getMesh()->setDiffuseColour(glm::vec3(1.0f, 0.0f, 0.0f));
						vecPorts[i]->ship->getMesh()->setDiffuseColour(glm::vec3(1.0f, 0.0f, 0.0f));
						vecPorts[i]->ship->eShipState = DEAD;

						continue;
					}
					
					//go home
					ship->eShipState = GOING;
					ship->portToGoIndex = i;
				}

				//TODO do something about float comparision use distance maybe
				//if (ship->portToGoIndex == i && ship->getMesh()->position == vecPorts[i]->getMesh()->position)
				if(ship->eShipState == WAITING)
				{
					//reached home
					ship->eShipState = WAITING;
					vecPorts[i]->isShipDocked = true;
					//spend gold waiting
					ship->spendGoldWaiting();
					if (vecPorts[i]->treasureGold->numberOfTreasure <= 0)
					{
						//no gold on port, must die
						//ship->getMesh()->setDiffuseColour(glm::vec3(1.0f, 0.0f, 0.0f));
						vecPorts[i]->getMesh()->setDiffuseColour(glm::vec3(1.0f, 0.0f, 0.0f));
						vecPorts[i]->ship->getMesh()->setDiffuseColour(glm::vec3(1.0f, 0.0f, 0.0f));
						vecPorts[i]->ship->eShipState = DEAD;

						continue;
					}
					
					//find a port that's not dead
					size_t portToGoIndex = cRandomHelper::generateRandomIntInRange(0, NUMBER_OF_PORTS - 1);
					while (portToGoIndex == i || vecPorts[portToGoIndex]->ship->eShipState == DEAD)
					{
						portToGoIndex = cRandomHelper::generateRandomIntInRange(0, NUMBER_OF_PORTS - 1);
					}
					ship->portToGoIndex = portToGoIndex;

					//ship->timeToWait = SPEED_PER_TICK * cRandomHelper::generateRandomIntInRange(100, 200);
				}


			}

		}

		for (unsigned int objIndex = 0;
			objIndex != (unsigned int)cSceneUtils::getInstance()->vecObjectsToDraw.size();
			objIndex++)
		{
			iMeshObject* pCurrentMesh = cSceneUtils::getInstance()->vecObjectsToDraw[objIndex];

			glm::mat4x4 matModel = glm::mat4(1.0f);			// mat4x4 m, p, mvp;

			cSceneUtils::getInstance()->drawObject(pCurrentMesh, matModel, program);

		}//for ( unsigned int objIndex = 0; 

		double currentTime = glfwGetTime();
		double deltaTime = currentTime - lastTime;

		//cModelDrawInfo modelDrawInfo;
		//modelDrawInfo.meshFileName = "terrain_xyz_n.ply";
		//cVAOMeshUtils::getInstance()->findDrawInfoByModelName(modelDrawInfo);

		//physics->PhysicsUpdate(deltaTime, modelDrawInfo);
		//physics->addProjectileAim(deltaTime);

		lastTime = currentTime;

		/*cSceneUtils::getInstance()->drawEquipment(cueRack, program);
		for (size_t i = 0; i < ((cCueRack*)cueRack)->cues.size(); i++)
		{
			cSceneUtils::getInstance()->drawEquipment(((cCueRack*)cueRack)->cues[i], program);
		}*/

		glfwSwapBuffers(window);		// Shows what we drew

		glfwPollEvents();
		/*double currentTime = glfwGetTime();
		double deltaTime = currentTime - lastTime;
		lastTime = currentTime;*/

		cUserIO::processAsynKeys(window);
		cUserIO::processAsynMouse(window);
	}//while (!glfwWindowShouldClose(window))

	//soundManager->shutdownFmod();
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);

	return 0;
}

