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

eSelectionMode cUserIO::selectionMode = eSelectionMode::MESH_SELECTION;
bool cUserIO::includeInvisibleObjects = false;

void cUserIO::key_callback(GLFWwindow * window, int key, int scancode, int action, int mods)
{
	cSceneUtils* sceneUtils = cSceneUtils::getInstance();
	cLightsManager* lightManager = cLightsManager::getInstance();

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}

	if (glfwGetKey(window, GLFW_KEY_I) && action == GLFW_PRESS)
	{
		includeInvisibleObjects = true;
	}

	if (key == GLFW_KEY_TAB && action == GLFW_PRESS)
	{
		if(selectionMode == eSelectionMode::MESH_SELECTION){
			sceneUtils->selectNextMeshObject(includeInvisibleObjects);
		}
		else if (selectionMode == eSelectionMode::LIGHT_SELECTION)
		{
			lightManager->selectNextLight();
		}
	}

	if (key == GLFW_KEY_M && action == GLFW_PRESS)
	{
		selectionMode = eSelectionMode::MESH_SELECTION;
		std::cout << "Model selection mode" << std::endl;
	}

	if (key == GLFW_KEY_L && action == GLFW_PRESS)
	{
		selectionMode = eSelectionMode::LIGHT_SELECTION;
		std::cout << "Light selection mode" << std::endl;
	}

	if (selectionMode == MESH_SELECTION && sceneUtils->selectedMeshObject)
	{
		cMeshObject* selectedObject = (cMeshObject*) sceneUtils->selectedMeshObject;
		if (glfwGetKey(window, GLFW_KEY_0) && action == GLFW_PRESS)
		{
			selectedObject->isWireFrame = !selectedObject->isWireFrame;
		}
		if (glfwGetKey(window, GLFW_KEY_BACKSPACE) && action == GLFW_PRESS)
		{
			selectedObject->isVisible = !selectedObject->isVisible;
		}

	}
	else if(selectionMode == LIGHT_SELECTION && lightManager->selectedLight){

		cLight* selectedLight = lightManager->selectedLight;

		if (glfwGetKey(window, GLFW_KEY_0) && action == GLFW_PRESS)
		{
			selectedLight->useDebugSphere = !selectedLight->useDebugSphere;
		}
		if (glfwGetKey(window, GLFW_KEY_BACKSPACE) && action == GLFW_PRESS)
		{
			selectedLight->setOn(!selectedLight->getOn());
		}
	}
	//}

	//save settings
	if (glfwGetKey(window, GLFW_KEY_ENTER))
	{
		mSaveSettings();
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
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)	// "backwards"
		{
			cSceneUtils::cameraEye.z -= cameraSpeed;
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)	// "left"
		{
			cSceneUtils::cameraEye.x += cameraSpeed;
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)	// "right"
		{
			cSceneUtils::cameraEye.x -= cameraSpeed;
		}
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)	// "up"
		{
			cSceneUtils::cameraEye.y += cameraSpeed;
		}
		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)	// "down"
		{
			cSceneUtils::cameraEye.y -= cameraSpeed;
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
	if (mIsAltDown(window))
	{
		cMeshObject* selectedMeshObject = (cMeshObject*)cSceneUtils::getInstance()->selectedMeshObject;
		cLight* selectedLight = cLightsManager::getInstance()->selectedLight;
		if (selectionMode == MESH_SELECTION && selectedMeshObject)
		{
			if (glfwGetKey(window, GLFW_KEY_W))
			{
				selectedMeshObject->postRotation.x += glm::radians(1.0f);
			}
			if (glfwGetKey(window, GLFW_KEY_S))
			{
				selectedMeshObject->postRotation.x -= glm::radians(1.0f);
			}
			if (glfwGetKey(window, GLFW_KEY_A))
			{
				selectedMeshObject->postRotation.z -= glm::radians(1.0f);
			}
			if (glfwGetKey(window, GLFW_KEY_D))
			{
				selectedMeshObject->postRotation.z += glm::radians(1.0f);
			}
			if (glfwGetKey(window, GLFW_KEY_Q))
			{
				selectedMeshObject->postRotation.y += glm::radians(1.0f);
			}
			if (glfwGetKey(window, GLFW_KEY_E))
			{
				selectedMeshObject->postRotation.y -= glm::radians(1.0f);
			}
		}
	}

	if (mIsCtrlDown(window))
	{
		cMeshObject* selectedMeshObject = (cMeshObject*)cSceneUtils::getInstance()->selectedMeshObject;
		cLight* selectedLight = cLightsManager::getInstance()->selectedLight;
		
		if (selectionMode == MESH_SELECTION && (cMeshObject*)cSceneUtils::getInstance()->selectedMeshObject)
		{
			if (glfwGetKey(window, GLFW_KEY_W))
			{
				((cMeshObject*)cSceneUtils::getInstance()->selectedMeshObject)->position.z += 0.5f;
			}
			if (glfwGetKey(window, GLFW_KEY_S))
			{
				selectedMeshObject->position.z -= 0.5f;
			}
			if (glfwGetKey(window, GLFW_KEY_A))
			{
				selectedMeshObject->position.x += 0.5f;
			}
			if (glfwGetKey(window, GLFW_KEY_D))
			{
				selectedMeshObject->position.x -= 0.5f;
			}
			if (glfwGetKey(window, GLFW_KEY_Q))
			{
				selectedMeshObject->position.y += 0.5f;
			}
			if (glfwGetKey(window, GLFW_KEY_E))
			{
				selectedMeshObject->position.y -= 0.5f;
			}
			if (glfwGetKey(window, GLFW_KEY_EQUAL))
			{
				selectedMeshObject->setSpecularPower(selectedMeshObject->getSpecularPower() + 1.0f);
			}
			if (glfwGetKey(window, GLFW_KEY_MINUS))
			{
				if (selectedMeshObject->getSpecularPower() <= 1.0f)
				{
					return;
				}
				selectedMeshObject->setSpecularPower(selectedMeshObject->getSpecularPower() - 1.0f);
			}
		}
		else if (selectionMode == LIGHT_SELECTION && selectedLight)
		{
			if (glfwGetKey(window, GLFW_KEY_W))
			{
				selectedLight->position.z += 0.5f;
			}
			if (glfwGetKey(window, GLFW_KEY_S))
			{
				selectedLight->position.z -= 0.5f;
			}
			if (glfwGetKey(window, GLFW_KEY_A))
			{
				selectedLight->position.x += 0.5f;
			}
			if (glfwGetKey(window, GLFW_KEY_D))
			{
				selectedLight->position.x -= 0.5f;
			}
			if (glfwGetKey(window, GLFW_KEY_Q))
			{
				selectedLight->position.y += 0.5f;
			}
			if (glfwGetKey(window, GLFW_KEY_E))
			{
				selectedLight->position.y -= 0.5f;
			}

			//atten linear
			if (glfwGetKey(window, GLFW_KEY_EQUAL))
			{
				selectedLight->atten.y += 0.001f;
			}
			if (glfwGetKey(window, GLFW_KEY_MINUS))
			{
				selectedLight->atten.y -= 0.001f;
			}

			//atten quad
			if (glfwGetKey(window, GLFW_KEY_LEFT_BRACKET))
			{
				selectedLight->atten.z -= 0.0001f;
			}
			if (glfwGetKey(window, GLFW_KEY_RIGHT_BRACKET))
			{
				selectedLight->atten.z += 0.0001f;
			}
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

void cUserIO::mSaveSettings()
{
	cLightsManager* lightsManager = cLightsManager::getInstance();
	cSceneUtils* sceneUtils = cSceneUtils::getInstance();

	std::ofstream ofs;
	ofs.open("savefile.json", std::ofstream::out | std::ofstream::trunc);
	nlohmann::json json = cJsonUtils::getJsonInstance();

	//save camera
	json["cameraEye"]["x"] = sceneUtils->cameraEye.x;
	json["cameraEye"]["y"] = sceneUtils->cameraEye.y;
	json["cameraEye"]["z"] = sceneUtils->cameraEye.z;

	json["cameraAt"]["x"] = sceneUtils->cameraAt.x;
	json["cameraAt"]["y"] = sceneUtils->cameraAt.y;
	json["cameraAt"]["z"] = sceneUtils->cameraAt.z;

	//saving the lights

	size_t numLights = lightsManager->vecLights.size();
	for (unsigned int index = 0; index != numLights; index++)
	{
		cLight* light = lightsManager->vecLights[index];

		json["lights"][index]["friendlyName"] = light->friendlyName;
		json["lights"][index]["useDebugSphere"] = light->useDebugSphere;

		json["lights"][index]["position"]["x"] = light->position.x;
		json["lights"][index]["position"]["y"] = light->position.y;
		json["lights"][index]["position"]["z"] = light->position.z;
		json["lights"][index]["position"]["w"] = light->position.w;

		json["lights"][index]["attenuation"]["const"] = light->atten.x;
		json["lights"][index]["attenuation"]["linear"] = light->atten.y;
		json["lights"][index]["attenuation"]["quad"] = light->atten.z;
		json["lights"][index]["attenuation"]["distanceCutOff"] = light->atten.w;

		json["lights"][index]["diffuse"]["r"] = light->diffuse.r;
		json["lights"][index]["diffuse"]["g"] = light->diffuse.g;
		json["lights"][index]["diffuse"]["b"] = light->diffuse.b;
		json["lights"][index]["diffuse"]["a"] = light->diffuse.a;

		json["lights"][index]["specular"]["r"] = light->specular.r;
		json["lights"][index]["specular"]["g"] = light->specular.g;
		json["lights"][index]["specular"]["b"] = light->specular.b;
		json["lights"][index]["specular"]["a"] = light->specular.a;

		json["lights"][index]["direction"]["x"] = light->direction.x;
		json["lights"][index]["direction"]["y"] = light->direction.y;
		json["lights"][index]["direction"]["z"] = light->direction.z;
		json["lights"][index]["direction"]["w"] = light->direction.w;

		json["lights"][index]["param1"]["lightType"] = light->param1.x;
		json["lights"][index]["param1"]["innerAngle"] = light->param1.y;
		json["lights"][index]["param1"]["outerAngle"] = light->param1.z;
		json["lights"][index]["param1"]["w"] = light->param1.w;

		json["lights"][index]["param2"]["on"] = light->param2.x;
		json["lights"][index]["param2"]["y"] = light->param2.y;
		json["lights"][index]["param2"]["z"] = light->param2.z;
		json["lights"][index]["param2"]["w"] = light->param2.w;
	}

	//saving meshobjects
	size_t numObjects = sceneUtils->vecObjectsToDraw.size();
	for (unsigned int index = 0; index != numObjects; index++)
	{
		cMeshObject* object = (cMeshObject*) sceneUtils->vecObjectsToDraw[index];
		json["meshes"][index]["meshName"] = object->meshName;
		json["meshes"][index]["friendlyName"] = object->friendlyName;
		json["meshes"][index]["isWireFrame"] = object->isWireFrame;
		json["meshes"][index]["isVisible"] = object->isVisible;
		json["meshes"][index]["useVertexColor"] = object->useVertexColor;
		json["meshes"][index]["dontLight"] = object->dontLight;

		json["meshes"][index]["position"]["x"] = object->position.x;
		json["meshes"][index]["position"]["y"] = object->position.y;
		json["meshes"][index]["position"]["z"] = object->position.z;

		json["meshes"][index]["postRotation"]["x"] = object->postRotation.x;
		json["meshes"][index]["postRotation"]["y"] = object->postRotation.y;
		json["meshes"][index]["postRotation"]["z"] = object->postRotation.z;

		json["meshes"][index]["materialDiffuse"]["r"] = object->materialDiffuse.r;
		json["meshes"][index]["materialDiffuse"]["g"] = object->materialDiffuse.g;
		json["meshes"][index]["materialDiffuse"]["b"] = object->materialDiffuse.b;
		json["meshes"][index]["materialDiffuse"]["a"] = object->materialDiffuse.a;

		json["meshes"][index]["materialSpecular"]["r"] = object->materialSpecular.r;
		json["meshes"][index]["materialSpecular"]["g"] = object->materialSpecular.g;
		json["meshes"][index]["materialSpecular"]["b"] = object->materialSpecular.b;
		json["meshes"][index]["materialSpecular"]["power"] = object->materialSpecular.a;

		json["meshes"][index]["scale"] = object->scale;

		json["meshes"][index]["isUpdatedByPhysics"] = object->isUpdatedByPhysics;

		json["meshes"][index]["velocity"]["x"] = object->velocity.x;
		json["meshes"][index]["velocity"]["y"] = object->velocity.y;
		json["meshes"][index]["velocity"]["z"] = object->velocity.z;

		json["meshes"][index]["acceleration"]["x"] = object->acceleration.x;
		json["meshes"][index]["acceleration"]["y"] = object->acceleration.y;
		json["meshes"][index]["acceleration"]["z"] = object->acceleration.z;
	}

	ofs << std::setw(4) << json << std::endl;
	ofs.close();
	std::cout << "settings saved" << std::endl;
}
