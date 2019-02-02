#ifndef _VAO_MESH_UTILS_HG_
#define _VAO_MESH_UTILS_HG_

#include <glad/glad.h>
#include <string>
#include <map>
#include "cModelDrawInfo.h"

class cVAOMeshUtils
{
public:
	~cVAOMeshUtils();
	static cVAOMeshUtils* getInstance(void);
	static bool loadFromSaveFile;
	bool loadModels(GLuint program);
	// Note: the shader program ID is needed to tie 
	// the buffer to the vertex layout of the shader
	bool loadModelIntoVAO(cModelDrawInfo &drawInfo,
		unsigned int shaderProgramID);

	// Looks up draw info by name (meshFileName)			
	bool findDrawInfoByModelName(cModelDrawInfo &drawInfo);

	void shutDown(void);

	std::string getLastError(bool bClear = true);

	// Update the mesh information, then re-copy to GPU
	// - This will RE-COPY the VERTEX infoarmation into the vertex buffer
	//   (will copy ENTIRE vertex buffer, OVERWRITING what's there)
	void updateModelVertexInformation(cModelDrawInfo &drawInfo);

	GLuint mdp_vbo;
	GLuint mvao;

private:
	cVAOMeshUtils();
	static cVAOMeshUtils* pVAOMeshUtils;

	// LoadPlyFileData()
	bool mLoadModelFromFile(cModelDrawInfo &drawInfo);

	//LoadMeshIntoGPUBuffer
	bool mLoadDrawInfoIntoVAO(cModelDrawInfo &drawInfo,
		unsigned int shaderProgramID);


	// This holds the model information
	std::map< std::string /*model name*/, cModelDrawInfo > mMapModelNameToDrawInfo;

	std::string mLastErrorString;
	void mAppendTextToLastError(std::string text, bool addNewLineBefore = true);
};

#endif // !_VAO_MESH_UTILS_HG_



