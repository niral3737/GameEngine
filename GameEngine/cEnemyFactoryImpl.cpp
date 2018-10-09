#include "cEnemyFactoryImpl.h"

#include "cEnemy.h";

cEnemyFactoryImpl::cEnemyFactoryImpl()
{}


cEnemyFactoryImpl::~cEnemyFactoryImpl()
{}

iEnemy* cEnemyFactoryImpl::createEnemy(unsigned int enemyType)
{
	iEnemy* enemy = nullptr;

	if (enemyType == 1)
	{
		enemy = new cEnemy();
	}

	return enemy;
}