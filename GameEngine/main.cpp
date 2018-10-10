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

int main(void)
{

	cGLFWUtils::setUpGLFW();
	GLuint program = cShaderUtils::setUpShaders();

	cVAOMeshUtils::getInstance()->loadModels(program);

	cSceneUtils::initializeCamera();

	cSceneUtils::getInstance()->loadModelsIntoScene();

	double lastTime = glfwGetTime();

	cLightsManager* lightsManager = cLightsManager::getInstance();

	std::cout << "Load lights from previously saved file? (Y/y)" << std::endl;
	char answer;
	std::cin >> answer;

	if (answer == 'y' || answer == 'Y')
	{
		lightsManager->loadAllLightsFromSaveFile(program);
	}
	else
	{
		lightsManager->loadAllLights(program);
	}

	cShaderUtils::getInstance()->getUniformVariableLocation(program, "objectColour");

	GLint matView_location = glGetUniformLocation(program, "matView");
	GLint matProj_location = glGetUniformLocation(program, "matProj");

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
			1000.0f);	// Far clipping plane
		matView = glm::lookAt(cSceneUtils::cameraEye,	// Eye
			cSceneUtils::cameraAt,		// At
			glm::vec3(0.0f, 1.0f, 0.0f));// Up

		glUniformMatrix4fv(matView_location, 1, GL_FALSE, glm::value_ptr(matView));
		glUniformMatrix4fv(matProj_location, 1, GL_FALSE, glm::value_ptr(matProjection));

		lightsManager->copyLightValuesToShader();

		cMeshObject* attenSphere = (cMeshObject*)cSceneUtils::getInstance()->findObjectByFriendlyName("sphere");
		attenSphere->isVisible = true;
		attenSphere->dontLight = true;

		glm::mat4 matBall(1.0f);

		const float ACCURACY_OF_DISTANCE = 0.01f;
		const float INFINITE_DISTANCE = 10000.0f;

		for (std::vector<cLight*>::iterator it = lightsManager->vecLights.begin(); it != lightsManager->vecLights.end(); it++)
		{
			cLight* light = *it;

			if (!light->useDebugSphere)
				continue;

			attenSphere->position = light->position;


			attenSphere->objectColor = glm::vec3(1.0f, 1.0f, 0.0f);
			float distance90Percent = pLightHelper->calcApproxDistFromAtten(0.90f, ACCURACY_OF_DISTANCE,
				INFINITE_DISTANCE, light->atten.x, light->atten.y, light->atten.z);
			attenSphere->scale = distance90Percent;
			cSceneUtils::getInstance()->drawObject(attenSphere, matBall, program);

			attenSphere->objectColor = glm::vec3(0.0f, 1.0f, 0.0f);	// 50% brightness
			float distance50Percent =
				pLightHelper->calcApproxDistFromAtten(0.50f, ACCURACY_OF_DISTANCE,
					INFINITE_DISTANCE,
					light->atten.x,
					light->atten.y,
					light->atten.z);
			attenSphere->scale = distance50Percent;
			cSceneUtils::getInstance()->drawObject(attenSphere, matBall, program);

			attenSphere->objectColor = glm::vec3(1.0f, 0.0f, 0.0f);	// 25% brightness
			float distance25Percent =
				pLightHelper->calcApproxDistFromAtten(0.25f, ACCURACY_OF_DISTANCE,
					INFINITE_DISTANCE,
					light->atten.x,
					light->atten.y,
					light->atten.z);
			attenSphere->scale = distance25Percent;
			cSceneUtils::getInstance()->drawObject(attenSphere, matBall, program);

			attenSphere->objectColor = glm::vec3(0.0f, 0.0f, 1.0f);	// 1% brightness
			float distance1Percent =
				pLightHelper->calcApproxDistFromAtten(0.01f, ACCURACY_OF_DISTANCE,
					INFINITE_DISTANCE,
					light->atten.x,
					light->atten.y,
					light->atten.z);
			attenSphere->scale = distance1Percent;
			cSceneUtils::getInstance()->drawObject(attenSphere, matBall, program);
		}
		attenSphere->isVisible = false;
		/*lightSphere1->position = lightsManager->getLightByFriendlyName("light1")->position;
		lightSphere2->position = lightsManager->getLightByFriendlyName("light2")->position;
		lightSphere3->position = lightsManager->getLightByFriendlyName("light3")->position;
		lightSphere4->position = lightsManager->getLightByFriendlyName("light4")->position;*/

		// Draw all the objects in the "scene"
		for (unsigned int objIndex = 0;
			objIndex != (unsigned int) cSceneUtils::getInstance()->vecObjectsToDraw.size();
			objIndex++)
		{
			iMeshObject* pCurrentMesh = cSceneUtils::getInstance()->vecObjectsToDraw[objIndex];

			glm::mat4x4 matModel = glm::mat4(1.0f);			// mat4x4 m, p, mvp;

			cSceneUtils::getInstance()->drawObject(pCurrentMesh, matModel, program);

		}//for ( unsigned int objIndex = 0; 
		
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

