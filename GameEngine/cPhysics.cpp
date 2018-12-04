// Physics
//#include "globalStuff.h"

#include "cPhysics.h"
#include "cSceneUtils.h"
#include "cShaderUtils.h"
#include "cJsonUtils.h"
#include "cBase.h" 
#include "cCannonBall.h" 
#include "cRandomHelper.h"

const glm::vec3 cPhysics::ACCEL_GRAVITY = glm::vec3(0.0f, -5.0f, 0.0f);
float const INITIAL_Y_VELOCITY = 10.0f;

float cPhysics::initialProjectileVelocity = 10.0f;

void cPhysics::PhysicsUpdate(double deltaTime)
{
	cSceneUtils* scene = cSceneUtils::getInstance();
	//// BOX LOCK
	//const float LIMIT_POS_X = collisionTest.max.x;			// Lowest the objects can go
	//const float LIMIT_NEG_X = collisionTest.min.x;			// Lowest the objects can go
	//const float LIMIT_POS_Y = collisionTest.min.y + collisionTest.extent.x;			// Lowest the objects can go 
	//const float LIMIT_NEG_Y = collisionTest.min.y;			// Lowest the objects can go
	//const float LIMIT_POS_Z = collisionTest.max.z;			// Lowest the objects can go
	//const float LIMIT_NEG_Z = collisionTest.min.z;			// Lowest the objects can go

	// The longest deltatime to wait before another update
	const double largestDeltaTime = 0.10; // 10Hz

	if (deltaTime > largestDeltaTime)
	{
		deltaTime = largestDeltaTime;
	}


	for (size_t i = 0; i < scene->vecIslands.size(); i++)
	{
		cIsland* island = scene->vecIslands[i];

		if (island->highCannnonBall->getMesh()->isUpdatedByPhysics)
		{
			island->highCannnonBall->getMesh()->velocity.x += island->highCannnonBall->getMesh()->acceleration.x * deltaTime;
			island->highCannnonBall->getMesh()->velocity.y += island->highCannnonBall->getMesh()->acceleration.y * deltaTime;
			island->highCannnonBall->getMesh()->velocity.z += island->highCannnonBall->getMesh()->acceleration.z * deltaTime;

			island->highCannnonBall->getMesh()->position.x += island->highCannnonBall->getMesh()->velocity.x * deltaTime;
			island->highCannnonBall->getMesh()->position.y += island->highCannnonBall->getMesh()->velocity.y * deltaTime;
			island->highCannnonBall->getMesh()->position.z += island->highCannnonBall->getMesh()->velocity.z * deltaTime;
		}
		else
		{
			continue;
		}
		if (island->lowCannnonBall->getMesh()->isUpdatedByPhysics)
		{
			island->lowCannnonBall->getMesh()->velocity.x += island->lowCannnonBall->getMesh()->acceleration.x * deltaTime;
			island->lowCannnonBall->getMesh()->velocity.y += island->lowCannnonBall->getMesh()->acceleration.y * deltaTime;
			island->lowCannnonBall->getMesh()->velocity.z += island->lowCannnonBall->getMesh()->acceleration.z * deltaTime;

			island->lowCannnonBall->getMesh()->position.x += island->lowCannnonBall->getMesh()->velocity.x * deltaTime;
			island->lowCannnonBall->getMesh()->position.y += island->lowCannnonBall->getMesh()->velocity.y * deltaTime;
			island->lowCannnonBall->getMesh()->position.z += island->lowCannnonBall->getMesh()->velocity.z * deltaTime;
		}
		else
		{
			continue;
		}

		cCannonBall* highCannonBall = island->highCannnonBall;
		cCannonBall* lowCannonBall = island->lowCannnonBall;

		addProjectileAim(deltaTime, highCannonBall, island->bases[highCannonBall->shootingBaseIndex]);
		//collision logic
		for (size_t j = 0; j < island->islandOnWhichRandomBaseIs->bases.size() ; j++)
		{
			cBase* baseUnderAttack = island->islandOnWhichRandomBaseIs->bases[j];

			//check colision
			float distance = glm::abs(glm::distance(baseUnderAttack->getMesh()->position, highCannonBall->getMesh()->position));
			float totalRadius = baseUnderAttack->getMesh()->radius + highCannonBall->getMesh()->radius;
			if (distance < totalRadius)
			{
				//it's a hit
				highCannonBall->baseThatHitIndex = highCannonBall->shootingBaseIndex;
				highCannonBall->isHit = true;
				//baseUnderAttack->getMesh()->materialDiffuse.r -= 0.05;
			}

			distance = glm::abs(glm::distance(baseUnderAttack->getMesh()->position, lowCannonBall->getMesh()->position));
			totalRadius = baseUnderAttack->getMesh()->radius + lowCannonBall->getMesh()->radius;
			if (distance < totalRadius)
			{
				//it's a hit
				lowCannonBall->baseThatHitIndex = lowCannonBall->shootingBaseIndex;
				lowCannonBall->isHit = true;
				//baseUnderAttack->getMesh()->materialDiffuse.r -= 0.05;
			}
		}

		if (highCannonBall->getMesh()->position.y <= 0.0f || 
			highCannonBall->getMesh()->position.x >= 256.0f ||
			highCannonBall->getMesh()->position.x <= -256.0f ||
			highCannonBall->getMesh()->position.z >= 256.0f ||
			highCannonBall->getMesh()->position.z <= -256.0f)
		{

			if (highCannonBall->isHit)
			{
				// adjust all the cannons to a point
				std::cout << "high cannon ball hit" << std::endl;
				if (island->randomBaseToDestroy->health <= 0)
				{
					//base is destroyed
					island->randomBaseToDestroy->isDestroyed = true;
					island->randomBaseToDestroy->getMesh()->setDiffuseColour(glm::vec3(0.7f, 0.7f, 0.7f));

					//pick a randomBase to destroy
					for (size_t j = 0; j < scene->vecIslands.size(); j++)
					{
						unsigned int randomIslandIndex = cRandomHelper::generateRandomIntInRange(0, 3);
						while (randomIslandIndex == i)
						{
							randomIslandIndex = cRandomHelper::generateRandomIntInRange(0, 3);
						}

						unsigned int randomBaseOnThatIslandIndex = cRandomHelper::generateRandomIntInRange(0, 19);

						while (scene->vecIslands[randomIslandIndex]->bases[randomBaseOnThatIslandIndex]->isDestroyed)
						{
							randomBaseOnThatIslandIndex = cRandomHelper::generateRandomIntInRange(0, 19);
						}
						island->randomBaseToDestroy = scene->vecIslands[randomIslandIndex]->bases[randomBaseOnThatIslandIndex];
						island->islandOnWhichRandomBaseIs = scene->vecIslands[randomIslandIndex];
					}

					//set new velocity from old velocity
					island->highCannnonBall->getMesh()->velocity.y = island->highCannnonBall->lastInitialVelocity.y;

					island->highCannnonBall->lastInitialVelocity = island->highCannnonBall->getMesh()->velocity;

					island->highCannnonBall->getMesh()->velocity.x = island->randomBaseToDestroy->getMesh()->position.x
						- island->highCannnonBall->getMesh()->position.x;
					island->highCannnonBall->getMesh()->velocity.z = island->randomBaseToDestroy->getMesh()->position.z
						- island->highCannnonBall->getMesh()->position.z;

				}
				else
				{
					island->randomBaseToDestroy->health -= 5;
					island->randomBaseToDestroy->getMesh()->materialDiffuse.r += 0.05f;
					highCannonBall->getMesh()->velocity = highCannonBall->lastInitialVelocity;
					highCannonBall->getMesh()->velocity.x = island->randomBaseToDestroy->getMesh()->position.x
						- island->bases[highCannonBall->shootingBaseIndex]->getMesh()->position.x;
					highCannonBall->getMesh()->velocity.z = island->randomBaseToDestroy->getMesh()->position.z
						- island->bases[highCannonBall->shootingBaseIndex]->getMesh()->position.z;
				}
				highCannonBall->getMesh()->position = island->bases[highCannonBall->shootingBaseIndex]->getMesh()->position;
			}
			else
			{
				// turn over as it's going into oblivion
				highCannonBall->getMesh()->velocity = highCannonBall->lastInitialVelocity;
				highCannonBall->getMesh()->velocity.y -= 1.0f;
				highCannonBall->increaseShootingBaseIndex(0, 2);
				unsigned int counter = 0;
				while (counter < 3 && island->bases[highCannonBall->shootingBaseIndex]->isDestroyed)
				{
					highCannonBall->increaseShootingBaseIndex(0, 2);
					counter++;
				}

				//all bases destroyed
				if (counter == 3)
				{
					highCannonBall->getMesh()->isUpdatedByPhysics = false;
					island->isHighDestroyed = true;
				}
				highCannonBall->getMesh()->position = island->bases[highCannonBall->shootingBaseIndex]->getMesh()->position;
				highCannonBall->getMesh()->velocity.x = island->randomBaseToDestroy->getMesh()->position.x
					- island->bases[highCannonBall->shootingBaseIndex]->getMesh()->position.x;
				highCannonBall->getMesh()->velocity.z = island->randomBaseToDestroy->getMesh()->position.z
					- island->bases[highCannonBall->shootingBaseIndex]->getMesh()->position.z;

			}
		}

		if (lowCannonBall->getMesh()->position.y <= 0.0f ||
			lowCannonBall->getMesh()->position.x >= 256.0f ||
			lowCannonBall->getMesh()->position.x <= -256.0f ||
			lowCannonBall->getMesh()->position.z >= 256.0f ||
			lowCannonBall->getMesh()->position.z <= -256.0f)
		{
			if (lowCannonBall->isHit)
			{
				// adjust all the cannons to a point
				std::cout << "low cannon ball hit" << std::endl;
				if (island->randomBaseToDestroy->health <= 0)
				{
					//base is destroyed
					island->randomBaseToDestroy->isDestroyed = true;
					island->randomBaseToDestroy->getMesh()->setDiffuseColour(glm::vec3(0.7f, 0.7f, 0.7f));

					//pick a randomBase to destroy
					for (size_t j = 0; j < scene->vecIslands.size(); j++)
					{
						unsigned int randomIslandIndex = cRandomHelper::generateRandomIntInRange(0, 3);
						while (randomIslandIndex == i)
						{
							randomIslandIndex = cRandomHelper::generateRandomIntInRange(0, 3);
						}

						unsigned int randomBaseOnThatIslandIndex = cRandomHelper::generateRandomIntInRange(0, 19);

						while (scene->vecIslands[randomIslandIndex]->bases[randomBaseOnThatIslandIndex]->isDestroyed)
						{
							randomBaseOnThatIslandIndex = cRandomHelper::generateRandomIntInRange(0, 19);
						}
						island->randomBaseToDestroy = scene->vecIslands[randomIslandIndex]->bases[randomBaseOnThatIslandIndex];
						island->islandOnWhichRandomBaseIs = scene->vecIslands[randomIslandIndex];
					}

					//set new velocity from old velocity
					island->lowCannnonBall->getMesh()->velocity.y = island->lowCannnonBall->lastInitialVelocity.y;

					island->lowCannnonBall->lastInitialVelocity = island->lowCannnonBall->getMesh()->velocity;

					island->lowCannnonBall->getMesh()->velocity.x = island->randomBaseToDestroy->getMesh()->position.x
						- island->lowCannnonBall->getMesh()->position.x;
					island->lowCannnonBall->getMesh()->velocity.z = island->randomBaseToDestroy->getMesh()->position.z
						- island->lowCannnonBall->getMesh()->position.z;

				}
				else
				{
					island->randomBaseToDestroy->health -= 5;
					island->randomBaseToDestroy->getMesh()->materialDiffuse.r += 0.05f;
					lowCannonBall->getMesh()->velocity = lowCannonBall->lastInitialVelocity;
					lowCannonBall->getMesh()->velocity.x = island->randomBaseToDestroy->getMesh()->position.x
						- island->bases[lowCannonBall->shootingBaseIndex]->getMesh()->position.x;
					lowCannonBall->getMesh()->velocity.z = island->randomBaseToDestroy->getMesh()->position.z
						- island->bases[lowCannonBall->shootingBaseIndex]->getMesh()->position.z;
				}
				lowCannonBall->getMesh()->position = island->bases[lowCannonBall->shootingBaseIndex]->getMesh()->position;
			}
			else
			{
				lowCannonBall->getMesh()->velocity = lowCannonBall->lastInitialVelocity;
				lowCannonBall->getMesh()->velocity.y -= 0.1;
				lowCannonBall->increaseShootingBaseIndex(3, 19);
				unsigned int counter = 0;
				while (counter < 17 && island->bases[lowCannonBall->shootingBaseIndex]->isDestroyed)
				{
					lowCannonBall->increaseShootingBaseIndex(3, 19);
					counter++;
				}

				//all bases destroyed
				if (counter == 17)
				{
					lowCannonBall->getMesh()->isUpdatedByPhysics = false;
					island->isLowDestroyed = true;
				}

				lowCannonBall->getMesh()->position = island->bases[lowCannonBall->shootingBaseIndex]->getMesh()->position;
				lowCannonBall->getMesh()->velocity.x = island->randomBaseToDestroy->getMesh()->position.x
					- island->bases[lowCannonBall->shootingBaseIndex]->getMesh()->position.x;
				lowCannonBall->getMesh()->velocity.z = island->randomBaseToDestroy->getMesh()->position.z
					- island->bases[lowCannonBall->shootingBaseIndex]->getMesh()->position.z;

			}
		}
	}

	// Look for objects to update physics
	//for (std::vector<iMeshObject*>::iterator vecIter = scene->vecObjectsToDraw.begin(); vecIter != scene->vecObjectsToDraw.end(); vecIter++)
	//{
	//	cMeshObject* currentObject = (cMeshObject*) *vecIter;

	//	// Is it updated by physics?
	//	if (currentObject->isUpdatedByPhysics)
	//	{
	//		if (currentObject->friendlyName == "BlackPearl")
	//		{
	//			PlayerPhysicsUpdate(currentObject, deltaTime);
	//		}
	//		else
	//		{
	//			// EULER INTEGRATION
	//			currentObject->velocity.x += currentObject->acceleration.x * deltaTime;
	//			currentObject->velocity.y += currentObject->acceleration.y * deltaTime;
	//			currentObject->velocity.z += currentObject->acceleration.z * deltaTime;

	//			currentObject->position.x += currentObject->velocity.x * deltaTime;
	//			currentObject->position.y += currentObject->velocity.y * deltaTime;
	//			currentObject->position.z += currentObject->velocity.z * deltaTime;

	//			// Check for the ground and bounce it back
	//			//if (currentObject->position.y <= LIMIT_NEG_Y)
	//			//{
	//			//	// Normal to the ground is for now, just the Y vector (0, 1, 0)
	//			//	glm::vec3 normalToGround = glm::vec3(0.0f, 1.0f, 0.0f);

	//			//	// Invert the velocity downwards, simply by removing the negative
	//			//	currentObject->velocity.y = fabs(currentObject->velocity.y);
	//			//}

	//			//// BOX LOCK
	//			//if (currentObject->position.x >= LIMIT_POS_X)
	//			//{
	//			//	currentObject->velocity.x = -fabs(currentObject->velocity.x);
	//			//}
	//			//if (currentObject->position.x <= LIMIT_NEG_X)
	//			//{
	//			//	currentObject->velocity.x = fabs(currentObject->velocity.x);
	//			//}
	//			//if (currentObject->position.y >= LIMIT_POS_Y)
	//			//{
	//			//	currentObject->velocity.y = -fabs(currentObject->velocity.y);
	//			//}
	//			//if (currentObject->position.z >= LIMIT_POS_Z)
	//			//{
	//			//	currentObject->velocity.z = -fabs(currentObject->velocity.z);
	//			//}
	//			//if (currentObject->position.z <= LIMIT_NEG_Z)
	//			//{
	//			//	currentObject->velocity.z = fabs(currentObject->velocity.z);
	//			//}
	//		}
	//		

	//		// Collision stuff has been commented for now

	//		// Check collisions with terrain
	//		//if (currentObject->friendlyName == "box")
	//		//{
	//		//	std::vector<glm::vec3> vecClosestPoints;
	//		//	glm::vec3 finalPoint = { 0.0f, 0.0f, 0.0f };
	//		//	unsigned int finalIndex = 0;
	//		//	CalculateClosestPointsOnMesh(collisionTest, currentObject->position, vecClosestPoints);
	//		//	finalIndex = FindClosestPointOfAll(currentObject->position, vecClosestPoints, finalPoint);
	//		//	if (abs(glm::distance(currentObject->position, finalPoint) < 6))
	//		//	{
	//		//		// Colliding, change colour
	//		//		currentObject->velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	//		//	}
	//		//	else
	//		//	{
	//		//		//currentObject->colour = glm::vec3(1.0f, 1.0f, 1.0f);
	//		//	}
	//		//}

	//		for (std::vector<iMeshObject*>::iterator vecIter = scene->vecObjectsToDraw.begin(); vecIter != scene->vecObjectsToDraw.end(); vecIter++)
	//		{
	//			cMeshObject* obj = (cMeshObject*)*vecIter;

	//			if (currentObject->friendlyName == "cueBall" && obj->friendlyName == "box")
	//			{
	//				//taking ball's radius as 1 and box's radius as 6
	//				//collision if radius + radius > distance
	//				if (abs(glm::distance(currentObject->position, obj->position) < (6 + 1)))
	//				{
	//					obj->setDiffuseColour(glm::vec3(0.0f, 1.0f, 0.0f));
	//				}
	//			}

	//		}

	//		// Check collisions
	//		//for (std::vector<iEnemy*>::iterator enemyIter = vecpEnemiesToDraw.begin(); enemyIter != vecpEnemiesToDraw.end(); enemyIter++)
	//		//{
	//		//	// Dereference
	//		//	iEnemy* currentEnemy = *enemyIter;

	//		//	if (currentObject->name == "cube")
	//		//	{
	//		//		continue;
	//		//	}

	//		//	if (currentEnemy->bIsUpdatedByPhysics)
	//		//	{
	//		//		// Sphere collision
	//		//		if (currentEnemy->GetEnemyType() == "Ball")
	//		//		{
	//		//			if (currentObject != currentEnemy)
	//		//			{
	//		//				if (SphereSphereCollision(currentObject, currentEnemy))
	//		//				{
	//		//					currentObject->colour = glm::vec3(1.0f, 1.0f, 0.0f);
	//		//				}
	//		//			}
	//		//		}
	//		//		// Cube collision
	//		//		if (currentEnemy->GetEnemyType() == "Cube")
	//		//		{
	//		//			if (currentObject != currentEnemy)
	//		//			{
	//		//				//std::cout << "Cube testy" << std::endl;
	//		//				sModelDrawInfo enemyInfo;
	//		//				enemyInfo.meshFileName = currentEnemy->meshName;
	//		//				vao->FindDrawInfoByModelName(enemyInfo);
	//		//				if (SphereCubeCollision2(currentObject, currentEnemy, enemyInfo))
	//		//				{
	//		//					
	//		//					currentObject->colour = glm::vec3(0.0f, 0.0f, 1.0f);
	//		//				}
	//		//			}
	//		//		}
	//		//	}
	//		//}
	//	}
	//}
}

