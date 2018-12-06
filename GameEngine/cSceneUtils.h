#ifndef _SCENE_UTILS_
#define _SCENE_UTILS_

#include <glad/glad.h>
#include <vector>
#include <glm/mat4x4.hpp>

#include "iMeshObject.h"
#include "cMeshObject.h"
#include "cAABB.h"
#include "cJet.h"

class cSceneUtils
{
public:
	~cSceneUtils();
	static cSceneUtils* getInstance(void);

	void loadModelsIntoScene();
	static void initializeCamera();

	iMeshObject* findObjectByFriendlyName(std::string theNameToFind);
	void drawObject(iMeshObject* pCurrentMesh, glm::mat4x4 &matModel, GLuint shaderProgramID);
	void bindTextures(cMeshObject* pCurrentMesh, GLuint shaderProgramID);
	static glm::vec3 cameraEye;
	static glm::vec3 cameraAt;
	std::vector<iMeshObject*> vecObjectsToDraw;
	//cMeshObject* loadMeshInfoByFriendlyName( std::string friendlyName);
	void selectNextMeshObject(bool includeInvisibleObject);
	iMeshObject* selectedMeshObject;
	static bool loadFromSaveFile;

	std::vector<cMeshObject*> vecTrasparentObjects;
	cAABBHierarchy* terrainHierarchy;
	bool showAABBs;

	cJet* jet;
	void drawSkyBox(glm::vec3 eye, GLuint program);
	void drawAABBs(GLuint program);
private:
	static cSceneUtils* pSceneUtils;
	int selectedObjectIndex;
	cSceneUtils();
	void applyTranformations(iMeshObject* pCurrentMesh, glm::mat4x4& matModel, glm::mat4& matModelInvTrans);
	bool HACK_bTextureUniformLocationsLoaded = false;
	GLint tex00_UniLoc = -1;
	GLint tex01_UniLoc = -1;
	GLint tex02_UniLoc = -1;
	GLint tex03_UniLoc = -1;
	GLint tex04_UniLoc = -1;
	GLint tex05_UniLoc = -1;
	GLint tex06_UniLoc = -1;
	GLint tex07_UniLoc = -1;

	GLint texBW_0_UniLoc = -1;
	GLint texBW_1_UniLoc = -1;
};

#endif // !_SCENE_UTILS_



