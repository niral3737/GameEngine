#define _SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING
#define _CRT_SECURE_NO_WARNINGS

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr
#include <iostream>
#include <fstream>
#include <sstream>

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
#include "cCommandGroup.h"
#include "cMoveToCommand.h"
#include "cLuaBrain.h"

//#include <gtest/gtest.h>

//GLint get_fectorial(GLint input_number)
//{
//	GLint factorial = 1;
//
//	if (input_number < 0)
//	{
//		factorial = -1;
//	}
//	else if (input_number == 0)
//	{
//		factorial = 1;
//	}
//	else if (input_number > 0)
//	{
//		for (GLint i = 1; i <= input_number; i++)
//		{
//			factorial *= i;
//		}
//	}
//	return factorial;
//}

////test cases
//TEST(FactorialTest, Negative)
//{
//	EXPECT_EQ(-1, get_fectorial(-1));
//}

int main(int argc, char** argv)
{
	/*::testing::InitGoogleTest(&argc, argv);
	RUN_ALL_TESTS();*/

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

	/*******************AABBs*********************/
	cModelDrawInfo terrainMeshInfo;
	terrainMeshInfo.meshFileName = "islands.ply";
	cVAOMeshUtils::getInstance()->findDrawInfoByModelName(terrainMeshInfo);
	cSceneUtils::getInstance()->terrainHierarchy->loadTerrainAABB(terrainMeshInfo);
	/*********************************************/
	double lastTime = glfwGetTime();

	cLightsManager* lightsManager = cLightsManager::getInstance();

	cSceneUtils::initializeCamera();
	cSceneUtils::getInstance()->loadModelsIntoScene();

	lightsManager->loadAllLights(program);

	cSceneUtils* sceneUtils = cSceneUtils::getInstance();
	sceneUtils->initializeCamera();
	cCamera* camera = cCamera::getInstance();
	glm::vec4 waterOffset = glm::vec4(0.0f);
	GLint waterOffset_location = glGetUniformLocation(program, "waterOffset");
	GLint dayMix_location = glGetUniformLocation(program, "dayMix");
	
	cShaderUtils::getInstance()->getUniformVariableLocation(program, "objectColour");

	GLint matView_location = glGetUniformLocation(program, "matView");
	GLint matProj_location = glGetUniformLocation(program, "matProj");

	GLint eyeLocation_location = glGetUniformLocation(program, "eyeLocation");

	GLFWwindow* window = cGLFWUtils::getWindowInstance();

	cPhysics* physics = new cPhysics();

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

		glUniform1f(dayMix_location, sceneUtils->dayMix);

		glUniformMatrix4fv(matView_location, 1, GL_FALSE, glm::value_ptr(matView));
		glUniformMatrix4fv(matProj_location, 1, GL_FALSE, glm::value_ptr(matProjection));

		lightsManager->copyLightValuesToShader();
		lightsManager->drawAttenuationSpheres(program);

		lightsManager->dimLights(sceneUtils->dayMix);
		lightsManager->flickerLight("light3");
		lightsManager->flickerLight("light5");
		//jet->applyTransformationsToCollisionPoints();
		//jet->drawCollisionPoints(program);
		cSceneUtils::getInstance()->drawSkyBox(camera->eye, program);

		cSceneUtils::getInstance()->vecTrasparentObjects.clear();
		for (unsigned int objIndex = 0;
			objIndex != (unsigned int)cSceneUtils::getInstance()->vecObjectsToDraw.size();
			objIndex++)
		{
			cMeshObject* pCurrentMesh = (cMeshObject*) cSceneUtils::getInstance()->vecObjectsToDraw[objIndex];

			//sorting logic
			if (pCurrentMesh->materialDiffuse.a < 1.0f)
			{
				if (cSceneUtils::getInstance()->vecTrasparentObjects.empty())
				{
					cSceneUtils::getInstance()->vecTrasparentObjects.push_back(pCurrentMesh);
					continue;
				}
				size_t numTransObj = cSceneUtils::getInstance()->vecTrasparentObjects.size();
				for (size_t i = 0; i < numTransObj; i++)
				{
					cMeshObject* obj = cSceneUtils::getInstance()->vecTrasparentObjects[i];
					float distanceObj = glm::distance(obj->position, camera->eye);
					float distanceCurrentMesh = glm::distance(pCurrentMesh->position, camera->eye);
					if (distanceObj < distanceCurrentMesh )
					{
						cSceneUtils::getInstance()->vecTrasparentObjects.insert(cSceneUtils::getInstance()->vecTrasparentObjects.begin() + i, pCurrentMesh);
						break;
					}
					else if (i == numTransObj - 1)
					{
						cSceneUtils::getInstance()->vecTrasparentObjects.push_back(pCurrentMesh);
						break;
					}
				}
				continue;
			}

			glm::mat4x4 matModel = glm::mat4(1.0f);			// mat4x4 m, p, mvp;

			cSceneUtils::getInstance()->drawObject(pCurrentMesh, matModel, program);
		}//for ( unsigned int objIndex = 0; 

		//drawing trasparent objects
		for (size_t i = 0; i < cSceneUtils::getInstance()->vecTrasparentObjects.size(); i++)
		{
			glm::mat4x4 matModel = glm::mat4(1.0f);			// mat4x4 m, p, mvp;
			cSceneUtils::getInstance()->drawObject(cSceneUtils::getInstance()->vecTrasparentObjects[i], matModel, program);
		}

		sceneUtils->drawAABBs(program);
		//std::cout << cAABB::generateId(ship->position, 10.0f) << std::endl;
		double currentTime = glfwGetTime();
		double deltaTime = currentTime - lastTime;
		double MAX_DELTA_TIME = 0.1;	// 100 ms
		if (deltaTime > MAX_DELTA_TIME)
		{
			deltaTime = MAX_DELTA_TIME;
		}
		//sceneUtils->sceneCommandGroup.Update(deltaTime);

		//std::cout << ship->position.x <<" " << ship->position.z << std::endl;
		//cModelDrawInfo modelDrawInfo;
		//modelDrawInfo.meshFileName = "terrain_xyz_n.ply";
		//cVAOMeshUtils::getInstance()->findDrawInfoByModelName(modelDrawInfo);

		physics->PhysicsUpdate(deltaTime, program);
		//physics->addProjectileAim(deltaTime);
		waterOffset.x += (0.1f * deltaTime);
		waterOffset.y += (0.017f * deltaTime);
		waterOffset.z -= (0.13f * deltaTime);
		waterOffset.w -= (0.013f * deltaTime);

		glUniform4f(waterOffset_location, waterOffset.x, waterOffset.y, waterOffset.z, waterOffset.w);

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

