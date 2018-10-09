// Physics
//#include "globalStuff.h"

#include "cPhysics.h"
#include "cSceneUtils.h"

typedef glm::vec3 Point;
typedef glm::vec3 Vector;

void PhysicsUpdate(double deltaTime, const cModelDrawInfo& collisionTest)
{
	cVAOMeshUtils* vao = cVAOMeshUtils::getInstance();
	cSceneUtils* scene = cSceneUtils::getInstance();
	// BOX LOCK
	const float LIMIT_POS_X = collisionTest.max.x;			// Lowest the objects can go
	const float LIMIT_NEG_X = collisionTest.min.x;			// Lowest the objects can go
	const float LIMIT_POS_Y = collisionTest.min.y + collisionTest.extent.x;			// Lowest the objects can go 
	const float LIMIT_NEG_Y = collisionTest.min.y;			// Lowest the objects can go
	const float LIMIT_POS_Z = collisionTest.max.z;			// Lowest the objects can go
	const float LIMIT_NEG_Z = collisionTest.min.z;			// Lowest the objects can go

	// The longest deltatime to wait before another update
	const double largestDeltaTime = 0.10; // 10Hz

	if (deltaTime > largestDeltaTime)
	{
		deltaTime = largestDeltaTime;
	}

	// Look for objects to update physics
	for (std::vector<iMeshObject*>::iterator vecIter = scene->vecObjectsToDraw.begin(); vecIter != scene->vecObjectsToDraw.end(); vecIter++)
	{
		cMeshObject* currentObject = (cMeshObject*) *vecIter;

		// Is it updated by physics?
		if (currentObject->isUpdatedByPhysics)
		{
			if (currentObject->friendlyName == "BlackPearl")
			{
				PlayerPhysicsUpdate(currentObject, deltaTime);
			}
			else
			{
				// EULER INTEGRATION
				currentObject->velocity.x += currentObject->acceleration.x * deltaTime;
				currentObject->velocity.y += currentObject->acceleration.y * deltaTime;
				currentObject->velocity.z += currentObject->acceleration.z * deltaTime;

				currentObject->position.x += currentObject->velocity.x * deltaTime;
				currentObject->position.y += currentObject->velocity.y * deltaTime;
				currentObject->position.z += currentObject->velocity.z * deltaTime;

				// Check for the ground and bounce it back
				if (currentObject->position.y <= LIMIT_NEG_Y)
				{
					// Normal to the ground is for now, just the Y vector (0, 1, 0)
					glm::vec3 normalToGround = glm::vec3(0.0f, 1.0f, 0.0f);

					// Invert the velocity downwards, simply by removing the negative
					currentObject->velocity.y = fabs(currentObject->velocity.y);
				}

				// BOX LOCK
				if (currentObject->position.x >= LIMIT_POS_X)
				{
					currentObject->velocity.x = -fabs(currentObject->velocity.x);
				}
				if (currentObject->position.x <= LIMIT_NEG_X)
				{
					currentObject->velocity.x = fabs(currentObject->velocity.x);
				}
				if (currentObject->position.y >= LIMIT_POS_Y)
				{
					currentObject->velocity.y = -fabs(currentObject->velocity.y);
				}
				if (currentObject->position.z >= LIMIT_POS_Z)
				{
					currentObject->velocity.z = -fabs(currentObject->velocity.z);
				}
				if (currentObject->position.z <= LIMIT_NEG_Z)
				{
					currentObject->velocity.z = fabs(currentObject->velocity.z);
				}
			}
			

			// Collision stuff has been commented for now

			// Check collisions with terrain
			//if (currentObject->name == "Link")
			//{
			//	std::vector<glm::vec3> vecClosestPoints;
			//	glm::vec3 finalPoint = { 0.0f, 0.0f, 0.0f };
			//	unsigned int finalIndex = 0;
			//	CalculateClosestPointsOnMesh(collisionTest, currentObject->position, vecClosestPoints);
			//	finalIndex = FindClosestPointOfAll(currentObject->position, vecClosestPoints, finalPoint);
			//	if (abs(glm::distance(currentObject->position, finalPoint) < currentObject->radius))
			//	{
			//		// Colliding, change colour
			//		currentObject->colour = glm::vec3(1.0f, 0.0f, 0.0f);
			//	}
			//	else
			//	{
			//		//currentObject->colour = glm::vec3(1.0f, 1.0f, 1.0f);
			//	}
			//}

			// Check collisions
			//for (std::vector<iEnemy*>::iterator enemyIter = vecpEnemiesToDraw.begin(); enemyIter != vecpEnemiesToDraw.end(); enemyIter++)
			//{
			//	// Dereference
			//	iEnemy* currentEnemy = *enemyIter;

			//	if (currentObject->name == "cube")
			//	{
			//		continue;
			//	}

			//	if (currentEnemy->bIsUpdatedByPhysics)
			//	{
			//		// Sphere collision
			//		if (currentEnemy->GetEnemyType() == "Ball")
			//		{
			//			if (currentObject != currentEnemy)
			//			{
			//				if (SphereSphereCollision(currentObject, currentEnemy))
			//				{
			//					currentObject->colour = glm::vec3(1.0f, 1.0f, 0.0f);
			//				}
			//			}
			//		}
			//		// Cube collision
			//		if (currentEnemy->GetEnemyType() == "Cube")
			//		{
			//			if (currentObject != currentEnemy)
			//			{
			//				//std::cout << "Cube testy" << std::endl;
			//				sModelDrawInfo enemyInfo;
			//				enemyInfo.meshFileName = currentEnemy->meshName;
			//				vao->FindDrawInfoByModelName(enemyInfo);
			//				if (SphereCubeCollision2(currentObject, currentEnemy, enemyInfo))
			//				{
			//					
			//					currentObject->colour = glm::vec3(0.0f, 0.0f, 1.0f);
			//				}
			//			}
			//		}
			//	}
			//}
		}
	}
}

