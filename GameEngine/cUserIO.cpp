#include "cUserIO.h"

#include "cSceneUtils.h"

#include <iostream>

#include "cSoundManager.h"
#include "iMeshObject.h"
#include "cVAOMeshUtils.h"
#include "cMeshObject.h"


void cUserIO::key_callback(GLFWwindow * window, int key, int scancode, int action, int mods)
{
	//cSoundManager* soundManager = cSoundManager::getInstance();
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}

	//if (key == GLFW_KEY_C && action == GLFW_PRESS)
	//{
	//	soundManager->changeBackgroundMusic();
	//}

	//if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) || glfwGetKey(window, GLFW_KEY_RIGHT_ALT))	// "down"
	//{
	//	soundManager->playSFX("axeSound");
	//}

	//if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) || glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL))	// "down"
	//{
	//	soundManager->playSFX("punchSound");
	//}

	//if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)	// "down"
	//{
	//	soundManager->playSFX("jumpSound");
	//}

	//if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT))	// "down"
	//{
	//	soundManager->playSFX("appleSound");
	//}

	//if (glfwGetKey(window, GLFW_KEY_1))
	//{
	//	soundManager->selectedSound = soundManager->mapSounds["backgroundSound1"];
	//}
	//if (glfwGetKey(window, GLFW_KEY_2))
	//{
	//	soundManager->selectedSound = soundManager->mapSounds["backgroundSound2"];
	//}
	//if (glfwGetKey(window, GLFW_KEY_3))
	//{
	//	soundManager->selectedSound = soundManager->mapSounds["backgroundSound3"];
	//}
	//if (glfwGetKey(window, GLFW_KEY_4))
	//{
	//	soundManager->selectedSound = soundManager->mapSounds["axeSound"];
	//}
	//if (glfwGetKey(window, GLFW_KEY_5))
	//{
	//	soundManager->selectedSound = soundManager->mapSounds["appleSound"];
	//}
	//if (glfwGetKey(window, GLFW_KEY_6))
	//{
	//	soundManager->selectedSound = soundManager->mapSounds["jumpSound"];
	//}
	//if (glfwGetKey(window, GLFW_KEY_7))
	//{
	//	soundManager->selectedSound = soundManager->mapSounds["punchSound"];
	//}
	//if (glfwGetKey(window, GLFW_KEY_8))
	//{
	//	soundManager->selectedSound = soundManager->mapSounds["splashSound"];
	//}
	//if (glfwGetKey(window, GLFW_KEY_9))
	//{
	//	soundManager->selectedSound = soundManager->mapSounds["fireSound"];
	//}
	//if (glfwGetKey(window, GLFW_KEY_0))
	//{
	//	soundManager->selectedSound = soundManager->mapSounds["forestSound"];
	//}


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
			cSceneUtils::cameraEye.x -= cameraSpeed;
			//axe->position.x -= cameraSpeed;
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)	// "right"
		{
			cSceneUtils::cameraEye.x += cameraSpeed;
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

	if (mIsAltDown(window))
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
