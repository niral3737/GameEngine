#include <glad/glad.h>
#include <glm/glm.hpp>
#include <iostream>
#include <fstream>

#include "cGLFWUtils.h"
#include "cShaderUtils.h"
#include "cJsonUtils.h"
#include "cVAOMeshUtils.h"


int main(void)
{
	cGLFWUtils::setUpGLFW();
	GLuint program = cShaderUtils::setUpShaders();

	cVAOMeshUtils::getInstance()->loadModels(program);

	system("pause");
	return 0;
}