void PlayerPhysicsUpdate(cMeshObject* playerObject, double deltaTime)
{
	// EULER INTEGRATION

	// Player ship has to have it's velocity tending to go
	// back to 0, so we need to update the values.
	float currentVel = 0.0f;
	if (playerObject->velocity.z > 0.0f)
	{
		// Natural deceleration rate, or "friction"
		playerObject->acceleration.z -= 0.5f;
		if (playerObject->acceleration.z < -10.0f)
		{
			playerObject->acceleration.z = -10.0f;
		}
		currentVel = playerObject->velocity.z + playerObject->acceleration.z * deltaTime;
		if (currentVel <= 0.0f)
		{
			playerObject->acceleration.z = 0.0f;
			playerObject->velocity.z = 0.0f;
		}
	}
	if (playerObject->velocity.z < 0.0f)
	{
		// Natural deceleration rate, or "friction"
		playerObject->acceleration.z += 0.5f;
		if (playerObject->acceleration.z > 10.0f)
		{
			playerObject->acceleration.z = 10.0f;
		}
		currentVel = playerObject->velocity.z + playerObject->acceleration.z * deltaTime;
		if (currentVel >= 0.0f)
		{
			playerObject->acceleration.z = 0.0f;
			playerObject->velocity.z = 0.0f;
		}
	}

	if (playerObject->velocity.x > 0.0f)
	{
		// Natural deceleration rate, or "friction"
		playerObject->acceleration.x -= 0.5f;
		if (playerObject->acceleration.x < -3.0f)
		{
			playerObject->acceleration.x = -3.0f;
		}
		currentVel = playerObject->velocity.x + playerObject->acceleration.x * deltaTime;
		if (currentVel <= 0.0f)
		{
			playerObject->acceleration.x = 0.0f;
			playerObject->velocity.x = 0.0f;
		}
	}
	if (playerObject->velocity.x < 0.0f)
	{
		// Natural deceleration rate, or "friction"
		playerObject->acceleration.x += 0.5f;
		if (playerObject->acceleration.x > 3.0f)
		{
			playerObject->acceleration.x = 3.0f;
		}
		currentVel = playerObject->velocity.x + playerObject->acceleration.x * deltaTime;
		if (currentVel >= 0.0f)
		{
			playerObject->acceleration.x = 0.0f;
			playerObject->velocity.x = 0.0f;
		}
	}

	// Player ship will navigate forward only, and rotate to change direction.
	playerObject->velocity.z += playerObject->acceleration.z * deltaTime;
	playerObject->velocity.x += playerObject->acceleration.x * deltaTime;
	// Clamp max velocity
	playerObject->velocity.z = playerObject->velocity.z > 5.0f ? 5.0f : playerObject->velocity.z;
	playerObject->velocity.z = playerObject->velocity.z < -5.0f ? -5.0f : playerObject->velocity.z;
	playerObject->velocity.x = playerObject->velocity.x > 5.0f ? 5.0f : playerObject->velocity.x;
	playerObject->velocity.x = playerObject->velocity.x < -5.0f ? -5.0f : playerObject->velocity.x;
	// Update position
	playerObject->position.z += playerObject->velocity.z * deltaTime;
	playerObject->postRotation.y += playerObject->velocity.x * deltaTime;
}

