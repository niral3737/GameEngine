#ifndef _MODEL_DRAW_INFO_HG_
#define _MODEL_DRAW_INFO_HG_

#include <glm/glm.hpp>
#include <string>
#include <vector>

struct sVert_xyz_rgb_n
{
	glm::vec3 xyz {0.0f ,0.0f ,0.0f};
	glm::vec3 rgb { 0.0f ,0.0f ,0.0f };
	glm::vec3 n { 0.0f, 0.0f, 0.0f };
};

class cModelDrawInfo
{
public:
	cModelDrawInfo();
	~cModelDrawInfo();

	std::string meshFileName;

	unsigned int VAO_ID;					

	unsigned int vertexBufferID;
	unsigned int vertexBuffer_start_index;
	unsigned int numberOfVertices;

	unsigned int indexBufferID;
	unsigned int indexBuffer_start_index;
	unsigned int numberOfIndices;			
	unsigned int numberOfTriangles;

	// This is the vertex information as read from the file
	glm::vec3* pVerticesFromFile;
	glm::vec3* pNormalsFromFile;
	// These are the original triangle values from file,
	//	but in the 'triangle' (or face) format
	glm::ivec3 *pTriangles;

	// This is the vertex information being passed to the GPU
	// (is in format that the shader needs)
	sVert_xyz_rgb_n* pVerticesToVBO;

	// This buffer is indices as a 1D array (that the GPU needs)
	unsigned int* pIndices;		// = new unsigned int[ARRAYSIZE]

	glm::vec3 max;
	glm::vec3 min;
	glm::vec3 extent;
	float maxExtent;

	std::vector<glm::vec3> top3Vertices;

	// ******************************************
	// This is to draw "debug lines" in the scene
	// HACK: Can make this dynamic
	bool bVertexBufferIsDynamic;	// = false by default
	bool bIsIndexedModel;			// = true by default
};

#endif // !_MODEL_DRAW_INFO_HG_



