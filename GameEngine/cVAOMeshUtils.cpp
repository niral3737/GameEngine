#include "cVAOMeshUtils.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <string>
#include <iostream> 
#include <sstream>

#include "cJsonUtils.h"
#include "cShaderUtils.h"

cVAOMeshUtils::cVAOMeshUtils()
{
	return;
}


cVAOMeshUtils::~cVAOMeshUtils()
{
	return;
}

cVAOMeshUtils* cVAOMeshUtils::pVAOMeshUtils = 0;

cVAOMeshUtils* cVAOMeshUtils::getInstance(void)
{
	if (pVAOMeshUtils == 0)
	{
		pVAOMeshUtils = new cVAOMeshUtils();
	}
	return pVAOMeshUtils;
}

bool cVAOMeshUtils::loadModels(GLuint program)
{
	std::vector<std::string> meshes = cJsonUtils::getJsonInstance()["meshes"].get<std::vector<std::string>>();
	for (size_t i = 0; i < meshes.size(); i++)
	{
		cModelDrawInfo modelInfo;
		modelInfo.meshFileName = meshes[i];
		if (!loadModelIntoVAO(modelInfo, program))
		{
			std::cout << "Didn't load the " << modelInfo.meshFileName << std::endl;
			std::cout << getLastError() << std::endl;
			return false;
		}
	}

	std::cout << "Mesh load successful" << std::endl;
	return true;
}

bool cVAOMeshUtils::loadModelIntoVAO(
	cModelDrawInfo &drawInfo,
	unsigned int shaderProgramID)
{
	// See if this model has already been loaded
	std::map< std::string /*model name*/, cModelDrawInfo >::iterator itModel = this->mMapModelNameToDrawInfo.find(drawInfo.meshFileName);

	if (itModel != this->mMapModelNameToDrawInfo.end())
	{
		this->mAppendTextToLastError("Model ", false);
		this->mAppendTextToLastError(drawInfo.meshFileName, false);
		this->mAppendTextToLastError(" was already loaded.");
		return false;
	}



	// Load the model from the file
	if (!this->mLoadModelFromFile(drawInfo))
	{
		// Add some error code
		return false;
	}

	// Load the model data into the GPU
	//LoadMeshIntoGPUBuffer
	if (!this->mLoadDrawInfoIntoVAO(drawInfo, shaderProgramID))
	{
		// Add some error text
		return false;
	}

	// Store this mesh into the map
	this->mMapModelNameToDrawInfo[drawInfo.meshFileName] = drawInfo;

	return true;
}

// Looks up draw info by name (meshFileName)			
bool cVAOMeshUtils::findDrawInfoByModelName(cModelDrawInfo &drawInfo)
{
	std::map< std::string /*model name*/, cModelDrawInfo >::iterator itModel = this->mMapModelNameToDrawInfo.find(drawInfo.meshFileName);

	if (itModel == this->mMapModelNameToDrawInfo.end())
	{
		// Didn't find model
		return false;
	}

	// Copy the draw info back to the caller.
	drawInfo = itModel->second;

	return true;
}

