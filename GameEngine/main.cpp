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

int main(void)
{
	cGLFWUtils::setUpGLFW();
	GLuint program = cShaderUtils::setUpShaders();

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

	iEquipment* ball = equipmentFactory->createEquipment(1);
	mediator->LoadEquipment(ball);
	cMeshObject* ballMesh = (cMeshObject*) cSceneUtils::getInstance()->findObjectByFriendlyName("cueBall");
	ball->setMesh(ballMesh);

	cSceneUtils::getInstance()->vecEquipmentsToDraw.push_back(ball);

	iEquipment* cueStick = equipmentFactory->createEquipment(2);
	mediator->LoadEquipment(cueStick);
	cMeshObject* cueMesh = (cMeshObject*)cSceneUtils::getInstance()->findObjectByFriendlyName("cue0");
	cueStick->setMesh(cueMesh);

	cSceneUtils::getInstance()->vecEquipmentsToDraw.push_back(cueStick);

	iEquipment* cueRack = equipmentFactory->createEquipment(3);
	mediator->LoadEquipment(cueRack);
	cMeshObject* cueRackMesh = (cMeshObject*)cSceneUtils::getInstance()->findObjectByFriendlyName("cueRack");
	cueRack->setMesh(cueRackMesh);

	cEquipmentBuilder* builder = new cEquipmentBuilder();
	builder->buildEquipment(3, cueRack);

	for (size_t i = 0; i < ((cCueRack*)cueRack)->cues.size(); i++)
	{
		((cCueRack*)cueRack)->cues[i]->setMesh((cMeshObject*)cSceneUtils::getInstance()->findObjectByFriendlyName("cue1"));
	}

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
		matView = glm::lookAt(cSceneUtils::cameraEye,	// Eye
			cSceneUtils::cameraAt,		// At
			glm::vec3(0.0f, 1.0f, 0.0f));// Up

		glUniform3f(eyeLocation_location, cSceneUtils::cameraEye.x, cSceneUtils::cameraEye.y, cSceneUtils::cameraEye.z);

		glUniformMatrix4fv(matView_location, 1, GL_FALSE, glm::value_ptr(matView));
		glUniformMatrix4fv(matProj_location, 1, GL_FALSE, glm::value_ptr(matProjection));

		lightsManager->copyLightValuesToShader();

		for (size_t i = 0; i < cSceneUtils::getInstance()->vecEquipmentsToDraw.size(); i++)
		{
			iEquipment* equipment = cSceneUtils::getInstance()->vecEquipmentsToDraw[i];

			cSceneUtils::getInstance()->drawEquipment(equipment, program);
		}

		cSceneUtils::getInstance()->drawEquipment(cueRack, program);
		for (size_t i = 0; i < ((cCueRack*)cueRack)->cues.size(); i++)
		{
			cSceneUtils::getInstance()->drawEquipment(((cCueRack*)cueRack)->cues[i], program);
		}

		//for (unsigned int objIndex = 0;
		//	objIndex != (unsigned int) cSceneUtils::getInstance()->vecObjectsToDraw.size();
		//	objIndex++)
		//{
		//	iMeshObject* pCurrentMesh = cSceneUtils::getInstance()->vecObjectsToDraw[objIndex];

		//	glm::mat4x4 matModel = glm::mat4(1.0f);			// mat4x4 m, p, mvp;

		//	cSceneUtils::getInstance()->drawObject(pCurrentMesh, matModel, program);

		//}//for ( unsigned int objIndex = 0; 
		
		glfwSwapBuffers(window);		// Shows what we drew

		glfwPollEvents();
		double currentTime = glfwGetTime();
		double deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		/*cModelDrawInfo modelDrawInfo;
		modelDrawInfo.meshFileName = "terrain_xyz_n.ply";
		cVAOMeshUtils::getInstance()->findDrawInfoByModelName(modelDrawInfo);

		PhysicsUpdate(deltaTime, modelDrawInfo);*/
		cUserIO::processAsynKeys(window);
	}//while (!glfwWindowShouldClose(window))

	//soundManager->shutdownFmod();
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);

	return 0;
}