Point ClosestPtPointTriangle(Point p, Point a, Point b, Point c)
{
	Vector ab = b - a;
	Vector ac = c - a;
	Vector bc = c - b;

	// Compute parametric position s for projection P' of P on AB,
	// P' = A + s*AB, s = snom/(snom+sdenom)
	//   float snom = Dot(p - a, ab), sdenom = Dot(p - b, a - b);
	float snom = glm::dot(p - a, ab);
	float sdenom = glm::dot(p - b, a - b);

	// Compute parametric position t for projection P' of P on AC,
	// P' = A + t*AC, s = tnom/(tnom+tdenom)
	//    float tnom = Dot(p - a, ac), tdenom = Dot(p - c, a - c);
	float tnom = glm::dot(p - a, ac);
	float tdenom = glm::dot(p - c, a - c);

	if (snom <= 0.0f && tnom <= 0.0f) return a; // Vertex region early out

	// Compute parametric position u for projection P' of P on BC,
	// P' = B + u*BC, u = unom/(unom+udenom)
	//    float unom = Dot(p - b, bc), udenom = Dot(p - c, b - c);
	float unom = glm::dot(p - b, bc);
	float udenom = glm::dot(p - c, b - c);

	if (sdenom <= 0.0f && unom <= 0.0f) return b; // Vertex region early out
	if (tdenom <= 0.0f && udenom <= 0.0f) return c; // Vertex region early out


													// P is outside (or on) AB if the triple scalar product [N PA PB] <= 0
													//    Vector n = Cross(b - a, c - a);
	Vector n = glm::cross(b - a, c - a);
	//    float vc = Dot(n, Cross(a - p, b - p));

	float vc = glm::dot(n, glm::cross(a - p, b - p));

	// If P outside AB and within feature region of AB,
	// return projection of P onto AB
	if (vc <= 0.0f && snom >= 0.0f && sdenom >= 0.0f)
		return a + snom / (snom + sdenom) * ab;

	// P is outside (or on) BC if the triple scalar product [N PB PC] <= 0
	//    float va = Dot(n, Cross(b - p, c - p));
	float va = glm::dot(n, glm::cross(b - p, c - p));


	// If P outside BC and within feature region of BC,
	// return projection of P onto BC
	if (va <= 0.0f && unom >= 0.0f && udenom >= 0.0f)
		return b + unom / (unom + udenom) * bc;

	// P is outside (or on) CA if the triple scalar product [N PC PA] <= 0
	//    float vb = Dot(n, Cross(c - p, a - p));
	float vb = glm::dot(n, glm::cross(c - p, a - p));

	// If P outside CA and within feature region of CA,
	// return projection of P onto CA
	if (vb <= 0.0f && tnom >= 0.0f && tdenom >= 0.0f)
		return a + tnom / (tnom + tdenom) * ac;

	// P must project inside face region. Compute Q using barycentric coordinates
	float u = va / (va + vb + vc);
	float v = vb / (va + vb + vc);
	float w = 1.0f - u - v; // = vc / (va + vb + vc)
	return u * a + v * b + w * c;
}

