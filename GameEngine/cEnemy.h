#ifndef _C_ENEMY_HG_
#define _C_ENEMY_HG_

#include "iEnemy.h"
#include "iMeshObject.h"

class cEnemy : public iEnemy
{
public:
	cEnemy();
	virtual ~cEnemy();

	virtual void kill();

	iMeshObject* meshObject;
};

#endif // !_C_ENEMY_HG_



