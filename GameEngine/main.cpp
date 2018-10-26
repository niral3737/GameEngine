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
//#include "iEquipment.h"
#include "cMediator.h"
#define GLM_ENABLE_EXPERIMENTAL		// To get glm quaternion stuff to compile
#include <glm/gtx/quaternion.hpp>	// Note strange folder

#include "cCamera.h"
#include "cRandomHelper.h"
//#include "cPort.h"
#include "cEntityFactory.h"
//#include "cGoldCoins.h"
#include "cBase.h"
#include "cIsland.h"
#include "cCannonBall.h" 

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
	/*camera->orientation.x = 0.507363f;
	camera->orientation.y = -0.0132092f;
	camera->orientation.z = -0.0100001f;
	camera->orientation.w = 0.861575f;

	camera->eye.x = 54.492;
	camera->eye.y = 738.26;
	camera->eye.z = -458.177;*/

	cSceneUtils* sceneUtils = cSceneUtils::getInstance();

	float const INITIAL_Y_VELOCITY = 10.0f;
	float const GRAVITY = -5.0f;
	cMediator* mediator = new cMediator();
	cEntityFactory* entityFactory = new cEntityFactory(mediator);
	//meshfile island3 has port 0 to 19
	cIsland* island1 = (cIsland*) entityFactory->createEntity("island");
	island1->setMesh(sceneUtils->findObjectByFriendlyName("island3"));
	island1->highCannnonBall = (cCannonBall*)entityFactory->createEntity("cannonBall");
	island1->lowCannnonBall = (cCannonBall*)entityFactory->createEntity("cannonBall");
	island1->highCannnonBall->setMesh(sceneUtils->findObjectByFriendlyName("cannonBall0"));
	island1->lowCannnonBall->setMesh(sceneUtils->findObjectByFriendlyName("cannonBall1"));
	island1->highCannnonBall->getMesh()->radius = 2.0f;
	island1->highCannnonBall->getMesh()->acceleration = glm::vec3(0.0f, GRAVITY, 0.0f);
	island1->highCannnonBall->getMesh()->velocity.y = INITIAL_Y_VELOCITY;
	island1->lowCannnonBall->getMesh()->radius = 2.0f;
	island1->lowCannnonBall->getMesh()->acceleration = glm::vec3(0.0f, GRAVITY, 0.0f);
	island1->lowCannnonBall->getMesh()->velocity.y = INITIAL_Y_VELOCITY;

	//meshfile island4 has port 20 to 39
	cIsland* island2 = (cIsland*)entityFactory->createEntity("island");
	island2->setMesh(sceneUtils->findObjectByFriendlyName("island4"));
	island2->highCannnonBall = (cCannonBall*)entityFactory->createEntity("cannonBall");
	island2->lowCannnonBall = (cCannonBall*)entityFactory->createEntity("cannonBall");
	island2->highCannnonBall->setMesh(sceneUtils->findObjectByFriendlyName("cannonBall2"));
	island2->lowCannnonBall->setMesh(sceneUtils->findObjectByFriendlyName("cannonBall3"));
	island2->highCannnonBall->getMesh()->radius = 2.0f;
	island2->highCannnonBall->getMesh()->acceleration = glm::vec3(0.0f, GRAVITY, 0.0f);
	island2->highCannnonBall->getMesh()->velocity.y = INITIAL_Y_VELOCITY;
	island2->lowCannnonBall->getMesh()->radius = 2.0f;
	island2->lowCannnonBall->getMesh()->acceleration = glm::vec3(0.0f, GRAVITY, 0.0f);
	island2->lowCannnonBall->getMesh()->velocity.y = INITIAL_Y_VELOCITY;

	//meshfile island1 has port 40 to 59
	cIsland* island3 = (cIsland*)entityFactory->createEntity("island");
	island3->setMesh(sceneUtils->findObjectByFriendlyName("island1"));
	island3->highCannnonBall = (cCannonBall*)entityFactory->createEntity("cannonBall");
	island3->lowCannnonBall = (cCannonBall*)entityFactory->createEntity("cannonBall");
	island3->highCannnonBall->setMesh(sceneUtils->findObjectByFriendlyName("cannonBall4"));
	island3->lowCannnonBall->setMesh(sceneUtils->findObjectByFriendlyName("cannonBall5"));
	island3->highCannnonBall->getMesh()->radius = 2.0f;
	island3->highCannnonBall->getMesh()->acceleration = glm::vec3(0.0f, GRAVITY, 0.0f);
	island3->highCannnonBall->getMesh()->velocity.y = INITIAL_Y_VELOCITY;
	island3->lowCannnonBall->getMesh()->radius = 2.0f;
	island3->lowCannnonBall->getMesh()->acceleration = glm::vec3(0.0f, GRAVITY, 0.0f);
	island3->lowCannnonBall->getMesh()->velocity.y = INITIAL_Y_VELOCITY;
	
	//meshfile island2 has port 60 to 79
	cIsland* island4 = (cIsland*)entityFactory->createEntity("island");
	island4->setMesh(sceneUtils->findObjectByFriendlyName("island2"));
	island4->highCannnonBall = (cCannonBall*)entityFactory->createEntity("cannonBall");
	island4->lowCannnonBall = (cCannonBall*)entityFactory->createEntity("cannonBall");
	island4->highCannnonBall->setMesh(sceneUtils->findObjectByFriendlyName("cannonBall6"));
	island4->lowCannnonBall->setMesh(sceneUtils->findObjectByFriendlyName("cannonBall7"));
	island4->highCannnonBall->getMesh()->radius = 2.0f;
	island4->highCannnonBall->getMesh()->acceleration = glm::vec3(0.0f, GRAVITY, 0.0f);
	island4->highCannnonBall->getMesh()->velocity.y = INITIAL_Y_VELOCITY;
	island4->lowCannnonBall->getMesh()->radius = 2.0f;
	island4->lowCannnonBall->getMesh()->acceleration = glm::vec3(0.0f, GRAVITY, 0.0f);
	island4->lowCannnonBall->getMesh()->velocity.y = INITIAL_Y_VELOCITY;
	
	glm::vec3 baseColor = glm::vec3(0.0f, 0.0f, 0.0f);

	{
		cBase* highBase1 = (cBase*) entityFactory->createEntity("base");
		highBase1->setMesh(sceneUtils->findObjectByFriendlyName("base0"));
		highBase1->getMesh()->setDiffuseColour(baseColor);
		highBase1->getMesh()->radius = 8.0f;
		island1->bases.push_back(highBase1);

		cBase* highBase2 = (cBase*)entityFactory->createEntity("base");
		highBase2->setMesh(sceneUtils->findObjectByFriendlyName("base1"));
		highBase2->getMesh()->setDiffuseColour(baseColor);
		highBase2->getMesh()->radius = 8.0f;
		island1->bases.push_back(highBase2);

		cBase* highBase3 = (cBase*)entityFactory->createEntity("base");
		highBase3->setMesh(sceneUtils->findObjectByFriendlyName("base2"));
		highBase3->getMesh()->setDiffuseColour(baseColor);
		highBase3->getMesh()->radius = 8.0f;
		island1->bases.push_back(highBase3);
	}

	{
		cBase* highBase1 = (cBase*)entityFactory->createEntity("base");
		highBase1->setMesh(sceneUtils->findObjectByFriendlyName("base20"));
		highBase1->getMesh()->setDiffuseColour(baseColor);
		highBase1->getMesh()->radius = 8.0f;
		island2->bases.push_back(highBase1);

		cBase* highBase2 = (cBase*)entityFactory->createEntity("base");
		highBase2->setMesh(sceneUtils->findObjectByFriendlyName("base21"));
		highBase2->getMesh()->setDiffuseColour(baseColor);
		highBase2->getMesh()->radius = 8.0f;
		island2->bases.push_back(highBase2);

		cBase* highBase3 = (cBase*)entityFactory->createEntity("base");
		highBase3->setMesh(sceneUtils->findObjectByFriendlyName("base22"));
		highBase3->getMesh()->setDiffuseColour(baseColor);
		highBase3->getMesh()->radius = 8.0f;
		island2->bases.push_back(highBase3);
	}

	{
		cBase* highBase1 = (cBase*)entityFactory->createEntity("base");
		highBase1->setMesh(sceneUtils->findObjectByFriendlyName("base40"));
		highBase1->getMesh()->setDiffuseColour(baseColor);
		highBase1->getMesh()->radius = 8.0f;
		island3->bases.push_back(highBase1);

		cBase* highBase2 = (cBase*)entityFactory->createEntity("base");
		highBase2->setMesh(sceneUtils->findObjectByFriendlyName("base41"));
		highBase2->getMesh()->setDiffuseColour(baseColor);
		highBase2->getMesh()->radius = 8.0f;
		island3->bases.push_back(highBase2);

		cBase* highBase3 = (cBase*)entityFactory->createEntity("base");
		highBase3->setMesh(sceneUtils->findObjectByFriendlyName("base42"));
		highBase3->getMesh()->setDiffuseColour(baseColor);
		highBase3->getMesh()->radius = 8.0f;
		island3->bases.push_back(highBase3);
	}

	{
		cBase* highBase1 = (cBase*)entityFactory->createEntity("base");
		highBase1->setMesh(sceneUtils->findObjectByFriendlyName("base60"));
		highBase1->getMesh()->setDiffuseColour(baseColor);
		highBase1->getMesh()->radius = 8.0f;
		island4->bases.push_back(highBase1);

		cBase* highBase2 = (cBase*)entityFactory->createEntity("base");
		highBase2->setMesh(sceneUtils->findObjectByFriendlyName("base61"));
		highBase2->getMesh()->setDiffuseColour(baseColor);
		highBase2->getMesh()->radius = 8.0f;
		island4->bases.push_back(highBase2);

		cBase* highBase3 = (cBase*)entityFactory->createEntity("base");
		highBase3->setMesh(sceneUtils->findObjectByFriendlyName("base62"));
		highBase3->getMesh()->setDiffuseColour(baseColor);
		highBase3->getMesh()->radius = 8.0f;
		island4->bases.push_back(highBase3);
	}


	const size_t NUMBER_OF_SMALL_BASES = 17;
	//17 bases for island 3
	cModelDrawInfo modelDrawInfo;
	modelDrawInfo.meshFileName = "island3.ply";
	cVAOMeshUtils::getInstance()->findDrawInfoByModelName(modelDrawInfo);

	size_t offset = 3;
	for (size_t i = offset; i < NUMBER_OF_SMALL_BASES + offset; i++)
	{
		float extentY3Percent = modelDrawInfo.extent.y * 0.03f;
		float extentY5Percent = modelDrawInfo.extent.y * 0.05f;

		unsigned int randomVerticeIndex = cRandomHelper::generateRandomIntInRange(0, modelDrawInfo.numberOfVertices - 1);
		
		while (modelDrawInfo.pVerticesFromFile[randomVerticeIndex].y < extentY3Percent || modelDrawInfo.pVerticesFromFile[randomVerticeIndex].y > extentY5Percent)
		{
			randomVerticeIndex = cRandomHelper::generateRandomIntInRange(0, modelDrawInfo.numberOfVertices - 1);
		}
		cMeshObject* baseMesh = (cMeshObject*)sceneUtils->findObjectByFriendlyName("base" + std::to_string(i));
		baseMesh->position.x = modelDrawInfo.pVerticesFromFile[randomVerticeIndex].x;
		baseMesh->position.y = modelDrawInfo.pVerticesFromFile[randomVerticeIndex].y;
		baseMesh->position.z = modelDrawInfo.pVerticesFromFile[randomVerticeIndex].z;
		baseMesh->radius = 8.0f;
		baseMesh->setDiffuseColour(baseColor);

		cBase* base = (cBase*) entityFactory->createEntity("base");
		base->setMesh(baseMesh);
		island1->bases.push_back(base);
	}

	modelDrawInfo.meshFileName = "island4.ply";
	cVAOMeshUtils::getInstance()->findDrawInfoByModelName(modelDrawInfo);

	offset = 23;
	for (size_t i = offset; i < NUMBER_OF_SMALL_BASES + offset; i++)
	{
		float extentY3Percent = modelDrawInfo.extent.y * 0.03f;
		float extentY5Percent = modelDrawInfo.extent.y * 0.05f;

		unsigned int randomVerticeIndex = cRandomHelper::generateRandomIntInRange(0, modelDrawInfo.numberOfVertices - 1);

		while (modelDrawInfo.pVerticesFromFile[randomVerticeIndex].y < extentY3Percent || modelDrawInfo.pVerticesFromFile[randomVerticeIndex].y > extentY5Percent)
		{
			randomVerticeIndex = cRandomHelper::generateRandomIntInRange(0, modelDrawInfo.numberOfVertices - 1);
		}
		cMeshObject* baseMesh = (cMeshObject*)sceneUtils->findObjectByFriendlyName("base" + std::to_string(i));
		baseMesh->position.x = modelDrawInfo.pVerticesFromFile[randomVerticeIndex].x;
		baseMesh->position.y = modelDrawInfo.pVerticesFromFile[randomVerticeIndex].y;
		baseMesh->position.z = modelDrawInfo.pVerticesFromFile[randomVerticeIndex].z;
		baseMesh->setDiffuseColour(baseColor);

		cBase* base = (cBase*)entityFactory->createEntity("base");
		base->setMesh(baseMesh);
		island2->bases.push_back(base);
	}

	modelDrawInfo.meshFileName = "island1.ply";
	cVAOMeshUtils::getInstance()->findDrawInfoByModelName(modelDrawInfo);

	offset = 43;
	for (size_t i = offset; i < NUMBER_OF_SMALL_BASES + offset; i++)
	{
		float extentY3Percent = modelDrawInfo.extent.y * 0.03f;
		float extentY5Percent = modelDrawInfo.extent.y * 0.05f;

		unsigned int randomVerticeIndex = cRandomHelper::generateRandomIntInRange(0, modelDrawInfo.numberOfVertices - 1);

		while (modelDrawInfo.pVerticesFromFile[randomVerticeIndex].y < extentY3Percent || modelDrawInfo.pVerticesFromFile[randomVerticeIndex].y > extentY5Percent)
		{
			randomVerticeIndex = cRandomHelper::generateRandomIntInRange(0, modelDrawInfo.numberOfVertices - 1);
		}
		cMeshObject* baseMesh = (cMeshObject*)sceneUtils->findObjectByFriendlyName("base" + std::to_string(i));
		baseMesh->position.x = modelDrawInfo.pVerticesFromFile[randomVerticeIndex].x;
		baseMesh->position.y = modelDrawInfo.pVerticesFromFile[randomVerticeIndex].y;
		baseMesh->position.z = modelDrawInfo.pVerticesFromFile[randomVerticeIndex].z;
		baseMesh->setDiffuseColour(baseColor);

		cBase* base = (cBase*)entityFactory->createEntity("base");
		base->setMesh(baseMesh);
		island3->bases.push_back(base);
	}

	modelDrawInfo.meshFileName = "island2.ply";
	cVAOMeshUtils::getInstance()->findDrawInfoByModelName(modelDrawInfo);

	offset = 63;
	for (size_t i = offset; i < NUMBER_OF_SMALL_BASES + offset; i++)
	{
		float extentY3Percent = modelDrawInfo.extent.y * 0.03f;
		float extentY5Percent = modelDrawInfo.extent.y * 0.05f;

		unsigned int randomVerticeIndex = cRandomHelper::generateRandomIntInRange(0, modelDrawInfo.numberOfVertices - 1);

		while (modelDrawInfo.pVerticesFromFile[randomVerticeIndex].y < extentY3Percent || modelDrawInfo.pVerticesFromFile[randomVerticeIndex].y > extentY5Percent)
		{
			randomVerticeIndex = cRandomHelper::generateRandomIntInRange(0, modelDrawInfo.numberOfVertices - 1);
		}
		cMeshObject* baseMesh = (cMeshObject*)sceneUtils->findObjectByFriendlyName("base" + std::to_string(i));
		baseMesh->position.x = modelDrawInfo.pVerticesFromFile[randomVerticeIndex].x;
		baseMesh->position.y = modelDrawInfo.pVerticesFromFile[randomVerticeIndex].y;
		baseMesh->position.z = modelDrawInfo.pVerticesFromFile[randomVerticeIndex].z;
		baseMesh->setDiffuseColour(baseColor);

		cBase* base = (cBase*)entityFactory->createEntity("base");
		base->setMesh(baseMesh);
		island4->bases.push_back(base);
	}

	island1->highCannnonBall->getMesh()->position = island1->bases[0]->getMesh()->position;
	island1->highCannnonBall->shootingBaseIndex = 0;
	island2->highCannnonBall->getMesh()->position = island2->bases[0]->getMesh()->position;
	island2->highCannnonBall->shootingBaseIndex = 0;
	island3->highCannnonBall->getMesh()->position = island3->bases[0]->getMesh()->position;
	island3->highCannnonBall->shootingBaseIndex = 0;
	island4->highCannnonBall->getMesh()->position = island4->bases[0]->getMesh()->position;
	island4->highCannnonBall->shootingBaseIndex = 0;

	island1->lowCannnonBall->getMesh()->position = island1->bases[3]->getMesh()->position;
	island1->lowCannnonBall->shootingBaseIndex = 3;
	island2->lowCannnonBall->getMesh()->position = island2->bases[3]->getMesh()->position;
	island2->lowCannnonBall->shootingBaseIndex = 3;
	island3->lowCannnonBall->getMesh()->position = island3->bases[3]->getMesh()->position;
	island3->lowCannnonBall->shootingBaseIndex = 3;
	island4->lowCannnonBall->getMesh()->position = island4->bases[3]->getMesh()->position;
	island4->lowCannnonBall->shootingBaseIndex = 3;

	sceneUtils->vecIslands.push_back(island1);
	sceneUtils->vecIslands.push_back(island2);
	sceneUtils->vecIslands.push_back(island3);
	sceneUtils->vecIslands.push_back(island4);

	//pick a randomBase to destroy
	for (size_t i = 0; i < sceneUtils->vecIslands.size(); i++)
	{
		unsigned int randomIslandIndex = cRandomHelper::generateRandomIntInRange(0, 3);
		while (randomIslandIndex == i)
		{
			randomIslandIndex = cRandomHelper::generateRandomIntInRange(0, 3);
		}

		unsigned int randomBaseOnThatIslandIndex = cRandomHelper::generateRandomIntInRange(0, 19);

		while (sceneUtils->vecIslands[randomIslandIndex]->bases[randomBaseOnThatIslandIndex]->isDestroyed)
		{
			randomBaseOnThatIslandIndex = cRandomHelper::generateRandomIntInRange(0, 19);
		}
		sceneUtils->vecIslands[i]->randomBaseToDestroy = sceneUtils->vecIslands[randomIslandIndex]->bases[randomBaseOnThatIslandIndex];
		sceneUtils->vecIslands[i]->islandOnWhichRandomBaseIs = sceneUtils->vecIslands[randomIslandIndex];

		//setting x-z direction
		sceneUtils->vecIslands[i]->highCannnonBall->getMesh()->velocity.x = (sceneUtils->vecIslands[i]->randomBaseToDestroy->getMesh()->position.x
			- sceneUtils->vecIslands[i]->highCannnonBall->getMesh()->position.x) * 0.5f;
		sceneUtils->vecIslands[i]->highCannnonBall->getMesh()->velocity.z = (sceneUtils->vecIslands[i]->randomBaseToDestroy->getMesh()->position.z
			- sceneUtils->vecIslands[i]->highCannnonBall->getMesh()->position.z) * 0.5f;

		sceneUtils->vecIslands[i]->highCannnonBall->lastInitialVelocity =  sceneUtils->vecIslands[i]->highCannnonBall->getMesh()->velocity;

		sceneUtils->vecIslands[i]->lowCannnonBall->getMesh()->velocity.x = sceneUtils->vecIslands[i]->randomBaseToDestroy->getMesh()->position.x
			- sceneUtils->vecIslands[i]->lowCannnonBall->getMesh()->position.x;
		sceneUtils->vecIslands[i]->lowCannnonBall->getMesh()->velocity.y *= 1.0f;
		sceneUtils->vecIslands[i]->lowCannnonBall->getMesh()->velocity.z = sceneUtils->vecIslands[i]->randomBaseToDestroy->getMesh()->position.z
			- sceneUtils->vecIslands[i]->lowCannnonBall->getMesh()->position.z;

		sceneUtils->vecIslands[i]->lowCannnonBall->lastInitialVelocity = sceneUtils->vecIslands[i]->lowCannnonBall->getMesh()->velocity;

	}

	cShaderUtils::getInstance()->getUniformVariableLocation(program, "objectColour");

	GLint matView_location = glGetUniformLocation(program, "matView");
	GLint matProj_location = glGetUniformLocation(program, "matProj");

	GLint eyeLocation_location = glGetUniformLocation(program, "eyeLocation");

	GLFWwindow* window = cGLFWUtils::getWindowInstance();

	cLightHelper* pLightHelper = new cLightHelper();

	cPhysics* physics = new cPhysics();

	while (!glfwWindowShouldClose(window))
	{
		/*std::cout << "x: " << camera->orientation.x << std::endl;
		std::cout << camera->orientation.y << std::endl;
		std::cout << camera->orientation.z << std::endl;
		std::cout << camera->orientation.w << std::endl;

		std::cout << "eye: " << camera->eye.x << std::endl;
		std::cout << camera->eye.y << std::endl;
		std::cout << camera->eye.z << std::endl;*/

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

		physics->PhysicsUpdate(deltaTime);
		//physics->addProjectileAim(deltaTime);

		lastTime = currentTime;

		size_t islandIndex;
		unsigned int counter = 0;
		for (size_t i = 0; i < sceneUtils->vecIslands.size(); i++)
		{
			cIsland* island = sceneUtils->vecIslands[i];

			if (island->isHighDestroyed && island->isLowDestroyed)
			{
				island->lost = true;
			}
		}

		for (size_t i = 0; i < sceneUtils->vecIslands.size(); i++)
		{
			cIsland* island = sceneUtils->vecIslands[i];
			if (!island->lost)
			{
				counter++;
				islandIndex = i;
			}
		}

		//make'em yellow
		if (counter == 1)
		{
			cIsland* island = sceneUtils->vecIslands[islandIndex];
			for (size_t i = 0; i < 20; i++)
			{
				island->bases[i]->getMesh()->setDiffuseColour(glm::vec3(1.0f, 1.0f, 0.0f));
			}
		}

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

