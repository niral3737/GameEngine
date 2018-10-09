#ifndef _ENEMY_FACTORY_IMPL_HG_
#define _ENEMY_FACTORY_IMPL_HG_

#include "iEnemy.h"

class cEnemyFactoryImpl
{
public:
	cEnemyFactoryImpl();
	~cEnemyFactoryImpl();

	iEnemy* createEnemy(unsigned int enemyType);
};

#endif // !_ENEMY_FACTORY_IMPL_HG_



