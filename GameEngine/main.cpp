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
#include "cEquipmentFactory.h"
#include "cEquipmentBuilder.h"
#include "cMediator.h"
#include "cCueStick.h"
#include "cCueRack.h"
#define GLM_ENABLE_EXPERIMENTAL		// To get glm quaternion stuff to compile
#include <glm/gtx/quaternion.hpp>	// Note strange folder

#include "cCamera.h"
#include "cPhysics.h"

int main(void)
{
	cGLFWUtils::setUpGLFW();
	GLuint program = cShaderUtils::setUpShaders();

	/*glm::vec3 eularAngles = glm::vec3(0.06f, 0.8f, 0.2f);
	std::cout << "eular before: " << eularAngles.x << " " << eularAngles.y << " " << eularAngles.z << std::endl;
	glm::quat qOrientation = glm::quat(eularAngles);
	std::cout << "quat : " << qOrientation.x << " " << qOrientation.y << " " << qOrientation.z << " " << qOrientation.w << std::endl;
	glm::vec3 eularAfter = glm::eulerAngles(qOrientation);
	std::cout << "eular after: " << eularAfter.x << " " << eularAfter.y << " " << eularAfter.z << std::endl;*/

	std::cout << "Load from previously saved file? (Y/y)" << std::endl;
	char answer;
	std::cin >> answer;

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

	cMediator* mediator = new cMediator();

	cEquipmentFactory* equipmentFactory = new cEquipmentFactory(mediator);

	//iEquipment* ball = equipmentFactory->createEquipment(1);
	//mediator->LoadEquipment(ball);
	//cMeshObject* ballMesh = (cMeshObject*) cSceneUtils::getInstance()->findObjectByFriendlyName("cueBall");
	//ball->setMesh(ballMesh);

	//cSceneUtils::getInstance()->vecEquipmentsToDraw.push_back(ball);
	cCamera* camera = cCamera::getInstance();

	iEquipment* cueStick = equipmentFactory->createEquipment(2);
	mediator->LoadEquipment(cueStick);
	cMeshObject* cueMesh = (cMeshObject*)cSceneUtils::getInstance()->findObjectByFriendlyName("terrain");
	cueMesh->setDiffuseColour(glm::vec3(1.0f, 1.0f, 0.0f));
	cueMesh->isWireFrame = false;
	cueStick->setMesh(cueMesh);

	cSceneUtils::getInstance()->vecEquipmentsToDraw.push_back(cueStick);

	iEquipment* cueStick1 = equipmentFactory->createEquipment(2);
	mediator->LoadEquipment(cueStick1);
	cMeshObject* cueMesh1 = (cMeshObject*)cSceneUtils::getInstance()->findObjectByFriendlyName("ring");
	cueMesh1->setDiffuseColour(glm::vec3(1.0f, 1.0f, 0.0f));
	cueMesh1->isWireFrame = false;
	cueMesh1->scale = 0.01f;
	cueStick1->setMesh(cueMesh1);

	cSceneUtils::getInstance()->vecEquipmentsToDraw.push_back(cueStick1);

	iEquipment* cueStick2 = equipmentFactory->createEquipment(2);
	mediator->LoadEquipment(cueStick2);
	cMeshObject* cueMesh2 = (cMeshObject*)cSceneUtils::getInstance()->findObjectByFriendlyName("cueBall");
	cueMesh2->setDiffuseColour(glm::vec3(1.0f, 1.0f, 0.0f));
	cueMesh2->isWireFrame = false;
	cueStick2->setMesh(cueMesh2);

	cSceneUtils::getInstance()->vecEquipmentsToDraw.push_back(cueStick2);
	cMeshObject* cueMesh3 = (cMeshObject*)cSceneUtils::getInstance()->findObjectByFriendlyName("box");
	for (size_t i = 0; i < 30; i++)
	{
		/*iEquipment* cueStick3 = equipmentFactory->createEquipment(2);
		mediator->LoadEquipment(cueStick3);*/
		cMeshObject* newObeject = cueMesh3->cloneObject();
		float x = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		float z = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		newObeject->position = glm::vec3(x * 300.0f, -20.0f, z * 300.0f);
		newObeject->setDiffuseColour(glm::vec3(1.0f, 0.0f, 0.0f));
		newObeject->isWireFrame = false;
		newObeject->isUpdatedByPhysics = true;
		newObeject->velocity = glm::vec3(0.0f, 0.0f, 0.0f);
		//cueStick3->setMesh(cueMesh3);

		cSceneUtils::getInstance()->vecObjectsToDraw.push_back(newObeject);
	}

	


	/*
	iEquipment* cueRack = equipmentFactory->createEquipment(3);
	mediator->LoadEquipment(cueRack);
	cMeshObject* cueRackMesh = (cMeshObject*)cSceneUtils::getInstance()->findObjectByFriendlyName("cueRack");
	cueRack->setMesh(cueRackMesh);

	cEquipmentBuilder* builder = new cEquipmentBuilder();
	builder->buildEquipment(3, cueRack);

	for (size_t i = 0; i < ((cCueRack*)cueRack)->cues.size(); i++)
	{
		((cCueRack*)cueRack)->cues[i]->setMesh((cMeshObject*)cSceneUtils::getInstance()->findObjectByFriendlyName("cue1"));
	}*/
	cPhysics* physics = new cPhysics();
	cShaderUtils::getInstance()->getUniformVariableLocation(program, "objectColour");

	GLint matView_location = glGetUniformLocation(program, "matView");
	GLint matProj_location = glGetUniformLocation(program, "matProj");

	GLint eyeLocation_location = glGetUniformLocation(program, "eyeLocation");

	GLFWwindow* window = cGLFWUtils::getWindowInstance();

	cLightHelper* pLightHelper = new cLightHelper();
	while (!glfwWindowShouldClose(window))
	{
		//soundManager->printInfo();
		float ratio;
		int width, height;
		//glm::mat4x4 mvp;		
		// Now these are split into separate parts
		// (because we need to know whe
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

		//for (size_t i = 0; i < cSceneUtils::getInstance()->vecEquipmentsToDraw.size(); i++)
		//{
		//	iEquipment* equipment = cSceneUtils::getInstance()->vecEquipmentsToDraw[i];

		//	cSceneUtils::getInstance()->drawEquipment(equipment, program);
		//}

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

		cModelDrawInfo modelDrawInfo;
		modelDrawInfo.meshFileName = "terrain_xyz_n.ply";
		cVAOMeshUtils::getInstance()->findDrawInfoByModelName(modelDrawInfo);

		physics->PhysicsUpdate(deltaTime, modelDrawInfo);
		physics->addProjectileAim(deltaTime);

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

