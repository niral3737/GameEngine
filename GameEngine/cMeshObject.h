#ifndef _MESH_OBJECT_HG_
#define _MESH_OBJECT_HG_

#include <glm/vec3.hpp>
#include <string>
#include <vector>

class cMeshObject
{
public:
	cMeshObject();
	~cMeshObject();

	glm::vec3 position;
	glm::vec3 postRotation;
	glm::vec3 objectColor;
	float scale;

	std::string meshName;
	std::string friendlyName;

	bool isWireFrame;
	bool isVisible;
	bool useVertexColor;

	std::vector< cMeshObject* > vecChildObjectsToDraw;

	inline unsigned int getUniqueId(void)
	{
		return this->mUniqueID;
	}

private:
	unsigned int mUniqueID;			// Number that's unique to this instance
	static unsigned int mNextID; 

	static const unsigned int STARTING_ID_VALUE = 1000;
};

#endif // !_MESH_OBJECT_HG_


