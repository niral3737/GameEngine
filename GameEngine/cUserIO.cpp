#include "cUserIO.h"

#include "cSceneUtils.h"

#include <iostream>
#include <fstream>
#include <iomanip>

#include "iMeshObject.h"
#include "cVAOMeshUtils.h"
#include "cMeshObject.h"
#include "cLight.h"
#include "cLightsManager.h"
#include "cJsonUtils.h"
#include "json.hpp"


void cUserIO::key_callback(GLFWwindow * window, int key, int scancode, int action, int mods)
{
	cLightsManager* lightsManager = cLightsManager::getInstance();
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}

	if (glfwGetKey(window, GLFW_KEY_TAB))
	{
		lightsManager->selectNextLight();
		std::cout << lightsManager->selectedLight->friendlyName << " selected" << std::endl;
	}

	if(mIsCtrlDown(window) && lightsManager->selectedLight){
		//on/off
		if (glfwGetKey(window, GLFW_KEY_0))
		{
			lightsManager->selectedLight->param2.x = lightsManager->selectedLight->param2.x == 1.0f ? 0.0f : 1.0f;
		}

		//random color
		if (glfwGetKey(window, GLFW_KEY_9))
		{
			float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
			float g = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
			float b = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
			lightsManager->selectedLight->diffuse = glm::vec4(r, g, b, 1.0f);
		}
	}

	//save light values
	if (glfwGetKey(window, GLFW_KEY_ENTER))
	{
		//save all light values
		std::fstream fs("savefile.json");
		nlohmann::json json = cJsonUtils::getJsonInstance();

		unsigned int index = 0;
		for (std::vector<cLight*>::iterator it = lightsManager->vecLights.begin(); it != lightsManager->vecLights.end(); it++)
		{
			cLight* light = *it;
			json["lights"][index]["position"]["x"] = light->position.x;
			json["lights"][index]["position"]["y"] = light->position.y;
			json["lights"][index]["position"]["z"] = light->position.z;
			json["lights"][index]["position"]["w"] = light->position.w;

			json["lights"][index]["attenuation"]["const"] = light->atten.x;
			json["lights"][index]["attenuation"]["linear"] = light->atten.y;
			json["lights"][index]["attenuation"]["quad"] = light->atten.z;
			json["lights"][index]["attenuation"]["distanceCutOff"] = light->atten.w;

			json["lights"][index]["diffuse"]["r"] = light->diffuse.r;
			json["lights"][index]["diffuse"]["g"] = light->diffuse.b;
			json["lights"][index]["diffuse"]["b"] = light->diffuse.b;
			json["lights"][index]["diffuse"]["a"] = light->diffuse.a;

			json["lights"][index]["param2"]["on"] = light->param2.x;

			index++;
		}

		fs << std::setw(4) << json << std::endl;
		fs.close();
	}
	return;
}

