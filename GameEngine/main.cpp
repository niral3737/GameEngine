#include <glad/glad.h>
#include <glm/glm.hpp>
#include <iostream>
#include <fstream>

#include "cGLFWUtils.h"
#include "cShaderUtils.h"
#include "cJsonUtils.h"


int main(void)
{
	cGLFWUtils::setUpGLFW();
	GLuint program = cShaderUtils::setUpShaders();

	system("pause");
	return 0;
}