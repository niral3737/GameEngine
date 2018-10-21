#include "cSceneUtils.h"
#include <vector>
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

#include <fstream>
#include <iostream>
#include "json.hpp"
#include "cJsonUtils.h"
#include "cMeshObjectFactory.h"
#include "cModelDrawInfo.h"
#include "cVAOMeshUtils.h"

cSceneUtils* cSceneUtils::pSceneUtils = 0;

glm::vec3 cSceneUtils::cameraEye = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cSceneUtils::cameraAt = glm::vec3(0.0f, 0.0f, 0.0f);

bool cSceneUtils::loadFromSaveFile = false;

cSceneUtils::cSceneUtils()
{
	this->selectedMeshObject = NULL;
	this->selectedObjectIndex = -1;
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
	//load models 
	std::vector<nlohmann::json> meshes;
	if (loadFromSaveFile)
	{
		std::ifstream ifs("savefile.json");
		nlohmann::json j = json::parse(ifs);
		ifs.close();
		meshes = j["meshes"].get<std::vector<nlohmann::json>>();
	}
	else
	{
		meshes = cJsonUtils::getJsonInstance()["meshes"].get<std::vector<nlohmann::json>>();
	}
	for (size_t i = 0; i < meshes.size(); i++)
	{
		cMeshObject* meshObject = (cMeshObject*) cMeshObjectFactory::createMeshObject();
		meshObject->meshName = meshes[i]["meshName"].get<std::string>();
		meshObject->friendlyName = meshes[i]["friendlyName"].get<std::string>();

		meshObject->isWireFrame = meshes[i]["isWireFrame"].get<bool>();
		meshObject->isVisible = meshes[i]["isVisible"].get<bool>();
		meshObject->useVertexColor = meshes[i]["useVertexColor"].get<bool>();
		meshObject->dontLight = meshes[i]["dontLight"].get<bool>();

		meshObject->position.x = meshes[i]["position"]["x"].get<float>();
		meshObject->position.y = meshes[i]["position"]["y"].get<float>();
		meshObject->position.z = meshes[i]["position"]["z"].get<float>();

		meshObject->postRotation.x = meshes[i]["postRotation"]["x"].get<float>();
		meshObject->postRotation.y = meshes[i]["postRotation"]["y"].get<float>();
		meshObject->postRotation.z = meshes[i]["postRotation"]["z"].get<float>();

		meshObject->materialDiffuse.r = meshes[i]["materialDiffuse"]["r"].get<float>();
		meshObject->materialDiffuse.g = meshes[i]["materialDiffuse"]["g"].get<float>();
		meshObject->materialDiffuse.b = meshes[i]["materialDiffuse"]["b"].get<float>();
		meshObject->materialDiffuse.a = meshes[i]["materialDiffuse"]["a"].get<float>();

		meshObject->materialSpecular.r = meshes[i]["materialSpecular"]["r"].get<float>();
		meshObject->materialSpecular.g = meshes[i]["materialSpecular"]["g"].get<float>();
		meshObject->materialSpecular.b = meshes[i]["materialSpecular"]["b"].get<float>();
		meshObject->materialSpecular.a = meshes[i]["materialSpecular"]["power"].get<float>();

		meshObject->scale = meshes[i]["scale"].get<float>();

		meshObject->isUpdatedByPhysics = meshes[i]["isUpdatedByPhysics"].get<bool>();

		meshObject->velocity.x = meshes[i]["velocity"]["x"].get<float>();
		meshObject->velocity.y = meshes[i]["velocity"]["y"].get<float>();
		meshObject->velocity.z = meshes[i]["velocity"]["z"].get<float>();

		meshObject->acceleration.x = meshes[i]["acceleration"]["x"].get<float>();
		meshObject->acceleration.y = meshes[i]["acceleration"]["y"].get<float>();
		meshObject->acceleration.z = meshes[i]["acceleration"]["z"].get<float>();

		vecObjectsToDraw.push_back(meshObject);
	}
}

cMeshObject* cSceneUtils::loadMeshInfoByFriendlyName( std::string friendlyName)
{
	//load models 
	std::vector<nlohmann::json> meshes;
	if (loadFromSaveFile)
	{
		std::ifstream ifs("savefile.json");
		nlohmann::json j = json::parse(ifs);
		ifs.close();
		meshes = j["meshes"].get<std::vector<nlohmann::json>>();
	}
	else
	{
		meshes = cJsonUtils::getJsonInstance()["meshes"].get<std::vector<nlohmann::json>>();
	}
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
			meshObject->dontLight = meshes[i]["dontLight"].get<bool>();

			meshObject->position.x = meshes[i]["position"]["x"].get<float>();
			meshObject->position.y = meshes[i]["position"]["y"].get<float>();
			meshObject->position.z = meshes[i]["position"]["z"].get<float>();

			meshObject->postRotation.x = meshes[i]["postRotation"]["x"].get<float>();
			meshObject->postRotation.y = meshes[i]["postRotation"]["y"].get<float>();
			meshObject->postRotation.z = meshes[i]["postRotation"]["z"].get<float>();

			meshObject->materialDiffuse.r = meshes[i]["materialDiffuse"]["r"].get<float>();
			meshObject->materialDiffuse.g = meshes[i]["materialDiffuse"]["g"].get<float>();
			meshObject->materialDiffuse.b = meshes[i]["materialDiffuse"]["b"].get<float>();
			meshObject->materialDiffuse.a = meshes[i]["materialDiffuse"]["a"].get<float>();

			meshObject->materialSpecular.r = meshes[i]["materialSpecular"]["r"].get<float>();
			meshObject->materialSpecular.g = meshes[i]["materialSpecular"]["g"].get<float>();
			meshObject->materialSpecular.b = meshes[i]["materialSpecular"]["b"].get<float>();
			meshObject->materialSpecular.a = meshes[i]["materialSpecular"]["power"].get<float>();

			meshObject->scale = meshes[i]["scale"].get<float>();

			meshObject->isUpdatedByPhysics = meshes[i]["isUpdatedByPhysics"].get<bool>();

			meshObject->velocity.x = meshes[i]["velocity"]["x"].get<float>();
			meshObject->velocity.y = meshes[i]["velocity"]["y"].get<float>();
			meshObject->velocity.z = meshes[i]["velocity"]["z"].get<float>();

			meshObject->acceleration.x = meshes[i]["acceleration"]["x"].get<float>();
			meshObject->acceleration.y = meshes[i]["acceleration"]["y"].get<float>();
			meshObject->acceleration.z = meshes[i]["acceleration"]["z"].get<float>();

			vecObjectsToDraw.push_back(meshObject);
			return meshObject;
		}
	}
	return NULL;
}

