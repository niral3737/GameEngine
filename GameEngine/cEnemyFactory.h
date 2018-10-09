#ifndef _ENEMY_FACTORY_HG_
#define _ENEMY_FACTORY_HG_

#include "iEnemy.h"
#include "cEnemyFactoryImpl.h"

class cEnemyFactoryImpl;

class cEnemyFactory
{
public:
	cEnemyFactory();
	~cEnemyFactory();

	iEnemy* createEnemy(unsigned int enemyType);

private:
	cEnemyFactoryImpl* impl;
};

#endif // !_ENEMY_FACTORY_HG_