//void CalculateClosestPointsOnMesh(cModelDrawInfo theMeshDrawInfo, glm::vec3 pointToTest, std::vector<glm::vec3> &vecPoints)
//{
//	vecPoints.clear();
//
//	// For each triangle in the mesh information...
//	for (unsigned int triIndex = 0; triIndex != theMeshDrawInfo.numberOfTriangles; triIndex++)
//	{
//		glm::ivec3 CurTri = theMeshDrawInfo.pTriangles[triIndex];
//
//		// ... call the ClosestPointToTriangle...
//		// Need to get the 3 vertices of the triangle
//		sPlyVertex corner_1 = theMeshDrawInfo.pVerticesFromFile[CurTri.vertexIndex1];
//		sPlyVertex corner_2 = theMeshDrawInfo.pVerticesFromFile[CurTri.vertexIndex2];
//		sPlyVertex corner_3 = theMeshDrawInfo.pVerticesFromFile[CurTri.vertexIndex3];
//
//		// Convert this to glm::vec3
//		glm::vec3 vert_1 = glm::vec3(corner_1.x, corner_1.y, corner_1.z);
//		glm::vec3 vert_2 = glm::vec3(corner_2.x, corner_2.y, corner_2.z);
//		glm::vec3 vert_3 = glm::vec3(corner_3.x, corner_3.y, corner_3.z);
//
//		glm::vec3 closestPoint = ClosestPtPointTriangle(pointToTest, vert_1, vert_2, vert_3);
//
//		vecPoints.push_back(closestPoint);
//	}
//}

// Find closest point from all points
unsigned int FindClosestPointOfAll(const glm::vec3& pointToTest, const std::vector<glm::vec3>& vecPoints, glm::vec3& finalPoint)
{
	glm::vec3 closestPoint = vecPoints[0]; // Begin assuming that the closes point is the first one
	float currentDist = glm::distance(pointToTest, closestPoint); // Check the distance with first assumed point

	unsigned int finalIndex = 0;
	for (unsigned int index = 1; index != vecPoints.size(); index++)
	{
		// Check next point distance
		// Should we set it to absolute?
		float nextDist = glm::distance(pointToTest, vecPoints[index]);
		if (nextDist < currentDist)
		{
			// If the distance checked now is smaller, this is the current smallest distanced point
			currentDist = nextDist;
			// And this is then our new closest point
			closestPoint = vecPoints[index];
			finalIndex = index;
		}
	}

	// In the end, pass the new closest point
	finalPoint = closestPoint;

	return finalIndex;
}

void GetClosestTriangle(unsigned int finalIndex, const cModelDrawInfo& info, glm::vec3& closestTriangle)
{
	// By logic, the closest point index is the closest triangle index.
	int x, y, z;
	x = info.pTriangles[finalIndex].x;
	y = info.pTriangles[finalIndex].y;
	z = info.pTriangles[finalIndex].z;

	// Update the closest triangle
	closestTriangle = { x, y, z };
}

//bool SphereSphereCollision(cMeshObject* pA, cMeshObject* pB)
//{
//	// Will be colliding if the size of their radius combined is higher than their distances.
//	// Seeing that the position is in the middle, the only time that position will be smaller
//	// is if the sphere radius are inside each other!
//	if (glm::distance(pA->position, pB->position) < (pA->radius + pB->radius))
//	{
//		return true;
//	}
//	return false;
//}
//
//bool SphereCubeCollision(cMeshObject* pSphere, cMeshObject* pCube, sModelDrawInfo& meshInfo)
//{
//	// We need to add the sphere radius to the extents of the cube.
//	// WARNING: THE MESH EXTENTS IS CALCULATED BEFORE MOVING THE OBJECT.
//	// THEREFORE, WE NEED TO TAKE THE MOVEMENT INTO ACCOUNT, ADDING THE POSITION
//	float newMaxX = meshInfo.maxX + pCube->position.x + pSphere->radius;
//	float newMaxY = meshInfo.maxY + pCube->position.y + pSphere->radius;
//	float newMaxZ = meshInfo.maxZ + pCube->position.z + pSphere->radius;
//	float newMinX = meshInfo.minX + pCube->position.x - pSphere->radius;
//	float newMinY = meshInfo.minY + pCube->position.y - pSphere->radius;
//	float newMinZ = meshInfo.minZ + pCube->position.z - pSphere->radius;
//
//	// Calculate the point to AABB, here the point is the center of the sphere.
//	// By adding the radius of the sphere to the box's extents, we then calculate
//	// if the sphere is inside the new created zone. That will only happen if the
//	// radius is getting inside of the box
//	if (pSphere->position.x > newMinX && pSphere->position.x < newMaxX &&
//		pSphere->position.y > newMinY && pSphere->position.y < newMaxY &&
//		pSphere->position.z > newMinZ && pSphere->position.z < newMaxZ)
//	{
//		
//		return true;
//	}
//	
//	return false;
//}