#include "cSceneUtils.h"
#include <vector>
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

#include "json.hpp"
#include "cJsonUtils.h"
#include "cMeshObjectFactory.h"
#include "cModelDrawInfo.h"
#include "cVAOMeshUtils.h"

cSceneUtils* cSceneUtils::pSceneUtils = 0;

glm::vec3 cSceneUtils::cameraEye = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cSceneUtils::cameraAt = glm::vec3(0.0f, 0.0f, 0.0f);

cSceneUtils::cSceneUtils()
{
}

cSceneUtils::~cSceneUtils()
{}

cSceneUtils * cSceneUtils::getInstance(void)
{
	if (pSceneUtils == 0)
	{
		pSceneUtils = new cSceneUtils();
	}
	return pSceneUtils;
}

iMeshObject* cSceneUtils::findObjectByFriendlyName(std::string theNameToFind)
{
	for (unsigned int index = 0; index != vecObjectsToDraw.size(); index++)
	{
		cMeshObject* object = (cMeshObject*)vecObjectsToDraw[index];
		
		if (object->friendlyName == theNameToFind)
		{
			return vecObjectsToDraw[index];
		}
	}

	return NULL;	
}

void cSceneUtils::loadModelsIntoScene()
{
	//load models from settings.json
	std::vector<nlohmann::json> meshes = cJsonUtils::getJsonInstance()["meshes"].get<std::vector<nlohmann::json>>();
	for (size_t i = 0; i < meshes.size(); i++)
	{
		cMeshObject* meshObject = (cMeshObject*) cMeshObjectFactory::createMeshObject();
		meshObject->meshName = meshes[i]["meshName"].get<std::string>();
		meshObject->friendlyName = meshes[i]["friendlyName"].get<std::string>();

		meshObject->isWireFrame = meshes[i]["isWireFrame"].get<bool>();
		meshObject->isVisible = meshes[i]["isVisible"].get<bool>();
		meshObject->useVertexColor = meshes[i]["useVertexColor"].get<bool>();

		meshObject->position.x = meshes[i]["position"]["x"].get<float>();
		meshObject->position.y = meshes[i]["position"]["y"].get<float>();
		meshObject->position.z = meshes[i]["position"]["z"].get<float>();

		meshObject->postRotation.x = meshes[i]["postRotation"]["x"].get<float>();
		meshObject->postRotation.y = meshes[i]["postRotation"]["y"].get<float>();
		meshObject->postRotation.z = meshes[i]["postRotation"]["z"].get<float>();

		meshObject->objectColor.r = meshes[i]["objectColor"]["r"].get<float>();
		meshObject->objectColor.g = meshes[i]["objectColor"]["g"].get<float>();
		meshObject->objectColor.b = meshes[i]["objectColor"]["b"].get<float>();

		meshObject->scale = meshes[i]["scale"].get<float>();

		vecObjectsToDraw.push_back(meshObject);
	}
}

cMeshObject* cSceneUtils::loadMeshInfoByFriendlyName( std::string friendlyName)
{
	std::vector<nlohmann::json> meshes = cJsonUtils::getJsonInstance()["meshes"].get<std::vector<nlohmann::json>>();
	for (size_t i = 0; i < meshes.size(); i++)
	{
		if (meshes[i]["friendlyName"].get<std::string>() == friendlyName)
		{
			cMeshObject* meshObject = (cMeshObject*)cMeshObjectFactory::createMeshObject();
			meshObject->meshName = meshes[i]["meshName"].get<std::string>();
			meshObject->friendlyName = meshes[i]["friendlyName"].get<std::string>();

			meshObject->isWireFrame = meshes[i]["isWireFrame"].get<bool>();
			meshObject->isVisible = meshes[i]["isVisible"].get<bool>();
			meshObject->useVertexColor = meshes[i]["useVertexColor"].get<bool>();

			meshObject->position.x = meshes[i]["position"]["x"].get<float>();
			meshObject->position.y = meshes[i]["position"]["y"].get<float>();
			meshObject->position.z = meshes[i]["position"]["z"].get<float>();

			meshObject->postRotation.x = meshes[i]["postRotation"]["x"].get<float>();
			meshObject->postRotation.y = meshes[i]["postRotation"]["y"].get<float>();
			meshObject->postRotation.z = meshes[i]["postRotation"]["z"].get<float>();

			meshObject->objectColor.r = meshes[i]["objectColor"]["r"].get<float>();
			meshObject->objectColor.g = meshes[i]["objectColor"]["g"].get<float>();
			meshObject->objectColor.b = meshes[i]["objectColor"]["b"].get<float>();

			meshObject->scale = meshes[i]["scale"].get<float>();

			vecObjectsToDraw.push_back(meshObject);
			return meshObject;
		}
	}
	return NULL;
}

void cSceneUtils::drawTreesAtRandomPositions()
{}

