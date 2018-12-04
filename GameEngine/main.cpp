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
#include "cCommandGroup.h"
#include "cMoveToCommand.h"

int main(void)
{
	cGLFWUtils::setUpGLFW();
	GLuint program = cShaderUtils::setUpShaders();
	cCamera* camera = cCamera::getInstance();
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

	cMeshObject* stadium = (cMeshObject*) cSceneUtils::getInstance()->findObjectByFriendlyName("stadium");
	
	sTextureInfo wood;
	wood.name = "wood.bmp";
	wood.strength = 0.5f;
	stadium->vecTextures.push_back(wood);

	sTextureInfo trump;
	trump.name = "trump.bmp";
	trump.strength = 0.5f;
	stadium->vecTextures.push_back(trump);

	cMeshObject* terrain = (cMeshObject*)cSceneUtils::getInstance()->findObjectByFriendlyName("terrain");

	sTextureInfo water;
	water.name = "water.bmp";
	water.strength = 1.0f;
	terrain->vecTextures.push_back(water);

	cMeshObject* ship = (cMeshObject*)cSceneUtils::getInstance()->findObjectByFriendlyName("ship");
	ship->setAlphaTransparency(0.5f);

	sTextureInfo fabric2;
	fabric2.name = "fabric2.bmp";
	fabric2.strength = 1.0f;
	ship->vecTextures.push_back(fabric2);
	
	lightsManager->loadAllLights(program);

	//*****************************
	cModelDrawInfo terrainMeshInfo;
	terrainMeshInfo.meshFileName = "terrain_xyz_n_uv.ply";
	cVAOMeshUtils::getInstance()->findDrawInfoByModelName(terrainMeshInfo);
	cSceneUtils::getInstance()->terrainHierarchy->loadTerrainAABB(terrainMeshInfo);
	//*****************************

	camera->eye.x = 50.0f;
	camera->eye.y = 0.0f;
	camera->eye.z = 50.0f;


	cSceneUtils* sceneUtils = cSceneUtils::getInstance();

	cShaderUtils::getInstance()->getUniformVariableLocation(program, "objectColour");

	GLint matView_location = glGetUniformLocation(program, "matView");
	GLint matProj_location = glGetUniformLocation(program, "matProj");

	GLint eyeLocation_location = glGetUniformLocation(program, "eyeLocation");

	GLFWwindow* window = cGLFWUtils::getWindowInstance();

	cLightHelper* pLightHelper = new cLightHelper();

	cPhysics* physics = new cPhysics();

	/*******************Move to Command*****************************/

	cCommandGroup sceneCommandGroup;

	cCommandGroup* pFollowCG = new cCommandGroup();

	cMoveToCommand* pMove = new cMoveToCommand();

	nlohmann::json values;

	values["from"]["x"] = ship->position.x;
	values["from"]["y"] = ship->position.y;
	values["from"]["z"] = ship->position.z;

	values["to"]["x"] = 255.0f;
	values["to"]["y"] = 0.0f;
	values["to"]["z"] = 255.0f;

	//time in seconds
	values["time"] = 30.0;

	pMove->initialize(values);
	pFollowCG->vecCommands.push_back(pMove);
	sceneCommandGroup.vecCommandGroups.push_back(pFollowCG);

	/*****************************************************/
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

		//std::cout << cAABB::generateId(ship->position, 10.0f) << std::endl;

		double currentTime = glfwGetTime();
		double deltaTime = currentTime - lastTime;
		double MAX_DELTA_TIME = 0.1;	// 100 ms
		if (deltaTime > MAX_DELTA_TIME)
		{
			deltaTime = MAX_DELTA_TIME;
		}

		//sceneCommandGroup.Update(deltaTime);
		//ship->position = pMove->currentLocation;
		//std::cout << ship->position.x <<" " << ship->position.z << std::endl;
		//cModelDrawInfo modelDrawInfo;
		//modelDrawInfo.meshFileName = "terrain_xyz_n.ply";
		//cVAOMeshUtils::getInstance()->findDrawInfoByModelName(modelDrawInfo);

		physics->PhysicsUpdate(deltaTime);
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