void cPhysics::PlayerPhysicsUpdate(cMeshObject* playerObject, double deltaTime)
{
	// EULER INTEGRATION

	// Player ship has to have it's velocity tending to go
	// back to 0, so we need to update the values.
	double currentVel = 0.0f;
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

glm::vec3 cPhysics::ClosestPtPointTriangle(glm::vec3 p, glm::vec3 a, glm::vec3 b, glm::vec3 c)
{
	glm::vec3 ab = b - a;
	glm::vec3 ac = c - a;
	glm::vec3 bc = c - b;

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
	glm::vec3 n = glm::cross(b - a, c - a);
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

void cPhysics::CalculateClosestPointsOnMesh(cModelDrawInfo theMeshDrawInfo, glm::vec3 pointToTest, std::vector<glm::vec3> &vecPoints)
{
	vecPoints.clear();

	// For each triangle in the mesh information...
	for (unsigned int triIndex = 0; triIndex != theMeshDrawInfo.numberOfTriangles; triIndex++)
	{
		glm::ivec3 CurTri = theMeshDrawInfo.pTriangles[triIndex];

		// ... call the ClosestPointToTriangle...
		// Need to get the 3 vertices of the triangle
		glm::vec3 corner_1 = theMeshDrawInfo.pVerticesFromFile[CurTri.x];
		glm::vec3 corner_2 = theMeshDrawInfo.pVerticesFromFile[CurTri.y];
		glm::vec3 corner_3 = theMeshDrawInfo.pVerticesFromFile[CurTri.z];

		// Convert this to glm::vec3
		glm::vec3 vert_1 = glm::vec3(corner_1.x, corner_1.y, corner_1.z);
		glm::vec3 vert_2 = glm::vec3(corner_2.x, corner_2.y, corner_2.z);
		glm::vec3 vert_3 = glm::vec3(corner_3.x, corner_3.y, corner_3.z);

		glm::vec3 closestPoint = ClosestPtPointTriangle(pointToTest, vert_1, vert_2, vert_3);

		vecPoints.push_back(closestPoint);
	}
}

// Find closest point from all points
unsigned int cPhysics::FindClosestPointOfAll(const glm::vec3& pointToTest, const std::vector<glm::vec3>& vecPoints, glm::vec3& finalPoint)
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

void cPhysics::GetClosestTriangle(unsigned int finalIndex, const cModelDrawInfo& info, glm::vec3& closestTriangle)
{
	// By logic, the closest point index is the closest triangle index.
	int x, y, z;
	x = info.pTriangles[finalIndex].x;
	y = info.pTriangles[finalIndex].y;
	z = info.pTriangles[finalIndex].z;

	// Update the closest triangle
	closestTriangle = { x, y, z };
}

void cPhysics::addProjectileAim(double deltaTime, cCannonBall* cannonBall, cBase* shootingBase)
{
//	cCamera* camera = cCamera::getInstance();
//
//	glm::vec3 cameraAt = camera->getAtInWorldSpace();
//
//	//cMeshObject* shootRing = (cMeshObject*)cSceneUtils::getInstance()->findObjectByFriendlyName("ring");
//
	cMeshObject* debugSphere = (cMeshObject*) cSceneUtils::getInstance()->findObjectByFriendlyName("base");
//
//	debugSphere->position = cameraAt;
//	debugSphere->setDiffuseColour(glm::vec3(1.0f, 0.0f, 0.0f));
//	debugSphere->isVisible = true;
//	debugSphere->scale = 0.1f;
//	debugSphere->isWireFrame = true;
//	debugSphere->isUpdatedByPhysics = false;
//
//	/*shootRing->position = cameraAt;
//	shootRing->setOrientationEulerAngles(glm::vec3(90.0f, 0.0f, 0.0f), true);
//	shootRing->setDiffuseColour(glm::vec3(1.0f, 0.0f, 0.0f));
//	shootRing->isVisible = true;
//	shootRing->scale = 0.01f;
//	shootRing->isWireFrame = true;
//	shootRing->isUpdatedByPhysics = false;
//*/
//	glm::mat4 matWorld = glm::mat4(1.0f);
	std::string shaderProgramName = cJsonUtils::getJsonInstance()["shaderProgramName"].get<std::string>();
//	cSceneUtils::getInstance()->drawObject(debugSphere, matWorld, cShaderUtils::getInstance()->getIdFromFriendlyName(shaderProgramName));

	glm::vec3 projVelWorldSpace = cannonBall->lastInitialVelocity;
	glm::vec3 projPosition = shootingBase->getMesh()->position;
	glm::vec3 projAccel = ACCEL_GRAVITY;


	// draw projectile path

	float timeStep = 0.25f;
	float howLongWeGonnaRun = 10.0f;

	for (float time = 0.0f; time < howLongWeGonnaRun; time += timeStep)
	{
		projVelWorldSpace.x = projVelWorldSpace.x + (projAccel.x * timeStep);
		projVelWorldSpace.y = projVelWorldSpace.y + (projAccel.y * timeStep);
		projVelWorldSpace.z = projVelWorldSpace.z + (projAccel.z * timeStep);

		// Update position from velocity
		projPosition.x = projPosition.x + (projVelWorldSpace.x * timeStep);
		projPosition.y = projPosition.y + (projVelWorldSpace.y * timeStep);
		projPosition.z = projPosition.z + (projVelWorldSpace.z * timeStep);

		// Draw a sphere at each of these locations...
		debugSphere->position = projPosition;
		debugSphere->setDiffuseColour(glm::vec3(0.8f, 0.8f, 0.8f));
		debugSphere->isVisible = true;
		debugSphere->scale = 0.075f;

		glm::mat4 matWorld = glm::mat4(1.0f);
		cSceneUtils::getInstance()->drawObject(debugSphere, matWorld, cShaderUtils::getInstance()->getIdFromFriendlyName(shaderProgramName));

		debugSphere->isVisible = false;
	}

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