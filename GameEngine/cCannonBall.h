#ifndef _CANNON_BALL_HG_
#define _CANNON_BALL_HG_

#include "iEntity.h"
#include "iMeshObject.h"
#include "cMeshObject.h"

class cCannonBall :
	public iEntity
{
public:
	cCannonBall();
	virtual ~cCannonBall();

	bool isHit;
	size_t shootingBaseIndex;
	size_t baseThatHitIndex;

	iMeshObject* mesh;
	virtual std::string getName();
	cMeshObject* getMesh();
	void setMesh(iMeshObject* mesh);
	glm::vec3 lastInitialVelocity;

	void increaseShootingBaseIndex(size_t start, size_t end);
};


#endif // !_CANNON_BALL_HG_