bool cVAOMeshUtils::mLoadModelFromFile(cModelDrawInfo &drawInfo)
{
	// Open the file that you asked.
	std::ifstream theFile(drawInfo.meshFileName.c_str());

	// if ( theFile.is_open() == false )
	if (!theFile.is_open())			// More "c" or "C++" ish
	{
		//std::cout << "Didn't open file" << std::endl;
		this->mAppendTextToLastError("Didn't load the ", false);
		this->mAppendTextToLastError(drawInfo.meshFileName, false);
		this->mAppendTextToLastError(" file.");
		return false;
	}//if (!theFile.is_open() 

	// file is open OK
	std::string nextData;
	while (theFile >> nextData)
	{
		if (nextData == "vertex")
		{
			break;		// exit while loop...
		}
	};
	// ...Jumping down to here

//	unsigned int numberOfVertices = 0;
//	theFile >> g_numberOfVertices;
	theFile >> drawInfo.numberOfVertices;

	//	std::cout << "vertices: " << g_numberOfVertices << std::endl;
	std::cout << "vertices: " << drawInfo.numberOfVertices << std::endl;

	// seach for "face"
	while (theFile >> nextData)
	{
		if (nextData == "face")
		{
			break;		// exit while loop...
		}
	};
	// ...Jumping down to here

//	unsigned int numberOfTriangles = 0;
//	theFile >> g_numberOfTriangles;
	theFile >> drawInfo.numberOfTriangles;

	//	std::cout << "triangles: " << g_numberOfTriangles << std::endl;
	std::cout << "triangles: " << drawInfo.numberOfTriangles << std::endl;

	while (theFile >> nextData)
	{
		if (nextData == "end_header")
		{
			break;		// exit while loop...
		}
	};
	// ...Jumping down to here

	//-0.036872 0.127727 0.00440925 
	//-0.0453607 0.128854 0.00114541 

	// Create an vertex array to store the data.
//	sPlyVertex tempVert; 
//	sPlyVertex tempArrayVert[1000];		// Static (stack)

//	sPlyVertex* pArrayVert = new sPlyVertex[numberOfVertices];	// HEAP
//	g_pArrayVert = new sPlyVertex[g_numberOfVertices];	// HEAP
	drawInfo.pVerticesFromFile = new glm::vec3[drawInfo.numberOfVertices];

	//	ZeroMemory(); win32
		// C call... (clears memory to all zeros)
	//	memset( g_pArrayVert, 0, sizeof( sPlyVertex ) * g_numberOfVertices );
	memset(drawInfo.pVerticesFromFile, 0, sizeof(glm::vec3) * drawInfo.numberOfVertices);

	// Read the vertex data into the array
	for (unsigned int index = 0; index != drawInfo.numberOfVertices; index++)
	{
		theFile >> drawInfo.pVerticesFromFile[index].x;
		theFile >> drawInfo.pVerticesFromFile[index].y;
		theFile >> drawInfo.pVerticesFromFile[index].z;

		//		theFile >> g_pArrayVert[index].;
	}//for ( unsigned int index...

	// Same with triangles

//	sPlyTriangle* pArrayTris = new sPlyTriangle[numberOfTriangles];	// HEAP
//	g_pArrayTris = new sPlyTriangle[g_numberOfTriangles];	// HEAP
	drawInfo.pTriangles = new glm::ivec3[drawInfo.numberOfTriangles];

	//	memset( g_pArrayTris, 0, sizeof( sPlyTriangle ) * g_numberOfTriangles );
	memset(drawInfo.pTriangles, 0, sizeof(glm::vec3) * drawInfo.numberOfTriangles);

	int TossThisAway = 0;
	for (unsigned int index = 0; index != drawInfo.numberOfTriangles; index++)
	{
		// 3 69 1322 70
		theFile >> TossThisAway;			// 3
		theFile >> drawInfo.pTriangles[index].x;
		theFile >> drawInfo.pTriangles[index].y;
		theFile >> drawInfo.pTriangles[index].z;
	}//for ( unsigned int index...

	std::cout << "Read from the file OK." << std::endl;

	// Calculating extents...

	// Assume the 1st one is the largest and smallest:
	drawInfo.min = drawInfo.pVerticesFromFile[0];

	drawInfo.max = drawInfo.pVerticesFromFile[0];

	for (unsigned int index = 0; index != drawInfo.numberOfVertices; index++)
	{
		if (drawInfo.pVerticesFromFile[index].x < drawInfo.min.x) { drawInfo.min.x = drawInfo.pVerticesFromFile[index].x; }
		if (drawInfo.pVerticesFromFile[index].y < drawInfo.min.y) { drawInfo.min.y = drawInfo.pVerticesFromFile[index].y; }
		if (drawInfo.pVerticesFromFile[index].z < drawInfo.min.z) { drawInfo.min.z = drawInfo.pVerticesFromFile[index].z; }

		if (drawInfo.pVerticesFromFile[index].x > drawInfo.max.x) { drawInfo.max.x = drawInfo.pVerticesFromFile[index].x; }
		if (drawInfo.pVerticesFromFile[index].y > drawInfo.max.y) { drawInfo.max.y = drawInfo.pVerticesFromFile[index].y; }
		if (drawInfo.pVerticesFromFile[index].z > drawInfo.max.z) { drawInfo.max.z = drawInfo.pVerticesFromFile[index].z; }
	}//for ( unsigned int index...

	drawInfo.extent.x = drawInfo.max.x - drawInfo.min.x;
	drawInfo.extent.y = drawInfo.max.y - drawInfo.min.y;
	drawInfo.extent.z = drawInfo.max.z - drawInfo.min.z;

	drawInfo.maxExtent = drawInfo.extent.x;
	if (drawInfo.maxExtent < drawInfo.extent.y) { drawInfo.maxExtent = drawInfo.extent.y; }
	if (drawInfo.maxExtent < drawInfo.extent.z) { drawInfo.maxExtent = drawInfo.extent.z; }

	return true;
}

