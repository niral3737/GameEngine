#include "cEnemyFactory.h"


cEnemyFactory::cEnemyFactory()
{
	this->impl = new cEnemyFactoryImpl();
}


cEnemyFactory::~cEnemyFactory()
{
	delete this->impl;
}

iEnemy* cEnemyFactory::createEnemy(unsigned int enemyType)
{
	return impl->createEnemy(enemyType);
}