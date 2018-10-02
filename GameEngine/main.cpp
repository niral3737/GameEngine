#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
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


unsigned int numberOfObjectsToDraw = 0;

//glm::vec3 g_CameraEye = glm::vec3(0.0f, 0.0f, +10.0f);
//glm::vec3 g_CameraAt = glm::vec3(0.0f, 0.0f, 0.0f);

int main(void)
{
	cGLFWUtils::setUpGLFW();
	GLuint program = cShaderUtils::setUpShaders();

	cVAOMeshUtils::getInstance()->loadModels(program);

	std::vector<iMeshObject*> vecObjectsToDraw;
	cSceneUtils::getInstance()->loadModelsIntoScene(vecObjectsToDraw);

	cShaderUtils::getInstance()->getUniformVariableLocation(program, "objectColour");

	GLint matView_location = glGetUniformLocation(program, "matView");
	GLint matProj_location = glGetUniformLocation(program, "matProj");

	GLFWwindow* window = cGLFWUtils::getWindowInstance();
	while (!glfwWindowShouldClose(window))
	{
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


		// Draw all the objects in the "scene"
		for (unsigned int objIndex = 0;
			objIndex != (unsigned int)vecObjectsToDraw.size();
			objIndex++)
		{
			iMeshObject* pCurrentMesh = vecObjectsToDraw[objIndex];

			glm::mat4x4 matModel = glm::mat4(1.0f);			// mat4x4 m, p, mvp;

			cSceneUtils::getInstance()->drawObject(pCurrentMesh, matModel, program);

		}//for ( unsigned int objIndex = 0; 

		glfwSwapBuffers(window);		// Shows what we drew

		glfwPollEvents();
		cUserIO::processAsynKeys(window);
	}//while (!glfwWindowShouldClose(window))

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);

	return 0;
}