#ifndef _SCENE_UTILS_
#define _SCENE_UTILS_

#include <glad/glad.h>
#include <vector>
#include <glm/mat4x4.hpp>

#include "iMeshObject.h"
#include "cMeshObject.h"	

class cSceneUtils
{
public:
	~cSceneUtils();
	static cSceneUtils* getInstance(void);

	void loadModelsIntoScene();
	static void initializeCamera();

	iMeshObject* findObjectByFriendlyName(std::string theNameToFind);
	void drawObject(iMeshObject* pCurrentMesh, glm::mat4x4 &matModel, GLuint shaderProgramID);
	static glm::vec3 cameraEye;
	static glm::vec3 cameraAt;
	std::vector<iMeshObject*> vecObjectsToDraw;
	void drawTreesAtRandomPositions();
	cMeshObject* loadMeshInfoByFriendlyName( std::string friendlyName);
private:
	static cSceneUtils* pSceneUtils;
	cSceneUtils();
	void applyTranformations(iMeshObject* pCurrentMesh, glm::mat4x4& matModel, glm::mat4& matModelInvTrans);
};

#endif // !_SCENE_UTILS_



