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

int main(void)
{

	cGLFWUtils::setUpGLFW();
	GLuint program = cShaderUtils::setUpShaders();

	cVAOMeshUtils::getInstance()->loadModels(program);

	cSceneUtils::initializeCamera();

	cSceneUtils::getInstance()->loadModelsIntoScene();
	cMeshObject* lightSphere1 = cSceneUtils::getInstance()->loadMeshInfoByFriendlyName("sphere");
	lightSphere1->isVisible = true;
	cMeshObject* lightSphere2 = cSceneUtils::getInstance()->loadMeshInfoByFriendlyName("sphere");
	lightSphere2->isVisible = true;
	cMeshObject* lightSphere3 = cSceneUtils::getInstance()->loadMeshInfoByFriendlyName("sphere");
	lightSphere3->isVisible = true;
	cMeshObject* lightSphere4 = cSceneUtils::getInstance()->loadMeshInfoByFriendlyName("sphere");
	lightSphere4->isVisible = true;

	double lastTime = glfwGetTime();

	cLightsManager* lightsManager = cLightsManager::getInstance();

	lightsManager->loadAllLights(program);

	cShaderUtils::getInstance()->getUniformVariableLocation(program, "objectColour");

	GLint matView_location = glGetUniformLocation(program, "matView");
	GLint matProj_location = glGetUniformLocation(program, "matProj");

	GLFWwindow* window = cGLFWUtils::getWindowInstance();
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
		lightSphere1->position = lightsManager->getLightByFriendlyName("light1")->position;
		lightSphere2->position = lightsManager->getLightByFriendlyName("light2")->position;
		lightSphere3->position = lightsManager->getLightByFriendlyName("light3")->position;
		lightSphere4->position = lightsManager->getLightByFriendlyName("light4")->position;

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

