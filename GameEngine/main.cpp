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
#include "cPlayer.h"
#include "cAgent.h"
#include "BehaviourManager.h"
#include "Behaviour.h"
#include "SeekBehaviour.h"
#include "FleeBehaviour.h"
#include "PersueBehaviour.h"
#include "EvadeBehaviour.h"
#include "ApproachBehaviour.h"
#include "WanderAndIdleBehaviour.h"

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

	double lastTime = glfwGetTime();

	cLightsManager* lightsManager = cLightsManager::getInstance();

	cSceneUtils::initializeCamera();
	cSceneUtils::getInstance()->loadModelsIntoScene();

	lightsManager->loadAllLights(program);

	cSceneUtils* sceneUtils = cSceneUtils::getInstance();
	sceneUtils->initializeCamera();
	cCamera* camera = cCamera::getInstance();

	sceneUtils->g_pFBOMain = new cFBO();
	std::string FBOErrorString;
	// This is a 16x9 aspect ratio
	if (sceneUtils->g_pFBOMain->init(1080, 1080, FBOErrorString))
		//	if ( ::g_pFBOMain->init( 256, 256, FBOErrorString ) )
	{
		std::cout << "Framebuffer is good to go!" << std::endl;
	}
	else
	{
		std::cout << "Framebuffer is NOT complete" << std::endl;
	}

	// Point back to default frame buffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	int renderPassNumber = 1;
	// 1 = 1st pass (the actual scene)
	// 2 = 2nd pass (rendering what we drew to the output)
	GLint renderPassNumber_UniLoc = glGetUniformLocation(program, "renderPassNumber");
	std::cout << renderPassNumber_UniLoc << std::endl;

	glm::vec4 waterOffset = glm::vec4(0.0f);
	GLint waterOffset_location = glGetUniformLocation(program, "waterOffset");
	GLint dayMix_location = glGetUniformLocation(program, "dayMix");
	
	cShaderUtils::getInstance()->getUniformVariableLocation(program, "objectColour");

	GLint matView_location = glGetUniformLocation(program, "matView");
	GLint matProj_location = glGetUniformLocation(program, "matProj");

	GLint eyeLocation_location = glGetUniformLocation(program, "eyeLocation");

	GLFWwindow* window = cGLFWUtils::getWindowInstance();

	cPhysics* physics = new cPhysics();

	// AI
	BehaviourManager behaviourManager;

	cPlayer* player = new cPlayer();
	player->mesh = (cMeshObject*)sceneUtils->findObjectByFriendlyName("ship");
	sceneUtils->aiEntities.push_back(player);

	/*cAgent* seekAgent = new cAgent();
	seekAgent->mesh = (cMeshObject*)sceneUtils->findObjectByFriendlyName("boat");
	sceneUtils->aiEntities.push_back(seekAgent);*/

	cAgent* fleeAgent = new cAgent();
	fleeAgent->mesh = (cMeshObject*)sceneUtils->findObjectByFriendlyName("eagle");
	sceneUtils->aiEntities.push_back(fleeAgent);

	cAgent* persueAgent = new cAgent();
	persueAgent->mesh = (cMeshObject*)sceneUtils->findObjectByFriendlyName("bunny");
	sceneUtils->aiEntities.push_back(persueAgent);

	cAgent* evadeAgent = new cAgent();
	evadeAgent->mesh = (cMeshObject*)sceneUtils->findObjectByFriendlyName("donut");
	sceneUtils->aiEntities.push_back(evadeAgent);

	cAgent* approachAgent = new cAgent();
	approachAgent->mesh = (cMeshObject*)sceneUtils->findObjectByFriendlyName("tenker");
	cCannonBall* cannonBall = new cCannonBall();
	cannonBall->mesh = (cMeshObject*)sceneUtils->findObjectByFriendlyName("cueBall5");
	approachAgent->cannonBall = cannonBall;
	sceneUtils->aiEntities.push_back(approachAgent);

	cAgent* wanderAgent = new cAgent();
	wanderAgent->mesh = (cMeshObject*)sceneUtils->findObjectByFriendlyName("box");
	sceneUtils->aiEntities.push_back(wanderAgent);

	for (size_t i = 0; i < 4; i++)
	{
		cCannonBall* cannonBall = new cCannonBall();
		cannonBall->mesh = (cMeshObject*)sceneUtils->findObjectByFriendlyName("cueBall" + std::to_string(i + 1));
		player->cannonBalls.push_back(cannonBall);
	}

	WanderAndIdleBehaviour* wanderBehaviour = new WanderAndIdleBehaviour(wanderAgent);
	//behaviourManager.SetBehaviour(seekAgent, new SeekBehaviour(seekAgent, player));
	behaviourManager.SetBehaviour(fleeAgent, new FleeBehaviour(fleeAgent, player));
	behaviourManager.SetBehaviour(persueAgent, new PersueBehaviour(persueAgent, player));
	behaviourManager.SetBehaviour(evadeAgent, new EvadeBehaviour(evadeAgent, player->cannonBalls[0]));
	behaviourManager.SetBehaviour(approachAgent, new ApproachBehaviour(approachAgent, player));
	behaviourManager.SetBehaviour(wanderAgent, wanderBehaviour);
	// !AI
	while (!glfwWindowShouldClose(window))
	{
		cShaderUtils::getInstance()->useShaderProgram("myShader");
		glBindFramebuffer(GL_FRAMEBUFFER, sceneUtils->g_pFBOMain->ID);

		//**********************************************************
		//     ___ _                _   _          ___ ___  ___  
		//    / __| |___ __ _ _ _  | |_| |_  ___  | __| _ )/ _ \ 
		//   | (__| / -_) _` | '_| |  _| ' \/ -_) | _|| _ \ (_) |
		//    \___|_\___\__,_|_|    \__|_||_\___| |_| |___/\___/ 
		//                                                       		
		// Clear the offscreen frame buffer
//		glViewport( 0, 0, g_FBO_SizeInPixes, g_FBO_SizeInPixes );
//		GLfloat	zero = 0.0f;
//		GLfloat one = 1.0f;
//		glClearBufferfv( GL_COLOR, 0, &zero );
//		glClearBufferfv( GL_DEPTH, 0, &one );

		// Clear colour and depth buffers
		sceneUtils->g_pFBOMain->clearBuffers(true, true);
		//**********************************************************

		glUniform1f(renderPassNumber_UniLoc, 1.0f);	// Tell shader it's the 1st pass

		float ratio;
		int width, height;
		glm::mat4x4 matProjection = glm::mat4(1.0f);
		glm::mat4x4	matView = glm::mat4(1.0f);

		//glfwGetFramebufferSize(window, &width, &height);
		//ratio = width / (float)height;
		//glViewport(0, 0, width, height);

		ratio = 1080 / (float)1080;
		glViewport(0, 0, 1080, 1080);

		glEnable(GL_DEPTH);		// Enables the KEEPING of the depth information
		glEnable(GL_DEPTH_TEST);	// When drawing, checked the existing depth
		glEnable(GL_CULL_FACE);	// Discared "back facing" triangles

		// Colour and depth buffers are TWO DIFF THINGS.
		//glClearColor(0.4f, 0.7f, 1.0f, 1.0f);
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		

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

		/*if (player->mesh->position.x < -120.0f ||
			player->mesh->position.x > 600.0f ||
			player->mesh->position.z < -660.0f ||
			player->mesh->position.z > -330.0f)
		{
			player->GetMesh()->position = glm::vec3(240.0f, 0.0f, -512.0f);
		}

		for (size_t i = 1; i < sceneUtils->aiEntities.size(); i++)
		{
			if (sceneUtils->aiEntities[i]->GetMesh()->position.x < -120.0f ||
				sceneUtils->aiEntities[i]->GetMesh()->position.x > 600.0f ||
				sceneUtils->aiEntities[i]->GetMesh()->position.z < -660.0f ||
				sceneUtils->aiEntities[i]->GetMesh()->position.z > -330.0f)
			{
				sceneUtils->aiEntities[i]->GetMesh()->position.x = cRandomHelper::generateRandomfloatInRange(-120.0f, 600.0f);
				sceneUtils->aiEntities[i]->GetMesh()->position.z = cRandomHelper::generateRandomfloatInRange(-660.0f, -330.0f);
				if (sceneUtils->aiEntities[i]->GetMesh()->friendlyName == "box")
				{
					wanderBehaviour->initialize();
				}
			}
			float distance = glm::distance(player->GetMesh()->position, sceneUtils->aiEntities[i]->GetMesh()->position);
			if (distance < 20.0f)
			{
				player->GetMesh()->position = glm::vec3(240.0f, 0.0f, -512.0f);
			}

			float distanceToCannonBall = glm::distance(player->cannonBalls[0]->GetMesh()->position, sceneUtils->aiEntities[i]->GetMesh()->position);

			if (distanceToCannonBall < 8.0f)
			{
				sceneUtils->aiEntities[i]->GetMesh()->position.x = cRandomHelper::generateRandomfloatInRange(-120.0f, 600.0f);
				sceneUtils->aiEntities[i]->GetMesh()->position.z = cRandomHelper::generateRandomfloatInRange(-660.0f, -330.0f);
				player->cannonBalls[0]->GetMesh()->position = player->GetMesh()->position;
				player->cannonBalls[0]->isShot = false;
				if (sceneUtils->aiEntities[i]->GetMesh()->friendlyName == "box")
				{
					wanderBehaviour->initialize();
				}
			}

			if (sceneUtils->aiEntities[i]->GetMesh()->friendlyName == "tenker")
			{
				cAgent* agent = (cAgent*) sceneUtils->aiEntities[i];
				float distanceToCannonBall = glm::distance(player->GetMesh()->position, agent->cannonBall->GetMesh()->position);
				if (distanceToCannonBall < 8.0f)
				{
					player->GetMesh()->position = glm::vec3(240.0f, 0.0f, -512.0f);
				}
			}
		}*/

		// ****************************************
		// Now the entire scene has been drawn 
		// ****************************************

		// *****************************************
		// 2nd pass
		// *****************************************

		// 1. Set the Framebuffer output to the main framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);		// Points to the "regular" frame buffer

		// Get the size of the actual (screen) frame buffer
		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / (float)height;
		glViewport(0, 0, width, height);

		glEnable(GL_DEPTH);		// Enables the KEEPING of the depth information
		glEnable(GL_DEPTH_TEST);	// When drawing, checked the existing depth
		glEnable(GL_CULL_FACE);	// Discared "back facing" triangles

		// 2. Clear everything **ON THE MAIN FRAME BUFFER** 
		//     (NOT the offscreen buffer)
		// This clears the ACTUAL screen framebuffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		cMeshObject* p2SidedQuad = (cMeshObject*) sceneUtils->findObjectByFriendlyName("2SidedQuad");
		p2SidedQuad->isVisible = true;
		p2SidedQuad->b_HACK_UsesOffscreenFBO = true;
		p2SidedQuad->dontLight = true;
		p2SidedQuad->useVertexColor = false;
		//p2SidedQuad->materialDiffuse = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
		p2SidedQuad->materialDiffuse = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		//p2SidedQuad->bIsWireFrame = true;
		// Rotate it so it's "up and down"
		p2SidedQuad->setOrientationEulerAngles(90.0f, 0.0f, 90.0f, true);
		p2SidedQuad->position.z = 1.0f;

		// Tell the shader this is the 2nd pass...
		// This will run a very simple shader, which
		//  does NOT lighting, and only samples from a single texture
		//  (for now: soon there will be multiple textures)
		glUniform1f(renderPassNumber_UniLoc, 2.0f);	// Tell shader it's the 1st pass

		// Set the view transform so that the camera movement isn't impacted 

		glm::vec3 cameraFullScreenQuad = glm::vec3(0.0, 0.0, -3.0f);

		glUniform3f(eyeLocation_location, cameraFullScreenQuad.x, cameraFullScreenQuad.y, cameraFullScreenQuad.z);
		matView = glm::lookAt(cameraFullScreenQuad,	// Eye
			glm::vec3(0.0f, 0.0f, 0.0f),		// At
			glm::vec3(0.0f, 1.0f, 0.0f));// Up

		glUniformMatrix4fv(matView_location, 1, GL_FALSE, glm::value_ptr(matView));

		// 4. Draw a single quad		
		glm::mat4 matModel = glm::mat4(1.0f);	// identity
		sceneUtils->drawObject(p2SidedQuad, matModel, program);
		p2SidedQuad->isVisible = false;
		/*************************************/

		sceneUtils->drawAABBs(program);
		//std::cout << cAABB::generateId(ship->position, 10.0f) << std::endl;
		double currentTime = glfwGetTime();
		double deltaTime = currentTime - lastTime;
		double MAX_DELTA_TIME = 0.1;	// 100 ms
		if (deltaTime > MAX_DELTA_TIME)
		{
			deltaTime = MAX_DELTA_TIME;
		}

		player->update(deltaTime);
		behaviourManager.Update(deltaTime);
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

