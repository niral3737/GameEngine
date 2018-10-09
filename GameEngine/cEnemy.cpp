#include "cEnemy.h"
#include <iostream>


cEnemy::cEnemy()
{
	this->meshObject = nullptr;

	std::cout << "Enemy created" << std::endl;
}

cEnemy::~cEnemy()
{
	std::cout << "Enemy destroyed" << std::endl;
}

void cEnemy::kill()
{
	std::cout << "Enemy Killed" << std::endl;
}