bool cVAOMeshUtils::mLoadDrawInfoIntoVAO(
	cModelDrawInfo &drawInfo,
	unsigned int shaderProgramID)
{
	//sVert_xyz_rgb* pVertices = new sVert_xyz_rgb[g_numberOfVertices];
	drawInfo.pVerticesToVBO = new sVert_xyz_rgb[drawInfo.numberOfVertices];

	// Copy the data from the PLY format to the vertex buffer format
	for (unsigned int index = 0; index != drawInfo.numberOfVertices; index++)
	{
		drawInfo.pVerticesToVBO[index].xyz = drawInfo.pVerticesFromFile[index];

		drawInfo.pVerticesToVBO[index].rgb.r = 1.0f;
		drawInfo.pVerticesToVBO[index].rgb.g = 1.0f;
		drawInfo.pVerticesToVBO[index].rgb.b = 1.0f;
	}


	// Creates a VAO (Vertex Array Object)
	glGenVertexArrays(1, &(drawInfo.VAO_ID));
	glBindVertexArray(drawInfo.VAO_ID);


	// Allocate a buffer and copy
	//GLuint vertex_buffer;

	// Just to confuse you, this is called VBO (Vertex Buffer Object)
	// NOTE: OpenGL error checks have been omitted for brevity
	glGenBuffers(1, &(drawInfo.vertexBufferID));			// vertex_buffer
	// "Vertex" buffer
	// - Sets the "type" of buffer
	// - Makes it the 'current' buffer
	glBindBuffer(GL_ARRAY_BUFFER, drawInfo.vertexBufferID);

	unsigned int vertexBufferSizeInBytes =
		sizeof(sVert_xyz_rgb) * drawInfo.numberOfVertices;

	//	vector<sVert_xyz_rgb> vecVerticesON_THE_CPU;
	//	sVert_xyz_rgb* pVerticesToVBO = new sVert_xyz_rgb[ARRAYSIZE]

	if (drawInfo.bVertexBufferIsDynamic)
	{
		// CAN update the vertex information (later)
		glBufferData(GL_ARRAY_BUFFER,
			vertexBufferSizeInBytes,		// sizeof(vertices), 
			drawInfo.pVerticesToVBO,			// vertices, 
			GL_DYNAMIC_DRAW);	//*************
	}
	else
	{
		// DON'T INDEND to update the vertex buffer info (faster)
		glBufferData(GL_ARRAY_BUFFER,
			vertexBufferSizeInBytes,	// sizeof(vertices), 
			drawInfo.pVerticesToVBO,			// vertices, 
			GL_STATIC_DRAW);	//*************
	}

	// Create the index buffer...

	// Three indices (of vertices) per triangle
	drawInfo.numberOfIndices = drawInfo.numberOfTriangles * 3;
	drawInfo.pIndices = new unsigned int[drawInfo.numberOfIndices];
	memset(drawInfo.pIndices, 0, sizeof(unsigned int) * drawInfo.numberOfIndices);


	// Load the index buffer...
	unsigned int indexIndex = 0;
	for (unsigned int triIndex = 0; triIndex != drawInfo.numberOfTriangles;
		triIndex++, indexIndex += 3)
	{
		drawInfo.pIndices[indexIndex + 0] = drawInfo.pTriangles[triIndex].x;
		drawInfo.pIndices[indexIndex + 1] = drawInfo.pTriangles[triIndex].y;
		drawInfo.pIndices[indexIndex + 2] = drawInfo.pTriangles[triIndex].z;
	}


	glGenBuffers(1, &(drawInfo.indexBufferID));			// vertex_buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, drawInfo.indexBufferID);

	unsigned int indexBufferSizeInBytes = sizeof(unsigned int) * drawInfo.numberOfIndices;

	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		indexBufferSizeInBytes,	// sizeof(vertices), 
		drawInfo.pIndices,			// vertices, 
		GL_STATIC_DRAW);

	// Set the vertex layout

	GLint vpos_location = glGetAttribLocation(shaderProgramID, "vPosition");
	GLint vcol_location = glGetAttribLocation(shaderProgramID, "vColour");

	glEnableVertexAttribArray(vpos_location);
	glVertexAttribPointer(vpos_location,	// "vPosition"
		3,				//  vec3 (xyz)
		GL_FLOAT,
		GL_FALSE,		// DON'T "normalize"
		sizeof(float) * 6,
		(void*)0);

	glEnableVertexAttribArray(vcol_location);
	glVertexAttribPointer(vcol_location,		// cColour
		3,					// vec3 (rgb)
		GL_FLOAT,
		GL_FALSE,
		sizeof(float) * 6,
		(void*)(sizeof(float) * 3));

	// Get rid of everything we don't need.

	// Set the "current" VAO to nothing.
	glBindVertexArray(0);

	return true;
}

void cVAOMeshUtils::mAppendTextToLastError(std::string text, bool addNewLineBefore /*=true*/)
{
	std::stringstream ssError;
	ssError.str() = this->mLastErrorString;

	if (addNewLineBefore)
	{
		ssError << std::endl;
	}
	ssError << text;

	this->mLastErrorString = ssError.str();

	return;
}

std::string cVAOMeshUtils::getLastError(bool bAndClear /*=true*/)
{
	std::string errorText = this->mLastErrorString;
	if (bAndClear)
	{
		this->mLastErrorString = "";
	}
	return errorText;
}

void cVAOMeshUtils::shutDown(void)
{
	//clear the map
	return;
}

void cVAOMeshUtils::updateModelVertexInformation(cModelDrawInfo &drawInfo)
{
	glBindBuffer(GL_ARRAY_BUFFER, drawInfo.vertexBufferID);

	unsigned int vertexBufferSizeInBytes =
		sizeof(sVert_xyz_rgb) * drawInfo.numberOfVertices;

	// ASSUME it's a dynmaic buffer
//	if ( drawInfo.bVertexBufferIsDynamic )
//	{	
		// CAN update the vertex information (later)
	glBufferData(GL_ARRAY_BUFFER,
		vertexBufferSizeInBytes,		// sizeof(vertices), 
		drawInfo.pVerticesToVBO,			// vertices, 
		GL_DYNAMIC_DRAW);	//*************
//	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);	// "un-bind" the buffer

	return;
}