void cUserIO::processAsynKeys(GLFWwindow* window)
{
	const float CAMERA_SPEED_SLOW = 0.03f;
	const float CAMERA_SPEED_FAST = 1.0f;

	float cameraSpeed = CAMERA_SPEED_SLOW;
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		cameraSpeed = CAMERA_SPEED_FAST;
	}

	// If no keys are down, move the camera
	if (mAreAllModifiersUp(window))
	{
		// Note: The "== GLFW_PRESS" isn't really needed as it's actually "1" 
		// (so the if() treats the "1" as true...)

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			cSceneUtils::cameraEye.z += cameraSpeed;
			//axe->position.z += cameraSpeed;
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)	// "backwards"
		{
			cSceneUtils::cameraEye.z -= cameraSpeed;
			//axe->position.z -= cameraSpeed;
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)	// "left"
		{
			cSceneUtils::cameraEye.x += cameraSpeed;
			//axe->position.x -= cameraSpeed;
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)	// "right"
		{
			cSceneUtils::cameraEye.x -= cameraSpeed;
			//axe->position.x += cameraSpeed;
		}
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)	// "up"
		{
			cSceneUtils::cameraEye.y += cameraSpeed;
			//axe->position.y += cameraSpeed;
		}
		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)	// "down"
		{
			cSceneUtils::cameraEye.y -= cameraSpeed;
			//axe->position.y -= cameraSpeed;
		}

	}//if(AreAllModifiersUp(window)

	/*if (mIsAltDown(window))
	{
		cMeshObject* pPlayerShip = (cMeshObject*) cSceneUtils::getInstance()->findObjectByFriendlyName("BlackPearl");
		cSceneUtils::getInstance()->cameraEye = glm::vec3(pPlayerShip->position.x, pPlayerShip->position.y + 20.0f, pPlayerShip->position.z - 130.0f);
		cSceneUtils::getInstance()->cameraAt = glm::vec3(pPlayerShip->position.x, pPlayerShip->position.y, pPlayerShip->position.z);

		if (glfwGetKey(window, GLFW_KEY_W))
		{
			pPlayerShip->acceleration.z = 10.0f;
		}
		if (glfwGetKey(window, GLFW_KEY_S))
		{
			pPlayerShip->acceleration.z = -10.0f;
		}
		if (glfwGetKey(window, GLFW_KEY_A))
		{
			pPlayerShip->acceleration.x = 3.0f;
		}
		if (glfwGetKey(window, GLFW_KEY_D))
		{
			pPlayerShip->acceleration.x = -3.0f;
		}
	}*/

	if (mIsCtrlDown(window))
	{
		cLight* selectedLight = cLightsManager::getInstance()->selectedLight;

		if (!selectedLight)
		{
			return;
		}

		if (glfwGetKey(window, GLFW_KEY_W))
		{
			selectedLight->position.z += 1.0f;
		}
		if (glfwGetKey(window, GLFW_KEY_S))
		{
			selectedLight->position.z -= 1.0f;
		}
		if (glfwGetKey(window, GLFW_KEY_A))
		{
			selectedLight->position.x += 1.0f;
		}
		if (glfwGetKey(window, GLFW_KEY_D))
		{
			selectedLight->position.x -= 1.0f;
		}
		if (glfwGetKey(window, GLFW_KEY_Q))
		{
			selectedLight->position.y += 1.0f;
		}
		if (glfwGetKey(window, GLFW_KEY_E))
		{
			selectedLight->position.y -= 1.0f;
		}
		//atten linear
		if (glfwGetKey(window, GLFW_KEY_EQUAL))
		{
			selectedLight->atten.y += 0.01f;
		}
		if (glfwGetKey(window, GLFW_KEY_MINUS))
		{
			selectedLight->atten.y -= 0.01f;
		}

		//atten quad
		if (glfwGetKey(window, GLFW_KEY_LEFT_BRACKET))
		{
			selectedLight->atten.z -= 0.001f;
		}
		if (glfwGetKey(window, GLFW_KEY_RIGHT_BRACKET))
		{
			selectedLight->atten.z += 0.001f;
		}
	}

}

bool cUserIO::mIsShiftDown(GLFWwindow * window)
{
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT)) { return true; }
	if (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT)) { return true; }
	// both are up
	return false;
}

bool cUserIO::mIsCtrlDown(GLFWwindow * window)
{
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL)) { return true; }
	if (glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL)) { return true; }
	// both are up
	return false;
}

bool cUserIO::mIsAltDown(GLFWwindow * window)
{
	if (glfwGetKey(window, GLFW_KEY_LEFT_ALT)) { return true; }
	if (glfwGetKey(window, GLFW_KEY_RIGHT_ALT)) { return true; }
	// both are up
	return false;
}

bool cUserIO::mAreAllModifiersUp(GLFWwindow * window)
{
	if (mIsShiftDown(window)) { return false; }
	if (mIsCtrlDown(window)) { return false; }
	if (mIsAltDown(window)) { return false; }
	// Yup, they are all UP
	return true;
}
