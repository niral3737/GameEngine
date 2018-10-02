#include <stdlib.h>
#include <stdio.h>

#include "cGLFWUtils.h"
#include "cUserIO.h"



GLFWwindow* cGLFWUtils::window = 0;

GLFWwindow* cGLFWUtils::getWindowInstance(void)
{
	if (window == 0)
	{
		window = glfwCreateWindow(1024, 480, "Hello there", NULL, NULL);
	}
	return window;
}

void cGLFWUtils::setUpGLFW(void)
{
	glfwSetErrorCallback(error_callback);

	if (!glfwInit())
		exit(EXIT_FAILURE);

	GLFWwindow* window = getWindowInstance();
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwSetKeyCallback(window, cUserIO::key_callback);
	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
	glfwSwapInterval(1);
}

void cGLFWUtils::error_callback(int error, const char * description)
{
	fprintf(stderr, "Error: %s\n", description);
}