void cSceneUtils::initializeCamera()
{
	nlohmann::json json;
	if (loadFromSaveFile)
	{
		std::ifstream ifs("savefile.json");
		json = json::parse(ifs);
		ifs.close();
	}
	else
	{
		json = cJsonUtils::getJsonInstance();
	}
	cSceneUtils::cameraEye = glm::vec3(
		json["cameraEye"]["x"].get<float>(),
		json["cameraEye"]["y"].get<float>(),
		json["cameraEye"]["z"].get<float>()
	);

	cSceneUtils::cameraAt = glm::vec3(
		json["cameraAt"]["x"].get<float>(),
		json["cameraAt"]["y"].get<float>(),
		json["cameraAt"]["z"].get<float>()
	);
}

void cSceneUtils::drawObject(iMeshObject* pCurrentMesh, glm::mat4x4& matModel, GLuint shaderProgramID)
{
	cMeshObject* currentMesh = (cMeshObject*) pCurrentMesh;

	if (!currentMesh->isVisible)
		return;

	glm::mat4 matModelInvTrans;
	applyTranformations(currentMesh, matModel, matModelInvTrans);

	glUseProgram(shaderProgramID);


	GLint objectDiffuse_UniLoc = glGetUniformLocation(shaderProgramID, "objectDiffuse");
	GLint objectSpecular_UniLoc = glGetUniformLocation(shaderProgramID, "objectSpecular");

	GLint useVertexColour_UniLoc = glGetUniformLocation(shaderProgramID, "useVertexColour");

	GLint matModel_location = glGetUniformLocation(shaderProgramID, "matModel");
	GLint matModelInvTrans_location = glGetUniformLocation(shaderProgramID, "matModelInvTrans");
	GLint matView_location = glGetUniformLocation(shaderProgramID, "matView");
	GLint matProj_location = glGetUniformLocation(shaderProgramID, "matProj");

	GLint bDontUseLighting_UniLoc = glGetUniformLocation(shaderProgramID, "bDontUseLighting");

	glUniformMatrix4fv(matModel_location, 1, GL_FALSE, glm::value_ptr(matModel));
	glUniformMatrix4fv(matModelInvTrans_location, 1, GL_FALSE, glm::value_ptr(matModelInvTrans));

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glUniform4f(objectDiffuse_UniLoc,
		currentMesh->materialDiffuse.r,
		currentMesh->materialDiffuse.g,
		currentMesh->materialDiffuse.b,
		currentMesh->materialDiffuse.a);
	glUniform4f(objectSpecular_UniLoc,
		currentMesh->materialSpecular.r,
		currentMesh->materialSpecular.g,
		currentMesh->materialSpecular.b,
		currentMesh->materialSpecular.a);

	if (currentMesh->useVertexColor)
	{
		glUniform1f(useVertexColour_UniLoc, (float)GL_TRUE);
	}
	else
	{
		glUniform1f(useVertexColour_UniLoc, (float)GL_FALSE);
	}

	if (currentMesh->dontLight)
	{
		glUniform1f(bDontUseLighting_UniLoc, (float)GL_TRUE);
	}
	else
	{
		glUniform1f(bDontUseLighting_UniLoc, (float)GL_FALSE);
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

void cSceneUtils::applyTranformations(iMeshObject* pCurrentMesh, glm::mat4x4& matModel, glm::mat4& matModelInvTrans)
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
	matModelInvTrans = glm::inverse(glm::transpose(matModel));

	float scale = currentMesh->scale;
	glm::mat4 matScale = glm::scale(glm::mat4(1.0f), glm::vec3(scale, scale, scale));
	matModel = matModel * matScale;
}

void cSceneUtils::selectNextMeshObject(bool includeInvisibleObject)
{
	if (selectedObjectIndex >= vecObjectsToDraw.size() - 1)
	{
		selectedObjectIndex = 0;
	}
	else
	{
		selectedObjectIndex++;
	}
	selectedMeshObject = vecObjectsToDraw[selectedObjectIndex];
	std::cout << ((cMeshObject*)selectedMeshObject)->friendlyName << " selected, isVisible : " << ((cMeshObject*)selectedMeshObject)->isVisible << std::endl;
}

void cSceneUtils::drawEquipment(iEquipment* equipment, GLuint shaderProgramID)
{
	glm::mat4 matEquip = glm::mat4(1.0f);
	if (equipment->getMesh() != NULL)
	{
		drawObject(equipment->getMesh(), matEquip, shaderProgramID);
	}
}
