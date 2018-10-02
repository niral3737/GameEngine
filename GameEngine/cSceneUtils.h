#ifndef _SCENE_UTILS_
#define _SCENE_UTILS_

#include <glad/glad.h>
#include <vector>
#include <glm/mat4x4.hpp>

#include "iMeshObject.h"

class cSceneUtils
{
public:
	~cSceneUtils();
	static cSceneUtils* getInstance(void);

	void loadModelsIntoScene(std::vector<iMeshObject*> &vecObjectsToDraw);

	void drawObject(iMeshObject* pCurrentMesh, glm::mat4x4 &matModel, GLuint shaderProgramID);
	static glm::vec3 cameraEye;
	static glm::vec3 cameraAt;
private:
	static cSceneUtils* pSceneUtils;
	cSceneUtils();
	void applyTranformations(iMeshObject* pCurrentMesh, glm::mat4x4& matModel);
};

#endif // !_SCENE_UTILS_



