#ifndef _MESH_OBJECT_HG_
#define _MESH_OBJECT_HG_

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#define GLM_ENABLE_EXPERIMENTAL		// To get glm quaternion stuff to compile
#include <glm/gtx/quaternion.hpp>	// Note strange folder
#include <string>
#include <vector>

#include "iMeshObject.h"

class cMeshObject : public iMeshObject
{
public:
	
	~cMeshObject();

	glm::vec3 position;
	glm::vec3 postRotation;
	glm::vec4 materialDiffuse;
	void setDiffuseColour(glm::vec3 newDiffuse);
	void setAlphaTransparency(float newAlpha);
	glm::vec4 materialSpecular;
	void setSpecularColour(glm::vec3 colourRGB);
	void setSpecularPower(float specPower);
	float getSpecularPower();
	float scale;

	std::string meshName;
	std::string friendlyName;

	bool isWireFrame;
	bool isVisible;
	bool useVertexColor;
	bool dontLight;

	//Physics
	bool isUpdatedByPhysics;
	glm::vec3 velocity;
	glm::vec3 acceleration;

	glm::quat getOrientation(void);
	void setOrientationEulerAngles(glm::vec3 newEulerAngles, bool bIsDegrees = false);
	void setOrientationEulerAngles(float x, float y, float z, bool bIsDegrees = false);
	void adjustOrientationEulerAngles(glm::vec3 adjEulerAngle, bool bIsDegrees = false);
	void adjustOrientationEulerAngles(float x, float y, float z, bool bIsDegrees = false);
	void adjustOrientation(glm::quat adjOrient);

	inline unsigned int getUniqueId(void)
	{
		return this->mUniqueID;
	}

	static iMeshObject* create(void);
	cMeshObject* cloneObject();
private:
	cMeshObject();
	unsigned int mUniqueID;			// Number that's unique to this instance
	static unsigned int mNextID; 

	static const unsigned int STARTING_ID_VALUE = 1000;

	glm::quat orientation;
};

#endif // !_MESH_OBJECT_HG_