void cSceneUtils::initializeCamera()
{
	cSceneUtils::cameraEye = glm::vec3(
		cJsonUtils::getJsonInstance()["cameraEye"]["x"].get<float>(),
		cJsonUtils::getJsonInstance()["cameraEye"]["y"].get<float>(),
		cJsonUtils::getJsonInstance()["cameraEye"]["z"].get<float>()
	);

	cSceneUtils::cameraAt = glm::vec3(
		cJsonUtils::getJsonInstance()["cameraAt"]["x"].get<float>(),
		cJsonUtils::getJsonInstance()["cameraAt"]["y"].get<float>(),
		cJsonUtils::getJsonInstance()["cameraAt"]["z"].get<float>()
	);
}

void cSceneUtils::drawObject(iMeshObject* pCurrentMesh, glm::mat4x4& matModel, GLuint shaderProgramID)
{
	cMeshObject* currentMesh = (cMeshObject*) pCurrentMesh;

	if (!currentMesh->isVisible)
		return;

	applyTranformations(currentMesh, matModel);

	glUseProgram(shaderProgramID);


	GLint objectColour_UniLoc = glGetUniformLocation(shaderProgramID, "objectColour");
	GLint lightPos_UniLoc = glGetUniformLocation(shaderProgramID, "lightPos");
	GLint lightBrightness_UniLoc = glGetUniformLocation(shaderProgramID, "lightBrightness");
	GLint useVertexColour_UniLoc = glGetUniformLocation(shaderProgramID, "useVertexColour");

	GLint matModel_location = glGetUniformLocation(shaderProgramID, "matModel");
	GLint matView_location = glGetUniformLocation(shaderProgramID, "matView");
	GLint matProj_location = glGetUniformLocation(shaderProgramID, "matProj");

	glUniformMatrix4fv(matModel_location, 1, GL_FALSE, glm::value_ptr(matModel));

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glUniform3f(objectColour_UniLoc,
		currentMesh->objectColor.r,
		currentMesh->objectColor.g,
		currentMesh->objectColor.b);

	glm::vec3 g_lightPos = glm::vec3(4.0f, 4.0f, 0.0f);
	float g_lightBrightness = 400000.0f;

	glUniform3f(lightPos_UniLoc, g_lightPos.x, g_lightPos.y, g_lightPos.z);
	glUniform1f(lightBrightness_UniLoc, g_lightBrightness);

	if (currentMesh->useVertexColor)
	{
		glUniform1f(useVertexColour_UniLoc, (float)GL_TRUE);
	}
	else
	{
		glUniform1f(useVertexColour_UniLoc, (float)GL_FALSE);
	}

	if (currentMesh->isWireFrame)
	{
		// Yes, draw it wireframe
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDisable(GL_CULL_FACE);	// Discared "back facing" triangles
		//glDisable( GL_DEPTH );		// Enables the KEEPING of the depth information
		//glDisable( GL_DEPTH_TEST );	// When drawing, checked the existing depth
	}
	else
	{
		// No, it's "solid" (or "Filled")
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glEnable(GL_CULL_FACE);	// Discared "back facing" triangles
		//glEnable( GL_DEPTH );		// Enables the KEEPING of the depth information
		//glEnable( GL_DEPTH_TEST );	// When drawing, checked the existing depth
	}

	cModelDrawInfo modelInfo;
	modelInfo.meshFileName = currentMesh->meshName;

	if (cVAOMeshUtils::getInstance()->findDrawInfoByModelName(modelInfo))
	{
		//glDrawArrays(GL_TRIANGLES, 0, bunnyInfo.numberOfIndices );

		glBindVertexArray(modelInfo.VAO_ID);

		glDrawElements(GL_TRIANGLES,
			modelInfo.numberOfIndices,
			GL_UNSIGNED_INT,
			0);

		glBindVertexArray(0);

	}

	return;
}

void cSceneUtils::applyTranformations(iMeshObject* pCurrentMesh, glm::mat4x4& matModel)
{
	cMeshObject* currentMesh = (cMeshObject*) pCurrentMesh;
	matModel = glm::mat4x4(1.0f);		// mat4x4_identity(m);

	glm::mat4 matTranslation = glm::translate(glm::mat4(1.0f),
		currentMesh->position);

	matModel = matModel * matTranslation;		// matMove

	glm::mat4 postRot_X = glm::rotate(glm::mat4(1.0f),
		currentMesh->postRotation.x,
		glm::vec3(1.0f, 0.0, 0.0f));
	matModel = matModel * postRot_X;

	glm::mat4 postRot_Y = glm::rotate(glm::mat4(1.0f),
		currentMesh->postRotation.y,
		glm::vec3(0.0f, 1.0, 0.0f));
	matModel = matModel * postRot_Y;

	glm::mat4 postRot_Z = glm::rotate(glm::mat4(1.0f),
		currentMesh->postRotation.z,
		glm::vec3(0.0f, 0.0, 1.0f));
	matModel = matModel * postRot_Z;

	// And now scale

	float scale = currentMesh->scale;
	glm::mat4 matScale = glm::scale(glm::mat4(1.0f), glm::vec3(scale, scale, scale));
	matModel = matModel * matScale;
}
