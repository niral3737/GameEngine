#include "cLightsManager.h"

#include "json.hpp"
#include "cJsonUtils.h"

cLightsManager* cLightsManager::instance = NULL;

cLightsManager::cLightsManager()
{}


cLightsManager::~cLightsManager()
{}


cLightsManager* cLightsManager::getInstance()
{
	if (instance == NULL)
	{
		instance = new cLightsManager();
	}
	return instance;
}

void cLightsManager::loadAllLights(GLuint program)
{
	std::vector<nlohmann::json> lights = cJsonUtils::getJsonInstance()["lights"].get<std::vector<nlohmann::json>>();
	for (size_t i = 0; i < lights.size(); i++)
	{
		cLight* light = new cLight();

		loadUniformLocations(light, program, i);

		light->friendlyName = lights[i]["friendlyName"].get<std::string>();

		light->position.x = lights[i]["position"]["x"].get<float>();
		light->position.y = lights[i]["position"]["y"].get<float>();
		light->position.z = lights[i]["position"]["z"].get<float>();
		light->position.w = lights[i]["position"]["w"].get<float>();

		light->setConstAttenuation(lights[i]["attenuation"]["const"].get<float>());
		light->setLinearAttenuation(lights[i]["attenuation"]["linear"].get<float>());
		light->setQuadAttenuation(lights[i]["attenuation"]["quad"].get<float>());
		light->setDistanceCutOff(lights[i]["attenuation"]["distanceCutOff"].get<float>());

		light->diffuse.r = lights[i]["diffuse"]["r"].get<float>();
		light->diffuse.g = lights[i]["diffuse"]["g"].get<float>();
		light->diffuse.b = lights[i]["diffuse"]["b"].get<float>();
		light->diffuse.a = lights[i]["diffuse"]["a"].get<float>();

		light->param2.x = lights[i]["param2"]["on"].get<float>();

		vecLights.push_back(light);
	}

}

cLight* cLightsManager::getLightByFriendlyName(std::string friendlyName)
{
	for (std::vector<cLight*>::iterator it = vecLights.begin(); it != vecLights.end(); it++)
	{
		cLight* light = *it;
		if (light->friendlyName == friendlyName)
			return light;
	}
	return nullptr;
}

void cLightsManager::loadUniformLocations(cLight* light, GLuint program, size_t index)
{
	std::string theLightsIndex = "theLights[";
	theLightsIndex += std::to_string(index);
	theLightsIndex += "].";
	std::string position = "pos";
	light->position_UniLoc = glGetUniformLocation(program, (theLightsIndex + "position").c_str());
	light->diffuse_UniLoc = glGetUniformLocation(program, (theLightsIndex + "diffuse").c_str());
	light->specular_UniLoc = glGetUniformLocation(program, (theLightsIndex + "specular").c_str());
	light->atten_UniLoc = glGetUniformLocation(program, (theLightsIndex + "atten").c_str());
	light->direction_UniLoc = glGetUniformLocation(program, (theLightsIndex + "direction").c_str());
	light->param1_UniLoc = glGetUniformLocation(program, (theLightsIndex + "param1").c_str());
	light->param2_UniLoc = glGetUniformLocation(program, (theLightsIndex + "param2").c_str());
}

void cLightsManager::copyLightValuesToShader()
{
	for (std::vector<cLight*>::iterator it = vecLights.begin(); it != vecLights.end(); it++)
	{
		cLight* light = *it;
		glUniform4f(light->position_UniLoc,
			light->position.x, light->position.y, light->position.z, light->position.w);
		glUniform4f(light->diffuse_UniLoc,
			light->diffuse.r, light->diffuse.g, light->diffuse.b, light->diffuse.a);
		glUniform4f(light->param2_UniLoc, light->param2.x, 0.0f, 0.0f, 0.0f);	// Turns it "on")
		glUniform4f(light->atten_UniLoc,
			light->atten.x, light->atten.y, light->atten.z, light->atten.w);
	}
	
}