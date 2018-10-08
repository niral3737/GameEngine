#ifndef _cPhysics_HG_
#define _cPhysics_HG_

#include <glm/glm.hpp>
#include <vector>
#include <iostream>
#include "cMeshObject.h"
#include "cModelDrawInfo.h"
#include "cVAOMeshUtils.h"

void PhysicsUpdate(double deltaTime, const cModelDrawInfo& collisionTest);

void PlayerPhysicsUpdate(cMeshObject* playerObject, double deltaTime);

void CalculateClosestPointsOnMesh(cModelDrawInfo theMeshDrawInfo, glm::vec3 pointToTest, std::vector<glm::vec3> &vecPoints);

unsigned int FindClosestPointOfAll(const glm::vec3& pointToTest, const std::vector<glm::vec3>& vecPoints, glm::vec3& finalPoint);

void GetClosestTriangle(unsigned int finalIndex, const cModelDrawInfo& info, glm::vec3& closestTriangle);

bool SphereSphereCollision(cMeshObject* pA, cMeshObject* pB);

bool SphereCubeCollision(cMeshObject* pSphere, cMeshObject* pCube, cModelDrawInfo& meshInfo);

#endif // !_cPhysics_HG_