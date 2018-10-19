#ifndef _LIGHTS_HG_
#define _LIGHTS_HG_

#include <glm/glm.hpp>
#include <vector>

class cLight
{
public:
	cLight();
	~cLight();
	glm::vec4 position;
	glm::vec4 diffuse;
	glm::vec4 specular;	// rgb = highlight colour, w = power
	glm::vec4 atten;		// x = constant, y = linear, z = quadratic, w = DistanceCutOff
	glm::vec4 direction;	// Spot, directional lights
	glm::vec4 param1;	// x = lightType, y = inner angle, z = outer angle, w = TBD
	glm::vec4 param2;	// x = 0 for off, 1 for on
	std::string friendlyName;
	bool useDebugSphere;

	void setLinearAttenuation(float newLinAtten);
	void setConstAttenuation(float newConstAtten);
	void setQuadAttenuation(float newQuadAtten);
	void setDistanceCutOff(float distanceCutOff);
	void setDiffuse(glm::vec3 diffuse);
	void setPosition(glm::vec3 newPosition);
	void setSpecularHighlight(glm::vec3 rgbColour, float power);
	void setOn(bool on);
	bool getOn();
	// .. and so on

	int position_UniLoc;
	int diffuse_UniLoc;
	int specular_UniLoc;
	int atten_UniLoc;
	int direction_UniLoc;
	int param1_UniLoc;
	int param2_UniLoc;

};

#endif // !_LIGHTS_HG_



