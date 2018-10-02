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

#include "console.h"

//Globals
Console _console;
bool _should_exit = false;


//FMOD Globals
FMOD_RESULT _result = FMOD_OK;
FMOD::System *_system = NULL;
FMOD::Sound *_sound = NULL;
FMOD::Channel *_channel = NULL;

bool _is_channel_paused = false;
bool _is_channel_playing = false;
unsigned int _channel_position = 0;
float _channel_frequency = 0.0f;
float _channel_volume = 0.0f;
float _channel_pan = 0.0f;

unsigned int _sound_length = 0;
int _sound_number_of_channels = 0;
int _sound_number_of_bits = 0;
FMOD_SOUND_TYPE _sound_type = FMOD_SOUND_TYPE_UNKNOWN;
FMOD_SOUND_FORMAT _sound_format = FMOD_SOUND_FORMAT_NONE;

bool init_fmod();
bool shutdown_fmod();

int main(void)
{
	cGLFWUtils::setUpGLFW();
	GLuint program = cShaderUtils::setUpShaders();

	cVAOMeshUtils::getInstance()->loadModels(program);

	std::vector<iMeshObject*> vecObjectsToDraw;
	cSceneUtils::initializeCamera();
	cSceneUtils::getInstance()->loadModelsIntoScene(vecObjectsToDraw);

	_console.print(HORIZONTAL_ROW);
	_console.print("Play sound with FMOD");
	_console.print(HORIZONTAL_ROW);
	assert(init_fmod());

	std::string file_name = cJsonUtils::getJsonInstance()["soundFile"].get<std::string>();
	_result = _system->createSound(file_name.c_str(), FMOD_DEFAULT, 0, &_sound);
	/*if(_result != FMOD_OK)*/
	assert(!_result);

	//TODO: Play sound
	_result = _system->playSound(_sound, 0, false, &_channel);
	assert(!_result);

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

bool init_fmod()
{

	//Create the main system object
	_result = FMOD::System_Create(&_system);
	//TODO: CHECK FOR FMOD ERRORS, IMPLEMENT YOUR OWN FUNCTION
	assert(!_result);
	//Initializes the system object, and the msound device. This has to be called at the start of the user's program
	_result = _system->init(512, FMOD_INIT_NORMAL, NULL);
	assert(!_result);


	return true;
}

bool shutdown_fmod()
{

	if (_sound)
	{
		_result = _sound->release();
		assert(!_result);
	}
	if (_system)
	{
		_result = _system->close();
		assert(!_result);
		_result = _system->release();
		assert(!_result);
	}

	return true;
}